/*
*********************************************************************************************************
*	                                  
*	模块名称 : 内存管理层
*	文件名称 : memory.c
*	版    本 : V1.0
*	说    明 :  提供内存管理层接口函数。 
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-05-25 armfly  由ST原厂 V3.3.0版本的 memory.c 文件修改而来
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "memory.h"
#include "usb_scsi.h"
#include "usb_bot.h"
#include "usb_regs.h"
#include "usb_mem.h"
#include "usb_conf.h"
#include "usb_hw.h"
#include "mass_mal.h"
#include "usb_lib.h"

#define TXFR_IDLE     0
#define TXFR_ONGOING  1

__IO uint32_t Block_Read_count = 0;
__IO uint32_t Block_offset;
__IO uint32_t Counter = 0;
uint32_t  Idx;
uint32_t Data_Buffer[BULK_MAX_PACKET_SIZE * 8]; /* 512 bytes*/
static uint8_t TransferState = TXFR_IDLE;

extern uint8_t Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
extern uint16_t Data_Len;
extern uint8_t Bot_State;
extern Bulk_Only_CSW CSW;
extern uint32_t Mass_Memory_Size[2];
extern uint32_t Mass_Block_Size[2];

/*
*********************************************************************************************************
*	函 数 名: Read_Memory
*	功能说明: 读存储器数据
*	形    参：lun ： SCSI逻辑单元号，0表示SD卡，1表示NAND Flash
*			  Memory_Offset : 存储器偏移地址
*			  Transfer_Length : 数据长度（单位：字节）
*	返 回 值: 无
*********************************************************************************************************
*/
void Read_Memory(uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{
	static uint32_t Offset, Length;	/* 静态变量 */
	
	if (TransferState == TXFR_IDLE )	/* 传输第一帧时，保存偏移量 */
	{
		Offset = Memory_Offset * Mass_Block_Size[lun];
		Length = Transfer_Length * Mass_Block_Size[lun];
		TransferState = TXFR_ONGOING;
	}

	if (TransferState == TXFR_ONGOING )
	{
		if (!Block_Read_count)
		{
			MAL_Read(lun, Offset, Data_Buffer, Mass_Block_Size[lun]);
			
			USB_SIL_Write(EP1_IN, (uint8_t *)Data_Buffer, BULK_MAX_PACKET_SIZE);
			
			Block_Read_count = Mass_Block_Size[lun] - BULK_MAX_PACKET_SIZE;
			Block_offset = BULK_MAX_PACKET_SIZE;
		}
		else
		{
			USB_SIL_Write(EP1_IN, (uint8_t *)Data_Buffer + Block_offset, BULK_MAX_PACKET_SIZE);
			
			Block_Read_count -= BULK_MAX_PACKET_SIZE;
			Block_offset += BULK_MAX_PACKET_SIZE;
		}

	    SetEPTxCount(ENDP1, BULK_MAX_PACKET_SIZE);
	    SetEPTxStatus(ENDP1, EP_TX_VALID);

	    Offset += BULK_MAX_PACKET_SIZE;
	    Length -= BULK_MAX_PACKET_SIZE;
	
	    CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
	    Led_RW_ON();
	}
  
	if (Length == 0)	/* 传输完毕 */
	{
		Block_Read_count = 0;
		Block_offset = 0;
		Offset = 0;
		Bot_State = BOT_DATA_IN_LAST;
		TransferState = TXFR_IDLE;
		Led_RW_OFF();
	}
}

/*
*********************************************************************************************************
*	函 数 名: Write_Memory
*	功能说明: 写存储器数据
*	形    参：lun ： SCSI逻辑单元号，0表示SD卡，1表示NAND Flash
*			  Memory_Offset : 存储器偏移地址
*			  Transfer_Length : 数据长度（单位：字节）
*	返 回 值: 无
*********************************************************************************************************
*/
void Write_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{

	static uint32_t W_Offset, W_Length;
	
	uint32_t temp =  Counter + 64;
	
	if (TransferState == TXFR_IDLE )		/* 传输第一帧时，保存偏移量 */
	{
		W_Offset = Memory_Offset * Mass_Block_Size[lun];
		W_Length = Transfer_Length * Mass_Block_Size[lun];
		TransferState = TXFR_ONGOING;
	}
	
	if (TransferState == TXFR_ONGOING )
	{
		for (Idx = 0 ; Counter < temp; Counter++)
		{
			*((uint8_t *)Data_Buffer + Counter) = Bulk_Data_Buff[Idx++];
		}
		
		W_Offset += Data_Len;
		W_Length -= Data_Len;
		
		if (!(W_Length % Mass_Block_Size[lun]))
		{
			Counter = 0;
			MAL_Write(lun,W_Offset - Mass_Block_Size[lun], Data_Buffer, Mass_Block_Size[lun]);
		}
		
		CSW.dDataResidue -= Data_Len;
		SetEPRxStatus(ENDP2, EP_RX_VALID); /* 使能下次传输 */   
		
		Led_RW_ON();
	}
	
	if ((W_Length == 0) || (Bot_State == BOT_CSW_Send))		/* 传输结束 */
	{
		Counter = 0;
		Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
		TransferState = TXFR_IDLE;
		
		Led_RW_OFF();
	}
}

