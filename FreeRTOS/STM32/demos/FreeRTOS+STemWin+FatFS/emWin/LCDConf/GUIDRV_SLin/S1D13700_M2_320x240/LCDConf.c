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
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUIDRV_SLin.h"

/*********************************************************************
*
*       Configuration
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS 320
#define YSIZE_PHYS 240

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_2

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_SLIN_2

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _Write0
*
* Purpose:
*   Write byte to controller, with A0 = 0
*/
static void _Write0(unsigned char c) {
  // TBD by customer
}

/*********************************************************************
*
*       _Write1
*
* Purpose:
*   Write byte to controller, with A0 = 1
*/
static void _Write1(unsigned char c) {
  // TBD by customer
}

/*********************************************************************
*
*       _WriteM0
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 0
*/
static void _WriteM0(unsigned char * pData, int NumItems) {
  do {
    _Write0(*pData++);
  } while (--NumItems);
}

/*********************************************************************
*
*       _Read1
*
* Purpose:
*   Read byte from controller, with A0 = 1
*/
static unsigned char _Read1(void) {
  // TBD by customer
  return 0;
}

/*********************************************************************
*
*       _Delay
*
* Purpose:
*   Waits a while...
*/
static void _Delay(int Cnt) {
  volatile I32 i, j; 
  j = Cnt << 8; 
  for (i = 0; i < j; i++);
}

/*********************************************************************
*
*       _InitController
*
* Purpose:
*   Initializes the display controller
*/
static void _InitController(void) {
  #ifndef WIN32
    #define BYTES_PER_LINE ((XSIZE_PHYS + 7) >> 2)
    LCD_X_Init();
    _Write1(0x40);                                 // SYSTEM SET:
    _Delay(255);                                   // Wait a while
    _Write0((1 << 5)                               //   P1: Screen Origin Compensation: not done.
          | (1 << 4)                               //       Reserved
          | (0 << 3)                               //       Panel Drive Select: single panel drive
          | (0 << 2)                               //       Character Height: 8 pixels
          | (0 << 1)                               //       Reserved
          | (0 << 0));                             //       Character Generator Select: CGROM
    _Write0((1 << 7)                               //   P2: MOD: two-frame AC drive
          | (7 << 0));                             //       Horizontal Character Size: 8
    _Write0((7 << 0));                             //   P3: Vertical Character Size: 8
    _Write0(BYTES_PER_LINE - 1);                   //   P4: Character Bytes Per Row
    _Write0(BYTES_PER_LINE + 2);                   //   P5: Total Character Bytes Per Row
    _Write0(YSIZE_PHYS - 1);                       //   P6: Frame Height
    _Write0(BYTES_PER_LINE);                       //   P7: Horizontal Address Range Register 0
    _Write0(0x00);                                 //   P8: Horizontal Address Range Register 1
    _Write1(0x44);                                 // SCROLL:
    _Write0((BYTES_PER_LINE * YSIZE_PHYS) & 0xff); //   Screen Block 1 Start Address Register 0 (Text)
    _Write0((BYTES_PER_LINE * YSIZE_PHYS) >> 8);   //   Screen Block 1 Start Address Register 1 (Text)
    _Write0(YSIZE_PHYS);                           //   Screen Block 1 Size Register
    _Write0(0);                                    //   Screen Block 2 Start Address Register 0 (Graphic)
    _Write0(0);                                    //   Screen Block 2 Start Address Register 0 (Graphic)
    _Write0(YSIZE_PHYS);                           //   Screen Block 2 Size Register
    _Write1(0x4C);                                 // CSR DIR: Set cursor shift direction to right
    _Write1(0x5A);                                 // HDOT SCR:
    _Write0(0 << 0);                               //   Horizontal Pixel Scroll bits: 0
    _Write1(0x5B);                                 // OVLAY:
    _Write0((0 << 4)                               //   two layers are used
          | (0 << 3)                               //   screen block 2 and 4 can display graphics
          | (0 << 2)                               //   screen block 2 and 4 can display graphics
          | (1 << 0));                             //   Layer Composition Method: Exclusive-OR
    _Write1(0x60);                                 // GRAYSCALE
    _Write0(0x01);                                 //   Bit-Per-Pixel Select bits
  #endif
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;

  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    _InitController();
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_SLIN Config = {0};

  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Common display driver configuration
  //
  if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
  //
  // Driver specific configuration
  //
  Config.UseCache  = 1;
  GUIDRV_SLin_Config(pDevice, &Config);
  //
  // Select display controller
  //
  GUIDRV_SLin_SetS1D13700(pDevice);
  //
  // Setup hardware access routines
  //
  #ifndef WIN32
  {
    GUI_PORT_API PortAPI;

    PortAPI.pfWrite16_A0  = _Write0;
    PortAPI.pfWrite16_A1  = _Write1;
    PortAPI.pfWriteM16_A0 = _WriteM0;
    PortAPI.pfRead16_A1   = _Read1;
    GUIDRV_SLin_SetBus8(pDevice, &PortAPI);
  }
  #endif
}

/*************************** End of file ****************************/
