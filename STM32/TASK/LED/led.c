/******************************************************************************
* @ File name --> led.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 10 - 31 - 2013
* @ Brief     --> LED控制相关函数
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.
* @ Author    -->
* @ Date      -->
* @ Revise    -->
*
******************************************************************************/

#include "led.h"

/******************************************************************************
* Function Name --> LED接口初始化
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void LED_Init(void)
{
	STM32_GPIOx_Init(LED0_Init);
	STM32_GPIOx_Init(LED1_Init);
	LED0 = 1;
	LED1 = 1;
}





