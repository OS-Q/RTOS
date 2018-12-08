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
#include "GUIDRV_SSD1926.h"

#ifndef WIN32
  #include "LCD_X_8080_16.h"
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
#define XSIZE_PHYS 320L
#define YSIZE_PHYS 240L

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_8666

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_SSD1926_8

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
*       _WriteReg
*
* Purpose:
*   Writes the given value to the required register.
*
* Format:
*   W0:15:   - M/R#, Bit15 = 1 means memory access, Bit15 = 0 means register access.
*   W0:14:11 - Unused, should be 0.
*   W0:10:00 - Represent the the address AB18:08.
*   W1:15:08 - Represent the address AB07:00.
*   W1:07:00 - Mode_ SL to select byte or word access during 16 bit mode. 0x00 means Byte access, 0x01 means word access.
*/
#ifndef WIN32
static void _WriteReg(U16 Reg, U16 Data) {
  U16 aCmd[2];

  //
  // Write value to register using byte mode
  //
  aCmd[0] = (0 << 15)             // Register access
          | ((Reg >> 8) & 0x7ff); // AB18:08
  aCmd[1] = ((Reg & 0xFF) << 8)   // AB07:00
          | (0 << 0);             // Select byte mode
  _PortAPI.pfWriteM16_A0(aCmd, 2);
  //
  // Write data to register
  //
  if ((Reg & 1) == 0) {
    Data <<= 8;
  }
  _PortAPI.pfWrite16_A1(Data);
}
#endif

/*********************************************************************
*
*       _InitController
*
* Purpose:
*   Initializes the display controller
*/
#ifndef WIN32
static void _InitController(void) {

  LCD_X_8080_16_Init();

  _WriteReg(0x0A2, 0x01); // Software reset
  _WriteReg(0x0A2, 0x00); // ""
  _WriteReg(0x0A8, 0x08); // I/O pin configuration register 0
  _WriteReg(0x0A9, 0x00); // I/O pin configuration register 1
  _WriteReg(0x0AC, 0x08); // I/O pin status/control register
  _WriteReg(0x0A0, 0x00); // Power saving register
  _WriteReg(0x0A1, 0x00); // Power save frame count register
  _WriteReg(0x126, 0x8A); // PLL clock setting register 0
  _WriteReg(0x127, 0xC8); // PLL clock setting register 1
  _WriteReg(0x12B, 0xAE); // PLL clock setting register 2
  _WriteReg(0x004, 0x00); // Memory clock configuration register
  _WriteReg(0x158, 0xFF); // PCLK frequency ratio register 0
  _WriteReg(0x159, 0xFF); // PCLK frequency ratio register 1
  _WriteReg(0x15A, 0x03); // PCLK frequency ratio register 2
  _WriteReg(0x001, 0x00); // Read dummy register
  _WriteReg(0x010, 0x71); // Panel type register
  _WriteReg(0x011, 0x00); // MOD rate register
  _WriteReg(0x050, 0x00); // Dithering / FRC control register
  _WriteReg(0x02C, 0x7F); // Display post processing saturation register
  _WriteReg(0x02D, 0x80); // Display post processing brightness control register
  _WriteReg(0x02E, 0x40); // Display post processing contrast control register
  _WriteReg(0x02F, 0x00); // Display post processing control register
  _WriteReg(0x012, 0x3D); // Horizontal total register 1
  _WriteReg(0x013, 0x04); // Horizontal total register 0
  _WriteReg(0x014, 0x27); // Horizontal display period register
  _WriteReg(0x016, 0x44); // Horizontal display period start position register 0
  _WriteReg(0x017, 0x00); // Horizontal display period start position register 1
  _WriteReg(0x018, 0x05); // Vertical total register 0
  _WriteReg(0x019, 0x01); // Vertical total register 1
  _WriteReg(0x01C, 0xEF); // Vertical display period register 0
  _WriteReg(0x01D, 0x00); // Vertical display period register 1
  _WriteReg(0x01E, 0x16); // Vertical display period start position register 0
  _WriteReg(0x01F, 0x00); // Vertical display period start position register 1
  _WriteReg(0x020, 0x54); // LLINE pulse width register
  _WriteReg(0x022, 0x00); // LLINE pulse start position register 0
  _WriteReg(0x023, 0x00); // LLINE pulse start position register 1
  _WriteReg(0x024, 0x07); // LFRAME pulse width register
  _WriteReg(0x026, 0x03); // LFRAME pulse start position register 0
  _WriteReg(0x027, 0x00); // LFRAME pulse start position register 1
  _WriteReg(0x030, 0x00); // LFRAME pulse start offset register 0
  _WriteReg(0x031, 0x00); // LFRAME pulse start offset register 1
  _WriteReg(0x034, 0x00); // LFRAME pulse stop offset register 0
  _WriteReg(0x035, 0x00); // LFRAME pulse stop offset register 1
  _WriteReg(0x038, 0x01); // LSHIFT polarity register
  _WriteReg(0x042, 0x00); // RGB sequence register
  _WriteReg(0x070, 0x43); // Display mode register
  #if 0
    //
    // Default mode
    //
    _WriteReg(0x071, 0x44); // Special effects register
    _WriteReg(0x074, 0x00); // Main window display start address register 0
    _WriteReg(0x075, 0x00); // Main window display start address register 1
    _WriteReg(0x076, 0x00); // Main window display start address register 2
  #else
    //
    // Rotated by 180 degrees
    //
    _WriteReg(0x071, 0x46); // Special effects register
    _WriteReg(0x074, ((((XSIZE_PHYS * YSIZE_PHYS) >> 2) - 1) >>  0) & 0xFF); // Main window display start address register 0
    _WriteReg(0x075, ((((XSIZE_PHYS * YSIZE_PHYS) >> 2) - 1) >>  8) & 0xFF); // Main window display start address register 1
    _WriteReg(0x076, ((((XSIZE_PHYS * YSIZE_PHYS) >> 2) - 1) >> 16) & 0x03); // Main window display start address register 2
  #endif
  _WriteReg(0x078, 0x50); // Main window line address offset register 0
  _WriteReg(0x079, 0x00); // Main window line address offset register 1
  _WriteReg(0x1A4, 0xC0); // RGB/YUV setting register
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
  case LCD_X_ON: {
    //
    // Please note that the lookup table is initialized after the
    // LCD_X_INITCONTROLLER command and changes do not appear when
    // the display is on. So the 'LCD_X_ON' - command is obused here
    // for executing initialization sequence again, so that the changes
    // of the LUT take effect.
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
  CONFIG_SSD1926 Config = {0};

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
  // Set driver specific configuration items
  //
  Config.UseCache  = 1;
  //
  // Setup hardware access routines
  //
  #ifndef WIN32
    _PortAPI.pfWrite16_A0  = LCD_X_8080_16_Write00_16;
    _PortAPI.pfWrite16_A1  = LCD_X_8080_16_Write01_16;
    _PortAPI.pfWriteM16_A0 = LCD_X_8080_16_WriteM00_16;
    _PortAPI.pfWriteM16_A1 = LCD_X_8080_16_WriteM01_16;
    _PortAPI.pfRead16_A1   = LCD_X_8080_16_Read01_16;
    GUIDRV_SSD1926_SetBus16(pDevice, &_PortAPI);
  #endif
  //
  // Pass configuration structure to driver
  //
  GUIDRV_SSD1926_Config(pDevice, &Config);
}

/*************************** End of file ****************************/
