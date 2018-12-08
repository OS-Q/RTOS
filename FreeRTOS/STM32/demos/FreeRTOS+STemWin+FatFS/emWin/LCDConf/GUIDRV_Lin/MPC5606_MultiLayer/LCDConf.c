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
Purpose     : Display controller configuration
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>

#include "GUI.h"

#include "GUIDRV_Lin.h"

/*********************************************************************
*
*       Layer configuration, to be modified
*
**********************************************************************
*/
//
// Physical layer size
//
#define _XSIZE_PHYS_0 64
#define _YSIZE_PHYS_0 64
#define _XSIZE_PHYS_1 64
#define _YSIZE_PHYS_1 64
#define _XSIZE_PHYS_2 64
#define _YSIZE_PHYS_2 64
#define _XSIZE_PHYS_3 64
#define _YSIZE_PHYS_3 64
#define _XSIZE_PHYS_4 64
#define _YSIZE_PHYS_4 64
#define _XSIZE_PHYS_5 64
#define _YSIZE_PHYS_5 64
#define _XSIZE_PHYS_6 64
#define _YSIZE_PHYS_6 64
#define _XSIZE_PHYS_7 64
#define _YSIZE_PHYS_7 64

//
// Virtual display size
//
#define _VXSIZE_PHYS_0 _XSIZE_PHYS_0
#define _VYSIZE_PHYS_0 _YSIZE_PHYS_0
#define _VXSIZE_PHYS_1 _XSIZE_PHYS_1
#define _VYSIZE_PHYS_1 _YSIZE_PHYS_1
#define _VXSIZE_PHYS_2 _XSIZE_PHYS_2
#define _VYSIZE_PHYS_2 _YSIZE_PHYS_2
#define _VXSIZE_PHYS_3 _XSIZE_PHYS_3
#define _VYSIZE_PHYS_3 _YSIZE_PHYS_3
#define _VXSIZE_PHYS_4 _XSIZE_PHYS_4
#define _VYSIZE_PHYS_4 _YSIZE_PHYS_4
#define _VXSIZE_PHYS_5 _XSIZE_PHYS_5
#define _VYSIZE_PHYS_5 _YSIZE_PHYS_5
#define _VXSIZE_PHYS_6 _XSIZE_PHYS_6
#define _VYSIZE_PHYS_6 _YSIZE_PHYS_6
#define _VXSIZE_PHYS_7 _XSIZE_PHYS_7
#define _VYSIZE_PHYS_7 _YSIZE_PHYS_7

//
// Initial layer position
//
#define _XPOS_0 0
#define _YPOS_0 0
#define _XPOS_1 80
#define _YPOS_1 0
#define _XPOS_2 160
#define _YPOS_2 0
#define _XPOS_3 0
#define _YPOS_3 80
#define _XPOS_4 80
#define _YPOS_4 80
#define _XPOS_5 160
#define _YPOS_5 80
#define _XPOS_6 0
#define _YPOS_6 160
#define _XPOS_7 80
#define _YPOS_7 160

//
// Color conversion and driver
//
#define _COLORCONV_0 &_API_ColorConv_8888
#define _DRIVER_0    GUIDRV_LIN_32
#define _COLORCONV_1 &_API_ColorConv_565
#define _DRIVER_1    GUIDRV_LIN_16
#define _COLORCONV_2 &_API_ColorConv_565
#define _DRIVER_2    GUIDRV_LIN_OSY_16
#define _COLORCONV_3 GUICC_8666
#define _DRIVER_3    GUIDRV_LIN_8
#define _COLORCONV_4 GUICC_8666
#define _DRIVER_4    GUIDRV_LIN_8
#define _COLORCONV_5 GUICC_0
#define _DRIVER_5    GUIDRV_LIN_4
#define _COLORCONV_6 GUICC_2
#define _DRIVER_6    GUIDRV_LIN_2
#define _COLORCONV_7 GUICC_1
#define _DRIVER_7    GUIDRV_LIN_1

//
// Palette definition
//
#define _PALETTE_5 &_aPalette_16

