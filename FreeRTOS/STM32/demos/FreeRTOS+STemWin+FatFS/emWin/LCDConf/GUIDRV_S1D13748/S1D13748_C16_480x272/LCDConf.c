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

#include "GUIDRV_S1D13748.h"

/*********************************************************************
*
*       Layer configuration
*
**********************************************************************
*/
//
// Physical display size of MAIN layer
//
#define XSIZE_PHYS 480
#define YSIZE_PHYS 272

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_M565

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_S1D13748_16

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
*       Display controller register definitions
*
**********************************************************************
*/
#ifndef WIN32
#define ADDR_A0 0x800000
#define ADDR_A1 0x800004

#define REG0000_PRODUCT_0        0x0000 // Product Information Register 0 [READONLY]
#define REG0002_PRODUCT_1        0x0002 // Product ID Register 1 [READONLY]
#define REG0004_CFG_PIN_STATUS   0x0004 // Configuration Pin Status Register
#define REG000C_PLL_0            0x000C // PLL Setting Register 0
#define REG000E_PLL_1            0x000E // PLL Setting Register 1
#define REG0010_PLL_2            0x0010 // PLL Setting Register 2
#define REG0012_PLL_3            0x0012 // PLL Setting Register 3
#define REG0014_MISC_CONFIG      0x0014 // Miscellaneous Configuration Register
#define REG0016_SOFT_RESET       0x0016 // Software Reset Register [WRITEONLY]
#define REG0018_SYSTEM_CLOCK     0x0018 // System Clock Setting Register
#define REG0030_LCD_CLOCK        0x0030 // LCD Interface Clock Setting Register
#define REG0032_LCD_CONFIG       0x0032 // LCD Interface Configuration Register
#define REG0034_LCD_COMMAND      0x0034 // LCD Interface Command Register
#define REG0036_LCD_PARAM        0x0036 // LCD Interface Parameter Register
#define REG0038_LCD_STATUS       0x0038 // LCD Interface Status Register [READONLY]
#define REG003A_LCD_FRAME_XFER   0x003A // LCD Interface Frame Transfer Register
#define REG003C_LCD_XFER         0x003C // LCD Interface Transfer Setting Register
#define REG0040_LCD1_HT          0x0040 // LCD1 Horizontal Total (FPLINE Period) Register
#define REG0042_LCD1_HDP         0x0042 // LCD1 Horizontal Display Period Register
#define REG0044_LCD1_HDPS        0x0044 // LCD1 Horizontal Display Period Start Position Register
#define REG0046_LCD1_HP          0x0046 // LCD1 Horizontal Pulse (FPLINE) Register
#define REG0048_LCD1_HPS         0x0048 // LCD1 Horizontal Pulse (FPLINE) Start Position Register
#define REG004A_LCD1_VT          0x004A // LCD1 Vertical Total (FPFRAME Period) Register
#define REG004C_LCD1_VDP         0x004C // LCD1 Vertical Display Period Register
#define REG004E_LCD1_VDPS        0x004E // LCD1 Vertical Display Period Start Position Register
#define REG0050_LCD1_VP          0x0050 // LCD1 Vertical Pulse (FPFRAME) Register
#define REG0052_LCD1_VPS         0x0052 // LCD1 Vertical Pulse (FPFRAME) Start Position Register
#define REG0054_LCD1_SERIAL      0x0054 // LCD1 Serial Interface Setting Register
#define REG0056_LCD1_PARALLEL    0x0056 // LCD1 Parallel Interface Setting Register
#define REG0058_LCD2_HDP         0x0058 // LCD2 Horizontal Display Period Register
#define REG005A_LCD2_VDP         0x005A // LCD2 Vertical Display Period Register
#define REG005C_LCD2_SERIAL      0x005C // LCD2 Serial Interface Setting Register
#define REG005E_LCD2_PARALLEL    0x005E // LCD2 Parallel Interface Setting Register
#define REG0068_LCD1_VSYNC_OUT   0x0068 // LCD1 VSYNC Output Register
#define REG006A_LCD2_VSYNC_OUT   0x006A // LCD2 VSYNC Output Register
#define REG00FE_LCD_ID           0x00FE // LCD Interface ID Register
#define REG0180_HOST_CONFIG      0x0180 // Host Interface Configuration Register
#define REG0182_MEM_SADDR_0      0x0182 // Memory Start Address Register 0
#define REG0184_MEM_SADDR_1      0x0184 // Memory Start Address Register 1
#define REG0186_HWC_WR_ADDR_OFF  0x0186 // HWC Memory Rectangular Write Address Offset Register
#define REG0188_HWC_WR_HSIZE     0x0188 // HWC Memory Rectangular Write Horizontal Size Register
#define REG018A_HWC_WR_VSIZE     0x018A // HWC Memory Rectangular Write Vertical Size Register
#define REG018C_MEM_ACCESS_PORT  0x018C // Memory Access Port Register
#define REG018E_HWC_RAW_STATUS   0x018E // HWC Raw Status Register [READONLY]
#define REG0190_HWC_IRQ_CONTROL  0x0190 // HWC Interrupt Control Register
#define REG0192_HWC_STATUS       0x0192 // HWC Status Register
#define REG0194_MEM_WR_ADDR_OFF  0x0194 // Memory Rectangular Write Address Offset Register
#define REG0196_MEM_WR_ADDR_W    0x0196 // Memory Rectangular Write Address Width Register
#define REG0198_VOUT_CONFIG      0x0198 // VOUT Configuration Register
#define REG0200_DISPLAY_MODE_0   0x0200 // Display Mode Setting Register 0
#define REG0202_DISPLAY_MODE_1   0x0202 // Display Mode Setting Register 1
#define REG0204_OVERLAY_ALPHA    0x0204 // Overlay and Alpha Blend Control Register
#define REG0206_BG_COLOR         0x0206 // Background Color Setting Register
#define REG0208_ALPHA_RATIO      0x0208 // Alpha Blend Ratio Setting Register
#define REG020C_PIP1_OVERLAY_KEY 0x020C // PIP1 Window Overlay Key Color Register
#define REG020E_PIP2_OVERLAY_KEY 0x020E // PIP2 Window Overlay Key Color Register
#define REG0210_ALPHA_BLEND1_KEY 0x0210 // Alpha Blend 1 Key Color Register
#define REG0212_ALPHA_BLEND2_KEY 0x0212 // Alpha Blend 2 Key Color Register
#define REG0214_ALPHA_BLEND3_KEY 0x0214 // Alpha Blend 3 Key Color Register
#define REG0216_ALPHA_BLEND4_KEY 0x0216 // Alpha Blend 4 Key Color Register
#define REG0218_MAIN1_X_START    0x0218 // Main Window 1 X Start Position Register
#define REG021A_MAIN1_Y_START    0x021A // Main Window 1 Y Start Position Register
#define REG0220_MAIN2_X_START    0x0220 // Main Window 2 X Start Position Register
#define REG0222_MAIN2_Y_START    0x0222 // Main Window 2 Y Start Position Register
#define REG0228_PIP1_X_START     0x0228 // PIP1 Window X Start Position Register
#define REG022A_PIP1_Y_START     0x022A // PIP1 Window Y Start Position Register
#define REG022C_PIP1_X_END       0x022C // PIP1 Window X End Position Register
#define REG022E_PIP1_Y_END       0x022E // PIP1 Window Y End Position Register
#define REG0230_PIP2_X_START     0x0230 // PIP2 Window X Start Position Register
#define REG0232_PIP2_Y_START     0x0232 // PIP2 Window Y Start Position Register
#define REG0234_PIP2_X_END       0x0234 // PIP2 Window X End Position Register
#define REG0236_PIP2_Y_END       0x0236 // PIP2 Window Y End Position Register
#define REG0238_MAIN_SCROLL_SA_0 0x0238 // Main Window Scroll Start Address Register 0
#define REG023A_MAIN_SCROLL_SA_1 0x023A // Main Window Scroll Start Address Register 1
#define REG023C_MAIN_SCROLL_EA_0 0x023C // Main Window Scroll End Address Register 0
#define REG023E_MAIN_SCROLL_EA_1 0x023E // Main Window Scroll End Address Register 1
#define REG0240_MAIN1_SADDR_0    0x0240 // Main Window 1 Display Start Address Register 0
#define REG0242_MAIN1_SADDR_1    0x0242 // Main Window 1 Display Start Address Register 1
#define REG0244_MAIN1_STRIDE     0x0244 // Main Window 1 Line Address Offset Register
#define REG0246_MAIN1_HORZ_SIZE  0x0246 // Main Window 1 Image Horizontal Size Register
#define REG0248_MAIN1_VERT_SIZE  0x0248 // Main Window 1 Image Vertical Size Register
#define REG024A_MAIN2_SADDR_0    0x024A // Main Window 2 Display Start Address Register 0
#define REG024C_MAIN2_SADDR_1    0x024C // Main Window 2 Display Start Address Register 1
#define REG024E_MAIN2_STRIDE     0x024E // Main Window 2 Line Address Offset Register
#define REG0250_MAIN2_HORZ_SIZE  0x0250 // Main Window 2 Image Horizontal Size Register
#define REG0252_MAIN2_VERT_SIZE  0x0252 // Main Window 2 Image Vertical Size Register
#define REG0260_PIP1_SCALING     0x0260 // PIP1 Scaling Mode Register
#define REG0262_PIP1_HORZ_SCALE  0x0262 // PIP1 Scalar Horizontal Scale Register
#define REG0264_PIP1_VERT_SCALE  0x0264 // PIP1 Scalar Vertical Scale Register
#define REG0266_PIP1_SCALAR_PORT 0x0266 // PIP1 Scalar Port Address Counter Control Register
#define REG0268_PIP1_SCALAR_COEF 0x0268 // PIP1 Scalar Coefficient Table Access Port Register
#define REG026E_PIP1_SCALAR_CTRL 0x026E // PIP1 Scalar Control Register
#define REG0270_PIP1_SCROLL_SA_0 0x0270 // PIP1 Window Scroll Start Address Register 0
#define REG0272_PIP1_SCROLL_SA_1 0x0272 // PIP1 Window Scroll Start Address Register 1
#define REG0274_PIP1_SCROLL_EA_0 0x0274 // PIP1 Window Scroll End Address Register 0
#define REG0276_PIP1_SCROLL_EA_1 0x0276 // PIP1 Window Scroll End Address Register 1
#define REG0278_PIP1_SADDR_0     0x0278 // PIP1 Window 1 Display Start Address Register 0
#define REG027A_PIP1_SADDR_1     0x027A // PIP1 Window 1 Display Start Address Register 1
#define REG027C_PIP1_STRIDE      0x027C // PIP1 Window 1 Line Address Offset Register
#define REG027E_PIP1_HORZ_SIZE   0x027E // PIP1 Window 1 Image Horizontal Size Register
#define REG0280_PIP1_VERT_SIZE   0x0280 // PIP1 Window 1 Image Vertical Size Register
#define REG0282_PIP1_PSEUDO      0x0282 // PIP1 Pseudo Setting Register
#define REG02A0_PIP2_SCALING     0x02A0 // PIP2 Scaling Mode Register
#define REG02A2_PIP2_HORZ_SCALE  0x02A2 // PIP2 Scalar Horizontal Scale Register
#define REG02A4_PIP2_VERT_SCALE  0x02A4 // PIP2 Scalar Vertical Scale Register
#define REG02A6_PIP2_SCALAR_PORT 0x02A6 // PIP2 Scalar Port Address Counter Control Register
#define REG02A8_PIP2_SCALAR_COEF 0x02A8 // PIP2 Scalar Coefficient Table Access Port Register
#define REG02AE_PIP2_SCALAR_CTRL 0x02AE // PIP2 Scalar Control Register
#define REG02B0_PIP2_SCROLL_SA_0 0x02B0 // PIP2 Window Scroll Start Address Register 0
#define REG02B2_PIP2_SCROLL_SA_1 0x02B2 // PIP2 Window Scroll Start Address Register 1
#define REG02B4_PIP2_SCROLL_EA_0 0x02B4 // PIP2 Window Scroll End Address Register 0
#define REG02B6_PIP2_SCROLL_EA_1 0x02B6 // PIP2 Window Scroll End Address Register 1
#define REG02B8_PIP2_SADDR_0     0x02B8 // PIP2 Window 1 Display Start Address Register 0
#define REG02BA_PIP2_SADDR_1     0x02BA // PIP2 Window 1 Display Start Address Register 1
#define REG02BC_PIP2_STRIDE      0x02BC // PIP2 Window 1 Line Address Offset Register
#define REG02BE_PIP2_HORZ_SIZE   0x02BE // PIP2 Window 1 Image Horizontal Size Register
#define REG02C0_PIP2_VERT_SIZE   0x02C0 // PIP2 Window 1 Image Vertical Size Register
#define REG02C2_PIP2_PAN_AREA_A  0x02C2 // PIP2 Panarama Area A Vertical Scale Register
#define REG02C4_PIP2_PAN_AREA_B  0x02C4 // PIP2 Panarama Area B Vertical Scale Register
#define REG02C6_PIP2_PAN_AREA_1  0x02C6 // PIP2 Panarama Area 1 Vertical Start Line Register
#define REG02C8_PIP2_PAN_AREA_2  0x02C8 // PIP2 Panarama Area 2 Vertical Start Line Register
#define REG02CA_PIP2_PAN_AREA_3  0x02CA // PIP2 Panarama Area 3 Vertical Start Line Register
#define REG02CE_PIP2_PAN_DELTA   0x02CE // PIP2 Panarama Area Vertical Delta Register
#define REG02CC_PIP2_PAN_AREA_4  0x02CC // PIP2 Panarama Area 4 Vertical Start Line Register
#define REG0300_GPIO_CONFIG_0    0x0300 // GPIO Configuration Register 0
#define REG0302_GPIO_CONFIG_1    0x0302 // GPIO Configuration Register 1
#define REG0304_GPIO_INPUT_0     0x0304 // GPIO Input Enable Register 0
#define REG0306_GPIO_INPUT_1     0x0306 // GPIO Input Enable Register 1
#define REG0308_GPIO_PULLDOWN_0  0x0308 // GPIO Pull Down Control Register 0
#define REG030A_GPIO_PULLDOWN_1  0x030A // GPIO Pull Down Control Register 1
#define REG030C_GPIO_STATUS_0    0x030C // GPIO Status Register 0
#define REG030E_GPIO_STATUS_1    0x030E // GPIO Status Register 1
#define REG0310_GPIO_POS_EDGE_0  0x0310 // GPIO Positive Edge Interrupt Trigger Register 0
#define REG0312_GPIO_POS_EDGE_1  0x0312 // GPIO Positive Edge Interrupt Trigger Register 1
#define REG0314_GPIO_NEG_EDGE_0  0x0314 // GPIO Negative Edge Interrupt Trigger Register 0
#define REG0316_GPIO_NEG_EDGE_1  0x0316 // GPIO Negative Edge Interrupt Trigger Register 1
#define REG0318_GPIO_IRQ_0       0x0318 // GPIO Interrupt Status Register 0
#define REG031A_GPIO_IRQ_1       0x031A // GPIO Interrupt Status Register 1
#define REG0400_PIP2LUT_COUNTER  0x0400 // PIP2 LUT Address Counter Register
#define REG0402_PIP2LUT_DATAPORT 0x0402 // PIP2 LUT Data Port Register
#define REG0500_LCDLUT_COUNTER   0x0500 // LCD LUT Address Counter Register
#define REG0502_LCDLUT_DATAPORT  0x0502 // LCD LUT Data Port Register
#define REG0A00_IRQ_STATUS       0x0A00 // Interrupt Status Register [READONLY]
#define REG0A02_IRQ_CONTROL_0    0x0A02 // Interrupt Control Register 0
#define REG0A04_IRQ_CONTROL_1    0x0A04 // Interrupt Control Register 1
#define REGFLAG_DELAY            0xFFFF
#endif

