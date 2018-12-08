/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.30 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information

Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : LCD_X_8080_16.h
Purpose     : Interface definition:
              Port routines 8080 interface, 16 bit data bus
----------------------------------------------------------------------
*/

#ifndef LCD_X_8080_16_H
#define LCD_X_8080_16_H

/*********************************************************************
*
*       Public routines
*/
void LCD_X_8080_16_Init       (void);
void LCD_X_8080_16_Write00_16 (U16 c);
void LCD_X_8080_16_Write01_16 (U16 c);
void LCD_X_8080_16_WriteM01_16(U16 * pData, int NumWords);
void LCD_X_8080_16_WriteM00_16(U16 * pData, int NumWords);
U16  LCD_X_8080_16_Read00_16  (void);
U16  LCD_X_8080_16_Read01_16  (void);
void LCD_X_8080_16_ReadM00_16 (U16 * pData, int NumWords);
void LCD_X_8080_16_ReadM01_16 (U16 * pData, int NumWords);

#endif /* LCD_X_8080_16_H */

/*************************** End of file ****************************/