/*********************************************************************
*
*       Generic configuration and checking
*
* Purpose:
*   Checks if the layer configurations are complete and defines the
*   required macros for the layer configuration table.
*   Works up to 8 layers and can easily be enhanced.
*
**********************************************************************
*/
/*********************************************************************
*
*       Layer 0
*/
#if    defined(_XSIZE_PHYS_0)
  #if !defined(_YSIZE_PHYS_0)  \
   || !defined(_VXSIZE_PHYS_0) \
   || !defined(_VYSIZE_PHYS_0) \
   || !defined(_COLORCONV_0)   \
   || !defined(_DRIVER_0)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_0
      #define _XPOS_0 0
    #endif
    #ifndef   _YPOS_0
      #define _YPOS_0 0
    #endif
    #ifndef   _PALETTE_0
      #define _PALETTE_0 NULL
    #endif
    #define _SCREENSIZE_0   _XSIZE_PHYS_0,  _YSIZE_PHYS_0
    #define _VIRTUALSIZE_0  _VXSIZE_PHYS_0, _VYSIZE_PHYS_0
    #define _CONFIGURATON_0 _SCREENSIZE_0,  _VIRTUALSIZE_0, _XPOS_0, _YPOS_0, _COLORCONV_0
  #endif
#endif

/*********************************************************************
*
*       Layer 1
*/
#if    defined(_XSIZE_PHYS_1)
  #if !defined(_YSIZE_PHYS_1)  \
   || !defined(_VXSIZE_PHYS_1) \
   || !defined(_VYSIZE_PHYS_1) \
   || !defined(_COLORCONV_1)   \
   || !defined(_DRIVER_1)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_1
      #define _XPOS_1 0
    #endif
    #ifndef   _YPOS_1
      #define _YPOS_1 0
    #endif
    #ifndef   _PALETTE_1
      #define _PALETTE_1 NULL
    #endif
    #define _SCREENSIZE_1   _XSIZE_PHYS_1,  _YSIZE_PHYS_1
    #define _VIRTUALSIZE_1  _VXSIZE_PHYS_1, _VYSIZE_PHYS_1
    #define _CONFIGURATON_1 _SCREENSIZE_1,  _VIRTUALSIZE_1, _XPOS_1, _YPOS_1, _COLORCONV_1
  #endif
#endif

/*********************************************************************
*
*       Layer 2
*/
#if    defined(_XSIZE_PHYS_2)
  #if !defined(_YSIZE_PHYS_2)  \
   || !defined(_VXSIZE_PHYS_2) \
   || !defined(_VYSIZE_PHYS_2) \
   || !defined(_COLORCONV_2)   \
   || !defined(_DRIVER_2)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_2
      #define _XPOS_2 0
    #endif
    #ifndef   _YPOS_2
      #define _YPOS_2 0
    #endif
    #ifndef   _PALETTE_2
      #define _PALETTE_2 NULL
    #endif
    #define _SCREENSIZE_2   _XSIZE_PHYS_2,  _YSIZE_PHYS_2
    #define _VIRTUALSIZE_2  _VXSIZE_PHYS_2, _VYSIZE_PHYS_2
    #define _CONFIGURATON_2 _SCREENSIZE_2,  _VIRTUALSIZE_2, _XPOS_2, _YPOS_2, _COLORCONV_2
  #endif
#endif

/*********************************************************************
*
*       Layer 3
*/
#if    defined(_XSIZE_PHYS_3)
  #if !defined(_YSIZE_PHYS_3)  \
   || !defined(_VXSIZE_PHYS_3) \
   || !defined(_VYSIZE_PHYS_3) \
   || !defined(_COLORCONV_3)   \
   || !defined(_DRIVER_3)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_3
      #define _XPOS_3 0
    #endif
    #ifndef   _YPOS_3
      #define _YPOS_3 0
    #endif
    #ifndef   _PALETTE_3
      #define _PALETTE_3 NULL
    #endif
    #define _SCREENSIZE_3   _XSIZE_PHYS_3,  _YSIZE_PHYS_3
    #define _VIRTUALSIZE_3  _VXSIZE_PHYS_3, _VYSIZE_PHYS_3
    #define _CONFIGURATON_3 _SCREENSIZE_3,  _VIRTUALSIZE_3, _XPOS_3, _YPOS_3, _COLORCONV_3
  #endif
#endif

/*********************************************************************
*
*       Layer 4
*/
#if    defined(_XSIZE_PHYS_4)
  #if !defined(_YSIZE_PHYS_4)  \
   || !defined(_VXSIZE_PHYS_4) \
   || !defined(_VYSIZE_PHYS_4) \
   || !defined(_COLORCONV_4)   \
   || !defined(_DRIVER_4)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_4
      #define _XPOS_4 0
    #endif
    #ifndef   _YPOS_4
      #define _YPOS_4 0
    #endif
    #ifndef   _PALETTE_4
      #define _PALETTE_4 NULL
    #endif
    #define _SCREENSIZE_4   _XSIZE_PHYS_4,  _YSIZE_PHYS_4
    #define _VIRTUALSIZE_4  _VXSIZE_PHYS_4, _VYSIZE_PHYS_4
    #define _CONFIGURATON_4 _SCREENSIZE_4,  _VIRTUALSIZE_4, _XPOS_4, _YPOS_4, _COLORCONV_4
  #endif
