/*
*********************************************************************************************************
*	                                  
*	模块名称 : 大容量存储中间层模块  MAL = Middle Access Layer
*	文件名称 : mass_mal.c
*	版    本 : V1.0
*	说    明 :  提供大容量存储中间层接口函数。 实现USB Mass Storage不需要文件系统支持，文件分区表和文件的
*			访问完全由windows系统控制，板子只需要应答USB Mass相关的命令（读写扇区）即可。
*			FAT文件系统（含Fat32）扇区大小为512字节。开发板实际使用的Nand Flash扇区大小为2048字节，因
*			此需要做封装变换处理。
*				
*			宏定义  mass_printf_err() 用于打印底层读写存储设备函数执行过程，主要用于跟踪调试。
*			修改 mass_mal.h文件可以取消或使能调试打印功能
*				#define  mass_printf_err_EN	1     1表示打印调试信息使能，0禁止
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-05-25 armfly  由ST原厂 V3.3.0版本的mass_mal.c文件修改而来
*		V1.1	2015-07-26 armfly  询问容量时，返回格式化后的容量，而不是物理容量. 该驱动只支持4G内的卡。
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_sdio_sd.h"
#include "bsp_nand_flash.h"
#include "mass_mal.h"

uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];
//__IO uint32_t Status = 0;

uint32_t Max_Lun = 1;	/* 0 表示SD卡， 1表示NAND Flash */

/*
*********************************************************************************************************
*	函 数 名: MAL_Init
*	功能说明: 初始化存储设备（用于USB Mass Storage）
*	形    参：lun ： SCSI逻辑单元号，0表示SD卡，1表示NAND Flash
*	返 回 值: MAL_OK : 成功；MAL_FAIL : 失败
*********************************************************************************************************
*/
uint16_t MAL_Init(uint8_t lun)
{
	uint16_t status = MAL_OK;
	
	switch (lun)
	{
		case MASS_SD:
			status = SD_Init();
			if (status != SD_OK)		/* 初始化SDIO硬件设备（配置时钟、GPIO、中断并启动SD卡) */
			{
				mass_printf_err("SD_Init() fail (%d) : file %s on line %d\r\n", status, __FILE__, __LINE__);		
				status = MAL_FAIL;
			}
			else
			{
				mass_printf_ok("SD_Init() Ok\r\n");
				status = MAL_OK;			
			}
			break;
		
		case MASS_NAND:
			if (NAND_Init() != NAND_OK)	/* 初始化NAND Flash 硬件设备（配置时钟、FSMC、GPIO并使能FSMC */
			{
				mass_printf_err("NAND_Init() fail : file %s on line %d\r\n", __FILE__, __LINE__);			
				status = MAL_FAIL;
			}
			else
			{
				mass_printf_ok("NAND_Init() Ok\r\n");
				status = MAL_OK;
			}
			break;
		
		default:
			break;
	}
	
	return status;
}

/*
*********************************************************************************************************
*	函 数 名: MAL_Write
*	功能说明: 写扇区
*	形    参：lun ： SCSI逻辑单元号，0表示SD卡，1表示NAND Flash
*			  Memory_Offset : 存储单元偏移地址
*			  Writebuff     ：待写入的数据缓冲区的指针
*			  Transfer_Length ：待写入的字节数， 不大于 512
*	返 回 值: MAL_OK : 成功；MAL_FAIL : 失败
*********************************************************************************************************
*/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{
	uint16_t status = MAL_OK;
		
	switch (lun)
	{
		case MASS_SD:
			status = SD_WriteBlock((uint8_t*)Writebuff, Memory_Offset, Transfer_Length);
			if (status != SD_OK)
			{
				mass_printf_err("SD_WriteBlock(, 0x%X, 0x%X) Fail(%d) \r\n", Memory_Offset, Transfer_Length, status);				
				status = MAL_FAIL;
			}
			else
			{
				mass_printf_ok("SD_WriteBlock(, 0x%X, 0x%X) Ok\r\n", Memory_Offset, Transfer_Length);			
			}
			break;
		
		case MASS_NAND:
			if (NAND_Write(Memory_Offset, Writebuff, Transfer_Length) != NAND_OK)
			{
				mass_printf_err("NAND_Write(0x%X, ,0x%X) Fail\r\n", Memory_Offset, Transfer_Length);
				status = MAL_FAIL;
			}
			else
			{
				mass_printf_ok("NAND_Write(0x%X, ,0x%X) Ok\r\n", Memory_Offset, Transfer_Length);			
				status = MAL_OK;
			}
			break;
		
		default:
			break;
	}
	return status;
}

/*
*********************************************************************************************************
*	函 数 名: MAL_Read
*	功能说明: 读扇区，支持多个扇区
*	形    参：lun ： SCSI逻辑单元号，0表示SD卡，1表示NAND Flash
*			  Memory_Offset : 存储单元偏移地址
*			  Readbuff      ：存储读出的数据的缓冲区的指针
*			  Transfer_Length ：需要读出的字节数， 不大于 512
*	返 回 值: MAL_OK : 成功；MAL_FAIL : 失败
*********************************************************************************************************
*/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{
	uint16_t status = MAL_OK;
	
	switch (lun)
	{
		case MASS_SD:
			status = SD_ReadBlock((uint8_t*)Readbuff, Memory_Offset, Transfer_Length);
			if (status != SD_OK)
			{
				mass_printf_err("SD_ReadBlock(, 0x%X, 0x%X) Fail(%d) \r\n", Memory_Offset, Transfer_Length, status);
				status = MAL_FAIL;
			}
			else
			{
				mass_printf_ok("SD_ReadBlock(, 0x%X, 0x%X) Ok\r\n", Memory_Offset, Transfer_Length);			
				status = MAL_OK;
			}			
			break;
	
		case MASS_NAND:
			if (NAND_Read(Memory_Offset, Readbuff, Transfer_Length) != NAND_OK)
			{
				mass_printf_err("NAND_Read(0x%X, ,0x%X) Fail\r\n", Memory_Offset, Transfer_Length);
				status = MAL_FAIL;
			}
			else
			{
				mass_printf_ok("NAND_Read(0x%X, ,0x%X) Ok\r\n", Memory_Offset, Transfer_Length);			
				status = MAL_OK;
			}			
			break;
			
		default:
			break;
	}
	return status;
}

