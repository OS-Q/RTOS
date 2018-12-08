/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : mass_mal.h
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Header for mass_mal.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MASS_MAL_H
#define __MASS_MAL_H

#define MASS_PRINTF_OK_EN	0		/* 1表示打印执行成功的调试信息 */
#define MASS_PRINTF_ERR_EN	1		/* 1表示打印执行失败的调试信息 */


#if MASS_PRINTF_EN == 1
	#define mass_printf_ok(...)	printf(__VA_ARGS__)
#else
	#define mass_printf_ok(...)
#endif	

#if MASS_PRINTF_ERR_EN == 1
	#define mass_printf_err(...)	printf(__VA_ARGS__)
#else
	#define mass_printf_err(...)
#endif	


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MAL_OK   0
#define MAL_FAIL 1

/* 定义大容量存储设备ID */
typedef enum _MASS_ID
{
	MASS_SD = 0,
	MASS_NAND = 1,
}MASS_ID_E;

/* LUN是Logical Unit Number的简写，即逻辑单元号，内部逻辑单元的SCSI标识符 */
extern uint32_t Max_Lun;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint16_t MAL_Init (uint8_t lun);
uint16_t MAL_GetStatus (uint8_t lun);
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length);
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length);
#endif /* __MASS_MAL_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