/*********************************************************************
*
*       SFR definitions of CPU
*
**********************************************************************
*/
#ifndef WIN32
#define REG_HSDMAADV_SEL         (*(volatile unsigned char*)(0x30119C))
#define REG_HSDMAADVCH3_CTRL     (*(volatile unsigned char*)(0x301192))
#define REG_HSDMACH3_TC          (*(volatile unsigned long*)(0x301150))
#define REG_HSDMACH3_SRCADDR     (*(volatile unsigned long*)(0x301154))
#define REG_HSDMAADVCH3_SRCADDR  (*(volatile unsigned long*)(0x301194))
#define REG_HSDMACH3_DESTADDR    (*(volatile unsigned long*)(0x301158))
#define REG_HSDMAADVCH3_DESTADDR (*(volatile unsigned long*)(0x301198))
#define REG_HSDMACH3_EN          (*(volatile unsigned char*)(0x30115C))
#define REG_HSDMA_HTGR2          (*(volatile unsigned char*)(0x300299))
#define REG_HSDMA_HSOFTTGR       (*(volatile unsigned char*)(0x30029A))
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
#ifndef WIN32
typedef struct {
  U16 Register;
  U16 Content;
} REGS;
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _aRegs
*/
#ifndef WIN32
static REGS _aRegs[] = {
  { REG0018_SYSTEM_CLOCK,     0x0000 }, // System clock divide ratio is 1:1
  { REG000C_PLL_0,            0x3B03 }, // L-C=0x3B, M-D=0x03(4:1)
  { REG000E_PLL_1,            0x0002 }, // HCP=0, VC=0x02
  { REG0010_PLL_2,            0x0810 },
  { REG0012_PLL_3,            0x0000 }, // using PLL
  { REGFLAG_DELAY,            0x03E8 },

  // LCDc
  { REG0014_MISC_CONFIG,      0x04D0 }, // Power save mode disable
  { REG0030_LCD_CLOCK,        0x0507 }, // SCLK=12:1, PCLK=9:1
  { REG0032_LCD_CONFIG,       0x0040 }, // LCD1 is 24Bit RGB interface
                                        // FPSHIFT is rising edge
                                        // FPCS1 is not reversed
                                        // FPDRDY is not reversed
  { REG0040_LCD1_HT,          0x0041 }, // HT = (528/8)-1=0x41
  { REG0042_LCD1_HDP,         0x00EF }, // HDP = (480/2)-1=0xEF
  { REG0044_LCD1_HDPS,        0x0022 }, // HDPS = 43-9=0x22
  { REG0046_LCD1_HP,          0x0028 }, // HPLINE is active low, HPW = 41-1=0x28
  { REG0048_LCD1_HPS,         0x0004 }, // HPP = 5-1=0x4
  { REG004A_LCD1_VT,          0x011E }, // VT = 287-1=0x11E
  { REG004C_LCD1_VDP,         0x010F }, // VDP = 272 - 1 = 0x10F
  { REG004E_LCD1_VDPS,        0x000E }, // VDPS = 14 = 0xE
  { REG0050_LCD1_VP,          0x0007 }, // FPFRAME is active low, VPW = 8-1=0x07
  { REG0052_LCD1_VPS,         0x0002 }, // VPP = 0x2
  { REG0054_LCD1_SERIAL,      0x00E1 },
  { REG0056_LCD1_PARALLEL,    0x0400 },
  { REG0058_LCD2_HDP,         0x0041 },
  { REG005A_LCD2_VDP,         0x00AF },
  { REG005C_LCD2_SERIAL,      0x000A },
  { REG005E_LCD2_PARALLEL,    0x0100 },
  { REG0068_LCD1_VSYNC_OUT,   0x0000 },
  { REG006A_LCD2_VSYNC_OUT,   0x0000 },

  // Display
  { REG0182_MEM_SADDR_0,      0x0000 }, // Memory Start Address 0x0
  { REG0184_MEM_SADDR_1,      0x0000 },
  { REG0188_HWC_WR_HSIZE,     0x0000 },
  { REG018A_HWC_WR_VSIZE,     0x0000 },

  // Display Configuration
  { REG0200_DISPLAY_MODE_0,   0x0030 }, // LUT bypass

  { REG0202_DISPLAY_MODE_1,   0x0461 }, // LCD1, Normal display.
                                        // Layer mode is 11B & Main1 window only
                                        // PIP1/2 disable & Main enable
  { REG0218_MAIN1_X_START,    0x0000 }, // MAIN1 X/Y Start (0,0)
  { REG021A_MAIN1_Y_START,    0x0000 },
  { REG0220_MAIN2_X_START,    0x0000 }, // MAIN2 X/Y Start (0,0)
  { REG0222_MAIN2_Y_START,    0x0000 },
  { REG0228_PIP1_X_START,     0x0000 }, // PIP1 X/Y Start (0,0)
  { REG022A_PIP1_Y_START,     0x0000 },
  { REG022C_PIP1_X_END,       0x0000 }, // PIP1 X/Y End (0,0)
  { REG022E_PIP1_Y_END,       0x0000 },
  { REG0230_PIP2_X_START,     0x0000 }, // PIP2 X/Y Start (0,0)
  { REG0232_PIP2_Y_START,     0x0000 },
  { REG0234_PIP2_X_END,       0x0000 }, // PIP2 X/Y End (0,0)
  { REG0236_PIP2_Y_END,       0x0000 },
  { REG0238_MAIN_SCROLL_SA_0, 0x0000 }, // Main1 Window Scroll start address 0x0
  { REG023A_MAIN_SCROLL_SA_1, 0x0000 },
  { REG023C_MAIN_SCROLL_EA_0, 0xFFFE }, // Main1 Window Scroll End address 0x0FFFFE
  { REG023E_MAIN_SCROLL_EA_1, 0x000F },
  { REG0240_MAIN1_SADDR_0,    0x0000 }, // Main1 Window Display start address 0x0
  { REG0242_MAIN1_SADDR_1,    0x0000 },
  { REG0244_MAIN1_STRIDE,     0x03C0 }, // Main1 Window line address offset is 480*16/8=0x3C0
                                        // Vertical/Horizontal Pixel Double disable
  { REG0246_MAIN1_HORZ_SIZE,  0x01DF }, // Main1 Window Image Horizontal Size is 480 - 1 =0x1DF
  { REG0248_MAIN1_VERT_SIZE,  0x010F }, // Main1 Window Image Vertical Size is 272 - 1=0x10F

  // PIP Configuration

  // GPIO
  { REG0300_GPIO_CONFIG_0,    0x0000 },
  { REG0302_GPIO_CONFIG_1,    0x0000 },
  { REG0304_GPIO_INPUT_0,     0x0000 },
  { REG0306_GPIO_INPUT_1,     0x0000 },
  { REG0308_GPIO_PULLDOWN_0,  0xFFFF },
  { REG030A_GPIO_PULLDOWN_1,  0x00FF },
  { REG030C_GPIO_STATUS_0,    0x0000 },
  { REG030E_GPIO_STATUS_1,    0x0000 },
  { REG0310_GPIO_POS_EDGE_0,  0x0000 },
  { REG0312_GPIO_POS_EDGE_1,  0x0000 },
  { REG0314_GPIO_NEG_EDGE_0,  0x0000 },
  { REG0316_GPIO_NEG_EDGE_1,  0x0000 },
  { REG0318_GPIO_IRQ_0,       0x0000 },
  { REG031A_GPIO_IRQ_1,       0x0000 },
};
#endif

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
*       _WriteA0_16
*/
#ifndef WIN32
static void _WriteA0_16(U16 Data) {
  *((volatile U16 *)ADDR_A0) = Data;
}
#endif

