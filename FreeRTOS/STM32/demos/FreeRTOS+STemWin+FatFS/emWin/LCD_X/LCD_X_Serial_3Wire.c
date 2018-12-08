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
File        : LCD_X_SERIAL_3Wire.c
Purpose     : Port routines
----------------------------------------------------------------------
*/

#ifndef LCD_DELAY
  #define LCD_DELAY(x)
#endif

#ifndef LCD_SET_CS
  #define LCD_SET_CS()
  #define LCD_CLR_CS()
#endif

/*********************************************************************
*
*           Hardware configuration
*
**********************************************************************

  Needs to be adapted to your target hardware.
*/

/* Configuration example:

#define LCD_CLR_RESET()   P7 &= ~(1<<2)
#define LCD_SET_RESET()   P7 |=  (1<<2) 
#define LCD_CLR_CLK()     P7 &= ~(1<<3)
#define LCD_SET_CLK()     P7 |=  (1<<3) 
#define LCD_CLR_DATA()    P7 &= ~(1<<4)
#define LCD_SET_DATA()    P7 |=  (1<<4) 
#define LCD_SET_DIR_OUT() P7D = 0xff
#define LCD_CLR_CS()      P7 &= ~(1<<0)      // Optional
#define LCD_SET_CS()      P7 |=  (1<<0)      // Optional
#define LCD_DELAY(ms)     GUI_Delay(ms)      // Optional

*/

/*********************************************************************
*
*           Initialisation
*
**********************************************************************
  This routine should be called from your application program
  to set port pins to their initial values
*/

void LCD_X_Init(void) {
  LCD_CLR_RESET();
  LCD_SET_DIR_OUT();
  LCD_SET_CS();
  LCD_SET_RESET();
  LCD_SET_CLK();
  LCD_SET_DATA();
  LCD_DELAY(20);
  LCD_SET_RESET();
  LCD_DELAY(20);
}

/*********************************************************************
*
*           Send1 routine
*
**********************************************************************

  Usually, there is no need to modify this routine.
  It should be sufficient ot modify the low-level macros above or
  in a configuration file.
*/

/* Write 1 byte, MSB first */
static void Send1(unsigned char Data, unsigned char IsData) {
  if (IsData)          LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 7) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 6) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 5) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 4) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 3) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 2) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 1) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 0) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
}

/*********************************************************************
*
*           Controller 0
*
**********************************************************************

*/

/* Write to controller, with A0 = 1 */
void LCD_X_Write01(unsigned char Data) {
  LCD_CLR_CS();
  Send1(Data, 1);
  LCD_SET_CS();
}

/* Write multiple bytes to controller, with A0 = 1 */
void LCD_X_WriteM01(unsigned char * pData, int NumBytes) {
  LCD_CLR_CS();
  for (; NumBytes; NumBytes--) {
    Send1(*pData++, 1);
  }
  LCD_SET_CS();
}

/* Write to controller, with A0 = 0 */
void LCD_X_Write00(unsigned char Cmd) {
  LCD_CLR_CS();
  Send1(Cmd, 0);
  LCD_SET_CS();
}