#endif

/*********************************************************************
*
*       Layer 5
*/
#if    defined(_XSIZE_PHYS_5)
  #if !defined(_YSIZE_PHYS_5)  \
   || !defined(_VXSIZE_PHYS_5) \
   || !defined(_VYSIZE_PHYS_5) \
   || !defined(_COLORCONV_5)   \
   || !defined(_DRIVER_5)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_5
      #define _XPOS_5 0
    #endif
    #ifndef   _YPOS_5
      #define _YPOS_5 0
    #endif
    #ifndef   _PALETTE_5
      #define _PALETTE_5 NULL
    #endif
    #define _SCREENSIZE_5   _XSIZE_PHYS_5,  _YSIZE_PHYS_5
    #define _VIRTUALSIZE_5  _VXSIZE_PHYS_5, _VYSIZE_PHYS_5
    #define _CONFIGURATON_5 _SCREENSIZE_5,  _VIRTUALSIZE_5, _XPOS_5, _YPOS_5, _COLORCONV_5
  #endif
#endif

/*********************************************************************
*
*       Layer 6
*/
#if    defined(_XSIZE_PHYS_6)
  #if !defined(_YSIZE_PHYS_6)  \
   || !defined(_VXSIZE_PHYS_6) \
   || !defined(_VYSIZE_PHYS_6) \
   || !defined(_COLORCONV_6)   \
   || !defined(_DRIVER_6)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_6
      #define _XPOS_6 0
    #endif
    #ifndef   _YPOS_6
      #define _YPOS_6 0
    #endif
    #ifndef   _PALETTE_6
      #define _PALETTE_6 NULL
    #endif
    #define _SCREENSIZE_6   _XSIZE_PHYS_6,  _YSIZE_PHYS_6
    #define _VIRTUALSIZE_6  _VXSIZE_PHYS_6, _VYSIZE_PHYS_6
    #define _CONFIGURATON_6 _SCREENSIZE_6,  _VIRTUALSIZE_6, _XPOS_6, _YPOS_6, _COLORCONV_6
  #endif
#endif

/*********************************************************************
*
*       Layer 7
*/
#if    defined(_XSIZE_PHYS_7)
  #if !defined(_YSIZE_PHYS_7)  \
   || !defined(_VXSIZE_PHYS_7) \
   || !defined(_VYSIZE_PHYS_7) \
   || !defined(_COLORCONV_7)   \
   || !defined(_DRIVER_7)
    #error Incomplete layer configuration!
  #else
    #ifndef   _XPOS_7
      #define _XPOS_7 0
    #endif
    #ifndef   _YPOS_7
      #define _YPOS_7 0
    #endif
    #ifndef   _PALETTE_7
      #define _PALETTE_7 NULL
    #endif
    #define _SCREENSIZE_7   _XSIZE_PHYS_7,  _YSIZE_PHYS_7
    #define _VIRTUALSIZE_7  _VXSIZE_PHYS_7, _VYSIZE_PHYS_7
    #define _CONFIGURATON_7 _SCREENSIZE_7,  _VIRTUALSIZE_7, _XPOS_7, _YPOS_7, _COLORCONV_7
  #endif
#endif

/*********************************************************************
*
*       Register definitions
*/
#define CTRLDESCL1_HEIGHT      10, 16
#define CTRLDESCL1_WIDTH       8, 0

#define CTRLDESCL2_POSY        10, 16
#define CTRLDESCL2_POSX        8, 0

#define CTRLDESCL4_EN          1, 31
#define CTRLDESCL4_TRANS       8, 20

#define CTRLDESCL4_BPP         4, 16
#define CTRLDESCL4_LUOFFS      12, 3
#define CTRLDESCL4_BB          1, 2
#define CTRLDESCL4_AB          2, 0

#define CTRLDESCL5_CKMAX_R     8, 16
#define CTRLDESCL5_CKMAX_G     8, 8
#define CTRLDESCL5_CKMAX_B     8, 0

#define CTRLDESCL6_CKMIN_R     8, 16
#define CTRLDESCL6_CKMIN_G     8, 8
#define CTRLDESCL6_CKMIN_B     8, 0

#define CTRLDESCL7_TILEVERSIZE 10, 16
#define CTRLDESCL7_TILEHORSIZE 8, 0

#define DCUMODE_DCUMODE        1, 0

#define BGND_R                 8, 16
#define BGND_G                 8, 8
#define BGND_B                 8, 0

