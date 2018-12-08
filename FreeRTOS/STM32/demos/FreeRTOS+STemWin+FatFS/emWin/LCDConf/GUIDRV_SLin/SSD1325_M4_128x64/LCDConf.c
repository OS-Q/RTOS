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
Purpose     : Solomon SSD1325 configuration, internal use only
Display     : Newhaven NHD-2.7-12864UCY3

---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUIDRV_SLin.h"

#ifndef WIN32
  #include "LCD_X_8080_8.h"
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
#define XSIZE   128
#define YSIZE    64
#define VXSIZE  XSIZE
#define VYSIZE  YSIZE

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_4

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_SLIN_4

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#ifndef WIN32
  GUI_PORT_API PortAPI;
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
  int x;
  int y;

  LCD_X_8080_8_Init();
  // Set_Display_On_Off(0x00);               // Display Off (0x00/0x01) 
  PortAPI.pfWrite8_A0(0xAE);
  //PortAPI.pfWrite8_A0(0xAF);
  // Set_Display_Clock(0x91);                // Set Clock as 135 Frames/Sec
  PortAPI.pfWrite8_A0(0xB3);
  PortAPI.pfWrite8_A0(0x91);
  // Set_Multiplex_Ratio(0x3F);              // 1/64 Duty (0x0F~0x5F) 
  PortAPI.pfWrite8_A0(0xA8);
  PortAPI.pfWrite8_A0(0x3F);
  // Set_Display_Offset(0x4C);               // Shift Mapping RAM Counter (0x00~0x5F) 
  PortAPI.pfWrite8_A0(0xA2);
  PortAPI.pfWrite8_A0(0x4C);
  // Set_Start_Line(0x00);                   // Set Mapping RAM Display Start Line (0x00~0x5F) 
  PortAPI.pfWrite8_A0(0xA1);
  PortAPI.pfWrite8_A0(0x00);
  // Set_Master_Config(0x00);                // Disable Embedded DC/DC Converter (0x00/0x01) 
  PortAPI.pfWrite8_A0(0xAD);
  PortAPI.pfWrite8_A0(0x00);
  // Set_Remap_Format(0x50);                 // Set Column Address 0 Mapped to SEG0 
                                             //     Disable Nibble Remap 
                                             //     Horizontal Address Increment 
                                             //     Scan from COM[N-1] to COM0 
                                             //     Enable COM Split Odd Even 
  PortAPI.pfWrite8_A0(0xA0);
  PortAPI.pfWrite8_A0(0x50); // Default
  //PortAPI.pfWrite8_A0(0x40); // Mirror y
  //PortAPI.pfWrite8_A0(0x43); // Mirror y + Mirror x + Remap nibble
  //
  // Set_Current_Range(0x02);                // Set Full Current Range 
  //
  PortAPI.pfWrite8_A0(0x02);
  //
  // Set_Gray_Scale_Table();                 // Set Pulse Width for Gray Scale Table 
  //
  PortAPI.pfWrite8_A0(0xB8);
  PortAPI.pfWrite8_A0(0x01);
  PortAPI.pfWrite8_A0(0x01);
  PortAPI.pfWrite8_A0(0x01);
  PortAPI.pfWrite8_A0(0x01);
  PortAPI.pfWrite8_A0(0x01);
  PortAPI.pfWrite8_A0(0x01);
  PortAPI.pfWrite8_A0(0x01);
  PortAPI.pfWrite8_A0(0x01);
  //
  // Set_Contrast_Current(brightness);       // Set Scale Factor of Segment Output Current Control 
  //
  PortAPI.pfWrite8_A0(0x81);
  PortAPI.pfWrite8_A0(0x40);
  //
  // Set_Frame_Frequency(0x51);              // Set Frame Frequency 
  //
  PortAPI.pfWrite8_A0(0xB2);
  PortAPI.pfWrite8_A0(0x51);
  //
  // Set_Phase_Length(0x55);                 // Set Phase 1 as 5 Clocks & Phase 2 as 5 Clocks 
  //
  PortAPI.pfWrite8_A0(0xB1);
  PortAPI.pfWrite8_A0(0x55);
  //
  // Set_Precharge_Voltage(0x10);            // Set Pre-Charge Voltage Level 
  //
  PortAPI.pfWrite8_A0(0xBC);
  PortAPI.pfWrite8_A0(0x10);
  //
  // Set_Precharge_Compensation(0x20,0x02);  // Set Pre-Charge Compensation 
  //
  PortAPI.pfWrite8_A0(0xB0);
  PortAPI.pfWrite8_A0(0x08);
  //
  // Set_VCOMH(0x1C);                        // Set High Voltage Level of COM Pin 
  //
  PortAPI.pfWrite8_A0(0xBE);
  PortAPI.pfWrite8_A0(0x1C);
  //
  // Set_VSL(0x0D);                          // Set Low Voltage Level of SEG Pin 
  //
  PortAPI.pfWrite8_A0(0xBF);
  PortAPI.pfWrite8_A0(0x0D);
  //
  // Set_Display_Mode(0x00);                 // Normal Display Mode (0x00/0x01/0x02/0x03) 
  //
  PortAPI.pfWrite8_A0(0x00);
  //
  // Clear Screen
  //
  PortAPI.pfWrite8_A0(0x15);
  PortAPI.pfWrite8_A0(0x00);
  PortAPI.pfWrite8_A0(0x7F);
  PortAPI.pfWrite8_A0(0x75);
  PortAPI.pfWrite8_A0(0x00);
  PortAPI.pfWrite8_A0(0x3F);
  for (y = 0; y < 64; y += 1) {
    for (x = 0; x < 128; x += 2) {
      PortAPI.pfWrite8_A1(0x00);
    }
  }
  // Set_Display_On_Off(0x01);               // Display On (0x00/0x01)
  PortAPI.pfWrite8_A0(0xAF);
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
  CONFIG_SLIN   Config = { 0 };
  GUI_DEVICE  * pDevice;

  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Common display driver configuration
  //
  LCD_SetSizeEx(0, XSIZE, YSIZE);
  LCD_SetVSizeEx(0, VXSIZE, VYSIZE);
  #ifndef WIN32
    //
    // Setup hardware access routines
    //
    PortAPI.pfWrite8_A0  = LCD_X_8080_8_Write00;
    PortAPI.pfWrite8_A1  = LCD_X_8080_8_Write01;
    PortAPI.pfWriteM8_A1 = LCD_X_8080_8_WriteM01;
    PortAPI.pfRead8_A1   = LCD_X_8080_8_Read01;

    GUIDRV_SLin_SetBus8(pDevice, &PortAPI);
    //
    // Pass configuration structure to driver
    //
    Config.FirstCOM = 0;
    Config.UseCache = 1;
    GUIDRV_SLin_Config(pDevice, &Config);
    GUIDRV_SLin_SetSSD1325(pDevice);
  #else
    (void)pDevice;
  #endif
}

/*************************** End of file ****************************/
