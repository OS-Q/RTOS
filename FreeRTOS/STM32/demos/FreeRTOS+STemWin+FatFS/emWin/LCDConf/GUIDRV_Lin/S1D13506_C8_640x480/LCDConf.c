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
#include "GUIDRV_Lin.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS  640
#define YSIZE_PHYS  480
#define VYSIZE_PHYS (YSIZE_PHYS * 8)

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_8666

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_LIN_8

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
*       Custom defines
*
**********************************************************************
*/
#define LCD_CTRLR_STARTADR 0x0200000      /* 20:0000 up to 3F:FFFF (2MB for registers and mem) */

#ifdef WIN32
  #define xhuge
#endif

#define LCD_READ_REG(Off)         (*((volatile U16 xhuge *)(LCD_CTRLR_STARTADR + 0x0 + (((U32)(Off)) << 1))))
#define LCD_WRITE_REG(Off, data)  (*((volatile U16 xhuge *)(LCD_CTRLR_STARTADR + 0x0 + (((U32)(Off)) << 1))) = (U16)(data))
#define LCD_WRITE_REG8(Off, data) (*((volatile U8  xhuge *)(LCD_CTRLR_STARTADR + 0x0 + ( (U32)(Off))    ))   = (U8) (data))

#define LCD_WRITE_REGLH(Adr, d0, d1) LCD_WRITE_REG(Adr, ((d1) << 8) | (d0))

#ifndef WIN32
  sbit   MR  =  P2^13;
  sbit   DMR = DP2^13;
#endif

#ifndef WIN32
  #define LCD_ENABLE_REG_ACCESS()                              \
    K_Resource_Wait(RTXC_RESOURCE_GLCDREG, RTXC_WAIT_FOREVER); \
    (DMR = 1);                                                 \
    ( MR = 0);
#else
  #define LCD_ENABLE_REG_ACCESS()                             
#endif

#ifndef WIN32
  #define LCD_ENABLE_MEM_ACCESS() (MR = 1);  \
    (DMR=1);                                 \ 
    K_Resource_Release(RTXC_RESOURCE_GLCDREG)
#else
 #define LCD_ENABLE_MEM_ACCESS() 
#endif

#define READ_REG(Off, Data)  Data = LCD_READ_REG(Off)
#define WRITE_REG(Off, Data) LCD_WRITE_REG(Off, Data)

#define BYTESPERLINE XSIZE_PHYS

#define BITBLT_SET_DESTINATION(x,y)     {                                                \
                                          U32 DestOff = (U32)y * BYTESPERLINE + x;       \
                                          WRITE_REG(0x108 / 2, DestOff);                 \
                                          WRITE_REG(0x10a / 2, DestOff >> 16);           \
                                        }
#define BITBLT_SET_ACTIVE()             WRITE_REG(0x100 / 2, 0x0000); WRITE_REG(0x100 / 2, 0x0080)

/*********************************************************************
*
*       SFR definitions for initialization purpose
*
**********************************************************************
*/
#define REG40_BPP 3

#define LCD_REG0  (0x00) /* product code ( = 0x1c for the S1D13806)  */
                         /* read only                                */
#define LCD_REG1  (0x00) /* Select bit: must be cleared to 0 prior   */
                         /* first access to video RAM or any regs    */
#define LCD_REG4  (0xff) /* Direction: GPIO1 to 7 are inputs         */ // all outputs !!!!
#define LCD_REG5  (0x1f) /* Direction: GPIO11 is the only output pin */
#define LCD_REG8  (0x00) /* Outputs:   all GPIO0 to 7 to low         */
#define LCD_REG9  (0x08) /* Outputs:   all GPIO8 to 15 low, GPIO11=1 */