/*********************************************************************
*
*       _WriteA1_16
*/
#ifndef WIN32
static void _WriteA1_16(U16 Data) {
  *((volatile U16 *)ADDR_A1) = Data;
}
#endif

/*********************************************************************
*
*       _WriteMA1_16
*/
#ifndef WIN32
static void _WriteMA1_16(U16 * pData, int NumItems) {
  //Use the ADV mode
  REG_HSDMAADV_SEL = 0x01;
  REG_HSDMAADVCH3_CTRL = 0x0;

  //set transfer counter[15:0] = count( if single/successive transfer mode )
  //set address mode:dual addr
  REG_HSDMACH3_TC = (U32)NumItems | 0x80000000;

  // set source addr(increase addr with no init, half word)
  REG_HSDMACH3_SRCADDR = 0x70000000;
  REG_HSDMAADVCH3_SRCADDR = (U32)pData;

  //set destination addr(successive mode, increase addr with no init)
  REG_HSDMACH3_DESTADDR = 0x40000000;
  REG_HSDMAADVCH3_DESTADDR = ADDR_A1;

  //enable HSDMA
  REG_HSDMACH3_EN |= 0x01;

  //enable software trigger
  REG_HSDMA_HTGR2 &=0x0F;
  REG_HSDMA_HSOFTTGR |=0x08;

  //detect DMA end
  while ((REG_HSDMACH3_EN & 0x01) != 0x00);
}
#endif

