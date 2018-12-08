/* CH376芯片 软件模拟SPI串行连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

#include "stm32f10x.h"
#include	"HAL.H"

/* 安富莱STM32-V4开发板硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) 
	PF10             CH376的SCS引脚
	PA5/SPI1_SCK     CH376的SCK引脚
	PA6/SPI1_MISO    CH376的SDO引脚
	PA7/SPI1_MOSI	 CH376的SDI引脚
	PE2              CH376输出的中断信号
*/
#define CH376_SCK_0()	GPIO_ResetBits(GPIOA,  GPIO_Pin_5)
#define CH376_SCK_1()	GPIO_SetBits(GPIOA,  GPIO_Pin_5)

#define CH376_SDI_0()	GPIO_ResetBits(GPIOA,  GPIO_Pin_7)
#define CH376_SDI_1()	GPIO_SetBits(GPIOA,  GPIO_Pin_7)

#define CH376_SCS_0()	GPIO_ResetBits(GPIOF,  GPIO_Pin_10)
#define CH376_SCS_1()	GPIO_SetBits(GPIOF,  GPIO_Pin_10)

#define CH376_SDO_HIGH()	(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != Bit_RESET)
#define CH376_INT_HIGH()	(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) != Bit_RESET)

/* 使用软件模拟SPI读写时序,进行初始化 */
void CH376_PORT_INIT( void )  
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	
	/* 打开CPU的GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF
				| RCC_APB2Periph_GPIOG, ENABLE);

	/* 配置 PA5/SPI1_SCK,PA7/SPI1_MOSI为推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	/* 配置 PA6/SPI1_MISO 为输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置 PE2 为输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);							
	
	CH376_SCS_1();  /* 禁止SPI片选 */
	CH376_SCK_1();  /* 默认为高电平,SPI模式3,也可以用SPI模式0,但模拟程序可能需稍做修改 */	
		
	/* 如果是硬件SPI接口,那么可使用mode3(CPOL=1&CPHA=1)或mode0(CPOL=0&CPHA=0),
		CH376在时钟上升沿采样输入,下降沿输出,数据位是高位在前 */

	/* 将其他未用的SPI设备的片选设置为高电平 */
	#if 1	
		/* (1) SST25VF016B  , 片选为PB2 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出 */
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		
		/* (2) LCD模块的触摸IC(TS2046), 片选为PG11 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出 */
		GPIO_Init(GPIOG,&GPIO_InitStructure);		
		GPIO_SetBits(GPIOG, GPIO_Pin_11);		
		
		/* (3) CH376T (仅豪华版), 片选为PF10 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出 */
		GPIO_Init(GPIOF,&GPIO_InitStructure);		
		GPIO_SetBits(GPIOF, GPIO_Pin_10);	
		
		/* (4) VS1003B(仅豪华版)，片选为PF9 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出 */
		GPIO_Init(GPIOF,&GPIO_InitStructure);		
		GPIO_SetBits(GPIOF, GPIO_Pin_9);	
	#endif	
}

void mDelay0_5uS( void )  /* 至少延时0.5uS,根据单片机主频调整 */
{
	UINT8 i;

	for (i = 0; i < 30; i++);
}

/* SPI输出8个位数据 */
void Spi376OutByte(UINT8 d)  
{  
	/* 如果是硬件SPI接口,应该是先将数据写入SPI数据寄存器,然后查询SPI状态寄存器以等待SPI字节传输完成 */
	UINT8	i;
	for ( i = 0; i < 8; i ++ ) 
	{
		CH376_SCK_0();
		if ( d & 0x80 )
		{
			CH376_SDI_1();
		}
		else
		{
			CH376_SDI_0();
		}
		d <<= 1;  /* 数据位是高位在前 */
		CH376_SCK_1();  /* CH376在时钟上升沿采样输入 */
	}
}

/* SPI输入8个位数据 */
UINT8 Spi376InByte( void )
{  /* 如果是硬件SPI接口,应该是先查询SPI状态寄存器以等待SPI字节传输完成,然后从SPI数据寄存器读出数据 */
	UINT8	i, d;
	d = 0;
	for ( i = 0; i < 8; i ++ ) 
	{
		CH376_SCK_0();  /* CH376在时钟下降沿输出 */
		d <<= 1;  /* 数据位是高位在前 */
		if ( CH376_SDO_HIGH() ) d ++;
		CH376_SCK_1();
	}
	return( d );
}

void xEndCH376Cmd( )
{
	 CH376_SCS_1();
}

void	xWriteCH376Cmd( UINT8 mCmd )  /* 向CH376写命令 */
{
	CH376_SCS_1();  /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
	mDelay0_5uS( );
	
	CH376_SCS_0();  /* SPI片选有效 */
	Spi376OutByte( mCmd );  /* 发出命令码 */
	
	/* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
	mDelay0_5uS( ); 
	mDelay0_5uS( ); 
	mDelay0_5uS( );  
}

void xWriteCH376Data( UINT8 mData )  /* 向CH376写数据 */
{
	Spi376OutByte( mData );
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
}

UINT8 xReadCH376Data( void )  /* 从CH376读数据 */
{
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
	return( Spi376InByte( ) );
}

/* 查询CH376中断(INT#低电平) */
UINT8	Query376Interrupt( void )
{
	return( CH376_INT_HIGH() ? FALSE : TRUE );
}

UINT8 mInitCH376Host( void )  /* 初始化CH376 */
{
	UINT8	res;
	
	CH376_PORT_INIT( );  /* 接口硬件初始化 */

	//mDelaymS(200);
	
	xWriteCH376Cmd( CMD11_CHECK_EXIST );  /* 测试单片机与CH376之间的通讯接口 */
	xWriteCH376Data( 0x65 );
	res = xReadCH376Data( );
	xEndCH376Cmd( );
	if ( res != 0x9A ) return( ERR_USB_UNKNOWN );  /* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
	xWriteCH376Data( 0x06 );
	mDelayuS(50 );
	res = xReadCH376Data( );
	xEndCH376Cmd( );

	if ( res == CMD_RET_SUCCESS ) return( USB_INT_SUCCESS );
	else return( ERR_USB_UNKNOWN );  /* 设置模式错误 */
}
