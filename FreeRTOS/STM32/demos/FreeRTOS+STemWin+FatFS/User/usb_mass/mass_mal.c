/*
*********************************************************************************************************
*	                                  
*	ģ������ : �������洢�м��ģ��  MAL = Middle Access Layer
*	�ļ����� : mass_mal.c
*	��    �� : V1.0
*	˵    �� :  �ṩ�������洢�м��ӿں����� ʵ��USB Mass Storage����Ҫ�ļ�ϵͳ֧�֣��ļ���������ļ���
*			������ȫ��windowsϵͳ���ƣ�����ֻ��ҪӦ��USB Mass��ص������д���������ɡ�
*			FAT�ļ�ϵͳ����Fat32��������СΪ512�ֽڡ�������ʵ��ʹ�õ�Nand Flash������СΪ2048�ֽڣ���
*			����Ҫ����װ�任����
*				
*			�궨��  mass_printf_err() ���ڴ�ӡ�ײ��д�洢�豸����ִ�й��̣���Ҫ���ڸ��ٵ��ԡ�
*			�޸� mass_mal.h�ļ�����ȡ����ʹ�ܵ��Դ�ӡ����
*				#define  mass_printf_err_EN	1     1��ʾ��ӡ������Ϣʹ�ܣ�0��ֹ
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-05-25 armfly  ��STԭ�� V3.3.0�汾��mass_mal.c�ļ��޸Ķ���
*		V1.1	2015-07-26 armfly  ѯ������ʱ�����ظ�ʽ�������������������������. ������ֻ֧��4G�ڵĿ���
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
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

uint32_t Max_Lun = 1;	/* 0 ��ʾSD���� 1��ʾNAND Flash */

/*
*********************************************************************************************************
*	�� �� ��: MAL_Init
*	����˵��: ��ʼ���洢�豸������USB Mass Storage��
*	��    �Σ�lun �� SCSI�߼���Ԫ�ţ�0��ʾSD����1��ʾNAND Flash
*	�� �� ֵ: MAL_OK : �ɹ���MAL_FAIL : ʧ��
*********************************************************************************************************
*/
uint16_t MAL_Init(uint8_t lun)
{
	uint16_t status = MAL_OK;
	
	switch (lun)
	{
		case MASS_SD:
			status = SD_Init();
			if (status != SD_OK)		/* ��ʼ��SDIOӲ���豸������ʱ�ӡ�GPIO���жϲ�����SD��) */
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
			if (NAND_Init() != NAND_OK)	/* ��ʼ��NAND Flash Ӳ���豸������ʱ�ӡ�FSMC��GPIO��ʹ��FSMC */
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
*	�� �� ��: MAL_Write
*	����˵��: д����
*	��    �Σ�lun �� SCSI�߼���Ԫ�ţ�0��ʾSD����1��ʾNAND Flash
*			  Memory_Offset : �洢��Ԫƫ�Ƶ�ַ
*			  Writebuff     ����д������ݻ�������ָ��
*			  Transfer_Length ����д����ֽ����� ������ 512
*	�� �� ֵ: MAL_OK : �ɹ���MAL_FAIL : ʧ��
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
*	�� �� ��: MAL_Read
*	����˵��: ��������֧�ֶ������
*	��    �Σ�lun �� SCSI�߼���Ԫ�ţ�0��ʾSD����1��ʾNAND Flash
*			  Memory_Offset : �洢��Ԫƫ�Ƶ�ַ
*			  Readbuff      ���洢���������ݵĻ�������ָ��
*			  Transfer_Length ����Ҫ�������ֽ����� ������ 512
*	�� �� ֵ: MAL_OK : �ɹ���MAL_FAIL : ʧ��
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
*	�� �� ��: MAL_GetStatus
*	����˵��: ��ȡ�洢�豸��״̬��Ϣ
*	��    �Σ�lun �� SCSI�߼���Ԫ�ţ�0��ʾSD����1��ʾNAND Flash
*	�� �� ֵ: MAL_OK : �ɹ���MAL_FAIL : ʧ��
*********************************************************************************************************
*/
uint16_t MAL_GetStatus (uint8_t lun)
{
	SD_CardInfo mSDCardInfo;	/* ����SD��״̬��Ϣ�ṹ�� */
	uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;	
	uint32_t nand_id;			/* ����NAND ID */
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

			SD_GetCardInfo(&mSDCardInfo);	/* ��ȡSD������Ϣ */
			SD_SelectDeselect((uint32_t) (mSDCardInfo.RCA << 16));
			DeviceSizeMul = (mSDCardInfo.SD_csd.DeviceSizeMul + 2);
			
			if (mSDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)	/* ������SD�� SDHC */
			{
				Mass_Block_Count[0] = (mSDCardInfo.SD_csd.DeviceSize + 1) * 1024;
				mass_printf_ok("SD_GetCardInfo() Ok, SDHC Card\r\n");
			}
			else	/* ��ͨSD��, ���4G */
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
			
			status = SD_SetDeviceMode(SD_DMA_MODE);    /* ����SD������ģʽΪDMA, ����ģʽ���жϡ���ѯ */     
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
			nand_id = NAND_ReadID();	/* ��ȡNAND Flash��������ID������ID */

			/* �ж�NAND_ID�Ƿ���ȷ */
			if ((nand_id == HY27UF081G2A) || (nand_id == K9F1G08U0A) || (nand_id == K9F1G08U0B))
			{
				/*  ����HY27UF081G2A, 1������ÿ��1024�飬ÿ��64��ҳ��ÿҳ2048�ֽ�
					 Mass������ Mass_Block_Count = 1024 * 64 * 1 = 64K ��
					 Mass���С Mass_Block_Size = 2048 �ֽ�
					 �����洢����С Mass_Memory_Size = 64K * 2048 = 128M �ֽ�
				*/
			#if 1
				Mass_Block_Count[1] = NAND_FormatCapacity()/512;		/* ����Ϊ���õ���������������оƬ���������� */
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
