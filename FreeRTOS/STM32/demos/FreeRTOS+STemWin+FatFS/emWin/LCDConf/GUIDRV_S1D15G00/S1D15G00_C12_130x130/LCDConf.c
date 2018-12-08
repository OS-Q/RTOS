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
#include "GUIDRV_S1D15G00.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS 130
#define YSIZE_PHYS 130

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_M444_12

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_S1D15G00

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Command definitions of S1D15G00
*/
#define CMD_DISON   0xAF // Display on
#define CMD_DISOFF  0xAE // Display off
#define CMD_DISNOR  0xA6 // Normal display
#define CMD_DISINV  0xA7 // Inverse display
#define CMD_COMSCN  0xBB // Common scan direction
#define CMD_DISCTL  0xCA // Display control
#define CMD_SLPIN   0x95 // Sleep in
#define CMD_SLPOUT  0x94 // Sleep out
#define CMD_PASET   0x75 // Page address set
#define CMD_CASET   0x15 // Column address set
#define CMD_DATCTL  0xBC // Data scan direction, etc.
#define CMD_RGBSET8 0xCE // 256-color position set
#define CMD_RAMWR   0x5C // Writing to memory
#define CMD_RAMRD   0x5D // Reading from memory
#define CMD_PTLIN   0xA8 // Partial display in
#define CMD_PTLOUT  0xA9 // Partial display out
#define CMD_RMWIN   0xE0 // Read and modify write
#define CMD_RMWOUT  0xEE // End
#define CMD_ASCSET  0xAA // Area scroll set
#define CMD_SCSTART 0xAB // Scroll start set
#define CMD_OSCON   0xD1 // Internal oscillation on
#define CMD_OSCOFF  0xD2 // Internal oscillation off
#define CMD_PWRCTR  0x20 // Power control
#define CMD_VOLCTR  0x81 // Electronic volume control
#define CMD_VOLUP   0xD6 // Increment electronic control by 1
#define CMD_VOLDOWN 0xD7 // Decrement electronic control by 1
#define CMD_TMPGRD  0x82 // Temperature gradient set
#define CMD_EPCTIN  0xCD // Control EEPROM
#define CMD_EPCOUT  0xCC // Cancel EEPROM control
#define CMD_EPMWR   0xFC // Write into EEPROM
#define CMD_EPMRD   0xFD // Read from EEPROM
#define CMD_EPSRRD1 0x7C // Read register 1
#define CMD_EPSRRD2 0x7D // Read register 2
#define CMD_NOP     0x25 // NOP instruction

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
*       _Write_A0
*
* Purpose:
*   Write byte to controller, with A0 = 0
*/
static void _Write_A0(U8 Data) {
  /* TBD by customer... */
}

/*********************************************************************
*
*       _Write_A1
*
* Purpose:
*   Write byte to controller, with A0 = 1
*/
static void _Write_A1(U8 Data) {
  /* TBD by customer... */
}

/*********************************************************************
*
*       _WriteM_A1
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 1
*/
static void _WriteM_A1(U8 * pData, int NumBytes) {
  /* TBD by customer... */
}

/*********************************************************************
*
*       LCD_X_InitController
*
* Purpose:
*   Initializes the display controller
*/
static void _InitController(void) {
  #ifndef WIN32
  
  U32 NumBytes, i;
  
  //
  // Initialization
  //
  _PortAPI.pfWrite8_A0(CMD_DISCTL);
  _PortAPI.pfWrite8_A1(0x00);
  _PortAPI.pfWrite8_A1(0x20);
  _PortAPI.pfWrite8_A1(0x11);
  _PortAPI.pfWrite8_A0(CMD_OSCON);
  _PortAPI.pfWrite8_A0(CMD_VOLCTR);
  _PortAPI.pfWrite8_A1(0x20);
  _PortAPI.pfWrite8_A1(0x00);
  _PortAPI.pfWrite8_A0(CMD_SLPOUT);
  _PortAPI.pfWrite8_A0(CMD_DISINV);
  _PortAPI.pfWrite8_A0(CMD_DATCTL);
  _PortAPI.pfWrite8_A1(0x00);
  _PortAPI.pfWrite8_A1(0x00);
  _PortAPI.pfWrite8_A1(0x02);
  _PortAPI.pfWrite8_A0(CMD_PWRCTR);
  _PortAPI.pfWrite8_A1(0x1F);
  GUI_X_Delay(50);
  _PortAPI.pfWrite8_A0(CMD_DISON);
  //
  // Clear complete video RAM
  //
  _PortAPI.pfWrite8_A0(CMD_PASET);
  _PortAPI.pfWrite8_A1(0x00);
  _PortAPI.pfWrite8_A1(0x83);
  _PortAPI.pfWrite8_A0(CMD_CASET);
  _PortAPI.pfWrite8_A1(0x00);
  _PortAPI.pfWrite8_A1(0x83);
  _PortAPI.pfWrite8_A0(CMD_RAMWR);
  NumBytes = 132 * 132 * 12 / 8;
  for (i = 0; i < NumBytes; i++) {
    _PortAPI.pfWrite8_A1(0x00);
  }

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
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_S1D15G00 Config = {0};

  //
  // Set display driver and color conversion for 1st layer
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration
  //
  if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
  #ifndef WIN32
    //
    // Driver specific configuration
    //
    Config.FirstCOM  = 2;
    GUIDRV_S1D15G00_Config(pDevice, &Config);
    //
    // Setup hardware access routines
    //
    _PortAPI.pfWrite8_A0  = _Write_A0;
    _PortAPI.pfWrite8_A1  = _Write_A1;
    _PortAPI.pfWriteM8_A1 = _WriteM_A1;
    GUIDRV_S1D15G00_SetBus8(pDevice, &_PortAPI);
  #endif
}

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
*      0 - OK
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
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}
/*************************** End of file ****************************/