#define INTSTATUS_VSBLANK      1, 3

#define _DCU (*(volatile struct _DCU_tag *) 0xFFE7C000UL)

/*********************************************************************
*
*       Defines for 1555 mode
*/
#define B_BITS_1555 5
#define G_BITS_1555 5
#define R_BITS_1555 5

#define R_MASK_1555 ((1 << R_BITS_1555) - 1)
#define G_MASK_1555 ((1 << G_BITS_1555) - 1)
#define B_MASK_1555 ((1 << B_BITS_1555) - 1)

/*********************************************************************
*
*       Defines for 565 mode
*/
#define B_BITS_565 5
#define G_BITS_565 6
#define R_BITS_565 5

#define R_MASK_565 ((1 << R_BITS_565) - 1)
#define G_MASK_565 ((1 << G_BITS_565) - 1)
#define B_MASK_565 ((1 << B_BITS_565) - 1)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       _CONFIG_PARA
*/
typedef struct {
  int xSize, ySize, vxSize, vySize, xPos, yPos;
  const LCD_API_COLOR_CONV * pColorConv;
  const GUI_DEVICE_API     * pDisplayDriver;
  const LCD_PHYSPALETTE    * pPalette;
} _CONFIG_PARA;

/*********************************************************************
*
*       _DCU_LAYER_tag
*/
#ifndef WIN32
struct _DCU_LAYER_tag {
  volatile U32 CTRLDESCL1;
  volatile U32 CTRLDESCL2;
  volatile U32 CTRLDESCL3;
  volatile U32 CTRLDESCL4;
  volatile U32 CTRLDESCL5;
  volatile U32 CTRLDESCL6;
  volatile U32 CTRLDESCL7;
};
#endif

/*********************************************************************
*
*       _DCU_tag
*/
#ifndef WIN32
struct _DCU_tag {
  struct _DCU_LAYER_tag aLayer[16];
  volatile U32 aDummy0[4];
  volatile U32 DCUMODE;
  volatile U32 BGND;
  volatile U32 aDummy1[5];
  volatile U32 INTSTATUS;
  volatile U32 aDummy2[900];
  volatile U32 aCLUT[0x400];
};
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _aPalette_16
*
* Purpose:
*   User defined palette for 4bpp color mode
*/
static const LCD_COLOR _aColors_16[] = {
  0x000000,  0x0000FF,  0x00FF00,  0x00FFFF,  0xFF0000,  0xFF00FF,  0xFFFF00,  0xFFFFFF,
  0x000000,  0x000080,  0x008000,  0x008080,  0x800000,  0x800080,  0x808000,  0x808080,
};

static const LCD_PHYSPALETTE _aPalette_16 = {
  GUI_COUNTOF(_aColors_16), _aColors_16
};

/*********************************************************************
*
*       Static code: Color conversion routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _Color2Index_4444
*/
static unsigned _Color2Index_4444(LCD_COLOR Color) {
  U32 r, g, b, a, Index;
  
  //
  // Calculate index
  //
  r = Color         & 255;
  g = (Color >> 8)  & 255;
  b = (Color >> 16) & 255;
  a = (Color >> 24);
  r = (r + 8) / 17;
  g = (g + 8) / 17;
  b = (b + 8) / 17;
  a = (a + 8) / 17;
  Index = r + (g << 4) + (b << 8) + (a << 12);
  //
  // Swap bytes
  //
  Index = (Index << 8) | (Index >> 8);
  return Index;
}

/*********************************************************************
*
*       _Index2Color_4444
*/
static LCD_COLOR _Index2Color_4444(unsigned Index) {
  U32 r, g, b, a;
  
  //
  // Swap bytes
  //
  Index = (Index << 8) | (Index >> 8);
  //
  // Calculate color
  //
  r =  Index        & 0xf;
  g = (Index >>  4) & 0xf;
  b = (Index >>  8) & 0xf;
  a = (Index >> 12) & 0xf;
  r = r * 17;
  g = g * 17;
  b = b * 17;
  a = a * 17;
  return r + (g << 8) + (b << 16) + (a << 24);
}

/*********************************************************************
*
*       _GetIndexMask_4444
*/
static unsigned _GetIndexMask_4444(void) {
  return 0x0fff;
}