#define LCD_REG10 (0x00) /* MCLK 1:1 from CLKI    (27MHz)            */
#define LCD_REG14 (0x00) /* PCLK 1:2 from CLKI    (27MHz)            */
#define LCD_REG18 (0x00)
#define LCD_REG1C (0x00) /* MEDIA PLUG CONFIG. from CLKI */
#define LCD_REG21 (0x02) /* DRAM-refresh according to MCLK = 27MHz   */
#define LCD_REG2A (0x12) /* DRAM timing control according to 27MHz   */
#define LCD_REG2B (0x02) /* DRAM timing control according to 27MHz   */
#define LCD_REG34 (0x19)
                         /* horizontal non displ.period(->0x19) means 208*/
#define LCD_REG3B (0x0a)
                         /* means 0x0a FPFRAME start position (-> ? )     */

#define LCD_REG1E (0x01) /* CPUtoMem Wait State Select:1= 50MHz def.*/

#define LCD_REG30 (0x25) /* Panel Type: 18bit colorTFT, data format1 */
#define LCD_REG31 (0x00) /* for passive LCDs only: MOD/DRDY toggling */
#define LCD_REG32 ((XSIZE_PHYS / 8) - 1)
                         /* horizontal display width = 0x4f means 640 dez */
#define LCD_REG35 (0x01) /* ??? formerly: (0x0d-5)/8 */
                         /* means 0x0d FPLINE start position (-> 01 )     */
#define LCD_REG36 (0x0b | (0 << 7))        
/* 96 pulse width -> 0x0b */						
/* means 0x0d FPLINE pulse width       */
/* FPLINE polarity 0=low active TFT */

#define LCD_REG38 ((YSIZE_PHYS - 1) & 255) /* Vert. panel size, lsb */
#define LCD_REG39 ((YSIZE_PHYS - 1) >> 8)  /* Vert. panel size, msb */
#define LCD_REG3A (0x2c)
                         /* vertical non displ.period (->0x2C means 
                                            525-480 = 45 lines )     */
#define LCD_REG3C ((0x01) | (0 << 7))       
/* 2 FPFRAME VSYNC pulse width -> 0x01 */
/* means 0x0d FPLINE pulse width       */
/* FPFRAME polarity 0=low active TFT */

#define LCD_REG40 ((REG40_BPP << 0) /* Mode reg 1: Bits per pixel 2:4, 3:8 5:16 */  \
                 | (0 << 4)  /* Swivel Enable Bit 1 (see other at Reg01c) */ \
                 | (0 << 7)) /* 1 = force display all blank */
#define LCD_REG41 (0x00) /* Dithering, Dual panel buffer - both disabled */ 

#define LCD_REG42 (0x00) /* Startaddress 0 */ 
#define LCD_REG43 (0x00) /* Startaddress 1 */ 

#define LCD_REG44 (0x00) /* Startaddress 2 */ 

#define LCD_REG46 (0x40) /* Address Offset 0 */ 
#define LCD_REG47 (0x01) /* Address Offset 1 */ 

#define LCD_REG48 (0x00) /* Pixel Panning */ 

#define LCD_REG4A (0x00) /* FIFO high threshold to not used/automatic */ 
#define LCD_REG4B (0x00) /* FIFO high threshold to not used/automatic */ 

