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
#define XSIZE_PHYS 128
#define YSIZE_PHYS 108

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_1

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_SLIN_1

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
*       _WriteM1
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 1
*/
static void _WriteM1(unsigned char * pData, int NumItems) {
  do {
    _Write1(*pData++);
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
*       _InitController
*
* Purpose:
*   Initializes the display controller
*/
static void _InitController(void) {
  #ifndef WIN32
    LCD_X_Init();
    _Write0(0x94); // Exit sleep mode
    _Write0(0xD1); // Internal oscillator on
    _Write0(0xF7); // Mono selection
    _Write1(0x00); //   ''
    _Write1(0x0E); //   ''
    _Write1(0x41); //   ''
    _Write0(0xBB); // COM output scan direction
    _Write1(0x01); //   COM0->COM64, COM129->COM65
    _Write0(0xBC); // Data output scan direction
    _Write1(0x00); //   Normal page/column
    _Write1(0x24); //   Gray scale setting
    _Write1(0x00); //   PWM
    _Write0(0xCA); // Display control set
    _Write1(0x00); //   Dummy
    _Write1(0x1F); //   Duty = 1/128
    _Write1(0x00); //   Dummy
    _Write0(0x81); // Contrast level
    _Write1(0x10); //   X0-5: 010000
    _Write1(0x07); //   Y0-2: 111
    _Write0(0x20); // Power control register set
    _Write1(0x07); //   5x booster
    _Write0(0xFB); // Set biasing ratio
    _Write1(0x02); //   1/11 bias
    _Write0(0xAF); // Display on
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
  CONFIG_SLIN  Config  = {0};
  GUI_PORT_API PortAPI = {0};

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
  Config.UseMirror = 1;
  GUIDRV_SLin_Config(pDevice, &Config);
  //
  // Select display controller
  //
  GUIDRV_SLin_SetSSD1848(pDevice);
  //
  // Setup hardware access routines
  //
  PortAPI.pfWrite8_A0  = _Write0;
  PortAPI.pfWrite8_A1  = _Write1;
  PortAPI.pfWriteM8_A1 = _WriteM1;
  PortAPI.pfRead8_A1   = _Read1;
  GUIDRV_SLin_SetBus8(pDevice, &PortAPI);
}

/*************************** End of file ****************************/
