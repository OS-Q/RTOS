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
Purpose     : Display controller configuration, sample

Display     : T240128

---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUIDRV_SLin.h"

#ifndef WIN32
  #include "LCD_X_8080.h"
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
#define XSIZE 240
#define YSIZE 128

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
#define CMD_SET_ADDR   0x24
#define CMD_WRITE1     0xC0
#define CMD_AUTO_WRITE 0xB0
#define CMD_AUTO_READ  0xB1
#define CMD_AUTO_RESET 0xB2

#define LCD_RAMSIZE (0x2000)
#define LCD_GSTART  (0x0)
#define LCD_XSIZEB  ((XSIZE + 7) >> 3)
#define LCD_TSTART  (LCD_RAMSIZE - (LCD_XSIZEB) - (YSIZE >> 3))

/*********************************************************************
*
*       _SetAddr
*/
#ifndef WIN32
static void _SetAddr(U32 Off) {
  _PortAPI.pfWrite8_A0(Off & 0xff);
  _PortAPI.pfWrite8_A0(Off >> 8);
  _PortAPI.pfWrite8_A1(CMD_SET_ADDR);
}

/*********************************************************************
*
*       _InitController
*
* Purpose:
*   Initializes the display controller
*/
static void _InitController(void) {
  int i;

  LCD_X_Init();
  _SetAddr(0);
  for (i = 0; i < 100; i++) {
    _PortAPI.pfWrite8_A0(0xff);
    _PortAPI.pfWrite8_A1(CMD_WRITE1);
  }
  _SetAddr(0);
  for (i = 0; i < LCD_RAMSIZE; i++) {
    _PortAPI.pfWrite8_A0(0x0);
    _PortAPI.pfWrite8_A1(CMD_WRITE1);
  }
  _PortAPI.pfWrite8_A1(0x81);           // SetModeXor

  _PortAPI.pfWrite8_A0((U8)LCD_TSTART);// SetTextAdr 
  _PortAPI.pfWrite8_A0((U8)(LCD_TSTART >> 8));
  _PortAPI.pfWrite8_A1(0x40);

  _PortAPI.pfWrite8_A0(1);             // SetTextArea 
  _PortAPI.pfWrite8_A0(1 >> 8);
  _PortAPI.pfWrite8_A1(0x41);

  _PortAPI.pfWrite8_A1(0x9C);           // SetTextGraph 

  _PortAPI.pfWrite8_A0(LCD_GSTART);    // SetGraphAdr 
  _PortAPI.pfWrite8_A0(LCD_GSTART >> 8);
  _PortAPI.pfWrite8_A1(0x42);

  _PortAPI.pfWrite8_A0(LCD_XSIZEB);    // SetGraphArea 
  _PortAPI.pfWrite8_A0(LCD_XSIZEB >> 8);
  _PortAPI.pfWrite8_A1(0x43);
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
    // Nothing done here, see 'LCD_X_ON'.
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
  LCD_SetSizeEx(0, XSIZE, YSIZE);
  LCD_SetVSizeEx(0, XSIZE, YSIZE);
  //
  // Set driver specific configuration items
  //
  #ifndef WIN32
  {
    CONFIG_SLIN Config = {0};

    //
    // Setup hardware access routines
    //
    _PortAPI.pfWrite8_A0  = LCD_X_Write00;
    _PortAPI.pfWrite8_A1  = LCD_X_Write01;
    _PortAPI.pfWriteM8_A0 = LCD_X_WriteM00;
    _PortAPI.pfRead8_A0   = LCD_X_Read00;
    GUIDRV_SLin_SetBus8(pDevice, &_PortAPI);
    //
    // Pass configuration structure to driver
    //
    Config.UseCache  = 1;
    Config.CheckBusy = 1;
    GUIDRV_SLin_Config(pDevice, &Config);
    //
    // Set controller
    //
    GUIDRV_SLin_SetT6963(pDevice);
  }
  #endif
}

/*************************** End of file ****************************/
