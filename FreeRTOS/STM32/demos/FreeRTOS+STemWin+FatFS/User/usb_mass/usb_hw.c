/*
*********************************************************************************************************
*	                                  
*	ģ������ : USB Mass������ص�Ӳ�����á�
*	�ļ����� : usb_hw.c
*	��    �� : V1.0
*	˵    �� : USBӲ���豸�����ӿ�
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-05-13 armfly  ST�̼���V3.5.0�汾��
*		V1.1	2015-07-26 armfly  ɾ��LEDָʾ�Ƶ�������
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include "bsp_led.h"
#include "usb_hw.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_lib.h"
#include "mass_mal.h"
#include "usb_istr.h"

static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
static void USB_Disconnect_Config(void);
static void Set_USBClock(void);
static void USB_Interrupts_Config(void);

/*
*********************************************************************************************************
*	�� �� ��: MAL_Config
*	����˵��: MAL������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MAL_Config(void)
{
	MAL_Init(0);	/* ����SD�� */
	MAL_Init(1);	/* ����NAND Flash */
}

/*
*********************************************************************************************************
*	�� �� ��: InitUsbDev
*	����˵��: ��ʼ��USB��ص�Ӳ���豸
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitUsbDev(void)
{	
	USB_Disconnect_Config();/* ���ÿ���USB���ӺͶϿ���GPIO */

	MAL_Config();			/* MAL configuration */

	Set_USBClock();			/* ����USBʱ�� */
	USB_Interrupts_Config();/* ����USB�ж� */
	USB_Init();				/* ��ʼ��USB�豸, �˺��������UsbConnect()����USB�豸��USB���� */
}

/*
*********************************************************************************************************
*	�� �� ��: Led_RW_ON
*	����˵��: ������дָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Led_RW_ON(void)
{
	//bsp_LedOn(3);
}

/*
*********************************************************************************************************
*	�� �� ��: Led_RW_ON
*	����˵��: �رն�дָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Led_RW_OFF(void)
{
	//bsp_LedOff(3);
}

/*
*********************************************************************************************************
*	�� �� ��: USB_Configured_LED
*	����˵��: ����USB�豸���ӳɹ�ָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USB_Configured_LED(void)
{
	//bsp_LedOn(1);
}

/*
*********************************************************************************************************
*	�� �� ��: USB_NotConfigured_LED
*	����˵��: �ر�USB�豸���ӳɹ�ָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USB_NotConfigured_LED(void)
{
	//bsp_LedOff(1);
}

/*
*********************************************************************************************************
*	�� �� ��: Get_SerialNum
*	����˵��: �������ںź�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
void Get_SerialNum(void)
{
	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
	
	Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
	Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
	Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);
	
	Device_Serial0 += Device_Serial2;
	
	if (Device_Serial0 != 0)
	{
		IntToUnicode (Device_Serial0, &MASS_StringSerial[2] , 8);
		IntToUnicode (Device_Serial1, &MASS_StringSerial[18], 4);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: USB_Cable_Config
*	����˵��: �������USB�������ӻ�Ͽ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USB_Cable_Config (uint8_t NewState)
{
	/* ������������ʹ��PB14����USB���������������衣PB14=0��USB�������ӵ����� */
	if (NewState != DISABLE)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Enter_LowPowerMode
*	����˵��: ���������ģʽʱ�ر�ϵͳʱ�Ӻ͵�Դ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Enter_LowPowerMode(void)
{
	/* �����豸״̬Ϊ������ */
	bDeviceState = SUSPENDED;
}

/*
*********************************************************************************************************
*	�� �� ��: Leave_LowPowerMode
*	����˵��: ���˳�����ģʽʱ�ָ�ϵͳʱ�Ӻ͵�Դ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Leave_LowPowerMode(void)
{	
	DEVICE_INFO *pInfo = &Device_Info;
	
	/* �����豸״̬ */
	if (pInfo->Current_Configuration != 0)
	{
		/* �豸���ӵ����������óɹ� */
		bDeviceState = CONFIGURED;
	}
	else
	{
		bDeviceState = ATTACHED;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: USB_Disconnect_Config
*	����˵��: ���ÿ���USB���ӺͶϿ���GPIO
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void USB_Disconnect_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	/* ʹ��USB disconnect GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	USB_Cable_Config(DISABLE);	/* �Ͽ�USB���ߵ��������� */ 
		
	/* ����PB14Ϊ���������PB14 = 0ʱ��USB�豸���ӣ�PB14=1ʱ��USB�豸�Ͽ� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

/*
*********************************************************************************************************
*	�� �� ��: Set_USBClock
*	����˵��: ����USB����ʱ�ӣ�48MHz��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Set_USBClock(void)
{
	/* ѡ��USBCLKʱ��Դ */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	
	/* ʹ��USBʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: USB_Interrupts_Config
*	����˵��: ����USB�ж�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void USB_Interrupts_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  	/* ����SDIO�ж� */
  	{		  
		NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: IntToUnicode
*	����˵��: ��һ��32λ��HEXֵת��ΪUnicode�����ʽ���ַ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
	uint8_t idx = 0;
	
	for( idx = 0 ; idx < len ; idx ++)
	{
		if( ((value >> 28)) < 0xA )
		{
			pbuf[ 2* idx] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2* idx] = (value >> 28) + 'A' - 10; 
		}
		
		value = value << 4;
		
		pbuf[ 2* idx + 1] = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: USB_HP_CAN1_TX_IRQHandler
*	����˵��: USB�����ȼ��жϻ���CAN TX�жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void USB_HP_CAN1_TX_IRQHandler(void)
{
	CTR_HP();
}

/*
*********************************************************************************************************
*	�� �� ��: USB_LP_CAN1_RX0_IRQHandler
*	����˵��: USB�����ȼ��жϻ�CAN RX0�ж�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