/*
*********************************************************************************************************
*	函 数 名: MAL_GetStatus
*	功能说明: 读取存储设备的状态信息
*	形    参：lun ： SCSI逻辑单元号，0表示SD卡，1表示NAND Flash
*	返 回 值: MAL_OK : 成功；MAL_FAIL : 失败
*********************************************************************************************************
*/
uint16_t MAL_GetStatus (uint8_t lun)
{
	SD_CardInfo mSDCardInfo;	/* 定义SD卡状态信息结构体 */
	uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;	
	uint32_t nand_id;			/* 保存NAND ID */
	uint16_t status = MAL_OK;
	
	switch (lun)
	{
		case MASS_SD:
		{				
			status = SD_Init();
			if (status != SD_OK)	
			{
				mass_printf_err("SD_Init() fail (%d) : file %s on line %d\r\n", status, __FILE__, __LINE__);
				status = MAL_FAIL;
				break;
			}

			SD_GetCardInfo(&mSDCardInfo);	/* 读取SD卡的信息 */
			SD_SelectDeselect((uint32_t) (mSDCardInfo.RCA << 16));
			DeviceSizeMul = (mSDCardInfo.SD_csd.DeviceSizeMul + 2);
			
			if (mSDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)	/* 高容量SD卡 SDHC */
			{
				Mass_Block_Count[0] = (mSDCardInfo.SD_csd.DeviceSize + 1) * 1024;
				mass_printf_ok("SD_GetCardInfo() Ok, SDHC Card\r\n");
			}
			else	/* 普通SD卡, 最大4G */
			{
				NumberOfBlocks  = ((1 << (mSDCardInfo.SD_csd.RdBlockLen)) / 512);
				Mass_Block_Count[0] = ((mSDCardInfo.SD_csd.DeviceSize + 1) * (1 << DeviceSizeMul) << (NumberOfBlocks/2));
				mass_printf_ok("SD_GetCardInfo() Ok, Normal SD Card\r\n");
			}
			Mass_Block_Size[0]  = 512;
			
			status = SD_SelectDeselect((uint32_t) (mSDCardInfo.RCA << 16)); 
			status = SD_EnableWideBusOperation(SDIO_BusWide_4b); 
			if (status != SD_OK)
			{
				mass_printf_err("SD_EnableWideBusOperation(SDIO_BusWide_4b) Fail (%d)\r\n", status);
				status = MAL_FAIL;
				break;
			}
			
			status = SD_SetDeviceMode(SD_DMA_MODE);    /* 设置SD卡工作模式为DMA, 其它模式由中断、轮询 */     
			if (status != SD_OK)
			{
				mass_printf_err("SD_SetDeviceMode(SD_DMA_MODE) Fail (%d)\r\n", status);				
				status = MAL_FAIL;
				break;
			} 
			
			Mass_Memory_Size[0] = Mass_Block_Count[0] * Mass_Block_Size[0];
			
			mass_printf_ok("MAL_GetStatus(MASS_SD) Ok. Memory Size = %uMB\r\n", Mass_Memory_Size[0]/(1024*1024));		
			status = MAL_OK;
			break;
		}
		
		case MASS_NAND:
		{	
			nand_id = NAND_ReadID();	/* 读取NAND Flash的制造商ID和器件ID */

			/* 判断NAND_ID是否正确 */
			if ((nand_id == HY27UF081G2A) || (nand_id == K9F1G08U0A) || (nand_id == K9F1G08U0B))
			{
				/*  对于HY27UF081G2A, 1个区，每区1024块，每块64个页，每页2048字节
					 Mass块数量 Mass_Block_Count = 1024 * 64 * 1 = 64K 个
					 Mass块大小 Mass_Block_Size = 2048 字节
					 整个存储器大小 Mass_Memory_Size = 64K * 2048 = 128M 字节
				*/
			#if 1
				Mass_Block_Count[1] = NAND_FormatCapacity()/512;		/* 必须为可用的扇区个数，不是芯片的理论容量 */
				Mass_Block_Size[1]  = 512;
				Mass_Memory_Size[1] = (Mass_Block_Count[1] * Mass_Block_Size[1]);
			#else
				Mass_Block_Count[1] = NAND_ZONE_SIZE * NAND_BLOCK_SIZE * NAND_MAX_ZONE;
				Mass_Block_Size[1]  = NAND_PAGE_SIZE;
				Mass_Memory_Size[1] = (Mass_Block_Count[1] * Mass_Block_Size[1]);
			#endif	
				
				mass_printf_ok("MAL_GetStatus(MASS_NAND) Ok. Memory Size = %uMB\r\n", Mass_Memory_Size[1]/(1024*1024));
				status = MAL_OK;
			}
			else
			{				
				mass_printf_err("MAL_GetStatus(MASS_NAND) Fail\r\n");					
				status = MAL_FAIL;
			}			
			break;
		}
			
		default:
			status = MAL_FAIL;
			break;			
	}
	return status;
}
