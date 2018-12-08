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

#ifndef WIN32
  #include "spi_Driver.h"
#endif

#undef pr0

#include "GUI.h"
#include "GUIDRV_S1D13781.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS 240
#define YSIZE_PHYS 320

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_8666

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_S1D13781_8C0

//
// Multiple buffers
//
#define NUM_BUFFERS 2

//
// Hardware access
//
#define TI_SPI_DISP_C1    ti_u3c1
#define SPI_DISP_TB       u3tb
#define RI_SPI_DISP_C1    ri_u3c1
#define SPI_DISP_RB       u3rb
#define SPI_CS_GRAPH_P    p3_7
#define SPI_DISP_C0       u3c0

#define SPI_ON  (0)
#define SPI_OFF (1)

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VRAM_ADDR
  #define VRAM_ADDR 0 // TBD by customer: This has to be the frame buffer start address
#endif
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
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _WriteM8_A1
*/
#ifndef WIN32
static void _WriteM8_A1(U8 * pData, int NumItems) {
  volatile U8 Dummy;

  do {
    while(TI_SPI_DISP_C1 == 0);
    SPI_DISP_TB = *pData++;
    while(RI_SPI_DISP_C1 == 0);
    Dummy = (U8)SPI_DISP_RB;
  } while (--NumItems);
}

/*********************************************************************
*
*       _ReadM8_A1
*/
static void _ReadM8_A1(U8 * pData, int NumItems) {
  volatile U8 Dummy;

  SPI_DISP_C0 = 0xD0;
  do {
    while(TI_SPI_DISP_C1 == 0);
    if (RI_SPI_DISP_C1 == 1) {
      Dummy = (U8)SPI_DISP_RB;
    }
    SPI_DISP_TB = 0x00;
    while(RI_SPI_DISP_C1 == 0);
    *pData++ = (U8)SPI_DISP_RB;
  } while (--NumItems);
  SPI_DISP_C0 = 0x90;
}

/*********************************************************************
*
*       _SetCS
*/
static void _SetCS(U8 NotActive) {
  SPI_CS_GRAPH_P = NotActive ? SPI_OFF : SPI_ON;
}

/*********************************************************************
*
*       _WriteReg
*/
static void _WriteReg(U32 Reg, U16 Data) {
  U8 aData[6];
  _SetCS(SPI_ON);
  *(aData + 0) = 0x88;
  *(aData + 1) = (Reg  >> 16) & 0x07;
  *(aData + 2) = (Reg  >>  8) & 0xFF;
  *(aData + 3) =  Reg         & 0xFF;
  *(aData + 4) = (Data >>  8) & 0xFF;
  *(aData + 5) =  Data        & 0xFF;
  _WriteM8_A1(aData, sizeof(aData));
  _SetCS(SPI_OFF);
}
#endif

/*********************************************************************
*
*       _InitController
*/
static void _InitController(void) {
#ifndef WIN32
  _WriteReg(0x060806, 0x0100);          /* Software Reset register */
  _WriteReg(0x060804, 0x0000);          /* Power save configuration register */
  _WriteReg(0x060810, 0x0000);          /* PLL setting register 0 */
  _WriteReg(0x060812, 0x0017);          /* PLL setting register 1 */
  _WriteReg(0x060814, 0x0035);          /* PLL setting register 2 */
  _WriteReg(0x060810, 0x0001);          /* PLL setting register 0 */
  GUI_X_Delay(100);
  _WriteReg(0x060816, 0x0005);          /* Internal clock configuration register */
  _WriteReg(0x060804, 0x0002);          /* Power save configuraion register */
  _WriteReg(0x060820, 0x004D);          /* Panel setting miscellaneous register */
  _WriteReg(0x060822, 0x0001);          /* Display setting register */
  _WriteReg(0x060824, XSIZE_PHYS >> 3); /* Horizontal display width register */
  _WriteReg(0x060826, 0x0028);          /* Horizontal non-display period register */
  _WriteReg(0x060828, YSIZE_PHYS);      /* Vertical display height register */
  _WriteReg(0x06082A, 0x000A);          /* Vertical no-displayperiod register */
  _WriteReg(0x06082C, 0x0010);          /* HS pulse width register */
  _WriteReg(0x06082E, 0x0010);          /* HS pulse start position register */
  _WriteReg(0x060830, 0x0002);          /* VS Pulse width register */
  _WriteReg(0x060832, 0x0004);          /* VS Pulse start position register */
  _WriteReg(0x060850, 0x0000);          /* PIP layer setting register */
  _WriteReg(0x060852, 0x8400);          /* PIP layer start address register 0 */
  _WriteReg(0x060854, 0x0003);          /* PIP layer start address register 1 */
  _WriteReg(0x060856, 0x0028);          /* PIP layer width register */
  _WriteReg(0x060858, 0x0020);          /* PIP layer height register */
  _WriteReg(0x06085A, 0x0060);          /* PIP layer X start position register */
  _WriteReg(0x06085C, 0x00D0);          /* PIP layer Y start position register */
  _WriteReg(0x060862, 0x0040);          /* Alpha blending register */
  _WriteReg(0x060864, 0x0000);          /* Transparency register */
  _WriteReg(0x060866, 0x0000);          /* Transparency key color register 0 */
  _WriteReg(0x060868, 0x0000);          /* Transparency key color register 1 */
  _WriteReg(0x0608D0, 0x0001);          /* GPIO configuration register */
  _WriteReg(0x0608D2, 0x0001);          /* GPIO status and control regsietr */
  _WriteReg(0x0608D4, 0x0000);          /* GPIO Pull-down control register */
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
  CONFIG_S1D13781 Config;
  #ifndef WIN32
    GUI_PORT_API PortAPI = {0};
  #endif

  //
  // Initialize multibuffering
  //
  GUI_MULTIBUF_Config(NUM_BUFFERS);
  //
  // Setup port routines
  //
  #ifndef WIN32
    PortAPI.pfWriteM8_A1 = _WriteM8_A1;
    PortAPI.pfReadM8_A1  = _ReadM8_A1;
    PortAPI.pfSetCS      = _SetCS;
  #endif
  //
  // Set display driver and color conversion for 1st layer
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0); {
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
    // Driver configuration
    //
    #ifndef WIN32
      GUIDRV_S1D13781_SetBusSPI(pDevice, &PortAPI);
    #endif
    Config.BufferOffset    = 0;
    Config.WriteBufferSize = 320 + 5;
    Config.UseLayer        = 0;
    Config.WaitUntilVNDP   = 1;
    GUIDRV_S1D13781_Config(pDevice, &Config);
  }
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
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;

  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    if (LayerIndex == 0) {
      _InitController();
    }
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/
