/*
*********************************************************************************************************
*	                                  
*	模块名称 : USB鼠标主程序。
*	文件名称 : usb_mouse.h
*	版    本 : V1.0
*	说    明 : 通过开发板的摇杆模拟USB鼠标。
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-05-13 armfly  ST固件库V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __USB_MOUSE_H
#define __USB_MOUSE_H

#include "usb_type.h"

#define CURSOR_STEP     5	/* 每按一次摇杆方向键时，鼠标指针移动多少个像素（） */

void InitUsbMouse(void);
void RunUsbMouse(void);
void UsbConnect(void);
void UsbDisconnect(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void Get_SerialNum(void);

#endif