/*********************************************************************
*
*       _Color2Index_1555
*/
static unsigned _Color2Index_1555(LCD_COLOR Color) {
  U32 r, g, b, a, Index;
  
  //
  // Calculate index
  //
  r = (Color >> (8  - R_BITS_1555)) & R_MASK_1555;
  g = (Color >> (16 - G_BITS_1555)) & G_MASK_1555;
  b = (Color >> (24 - B_BITS_1555)) & B_MASK_1555;
  a = (Color & 0xFF000000) ? 1 : 0;
  Index = r + (g << R_BITS_1555) + (b << (G_BITS_1555 + R_BITS_1555)) + (a << 15);
  //
  // Swap bytes
  //
  Index = ((Index << 8) | (Index >> 8)) & 0xffff;
  return Index;
}

/*********************************************************************
*
*       _Index2Color_1555
*/
static LCD_COLOR _Index2Color_1555(unsigned Index) {
  U32 r, g, b, a;
  
  //
  // Swap bytes
  //
  Index = (Index << 8) | (Index >> 8);
  //
  // Calculate color
  //
  r = Index                                  & R_MASK_1555;
  g = (Index >> R_BITS_1555)                 & G_MASK_1555;
  b = (Index >> (R_BITS_1555 + G_BITS_1555)) & B_MASK_1555;
  a =  Index >> 15;
  r = r * 255 / R_MASK_1555;
  g = g * 255 / G_MASK_1555;
  b = b * 255 / B_MASK_1555;
  return r + (g << 8) + (b << 16);
}

/*********************************************************************
*
*       _GetIndexMask_1555
*/
static unsigned _GetIndexMask_1555(void) {
  return 0x7fff;
}

/*********************************************************************
*
*       _Color2Index_565
*/
static unsigned _Color2Index_565(LCD_COLOR Color) {
  int r, g, b;
  unsigned Index;

  //
  // Calculate index
  //
  r = (Color>> (8  - R_BITS_565)) & R_MASK_565;
  g = (Color>> (16 - G_BITS_565)) & G_MASK_565;
  b = (Color>> (24 - B_BITS_565)) & B_MASK_565;
  Index = b + (g << B_BITS_565) + (r << (G_BITS_565 + B_BITS_565));
  //
  // Swap bytes
  //
  Index = ((Index << 8) | (Index >> 8)) & 0xffff;
  return Index;
}

/*********************************************************************
*
*       _Index2Color_565
*/
static LCD_COLOR _Index2Color_565(unsigned Index) {
  unsigned int r, g, b;
  LCD_COLOR Color;
  
  //
  // Swap bytes
  //
  Index = (Index << 8) | (Index >> 8);
  //
  // Calculate color
  //
  b =  Index                               & B_MASK_565;
  g = (Index >> B_BITS_565)                & G_MASK_565;
  r = (Index >> (B_BITS_565 + G_BITS_565)) & R_MASK_565;
  r = r * 255 / R_MASK_565;
  g = g * 255 / G_MASK_565;
  b = b * 255 / B_MASK_565;
  Color = r + (g << 8) + (((U32)b) << 16);
  return Color;
}

/*********************************************************************
*
*       _GetIndexMask_565
*/
static unsigned _GetIndexMask_565(void) {
  return 0xffff;
}

/*********************************************************************
*
*       _Color2Index_8888
*/
static unsigned _Color2Index_8888(LCD_COLOR Color) {
  U32 r, g, b, a, Index;
  
  r =  Color & 0x000000ff;
  g = (Color & 0x0000ff00) >>  8;
  b = (Color & 0x00ff0000) >> 16;
  a = 255 - ((Color & 0xff000000) >> 24);
  Index = a | (r << 8) | (g << 16) | (b << 24);
  return Index;
}

/*********************************************************************
*
*       _Index2Color_8888
*/
static LCD_COLOR _Index2Color_8888(unsigned Index) {
  U32 r, g, b, a, Color;

  a = 255 - (Index & 0xff);
  r = (Index & 0x0000ff00) >>  8;
  g = (Index & 0x00ff0000) >> 16;
  b = (Index & 0xff000000) >> 24;
  Color = r | (b << 8) | (g << 16) | (a << 24);
  return Color;
}

/*********************************************************************
*
*       _GetIndexMask_8888
*/
static unsigned _GetIndexMask_8888(void) {
  return 0xffffffff;
}

/*********************************************************************
*
*       Static data: API tables for color conversion
*
**********************************************************************
*/
/*********************************************************************
*
*       _API_ColorConv_4444 (to be used in the next HW version)
*/
static const LCD_API_COLOR_CONV _API_ColorConv_4444 = {
  _Color2Index_4444,
  _Index2Color_4444,
  _GetIndexMask_4444
};

/*********************************************************************
*
*       _API_ColorConv_1555 (to be used in the next HW version)
*/
static const LCD_API_COLOR_CONV _API_ColorConv_1555 = {
  _Color2Index_1555,
  _Index2Color_1555,
  _GetIndexMask_1555
};

