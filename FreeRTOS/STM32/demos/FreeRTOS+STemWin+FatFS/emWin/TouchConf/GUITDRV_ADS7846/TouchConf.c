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
File        : TouchConf.c
Purpose     : Touch screen hardware access routines and driver
              configuration for Texas Instruments ADS7846 touch
              screen controller.
---------------------------END-OF-HEADER------------------------------
*/

#include "RTOS.h"
#include "GUI_Type.h"
#include "GUITDRV_ADS7846.h"

/*********************************************************************
*
*       Defines, sfrs and bits
*
**********************************************************************
*/

//
// PORT2
//
#define P2DDR  (*(volatile U8*)(0xFFFFFB81))  // Port 2 data direction register
#define P2ICR  (*(volatile U8*)(0xFFFFFB91))  // Port 2 input buffer control register
#define P2DR   (*(volatile U8*)(0xFFFFFF51))  // Port 2 data register
#define PORT2  (*(volatile U8*)(0xFFFFFF41))  // Port 2 register

//
// Touch screen
//
#define BUSY_BIT    0  // P20
#define CS_BIT      1  // P21
#define PENIRQ_BIT  3  // P23
#define CLK_BIT     4  // P24
#define DI_BIT      5  // P25
#define DO_BIT      6  // P26

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/

#define CS_SET()      P2DR  &= ~(1 << CS_BIT)
#define CS_CLR()      P2DR  |=  (1 << CS_BIT)
#define DO_SET()      P2DR  |=  (1 << DO_BIT)
#define DO_CLR()      P2DR  &= ~(1 << DO_BIT)
#define CLK_SET()     {OS_Delay(1); P2DR |=  (1 << CLK_BIT);}
#define CLK_CLR()     {OS_Delay(1); P2DR &= ~(1 << CLK_BIT);}
#define DI_VAL()      PORT2 &   (1 << DI_BIT)
#define BUSY_VAL()    PORT2 &   (1 << BUSY_BIT)
#define PENIRQ_VAL()  PORT2 &   (1 << PENIRQ_BIT)

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitPorts
*
* Purpose:
*   Initialize ports to peripheral and set to a good state.
*/
void _InitPorts(void) {
  U8 v;

  //
  // Setup port direction
  //
  P2DDR = 0
          | (0 << 0)  // P20: 0; Input for touch busy line
          | (1 << 1)  // P21: 1; Output for touch chip select
          | (1 << 4)  // P24: 1; Output for touch clock
          | (0 << 5)  // P25: 0; Input for touch data in
          | (1 << 6)  // P26: 1; Output for touch data out
          ;
  //
  // Set output ports to a good state
  //
  v     = P2DR;
  v    |=  (1 << 1);  // Clr touch CS
  v    &= ~(1 << 4);  // Clr touch CLK
  P2DR  = v;
}

/*********************************************************************
*
*       _SendCmd
*
* Purpose:
*   Write 8 bit command to peripheral.
*/
void _SendCmd(U8 Data) {
  int i;

  for (i = 0; i < 8; i++) {
    //
    // Output next bit
    //
    if (Data & (1 << 7)) {
      DO_SET();
    } else {
      DO_CLR();
    }
    //
    // Output one clock and prepare next bit
    //
    CLK_SET();
    CLK_CLR();
    Data <<= 1;
  }
}

/*********************************************************************
*
*       _GetResult
*
* Purpose:
*   Read 16 bit result from peripheral.
*
* Return value:
*   16 bit AD value.
*/
U16 _GetResult(void) {
  U16 r;
  int i;
  U8  v;

  for (i = 0; i < 16; i++) {
    r <<= 1;
    if (DI_VAL()) {
      r |= 1;
    }
    CLK_SET();
    CLK_CLR();
  }
  r >>= 4;  // Remove padding bytes
  return r;
}

/*********************************************************************
*
*       _GetBusy
*
* Purpose:
*   Checks the busy line of the peripheral.
*
* Return value:
*   0 = Peripheral ready
*   1 = Peripheral busy
*/
char _GetBusy(void) {
  //
  // Output one clock as the busy line will only get low when
  // providing a clock. After one clock the output data is ready
  // thus needing one clock less in the _GetResult() routine. The
  // _GetResult() routine can output more clocks than necessary as
  // they are ignored.
  //
  CLK_SET();
  CLK_CLR();
  //
  // Check busy line just to be sure that the peripheral is ready and data
  // has been already shifted out with the last clock.
  //
  if (BUSY_VAL()) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       _GetPENIRQ
*
* Purpose:
*   Checks the PENIRQ line of the peripheral for a touch event.
*
* Return value:
*   0 = No touch event
*   1 = Touch can be sampled
*/
char _GetPENIRQ(void) {
  if (PENIRQ_VAL()) {
    return 0;
  }
  return 1;
}

/*********************************************************************
*
*       _SetCS
*
* Purpose:
*   Sets the chip select line of the peripheral.
*/
void _SetCS(char OnOff) {
  if (OnOff) {
    CS_SET();
  } else {
    CS_CLR();
  }
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       TOUCH_X_Init
*
* Purpose:
*   Fill in the touch API routines and initalize the touch screen driver.
*/
void TOUCH_X_Init(void);
void TOUCH_X_Init(void) {
  GUITDRV_ADS7846_CONFIG Config = {0};

  Config.pfSendCmd   = _SendCmd;
  Config.pfGetResult = _GetResult;
  Config.pfGetBusy   = _GetBusy;
  Config.pfGetPENIRQ = _GetPENIRQ;
  Config.pfSetCS     = _SetCS;
  Config.xLog0       = 0;
  Config.xLog1       = 639;
  Config.xPhys0      = 0x0053;
  Config.xPhys1      = 0x0F51;
  Config.yLog0       = 0;
  Config.yLog1       = 479;
  Config.yPhys0      = 0x0F5E;
  Config.yPhys1      = 0x0083;
  GUITDRV_ADS7846_Config(&Config);
  _InitPorts();
}

/*************************** End of file ****************************/