/**************************** CRT / TV ***************************************/
#define LCD_REG50 (0x4f) /*  */ 
#define LCD_REG52 (0x13) /*  */ 
#define LCD_REG53 (0x01) /*  */
#define LCD_REG54 (0x0b) /*  */
#define LCD_REG56 (0xdf) /*  */
#define LCD_REG57 (0x01) /*  */
#define LCD_REG58 (0x2b) /*  */
#define LCD_REG59 (0x09) /*  */
#define LCD_REG5A (0x01) /*  */
#define LCD_REG5B (0x10) /*  */
#define LCD_REG60 (0x03) /* start address (must equal 42/43/44)              */
#define LCD_REG62 (0x00) /*                                                  */
#define LCD_REG63 (0x00) /*                                                  */
#define LCD_REG64 (0x00) /*  */
#define LCD_REG66 (0x40) /*  */
#define LCD_REG67 (0x01) /*  */
#define LCD_REG68 (0x00) /*  */
#define LCD_REG6A (0x00) /*  */
#define LCD_REG6B (0x00) /*  */
/**************************** INK / CURSOR ***********************************/
#define LCD_REG70 (0x00) /*  */
#define LCD_REG71 (0x01) /*  */
#define LCD_REG72 (0x00) /*  */
#define LCD_REG73 (0x00) /*  */
#define LCD_REG74 (0x00) /*  */
#define LCD_REG75 (0x00) /*  */
#define LCD_REG76 (0x00) /*  */
#define LCD_REG77 (0x00) /*  */
#define LCD_REG78 (0x00) /*  */
#define LCD_REG7A (0x1f) /*  */
#define LCD_REG7B (0x3f) /*  */
#define LCD_REG7C (0x1f) /*  */
#define LCD_REG7E (0x00) /*  */
#define LCD_REG80 (0x00) /*  */
#define LCD_REG81 (0x01) /* must equal 71, whenever CRT hardware-cursor is used  */
#define LCD_REG82 (0x00) /*  */
#define LCD_REG83 (0x00) /*  */
#define LCD_REG84 (0x00) /*  */
#define LCD_REG85 (0x00) /*  */
#define LCD_REG86 (0x00) /*  */
#define LCD_REG87 (0x00) /*  */
#define LCD_REG88 (0x00) /*  */
#define LCD_REG8A (0x1f) /*  */
#define LCD_REG8B (0x3f) /*  */
#define LCD_REG8C (0x1f) /*  */
#define LCD_REG8E (0x00) /*  */

#define LCD_REG100 (0x00) /*  */
#define LCD_REG101 (0x00) /*  */
#define LCD_REG102 (0x00) /*  */
#define LCD_REG103 (0x00) /*  */
#define LCD_REG104 (0x00) /*  */
#define LCD_REG105 (0x00) /*  */
#define LCD_REG106 (0x00) /*  */
#define LCD_REG108 (0x00) /*  */
#define LCD_REG109 (0x00) /*  */
#define LCD_REG10A (0x00) /*  */
#define LCD_REG10C (0x00) /*  */
#define LCD_REG10D (0x00) /*  */
#define LCD_REG110 (0x00) /*  */
#define LCD_REG111 (0x00) /*  */
#define LCD_REG112 (0x00) /*  */
#define LCD_REG113 (0x00) /*  */
#define LCD_REG114 (0x00) /*  */
#define LCD_REG115 (0x00) /*  */
#define LCD_REG118 (0x00) /*  */
#define LCD_REG119 (0x00) /*  */

#define LCD_REG1E0 (0x00) /* LCD LUT = CRT LUT for use with CRT! */
#define LCD_REG1E2 (0x00) /*  */
#define LCD_REG1F0 (0x10) /*  */
#define LCD_REG1F1 (0x00) /*  */
#define LCD_REG1F4 (0x00) /*  */
#define LCD_REG1FC (0x01) /*  */
/**************************** END OF REGISTER DEFINES ************************/


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _SetLUTEntry
*/
static void _SetLUTEntry(LCD_COLOR Color, U8 Pos) {
  int i;
  U16 aColorSep[3];

  for (i = 0; i < 3; i++) {
    aColorSep[i] = Color &0xff;
    Color >>= 8;
  }
  /* Convert 8 bit color separation into index */
  for (i = 0; i < 3; i++) {
    aColorSep[i] = (U16)(aColorSep[i] + 8) / 17;
  }
  /* Write into palette register */
  LCD_ENABLE_REG_ACCESS();
  WRITE_REG(0x01e2 / 2, Pos);   /* Select position */
  for (i = 0; i < 3; i++) {
    WRITE_REG(0x01e4 / 2, aColorSep[i] << 4);
  }
  LCD_ENABLE_MEM_ACCESS();
}

