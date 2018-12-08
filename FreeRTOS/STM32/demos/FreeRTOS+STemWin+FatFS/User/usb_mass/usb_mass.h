/*
*********************************************************************************************************
*	                                  
*	ģ������ : USB���������
*	�ļ����� : usb_mouse.h
*	��    �� : V1.0
*	˵    �� : ͨ���������ҡ��ģ��USB��ꡣ
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-05-13 armfly  ST�̼���V3.5.0�汾��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __USB_MOUSE_H
#define __USB_MOUSE_H

#include "usb_type.h"

#define CURSOR_STEP     5	/* ÿ��һ��ҡ�˷����ʱ�����ָ���ƶ����ٸ����أ��� */

void InitUsbMouse(void);
void RunUsbMouse(void);
void UsbConnect(void);
void UsbDisconnect(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void Get_SerialNum(void);

#endif
