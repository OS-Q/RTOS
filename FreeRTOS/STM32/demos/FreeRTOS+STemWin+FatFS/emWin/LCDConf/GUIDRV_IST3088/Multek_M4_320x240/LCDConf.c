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
Purpose     : Sample configuration file for GUIDRV_IST3088
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUIDRV_IST3088.h"

#ifndef WIN32
  #include "LCD_X_SPI4_16.h" // Prototypes of custom port routines
#endif

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
#define COLOR_CONVERSION GUICC_4

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_IST3088_4

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#ifndef WIN32
  GUI_PORT_API _PortAPI;
#endif

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitController
*
* Purpose:
*   Initializes the display controller
*/
#ifndef WIN32
static void _InitController(void) {
  int j;

  LCD_X_SPI4_16_Init();
  //
  // Set the Register Index for the Driver Control
  //
  _PortAPI.pfWrite16_A0(0x0001); // Command - R01h
  _PortAPI.pfWrite16_A1(0x0000); // Data - SHL(0), SGS(0), NL(0)
  //
  // Set the Register Index for the Polarity Control
  //
  _PortAPI.pfWrite16_A0(0x0002); // Command - R02h
  _PortAPI.pfWrite16_A1(0x014C); // Data - BC(1), EOR(0), NW(4C)
  //
  // Set the Register Index for the Contrast Control
  //
  _PortAPI.pfWrite16_A0(0x0005); // Command - R05h
  _PortAPI.pfWrite16_A1(0x0030); // Data - CT(0x30)
  //
  // Set the Register Index for the Power Control (1)
  //
  _PortAPI.pfWrite16_A0(0x0003); // Command - R03h
  _PortAPI.pfWrite16_A1(0x0070); // Data - VC(1), VR(1), VF(1), SLP(0), STB(0)
  //
  // Wait a while...
  //
  GUI_X_Delay(100);
  //
  // Set the Register Index for the Power Control (2)
  //
  _PortAPI.pfWrite16_A0(0x0004); // Command - R04h
  _PortAPI.pfWrite16_A1(0x0061); // Data - TC(0), BS(0), BT(6), VRG(1)
  //
  // Set the Register Index for the Entry Mode
  //
  _PortAPI.pfWrite16_A0(0x0006); // Command - R06h
  _PortAPI.pfWrite16_A1(0x0000); // Data - AM(0), ID(0)
  //
  // Set the Register Index for the Display Mode Control
  //
  _PortAPI.pfWrite16_A0(0x0023); // Command - R23h
  _PortAPI.pfWrite16_A1(0x0003); // Data - DSPM(3)
  //
  // Set the Register Index for the Frame Rate Control
  //
  _PortAPI.pfWrite16_A0(0x0028); // Command - R28h
  _PortAPI.pfWrite16_A1(0x0018); // Data - CSEL2(0), CSEL(1), CTN(0)
  //
  // Set the Register Index for power adjust
  //
  _PortAPI.pfWrite16_A0(0x0037); // Command - R37h
  _PortAPI.pfWrite16_A1(0x0000); // Data - VFR1(0), VFR4(0)
  //
  // Set the Register Index for the Display Control
  //
  _PortAPI.pfWrite16_A0(0x0007); // Command - R07h
  _PortAPI.pfWrite16_A1(0x0001); // Data - BW(0), REV(0), D(1)
  //
  // Wait a while...
  //
  GUI_X_Delay(100);
  //
  // Clear RAM
  //
  _PortAPI.pfWrite16_A0(0x0008); // Command - R08h
  _PortAPI.pfWrite16_A1(0x0000); // Data- AX(0), AY(0)
  _PortAPI.pfWrite16_A0(0x0009); // Command - R09h
  for (j = 0; j < 80 * 240; j++) {
    _PortAPI.pfWrite16_A1(0x0000);
  }
}
#endif

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

  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(pData);
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
    #ifndef WIN32
      _InitController();
    #endif
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
  // Setup hardware access routines
  //
  #ifndef WIN32
    _PortAPI.pfWrite16_A0  = LCD_X_SPI4_16_Write0;
    _PortAPI.pfWrite16_A1  = LCD_X_SPI4_16_Write1;
    _PortAPI.pfWriteM16_A1 = LCD_X_SPI4_16_WriteM1;
    GUIDRV_IST3088_SetBus16(pDevice, &_PortAPI);
  #endif
}

/*************************** End of file ****************************/