/*********************************************************************
*
*       _WaitForBltEnd
*/
static void _WaitForBltEnd(void) {
  volatile U16 tmp;

  do {
    READ_REG(0x100 / 2, tmp);
  } while (tmp & 0x80);
  READ_REG(0x100000 / 2, tmp);                                            /* dummy read */
}

/*********************************************************************
*
*       _FillRect
*/
static void _FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) {
  int _x1, _y0, _y1, DrawMode;

  DrawMode = GUI_GetDrawMode();
  LCD_ENABLE_REG_ACCESS(); {
    for (;x0 <= x1; x0 += 1024) {
      _y0 = y0;
      _x1 = x1;
      if (_x1 > (x0 + 1023)) {
        _x1 = x0 + 1023;
      }
      for (;_y0 <= y1; _y0 += 1024) {
        _y1 = y1;
        if (_y1 > (_y0 + 1023)) {
          _y1 = _y0 + 1023;
        }
        BITBLT_SET_DESTINATION(x0, _y0);                                  /* set destination start address */
        WRITE_REG(0x110 / 2, (_x1 - x0));                                 /* set width */
        WRITE_REG(0x112 / 2, (_y1 - _y0));                                /* set height */
        WRITE_REG(0x118 / 2, (PixelIndex));                               /* set foreground color  */
        if (DrawMode & LCD_DRAWMODE_XOR) {
          WRITE_REG(0x102 / 2, 0x0605);                                   /* pattern fill, ~D   */
        } else {
          WRITE_REG(0x102 / 2, 0x0c00);                                   /* solid fill, no ROP */
        }
        BITBLT_SET_ACTIVE();                                              /* engage bitblt engine */
        _WaitForBltEnd();                                                 /* wait for pending blit to end */
      }
    }
  } LCD_ENABLE_MEM_ACCESS();
}

/*********************************************************************
*
*       _DrawBitmap_1bpp
*/
static void _DrawBitmap_1bpp(int LayerIndex, int x, int y, U8 const * p, int Diff, int xsize, int ysize, int BytesPerLine, const LCD_PIXELINDEX * pTrans) {
  U8 const * pLine;
  U16 StartBit, Data;
  int NumWords, i, DrawMode;
  volatile U16 tmp;

  DrawMode = GUI_GetDrawMode();
  x += Diff;
  LCD_ENABLE_REG_ACCESS(); {
    StartBit = 7 - (Diff & 7);
    Data = StartBit | ((DrawMode & LCD_DRAWMODE_TRANS) ? 0x900 : 0x800);
    NumWords = ((Diff & 7) + xsize + 15) >> 4;
    WRITE_REG(0x102 / 2, Data);                                           /* set start bit and operation */
    WRITE_REG(0x104 / 2, 0);                                              /* set source start address */
    BITBLT_SET_DESTINATION(x, y);                                         /* set destination start address */
    WRITE_REG(0x110 / 2, (xsize - 1));                                    /* set width */
    WRITE_REG(0x112 / 2, (ysize - 1));                                    /* set height */
    WRITE_REG(0x114 / 2, (*(pTrans + 0)));                                /* set background color */
    WRITE_REG(0x118 / 2, (*(pTrans + 1)));                                /* set foreground color  */
    BITBLT_SET_ACTIVE();                                                  /* engage bitblt engine */
    do {
      READ_REG(0x100 / 2, tmp);
    } while ((tmp & 0x80) == 0);
    for (;ysize; ysize--, p += BytesPerLine) {
      pLine= p;
      for (i = NumWords; i; i--, pLine += 2) {
        do {
          READ_REG(0x100 / 2, tmp);
        } while ((tmp & 0x40) == 0x40);
        WRITE_REG(0x100000 / 2, ((*pLine) | ((*(pLine + 1)) << 8)));      /* write data into FIFO */
      }
    }
    _WaitForBltEnd();                                                     /* wait for pending blit to end */
  } LCD_ENABLE_MEM_ACCESS();
}