/*********************************************************************
*
*       _API_ColorConv_565
*/
static const LCD_API_COLOR_CONV _API_ColorConv_565 = {
  _Color2Index_565,
  _Index2Color_565,
  _GetIndexMask_565
};

/*********************************************************************
*
*       _API_ColorConv_8888
*/
static const LCD_API_COLOR_CONV _API_ColorConv_8888 = {
  _Color2Index_8888,
  _Index2Color_8888,
  _GetIndexMask_8888
};

/*********************************************************************
*
*       Static data: Layer initialization structure
*
**********************************************************************
*/
/*********************************************************************
*
*       _aConfigPara
*/
static const _CONFIG_PARA _aConfigPara[] = {
  //
  // Display driver selection
  //
  #if defined(_XSIZE_PHYS_0)
    { _CONFIGURATON_0, _DRIVER_0, _PALETTE_0},
  #endif
  #if defined(_XSIZE_PHYS_1)
    { _CONFIGURATON_1, _DRIVER_1, _PALETTE_1},
  #endif
  #if defined(_XSIZE_PHYS_2)
    { _CONFIGURATON_2, _DRIVER_2, _PALETTE_2},
  #endif
  #if defined(_XSIZE_PHYS_3)
    { _CONFIGURATON_3, _DRIVER_3, _PALETTE_3},
  #endif
  #if defined(_XSIZE_PHYS_4)
    { _CONFIGURATON_4, _DRIVER_4, _PALETTE_4},
  #endif
  #if defined(_XSIZE_PHYS_5)
    { _CONFIGURATON_5, _DRIVER_5, _PALETTE_5},
  #endif
  #if defined(_XSIZE_PHYS_6)
    { _CONFIGURATON_6, _DRIVER_6, _PALETTE_6},
  #endif
  #if defined(_XSIZE_PHYS_7)
    { _CONFIGURATON_7, _DRIVER_7, _PALETTE_7},
  #endif
};

/*********************************************************************
*
*       Static code: Display driver callback routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _SetValue
*/
#ifndef WIN32
static void _SetValue(volatile U32 * pReg, int NumBits, int Pos, U32 Value) {
  U32 Mask;

  Mask = ((1 << NumBits) - 1);
  *pReg &= ~(Mask  << Pos);
  *pReg |=  (Value << Pos);
}

/*********************************************************************
*
*       _GetValue
*/
static U32 _GetValue(volatile U32 * pReg, int NumBits, int Pos) {
  U32 Mask, r;

  Mask = ((1 << NumBits) - 1);
  r = (*pReg & (Mask << Pos)) >> Pos;
  return r;
}

/*********************************************************************
*
*       _WaitUntilBlanking
*
* Purpose:
*   The function waits until the DCU is in a non blanking period.
*/
static void _WaitUntilBlanking(void) {
  if (_GetValue(&_DCU.DCUMODE, DCUMODE_DCUMODE)) {
    if (_GetValue(&_DCU.INTSTATUS, INTSTATUS_VSBLANK)) {
      while (_GetValue(&_DCU.INTSTATUS, INTSTATUS_VSBLANK) == 0);
    }
    _SetValue(&_DCU.INTSTATUS, INTSTATUS_VSBLANK, 1);
  }
}

/*********************************************************************
*
*       _GetLUTOffset
*
* Purpose:
*   The function calculates the offset to be used to access the LUT
*   in the LUOFFS field of the CtrlDescLx_4 register.
*/
static int _GetLUTOffset(unsigned LayerIndex) {
  int BitsPerPixel;
  int OffsetLUT;
  int i;
  
  //
  // Swap layer index
  //
  LayerIndex = GUI_COUNTOF(_aConfigPara) - LayerIndex - 1;
  OffsetLUT = 0;
  for (i = 0; i < LayerIndex; i++) {
    BitsPerPixel = LCD_GetBitsPerPixelEx(i);
    switch (BitsPerPixel) {
    case 1:
      OffsetLUT += 2;
      break;
    case 2:
      OffsetLUT += 4;
      break;
    case 4:
      OffsetLUT += 16;
      break;
    case 8:
      OffsetLUT += 256;
      break;
    }
  }
  return OffsetLUT;
}