/*********************************************************************
*
*       _ReadA1_16
*/
#ifndef WIN32
static U16 _ReadA1_16(void) {
  U16 Data;

  Data = *((volatile U16 *)ADDR_A1);
  return Data;
}
#endif

/*********************************************************************
*
*       _ReadMA1_16
*/
#ifndef WIN32
static void _ReadMA1_16(U16 * pData, int NumItems) {
  do {
    *pData++ = *((volatile U16 *)ADDR_A1);
  } while (--NumItems);
}
#endif

/*********************************************************************
*
*       _Delay
*/
#ifndef WIN32
static void _Delay(int Time) {
  U32 i;
  while (Time--) {
    for (i = 0; i < 100; i++) { }
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
*       LCD_X_InitController
*
* Purpose:
*   Initializes the display controller
*/
#ifndef WIN32
static void _InitController(void) {
  int i;

  for (i = 0; i < GUI_COUNTOF(_aRegs); i++) {
    if (_aRegs[i].Register == REGFLAG_DELAY) {
      _Delay(_aRegs[i].Content);
    } else {
      _PortAPI.pfWrite16_A0(_aRegs[i].Register);
      _PortAPI.pfWrite16_A1(_aRegs[i].Content);
    }
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
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*/
void LCD_X_Config(void) {
  #ifndef WIN32
  GUI_DEVICE * pDevice;
  CONFIG_S1D13748 Config = {0};
  #endif

  //
  // Setup hardware access routines
  //
  #ifndef WIN32
  _PortAPI.pfWrite16_A0  = _WriteA0_16;
  _PortAPI.pfWrite16_A1  = _WriteA1_16;
  _PortAPI.pfWriteM16_A1 = _WriteMA1_16;
  _PortAPI.pfRead16_A1   = _ReadA1_16;
  _PortAPI.pfReadM16_A1  = _ReadMA1_16;
  #endif
  //
  // Configuration of MAIN layer
  //
  #ifndef WIN32
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  GUIDRV_S1D13748_SetBus16(pDevice, &_PortAPI);
  #else
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  #endif
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
  //
  // Configuration of PIP1 layer
  //
  #ifndef WIN32
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 1);
  GUIDRV_S1D13748_SetBus16(pDevice, &_PortAPI);
  if (LCD_GetSwapXYEx(1)) {
    LCD_SetSizeEx (1, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(1, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (1, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(1, XSIZE_PHYS, YSIZE_PHYS);
  }
  Config.BufferOffset = XSIZE_PHYS * YSIZE_PHYS * 2; // After MAIN layer
  Config.UseLayer     = GUIDRV_S1D13748_USE_PIP1;
  GUIDRV_S1D13748_Config(pDevice, &Config);
  #endif
  //
  // Configuration of PIP2 layer
  //
  #ifndef WIN32
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 2);
  GUIDRV_S1D13748_SetBus16(pDevice, &_PortAPI);
  if (LCD_GetSwapXYEx(2)) {
    LCD_SetSizeEx (2, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(2, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (2, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(2, XSIZE_PHYS, YSIZE_PHYS);
  }
  Config.BufferOffset = XSIZE_PHYS * YSIZE_PHYS * 4; // After PIP1 layer
  Config.UseLayer     = GUIDRV_S1D13748_USE_PIP2;
  GUIDRV_S1D13748_Config(pDevice, &Config);
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
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;

  (void)pData;
  (void)LayerIndex;
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

/*************************** End of file ****************************/