/*********************************************************************
*
*       _InitController
*/
static void _InitController(void) {
  LCD_WRITE_REG(0x000 >> 1,0x00);     /* needs 0 to enable accesses */
  LCD_WRITE_REG(0x1fc >> 1,0x00);     /* disable display while init */
  LCD_WRITE_REGLH(0x04 >> 1,LCD_REG4, LCD_REG5);
  LCD_WRITE_REGLH(0x08 >> 1,LCD_REG8, LCD_REG9);
  LCD_WRITE_REG(0x10 >> 1,LCD_REG10);
  LCD_WRITE_REG(0x14 >> 1,LCD_REG14);
  LCD_WRITE_REG(0x18 >> 1,LCD_REG18);
  LCD_WRITE_REG(0x1c >> 1,LCD_REG1C);
  LCD_WRITE_REG(0x1e >> 1,LCD_REG1E);
  LCD_WRITE_REG8(0x21,  LCD_REG21);     /* avoids clearing of REG20 here*/
  LCD_WRITE_REGLH(0x2a >> 1,LCD_REG2A, LCD_REG2B);
  LCD_WRITE_REG8(0x20,  0x80);          /* once after RESET: init DRAM */
  LCD_WRITE_REGLH(0x30 >> 1,LCD_REG30, LCD_REG31);
  LCD_WRITE_REG(0x32 >> 1,LCD_REG32);
  LCD_WRITE_REGLH(0x34 >> 1,LCD_REG34, LCD_REG35);
  LCD_WRITE_REG(0x36 >> 1,LCD_REG36);
  LCD_WRITE_REGLH(0x38 >> 1,LCD_REG38, LCD_REG39);
  LCD_WRITE_REGLH(0x3a >> 1,LCD_REG3A, LCD_REG3B);
  LCD_WRITE_REG(0x3c >> 1,LCD_REG3C);
  LCD_WRITE_REGLH(0x40 >> 1,LCD_REG40, LCD_REG41);
  LCD_WRITE_REGLH(0x42 >> 1,LCD_REG42, LCD_REG43);
  LCD_WRITE_REG(0x44 >> 1,LCD_REG44);
  LCD_WRITE_REGLH(0x46 >> 1,LCD_REG46, LCD_REG47);
  LCD_WRITE_REG(0x48 >> 1,LCD_REG48);
  LCD_WRITE_REGLH(0x4a >> 1,LCD_REG4A, LCD_REG4B);
  LCD_WRITE_REG(0x50 >> 1,LCD_REG50);
  LCD_WRITE_REGLH(0x52 >> 1,LCD_REG52, LCD_REG53);
  LCD_WRITE_REG(0x54 >> 1,LCD_REG54);
  LCD_WRITE_REGLH(0x56 >> 1,LCD_REG56, LCD_REG57);
  LCD_WRITE_REGLH(0x58 >> 1,LCD_REG58, LCD_REG59);
  LCD_WRITE_REGLH(0x5a >> 1,LCD_REG5A, LCD_REG5B);
  LCD_WRITE_REG(0x60 >> 1,LCD_REG60);
  LCD_WRITE_REGLH(0x62 >> 1,LCD_REG62, LCD_REG63);
  LCD_WRITE_REG(0x64 >> 1,LCD_REG64);
  LCD_WRITE_REGLH(0x66 >> 1,LCD_REG66, LCD_REG67);
  LCD_WRITE_REG(0x68 >> 1,LCD_REG68);
  LCD_WRITE_REGLH(0x6a >> 1,LCD_REG6A, LCD_REG6B);
  LCD_WRITE_REGLH(0x70 >> 1,LCD_REG70, LCD_REG71);
  LCD_WRITE_REGLH(0x72 >> 1,LCD_REG72, LCD_REG73);
  LCD_WRITE_REGLH(0x74 >> 1,LCD_REG74, LCD_REG75);
  LCD_WRITE_REGLH(0x76 >> 1,LCD_REG76, LCD_REG77);
  LCD_WRITE_REG(0x78 >> 1,LCD_REG78);
  LCD_WRITE_REGLH(0x7a >> 1,LCD_REG7A, LCD_REG7B);
  LCD_WRITE_REG(0x7c >> 1,LCD_REG7C);
  LCD_WRITE_REG(0x7e >> 1,LCD_REG7E);
  LCD_WRITE_REGLH(0x80 >> 1,LCD_REG80, LCD_REG81);
  LCD_WRITE_REGLH(0x82 >> 1,LCD_REG82, LCD_REG83);
  LCD_WRITE_REGLH(0x84 >> 1,LCD_REG84, LCD_REG85);
  LCD_WRITE_REGLH(0x86 >> 1,LCD_REG86, LCD_REG87);
  LCD_WRITE_REG(0x88 >> 1,LCD_REG88);
  LCD_WRITE_REGLH(0x8a >> 1,LCD_REG8A, LCD_REG8B);
  LCD_WRITE_REG(0x8c >> 1,LCD_REG8C);
  LCD_WRITE_REG(0x8e >> 1,LCD_REG8E);
  LCD_WRITE_REGLH(0x100 >> 1,LCD_REG100, LCD_REG101);
  LCD_WRITE_REGLH(0x102 >> 1,LCD_REG102, LCD_REG103);
  LCD_WRITE_REGLH(0x104 >> 1,LCD_REG104, LCD_REG105);
  LCD_WRITE_REG(0x106 >> 1,LCD_REG106);
  LCD_WRITE_REGLH(0x108 >> 1,LCD_REG108, LCD_REG109);
  LCD_WRITE_REG(0x10a >> 1,LCD_REG10A);
  LCD_WRITE_REGLH(0x10c >> 1,LCD_REG10C, LCD_REG10D);
  LCD_WRITE_REGLH(0x110 >> 1,LCD_REG110, LCD_REG111);
  LCD_WRITE_REGLH(0x112 >> 1,LCD_REG112, LCD_REG113);
  LCD_WRITE_REGLH(0x114 >> 1,LCD_REG114, LCD_REG115);
  LCD_WRITE_REGLH(0x118 >> 1,LCD_REG118, LCD_REG119);
  LCD_WRITE_REG(0x1e0 >> 1,LCD_REG1E0);     /* set LUT  */
  LCD_WRITE_REG(0x1e2 >> 1,LCD_REG1E2);     /* reset LUT base address */
  LCD_WRITE_REGLH(0x1f0 >> 1,LCD_REG1F0, LCD_REG1F1);
  LCD_WRITE_REG(0x1f4 >> 1,LCD_REG1F4);
  LCD_WRITE_REG(0x1fc >> 1,LCD_REG1FC); /* enable LCD after init */
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
  LCD_SetVRAMAddrEx(0, (void *)LCD_CTRLR_STARTADR);
  //
  // Set BitBLT-function for filling
  //
  LCD_SetDevFunc(0, LCD_DEVFUNC_FILLRECT, (void (*)(void))_FillRect);
  //
  // Set BitBLT-function for 1bpp bitmaps
  //
  LCD_SetDevFunc(0, LCD_DEVFUNC_DRAWBMP_1BPP, (void (*)(void))_DrawBitmap_1bpp);
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
  #ifndef WIN32
  case LCD_X_INITCONTROLLER: {
    _InitController();
    return 0;
  }
  case LCD_X_SETLUTENTRY: {
    LCD_X_SETLUTENTRY_INFO * p;
    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _SetLUTEntry(p->Color, p->Pos);
    return 0;
  }
  case LCD_X_ON:
    LCD_WRITE_REG(0x01fc >> 1, 0x03);
    break;
  case LCD_X_OFF:
    LCD_WRITE_REG(0x01fc >> 1, 0x00);
    break;
  #endif
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/