/*********************************************************************
*
*       _GetBPPValue
*
* Purpose:
*   The function calculates the BPP value to be stored in the BPP field
*   of the CtrlDescLx_4 register. 
*/
static int _GetBPPValue(unsigned LayerIndex) {
  int NumColors;
  int Value = -1;
  //
  // Swap layer index
  //
  LayerIndex = GUI_COUNTOF(_aConfigPara) - LayerIndex - 1;
  //
  // Get right value for BPP bits
  //
  if        (_aConfigPara[LayerIndex].pColorConv == GUICC_1) {
    Value =  0; // 1 bpp
  } else if (_aConfigPara[LayerIndex].pColorConv == GUICC_2) {
    Value =  1; // 2 bpp
  } else if (_aConfigPara[LayerIndex].pColorConv == GUICC_4) {
    Value =  2; // 4 bpp
  } else if (_aConfigPara[LayerIndex].pColorConv == GUICC_8666) {
    Value =  3; // 8 bpp
  } else if (_aConfigPara[LayerIndex].pColorConv == &_API_ColorConv_565) {
    Value =  4; // 16 bpp
  } else if (_aConfigPara[LayerIndex].pColorConv == &_API_ColorConv_8888) {
    Value =  6; // 32 bpp
  } else if (_aConfigPara[LayerIndex].pColorConv == &_API_ColorConv_1555) {
    Value = 11; // 16 bpp(argb1555)
  } else if (_aConfigPara[LayerIndex].pColorConv == &_API_ColorConv_4444) {
    Value = 12; // 16 bpp(argb4444)
  } else if (_aConfigPara[LayerIndex].pColorConv == GUICC_0) {
    if (_aConfigPara[LayerIndex].pPalette) {
      //
      // No usable color conversion, so use number of colors for the decision
      //
      switch (_aConfigPara[LayerIndex].pPalette->NumEntries) {
      case 2:
        Value = 0; // 1 bpp
        break;
      case 4:
        Value = 1; // 2 bpp
        break;
      case 16:
        Value = 2; // 4 bpp
        break;
      case 256:
        Value = 3; // 8 bpp
        break;
      }
    }
  }
  return Value;
}

