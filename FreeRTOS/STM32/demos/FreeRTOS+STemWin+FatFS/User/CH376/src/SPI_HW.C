/* CH376芯片 硬件标准SPI串行连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

#include	"HAL.H"

/* 本例中的硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) */
/* 单片机的引脚    CH376芯片的引脚
      P1.4                 SCS
      P1.5                 SDI
      P1.6                 SDO
      P1.7                 SCK      */
#define	CH376_SPI_SCS			P14		/* 假定CH376的SCS引脚 */
#define	CH376_SPI_SDO			P16		/* 假定CH376的SDO引脚 */

sfr		SPDR = 0x86;	/* SPI数据寄存器 */
sfr		SPSR = 0xAA;	/* SPI状态寄存器 */
sfr		SPCR = 0xD5;    /* SPI控制寄存器 */
#define	SPI_IF_TRANS	0x80	/* SPI字节传输完成标志,在SPSR的位7 */

#define CH376_INT_WIRE			INT0	/* 假定CH376的INT#引脚,如果未连接那么也可以通过查询兼做中断输出的SDO引脚状态实现 */

void	CH376_PORT_INIT( void )  /* 由于使用SPI读写时序,所以进行初始化 */
{
/* 如果是硬件SPI接口,那么可使用mode3(CPOL=1&CPHA=1)或mode0(CPOL=0&CPHA=0),CH376在时钟上升沿采样输入,下降沿输出,数据位是高位在前 */
	CH376_SPI_SCS = 1;  /* 禁止SPI片选 */
/* 对于双向I/O引脚模拟SPI接口,那么必须在此设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	SPCR = 0x5C;  /* 设置SPI模式3, DORD=0(MSB first), CPOL=1, CPHA=1, CH376也支持SPI模式0 */
}

void	mDelay0_5uS( void )  /* 至少延时0.5uS,根据单片机主频调整 */
{
}

UINT8	Spi376Exchange( UINT8 d )  /* 硬件SPI输出且输入8个位数据 */
{  /* 为了提高速度,可以将该子程序做成宏以减少子程序调用层次 */
	SPDR = d;  /* 先将数据写入SPI数据寄存器,然后查询SPI状态寄存器以等待SPI字节传输完成 */
	while ( ( SPSR & SPI_IF_TRANS ) == 0 );  /* 查询SPI状态寄存器以等待SPI字节传输完成 */
	SPSR &= ~ SPI_IF_TRANS;  /* 清除SPI字节传输完成标志,有的单片机会自动清除 */
	return( SPDR );  /* 先查询SPI状态寄存器以等待SPI字节传输完成,然后从SPI数据寄存器读出数据 */
}

#define	xEndCH376Cmd( )	{ CH376_SPI_SCS = 1; }  /* SPI片选无效,结束CH376命令,仅用于SPI接口方式 */

void	xWriteCH376Cmd( UINT8 mCmd )  /* 向CH376写命令 */
{
#ifdef	CH376_SPI_BZ
	UINT8	i;
#endif
	CH376_SPI_SCS = 1;  /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
/* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	CH376_SPI_SCS = 0;  /* SPI片选有效 */
	Spi376Exchange( mCmd );  /* 发出命令码 */
#ifdef	CH376_SPI_BZ
	for ( i = 30; i != 0 && CH376_SPI_BZ; -- i );  /* SPI忙状态查询,等待CH376不忙,或者下面一行的延时1.5uS代替 */
#else
	mDelay0_5uS( ); mDelay0_5uS( ); mDelay0_5uS( );  /* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
#endif
}

#ifdef	FOR_LOW_SPEED_MCU  /* 不需要延时 */
#define	xWriteCH376Data( d )	{ Spi376Exchange( d ); }  /* 向CH376写数据 */
#define	xReadCH376Data( )		( Spi376Exchange( 0xFF ) )  /* 从CH376读数据 */
#else
void	xWriteCH376Data( UINT8 mData )  /* 向CH376写数据 */
{
	Spi376Exchange( mData );
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
}
UINT8	xReadCH376Data( void )  /* 从CH376读数据 */
{
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
	return( Spi376Exchange( 0xFF ) );
}
#endif

/* 查询CH376中断(INT#低电平) */
UINT8	Query376Interrupt( void )
{
#ifdef	CH376_INT_WIRE
	return( CH376_INT_WIRE ? FALSE : TRUE );  /* 如果连接了CH376的中断引脚则直接查询中断引脚 */
#else
	return( CH376_SPI_SDO ? FALSE : TRUE );  /* 如果未连接CH376的中断引脚则查询兼做中断输出的SDO引脚状态 */
#endif
}

UINT8	mInitCH376Host( void )  /* 初始化CH376 */
{
	UINT8	res;
	CH376_PORT_INIT( );  /* 接口硬件初始化 */
	xWriteCH376Cmd( CMD11_CHECK_EXIST );  /* 测试单片机与CH376之间的通讯接口 */
	xWriteCH376Data( 0x65 );
	res = xReadCH376Data( );
	xEndCH376Cmd( );
	if ( res != 0x9A ) return( ERR_USB_UNKNOWN );  /* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
	xWriteCH376Data( 0x06 );
	mDelayuS( 20 );
	res = xReadCH376Data( );
	xEndCH376Cmd( );
#ifndef	CH376_INT_WIRE
#ifdef	CH376_SPI_SDO
	xWriteCH376Cmd( CMD20_SET_SDO_INT );  /* 设置SPI的SDO引脚的中断方式 */
	xWriteCH376Data( 0x16 );
	xWriteCH376Data( 0x90 );  /* SDO引脚在SCS片选无效时兼做中断请求输出 */
	xEndCH376Cmd( );
#endif
#endif
	if ( res == CMD_RET_SUCCESS ) return( USB_INT_SUCCESS );
	else return( ERR_USB_UNKNOWN );  /* 设置模式错误 */
}