/*********************************************************************
*
*       _SetChroma
*/
static void _SetChroma(unsigned LayerIndex, LCD_COLOR ChromaMin, LCD_COLOR ChromaMax) {
  _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL6, CTRLDESCL6_CKMIN_R,  ChromaMin & 0x0000FF);        // Min red   chroma
  _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL6, CTRLDESCL6_CKMIN_G, (ChromaMin & 0x00FF00) >>  8); // Min green chroma
  _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL6, CTRLDESCL6_CKMIN_B, (ChromaMin & 0xFF0000) >> 16) ;// Min blue  chroma
  _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL5, CTRLDESCL5_CKMAX_R,  ChromaMax & 0x0000FF);        // Max red   chroma
  _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL5, CTRLDESCL5_CKMAX_G, (ChromaMax & 0x00FF00) >>  8); // Max green chroma
  _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL5, CTRLDESCL5_CKMAX_B, (ChromaMax & 0xFF0000) >> 16) ;// Max blue  chroma
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
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * p) {
  #ifndef WIN32

  U32 Entry;
  U32 Color;
  int Value;
  int i;
  unsigned OffsetLUT;

  //
  // Swap layer index
  //
  LayerIndex = GUI_COUNTOF(_aConfigPara) - LayerIndex - 1;
  //
  // Process command
  //
  _WaitUntilBlanking();
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
    if (LayerIndex == 0) {
      _DCU.BGND = 0x00000080;
    }
    //
    // LUT offset
    //
    OffsetLUT = _GetLUTOffset(LayerIndex);
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_LUOFFS, OffsetLUT);
    //
    // BPP mode
    //
    Value = _GetBPPValue(LayerIndex);
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_BPP, Value);
    //
    // Layer blending and enable
    //
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_AB, 0);
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_BB, 0);
    //
    // Chroma setting
    //
    _SetChroma(LayerIndex, 0, 0);
    //
    // Tile settings
    //
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL7, CTRLDESCL7_TILEVERSIZE, 0);
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL7, CTRLDESCL7_TILEHORSIZE, 0);
    break;
  }
  case LCD_X_SETVRAMADDR: {
    //
    // Required for setting the address of the video RAM
    //
    LCD_X_SETVRAMADDR_INFO * pData;

    pData = (LCD_X_SETVRAMADDR_INFO *)p;
    _DCU.aLayer[LayerIndex].CTRLDESCL3 = (U32)pData->pVRAM;
    break;
  }
  //
  // Optional
  //
  // The following commands are optional and its not cogently required to
  // adapt them except the according command needs to be supported
  // by the display driver
  //
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    _SetValue(&_DCU.DCUMODE, DCUMODE_DCUMODE, 0);
    break;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    _SetValue(&_DCU.DCUMODE, DCUMODE_DCUMODE, 1);
    break;
  }
  case LCD_X_SETALPHA: {
    //
    // Required for setting the alpha value which is passed in the 'Alpha' element of pData 
    //
    LCD_X_SETALPH_INFO * pData;

    pData = (LCD_X_SETALPH_INFO *)p;
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_TRANS, 255 - pData->Alpha);
    break;
  }
  case LCD_X_SETALPHAMODE: {
    //
    // Required for setting the alpha mode which is passed in the 'AlphaMode' element of pData 
    //
    LCD_X_SETALPHAMODE_INFO * pData;

    pData = (LCD_X_SETALPHAMODE_INFO *)p;
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_AB, pData->AlphaMode);
    break;
  }
  case LCD_X_SETCHROMA: {
    //
    // Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData 
    //
    LCD_X_SETCHROMA_INFO * pData;

    pData = (LCD_X_SETCHROMA_INFO *)p;
    _SetChroma(LayerIndex, pData->ChromaMin, pData->ChromaMax);
    break;
  }
  case LCD_X_SETCHROMAMODE: {
    //
    // Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData 
    //
    LCD_X_SETCHROMAMODE_INFO * pData;

    pData = (LCD_X_SETCHROMAMODE_INFO *)p;
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_BB, pData->ChromaMode);
    break;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of pData 
    //
    LCD_X_SETLUTENTRY_INFO * pData;

    pData = (LCD_X_SETLUTENTRY_INFO *)p;
    Color = pData->Color;
    Entry = ((Color & 0xff0000) >> 16) // blue
          |  (Color & 0x00ff00)        // green
          | ((Color & 0xff) << 16);    // red
    OffsetLUT = _GetLUTOffset(LayerIndex);
    _DCU.aCLUT[OffsetLUT + pData->Pos] = Entry;
    break;
  }
  case LCD_X_SETORG: {
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of pData 
    //
    break;
  }
  case LCD_X_SETPOS: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData 
    //
    LCD_X_SETPOS_INFO * pData;

    pData = (LCD_X_SETPOS_INFO *)p;
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL2, CTRLDESCL2_POSX, pData->xPos >> 4);
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL2, CTRLDESCL2_POSY, pData->yPos);
    break;
  }
  case LCD_X_SETSIZE: {
    //
    // Required for setting the layer size which is passed in the 'xSize' and 'xSize' element of pData 
    //
    LCD_X_SETSIZE_INFO * pData;

    pData = (LCD_X_SETSIZE_INFO *)p;
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL1, CTRLDESCL1_HEIGHT, pData->ySize);
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL1, CTRLDESCL1_WIDTH,  pData->xSize >> 4);
    break;
  }
  case LCD_X_SETVIS: {
    //
    // Required for setting the layer visibility which is passed in the 'OnOff' element of pData 
    //
    LCD_X_SETVIS_INFO * pData;

    pData = (LCD_X_SETVIS_INFO *)p;
    _SetValue(&_DCU.aLayer[LayerIndex].CTRLDESCL4, CTRLDESCL4_EN, pData->OnOff);
    break;
  }
  }
  
  #endif
  
  return 0;
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
  int i, BitsPerPixel;
  U32 Addr;

  //
  // Video RAM starts at...
  //
  Addr = 0x60000000;
  //
  // Display driver selection and layer configuration
  //
  for (i = 0; i < GUI_COUNTOF(_aConfigPara); i++) {
    //
    // Set display driver and color conversion for 1st layer
    //
    GUI_DEVICE_CreateAndLink(_aConfigPara[i].pDisplayDriver, _aConfigPara[i].pColorConv, 0, i);
    //
    // Display driver configuration
    //
    LCD_SetPosEx     (i, _aConfigPara[i].xPos,   _aConfigPara[i].yPos);
    LCD_SetSizeEx    (i, _aConfigPara[i].xSize,  _aConfigPara[i].ySize);
    LCD_SetVSizeEx   (i, _aConfigPara[i].vxSize, _aConfigPara[i].vySize);
    LCD_SetVRAMAddrEx(i, (void *)Addr);
    LCD_SetVisEx     (i, 1);
    //
    // Set user palette data (only required if no fixed palette is used)
    //
    if (_aConfigPara[i].pPalette) {
      LCD_SetLUTEx(i, _aConfigPara[i].pPalette);
    }
    //
    // Calculate video RAM address for the following layer
    //
    BitsPerPixel = LCD_GetBitsPerPixelEx(i);
    Addr += ((_aConfigPara[i].vxSize * BitsPerPixel) * _aConfigPara[i].vySize) / 8;
  }
}

/*************************** End of file ****************************/
