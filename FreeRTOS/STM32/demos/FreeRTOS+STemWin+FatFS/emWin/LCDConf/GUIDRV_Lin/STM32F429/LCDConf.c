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

The part between 'DISPLAY CONFIGURATION START' and 'DISPLAY CONFIGURA-
TION END' can be used to configure the following for each layer:

- Color mode
- Layer size
- Layer orientation

Further the background color used on positions without a valid layer
can be set here.

---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>
#include <intrinsics.h>

#include "GUI.h"
#include "GUI_Private.h"
#include "GUIDRV_Lin.h"

/*********************************************************************
*
*       Supported orientation modes (not to be changed)
*/
#define ROTATION_0   0
#define ROTATION_CW  1
#define ROTATION_180 2
#define ROTATION_CCW 3

/*********************************************************************
*
*       Supported color modes (not to be changed)
*/
//      Color mode       Conversion     Driver (default orientation)
//      ---------------------------------------------
#define CM_ARGB8888 1 // GUICC_M8888I - GUIDRV_LIN_32
#define CM_RGB888   2 // GUICC_M888   - GUIDRV_LIN_24
#define CM_RGB565   3 // GUICC_M565   - GUIDRV_LIN_16
#define CM_ARGB1555 4 // GUICC_M1555I - GUIDRV_LIN_16
#define CM_ARGB4444 5 // GUICC_M4444I - GUIDRV_LIN_16
#define CM_L8       6 // GUICC_8666   - GUIDRV_LIN_8
#define CM_AL44     7 // GUICC_1616I  - GUIDRV_LIN_8
#define CM_AL88     8 // GUICC_88666I - GUIDRV_LIN_16

/*********************************************************************
**********************************************************************
*
*       DISPLAY CONFIGURATION START (TO BE MODIFIED)
*
**********************************************************************
**********************************************************************
*/
/*********************************************************************
*
*       Common
*/
//
// Physical display size
//
#define XSIZE_PHYS 480
#define YSIZE_PHYS 272

//
// Buffers / VScreens
//
#define NUM_BUFFERS  1 // Number of multiple buffers to be used (at least 1 buffer)
#define NUM_VSCREENS 3 // Number of virtual  screens to be used (at least 1 screen)

//
// Redefine number of layers for this configuration file. Must be equal or less than in GUIConf.h!
//
#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 2

/*********************************************************************
*
*       Layer 0
*/
//
// Color mode layer 0. Should be one of the above defined color modes
//
//#define COLOR_MODE_0 CM_RGB888
//#define COLOR_MODE_0 CM_ARGB8888
#define COLOR_MODE_0 CM_RGB565
//#define COLOR_MODE_0 CM_L8

//
// Size of layer 0. Ignored and replaced by XSIZE_PHYS and YSIZE_PHYS if GUI_NUM_LAYERS == 1
//
#define XSIZE_0 200
#define YSIZE_0 200

//
// Orientation of layer 0. Should be one of the above defined display orientations.
//
#define ORIENTATION_0 ROTATION_0

/*********************************************************************
*
*       Layer 1
*/
//
// Color mode layer 1. Should be one of the above defined color modes
//
//#define COLOR_MODE_1 CM_RGB888
#define COLOR_MODE_1 CM_ARGB8888
//#define COLOR_MODE_1 CM_RGB565
//#define COLOR_MODE_1 CM_L8

//
// Size of layer 1
//
#define XSIZE_1 200
#define YSIZE_1 200

//
// Orientation of layer 1. Should be one of the above defined display orientations.
//
#define ORIENTATION_1 ROTATION_0

/*********************************************************************
*
*       Background color shown where no layer is active
*/
#define BK_COLOR GUI_DARKBLUE

/*********************************************************************
**********************************************************************
*
*       DISPLAY CONFIGURATION END
*
**********************************************************************
**********************************************************************
*/
/*********************************************************************
*
*       Automatic selection of driver and color conversion
*/
#if   (COLOR_MODE_0 == CM_ARGB8888)
  #define COLOR_CONVERSION_0 GUICC_M8888I
#elif (COLOR_MODE_0 == CM_RGB888)
  #define COLOR_CONVERSION_0 GUICC_M888
#elif (COLOR_MODE_0 == CM_RGB565)
  #define COLOR_CONVERSION_0 GUICC_M565
#elif (COLOR_MODE_0 == CM_ARGB1555)
  #define COLOR_CONVERSION_0 GUICC_M1555I
#elif (COLOR_MODE_0 == CM_ARGB4444)
  #define COLOR_CONVERSION_0 GUICC_M4444I
#elif (COLOR_MODE_0 == CM_L8)
  #define COLOR_CONVERSION_0 GUICC_8666
#elif (COLOR_MODE_0 == CM_AL44)
  #define COLOR_CONVERSION_0 GUICC_1616I
#elif (COLOR_MODE_0 == CM_AL88)
  #define COLOR_CONVERSION_0 GUICC_88666I
#else
  #error Illegal color mode 0!
#endif

#if   (COLOR_MODE_0 == CM_ARGB8888)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_32
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_0 == CM_RGB888)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_24
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_0 == CM_RGB565)   \
   || (COLOR_MODE_0 == CM_ARGB1555) \
   || (COLOR_MODE_0 == CM_ARGB4444) \
   || (COLOR_MODE_0 == CM_AL88)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_0 == CM_L8)   \
   || (COLOR_MODE_0 == CM_AL44)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_8
  #endif
#endif


#if (GUI_NUM_LAYERS > 1)

#if   (COLOR_MODE_1 == CM_ARGB8888)
  #define COLOR_CONVERSION_1 GUICC_M8888I
#elif (COLOR_MODE_1 == CM_RGB888)
  #define COLOR_CONVERSION_1 GUICC_M888
#elif (COLOR_MODE_1 == CM_RGB565)
  #define COLOR_CONVERSION_1 GUICC_M565
#elif (COLOR_MODE_1 == CM_ARGB1555)
  #define COLOR_CONVERSION_1 GUICC_M1555I
#elif (COLOR_MODE_1 == CM_ARGB4444)
  #define COLOR_CONVERSION_1 GUICC_M4444I
#elif (COLOR_MODE_1 == CM_L8)
  #define COLOR_CONVERSION_1 GUICC_8666
#elif (COLOR_MODE_1 == CM_AL44)
  #define COLOR_CONVERSION_1 GUICC_1616I
#elif (COLOR_MODE_1 == CM_AL88)
  #define COLOR_CONVERSION_1 GUICC_88666I
#else
  #error Illegal color mode 0!
#endif

#if   (COLOR_MODE_1 == CM_ARGB8888)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_32
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_1 == CM_RGB888)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_24
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_1 == CM_RGB565)   \
   || (COLOR_MODE_1 == CM_ARGB1555) \
   || (COLOR_MODE_1 == CM_ARGB4444) \
   || (COLOR_MODE_1 == CM_AL88)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_1 == CM_L8)   \
   || (COLOR_MODE_1 == CM_AL44)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_8
  #endif
#endif

#else

/*********************************************************************
*
*       Use complete display automatically in case of only one layer
*/
#undef XSIZE_0
#undef YSIZE_0
#define XSIZE_0 XSIZE_PHYS
#define YSIZE_0 YSIZE_PHYS

#endif

/*********************************************************************
*
*       Redirect bulk conversion to DMA2D routines
*/
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}

/*********************************************************************
*
*       H/V front/backporch and synchronization width/height
*/
#define HBP 42
#define VBP 11
#define HSW 40
#define VSW  9
#define HFP  2
#define VFP  2

/*********************************************************************
*
*       Configuration checking
*/
#if NUM_BUFFERS > 3
  #error More than 3 buffers make no sense and are not supported in this configuration file!
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_BUFFERS
  #define NUM_BUFFERS 1
#else
  #if (NUM_BUFFERS <= 0)
    #error At least one buffer needs to be defined!
  #endif
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens together with multiple buffers are not allowed!
#endif

/*********************************************************************
*
*       Frame buffer base address
*/
#define LCD_FRAME_BUFFER ((U32)0xC0000000)

/*********************************************************************
*
*       SFRs
*/
#define PERIPH_BASE                ((U32)0x40000000)

#define AHB1PERIPH_BASE            (PERIPH_BASE + 0x00020000)
#define APB2PERIPH_BASE            (PERIPH_BASE + 0x00010000)

#define LTDC_BASE                  (APB2PERIPH_BASE + 0x6800)
#define LTDC_LAYER1_BASE           (LTDC_BASE + 0x84)
#define LTDC_LAYER2_BASE           (LTDC_BASE + 0x104)

#define RCC_BASE                   (AHB1PERIPH_BASE + 0x3800)
#define RCC                        ((RCC_T *)RCC_BASE)

#define LTDC                       ((LTDC_T *)LTDC_BASE)
#define LTDC_LAYER1                ((LTDC_LAYER_T *)LTDC_LAYER1_BASE)
#define LTDC_LAYER2                ((LTDC_LAYER_T *)LTDC_LAYER2_BASE)

#define SCS_BASE                   (0xE000E000UL)                 // System Control Space Base Address
#define NVIC_BASE                  (SCS_BASE + 0x0100UL)          // NVIC Base Address
#define NVIC                       ((NVIC_T *)NVIC_BASE )         // NVIC configuration struct

#define DMA2D_BASE                 (AHB1PERIPH_BASE + 0xB000)
#define DMA2D                      ((DMA2D_T *)DMA2D_BASE)

#define GPIOA_BASE                 (AHB1PERIPH_BASE + 0x0000)
#define GPIOC_BASE                 (AHB1PERIPH_BASE + 0x0800)
#define GPIOI_BASE                 (AHB1PERIPH_BASE + 0x2000)
#define GPIOJ_BASE                 (AHB1PERIPH_BASE + 0x2400)
#define GPIOK_BASE                 (AHB1PERIPH_BASE + 0x2800)

#define GPIOA                      ((GPIO_T *) GPIOA_BASE)
#define GPIOC                      ((GPIO_T *) GPIOC_BASE)
#define GPIOI                      ((GPIO_T *) GPIOI_BASE)
#define GPIOJ                      ((GPIO_T *) GPIOJ_BASE)
#define GPIOK                      ((GPIO_T *) GPIOK_BASE)

#define GPIO_Pin_0                 ((U16)0x0001)
#define GPIO_Pin_1                 ((U16)0x0002)
#define GPIO_Pin_2                 ((U16)0x0004)
#define GPIO_Pin_3                 ((U16)0x0008)
#define GPIO_Pin_4                 ((U16)0x0010)
#define GPIO_Pin_5                 ((U16)0x0020)
#define GPIO_Pin_6                 ((U16)0x0040)
#define GPIO_Pin_7                 ((U16)0x0080)
#define GPIO_Pin_8                 ((U16)0x0100)
#define GPIO_Pin_9                 ((U16)0x0200)
#define GPIO_Pin_10                ((U16)0x0400)
#define GPIO_Pin_11                ((U16)0x0800)
#define GPIO_Pin_12                ((U16)0x1000)
#define GPIO_Pin_13                ((U16)0x2000)
#define GPIO_Pin_14                ((U16)0x4000)
#define GPIO_Pin_15                ((U16)0x8000)
#define GPIO_Pin_All               ((U16)0xFFFF)

#define GPIO_PinSource0            ((U8)0x00)
#define GPIO_PinSource1            ((U8)0x01)
#define GPIO_PinSource2            ((U8)0x02)
#define GPIO_PinSource3            ((U8)0x03)
#define GPIO_PinSource4            ((U8)0x04)
#define GPIO_PinSource5            ((U8)0x05)
#define GPIO_PinSource6            ((U8)0x06)
#define GPIO_PinSource7            ((U8)0x07)
#define GPIO_PinSource8            ((U8)0x08)
#define GPIO_PinSource9            ((U8)0x09)
#define GPIO_PinSource10           ((U8)0x0A)
#define GPIO_PinSource11           ((U8)0x0B)
#define GPIO_PinSource12           ((U8)0x0C)
#define GPIO_PinSource13           ((U8)0x0D)
#define GPIO_PinSource14           ((U8)0x0E)
#define GPIO_PinSource15           ((U8)0x0F)

#define GPIO_MODER_MODER0          ((U32)0x00000003)
#define GPIO_OSPEEDER_OSPEEDR0     ((U32)0x00000003)
#define GPIO_OTYPER_OT_0           ((U32)0x00000001)
#define GPIO_PUPDR_PUPDR0          ((U32)0x00000003)

#define RCC_AHB1Periph_GPIOA       ((U32)0x00000001)
#define RCC_AHB1Periph_GPIOB       ((U32)0x00000002)
#define RCC_AHB1Periph_GPIOC       ((U32)0x00000004)
#define RCC_AHB1Periph_GPIOD       ((U32)0x00000008)
#define RCC_AHB1Periph_GPIOE       ((U32)0x00000010)
#define RCC_AHB1Periph_GPIOF       ((U32)0x00000020)
#define RCC_AHB1Periph_GPIOG       ((U32)0x00000040)
#define RCC_AHB1Periph_GPIOH       ((U32)0x00000080)
#define RCC_AHB1Periph_GPIOI       ((U32)0x00000100)
#define RCC_AHB1Periph_GPIOJ       ((U32)0x00000200)
#define RCC_AHB1Periph_GPIOK       ((U32)0x00000400)
#define RCC_AHB1Periph_DMA2D       ((U32)0x00800000)
#define RCC_APB2Periph_LTDC        ((U32)0x04000000)

#define RCC_DCKCFGR_PLLSAIDIVR     ((U32)0x00030000)

#define RCC_PLLSAIDivR_Div8        ((U32)0x00020000)

#define RCC_FLAG_PLLSAIRDY         ((U8)0x3D)

#define GPIO_AF_LTDC               ((U8)0x0E)                     // LCD-TFT Alternate Function mapping

// Bit definition for LTDC_SSCR register
#define LTDC_SSCR_VSH              ((U32)0x000007FF)              // Vertical Synchronization Height
#define LTDC_SSCR_HSW              ((U32)0x0FFF0000)              // Horizontal Synchronization Width

// Bit definition for LTDC_BPCR register
#define LTDC_BPCR_AVBP             ((U32)0x000007FF)              // Accumulated Vertical Back Porch
#define LTDC_BPCR_AHBP             ((U32)0x0FFF0000)              // Accumulated Horizontal Back Porch

// Bit definition for LTDC_AWCR register
#define LTDC_AWCR_AAH              ((U32)0x000007FF)              // Accumulated Active heigh
#define LTDC_AWCR_AAW              ((U32)0x0FFF0000)              // Accumulated Active Width

// Bit definition for LTDC_TWCR register
#define LTDC_TWCR_TOTALH           ((U32)0x000007FF)              // Total Heigh
#define LTDC_TWCR_TOTALW           ((U32)0x0FFF0000)              // Total Width

// Bit definition for LTDC_GCR register
#define LTDC_GCR_LTDCEN            ((U32)0x00000001)              // LCD-TFT controller enable bit
#define LTDC_GCR_DBW               ((U32)0x00000070)              // Dither Blue Width
#define LTDC_GCR_DGW               ((U32)0x00000700)              // Dither Green Width
#define LTDC_GCR_DRW               ((U32)0x00007000)              // Dither Red Width
#define LTDC_GCR_DTEN              ((U32)0x00010000)              // Dither Enable
#define LTDC_GCR_PCPOL             ((U32)0x10000000)              // Pixel Clock Polarity
#define LTDC_GCR_DEPOL             ((U32)0x20000000)              // Data Enable Polarity
#define LTDC_GCR_VSPOL             ((U32)0x40000000)              // Vertical Synchronization Polarity
#define LTDC_GCR_HSPOL             ((U32)0x80000000)              // Horizontal Synchronization Polarity

// Bit definition for LTDC_SRCR register
#define LTDC_SRCR_IMR              ((U32)0x00000001)              // Immediate Reload
#define LTDC_SRCR_VBR              ((U32)0x00000002)              // Vertical Blanking Reload

// Bit definition for LTDC_BCCR register
#define LTDC_BCCR_BCBLUE           ((U32)0x000000FF)              // Background Blue value
#define LTDC_BCCR_BCGREEN          ((U32)0x0000FF00)              // Background Green value
#define LTDC_BCCR_BCRED            ((U32)0x00FF0000)              // Background Red value

// Bit definition for LTDC_IER register
#define LTDC_IER_LIE               ((U32)0x00000001)              // Line Interrupt Enable
#define LTDC_IER_FUIE              ((U32)0x00000002)              // FIFO Underrun Interrupt Enable
#define LTDC_IER_TERRIE            ((U32)0x00000004)              // Transfer Error Interrupt Enable
#define LTDC_IER_RRIE              ((U32)0x00000008)              // Register Reload interrupt enable

// Bit definition for LTDC_ISR register
#define LTDC_ISR_LIF               ((U32)0x00000001)              // Line Interrupt Flag
#define LTDC_ISR_FUIF              ((U32)0x00000002)              // FIFO Underrun Interrupt Flag
#define LTDC_ISR_TERRIF            ((U32)0x00000004)              // Transfer Error Interrupt Flag
#define LTDC_ISR_RRIF              ((U32)0x00000008)              // Register Reload interrupt Flag

// Bit definition for LTDC_ICR register
#define LTDC_ICR_CLIF              ((U32)0x00000001)              // Clears the Line Interrupt Flag
#define LTDC_ICR_CFUIF             ((U32)0x00000002)              // Clears the FIFO Underrun Interrupt Flag
#define LTDC_ICR_CTERRIF           ((U32)0x00000004)              // Clears the Transfer Error Interrupt Flag
#define LTDC_ICR_CRRIF             ((U32)0x00000008)              // Clears Register Reload interrupt Flag

// Bit definition for LTDC_LIPCR register
#define LTDC_LIPCR_LIPOS           ((U32)0x000007FF)              // Line Interrupt Position

// Bit definition for LTDC_CPSR register
#define LTDC_CPSR_CYPOS            ((U32)0x0000FFFF)              // Current Y Position
#define LTDC_CPSR_CXPOS            ((U32)0xFFFF0000)              // Current X Position

// Bit definition for LTDC_CDSR register
#define LTDC_CDSR_VDES             ((U32)0x00000001)              // Vertical Data Enable Status
#define LTDC_CDSR_HDES             ((U32)0x00000002)              // Horizontal Data Enable Status
#define LTDC_CDSR_VSYNCS           ((U32)0x00000004)              // Vertical Synchronization Status
#define LTDC_CDSR_HSYNCS           ((U32)0x00000008)              // Horizontal Synchronization Status

// Bit definition for LTDC_LxCR register
#define LTDC_LxCR_LEN              ((U32)0x00000001)              // Layer Enable
#define LTDC_LxCR_COLKEN           ((U32)0x00000002)              // Color Keying Enable
#define LTDC_LxCR_CLUTEN           ((U32)0x00000010)              // Color Lockup Table Enable

// Bit definition for LTDC_LxWHPCR register
#define LTDC_LxWHPCR_WHSTPOS       ((U32)0x00000FFF)              // Window Horizontal Start Position
#define LTDC_LxWHPCR_WHSPPOS       ((U32)0xFFFF0000)              // Window Horizontal Stop Position

// Bit definition for LTDC_LxWVPCR register
#define LTDC_LxWVPCR_WVSTPOS       ((U32)0x00000FFF)              // Window Vertical Start Position
#define LTDC_LxWVPCR_WVSPPOS       ((U32)0xFFFF0000)              // Window Vertical Stop Position

// Bit definition for LTDC_LxCKCR register
#define LTDC_LxCKCR_CKBLUE         ((U32)0x000000FF)              // Color Key Blue value
#define LTDC_LxCKCR_CKGREEN        ((U32)0x0000FF00)              // Color Key Green value
#define LTDC_LxCKCR_CKRED          ((U32)0x00FF0000)              // Color Key Red value

// Bit definition for LTDC_LxPFCR register
#define LTDC_LxPFCR_PF             ((U32)0x00000007)              // Pixel Format

// Bit definition for LTDC_LxCACR register
#define LTDC_LxCACR_CONSTA         ((U32)0x000000FF)              // Constant Alpha

// Bit definition for LTDC_LxDCCR register
#define LTDC_LxDCCR_DCBLUE         ((U32)0x000000FF)              // Default Color Blue
#define LTDC_LxDCCR_DCGREEN        ((U32)0x0000FF00)              // Default Color Green
#define LTDC_LxDCCR_DCRED          ((U32)0x00FF0000)              // Default Color Red
#define LTDC_LxDCCR_DCALPHA        ((U32)0xFF000000)              // Default Color Alpha

// Bit definition for LTDC_LxBFCR register
#define LTDC_LxBFCR_BF2            ((U32)0x00000007)              // Blending Factor 2
#define LTDC_LxBFCR_BF1            ((U32)0x00000700)              // Blending Factor 1

// Bit definition for LTDC_LxCFBAR register
#define LTDC_LxCFBAR_CFBADD        ((U32)0xFFFFFFFF)              // Color Frame Buffer Start Address

// Bit definition for LTDC_LxCFBLR register
#define LTDC_LxCFBLR_CFBLL         ((U32)0x00001FFF)              // Color Frame Buffer Line Length
#define LTDC_LxCFBLR_CFBP          ((U32)0x1FFF0000)              // Color Frame Buffer Pitch in bytes

// Bit definition for LTDC_LxCFBLNR register
#define LTDC_LxCFBLNR_CFBLNBR      ((U32)0x000007FF)              // Frame Buffer Line Number

// Bit definition for LTDC_LxCLUTWR register
#define LTDC_LxCLUTWR_BLUE         ((U32)0x000000FF)              // Blue value
#define LTDC_LxCLUTWR_GREEN        ((U32)0x0000FF00)              // Green value
#define LTDC_LxCLUTWR_RED          ((U32)0x00FF0000)              // Red value
#define LTDC_LxCLUTWR_CLUTADD      ((U32)0xFF000000)              // CLUT address

// Bit definition for DMA2D_ISR register
#define DMA2D_ISR_TEIF             ((U32)0x00000001)              // Transfer Error Interrupt Flag
#define DMA2D_ISR_TCIF             ((U32)0x00000002)              // Transfer Complete Interrupt Flag
#define DMA2D_ISR_TWIF             ((U32)0x00000004)              // Transfer Watermark Interrupt Flag
#define DMA2D_ISR_CAEIF            ((U32)0x00000008)              // CLUT Access Error Interrupt Flag
#define DMA2D_ISR_CTCIF            ((U32)0x00000010)              // CLUT Transfer Complete Interrupt Flag
#define DMA2D_ISR_CEIF             ((U32)0x00000020)              // Configuration Error Interrupt Flag

// Bit definition for DMA2D_IFSR register
#define DMA2D_IFSR_CTEIF           ((U32)0x00000001)              // Clears Transfer Error Interrupt Flag
#define DMA2D_IFSR_CTCIF           ((U32)0x00000002)              // Clears Transfer Complete Interrupt Flag
#define DMA2D_IFSR_CTWIF           ((U32)0x00000004)              // Clears Transfer Watermark Interrupt Flag
#define DMA2D_IFSR_CCAEIF          ((U32)0x00000008)              // Clears CLUT Access Error Interrupt Flag
#define DMA2D_IFSR_CCTCIF          ((U32)0x00000010)              // Clears CLUT Transfer Complete Interrupt Flag
#define DMA2D_IFSR_CCEIF           ((U32)0x00000020)              // Clears Configuration Error Interrupt Flag

// Bit definition for DMA2D_CR register
#define DMA2D_CR_START             ((U32)0x00000001)              // Start transfer
#define DMA2D_CR_SUSP              ((U32)0x00000002)              // Suspend transfer
#define DMA2D_CR_ABORT             ((U32)0x00000004)              // Abort transfer
#define DMA2D_CR_TEIE              ((U32)0x00000100)              // Transfer Error Interrupt Enable
#define DMA2D_CR_TCIE              ((U32)0x00000200)              // Transfer Complete Interrupt Enable
#define DMA2D_CR_TWIE              ((U32)0x00000400)              // Transfer Watermark Interrupt Enable
#define DMA2D_CR_CAEIE             ((U32)0x00000800)              // CLUT Access Error Interrupt Enable
#define DMA2D_CR_CTCIE             ((U32)0x00001000)              // CLUT Transfer Complete Interrupt Enable
#define DMA2D_CR_CEIE              ((U32)0x00002000)              // Configuration Error Interrupt Enable
#define DMA2D_CR_MODE              ((U32)0x00030000)              // DMA2D Mode

// Common LTDC
#define GCR_MASK                   ((U32)0x0FFE888F)              // LTDC GCR Mask

#define LTDC_Pixelformat_ARGB8888  ((U32)0x00000000)
#define LTDC_Pixelformat_RGB888    ((U32)0x00000001)
#define LTDC_Pixelformat_RGB565    ((U32)0x00000002)
#define LTDC_Pixelformat_ARGB1555  ((U32)0x00000003)
#define LTDC_Pixelformat_ARGB4444  ((U32)0x00000004)
#define LTDC_Pixelformat_L8        ((U32)0x00000005)
#define LTDC_Pixelformat_AL44      ((U32)0x00000006)
#define LTDC_Pixelformat_AL88      ((U32)0x00000007)
#define LTDC_Pixelformat_L4        ((U32)0x00000008)

#define LTDC_BlendingFactor1_CA    ((U32)0x00000400)
#define LTDC_BlendingFactor2_CA    ((U32)0x00000005)
#define LTDC_BlendingFactor1_PAxCA ((U32)0x00000600)
#define LTDC_BlendingFactor2_PAxCA ((U32)0x00000007)

#define LTDC_PCPolarity_IPC        ((U32)0x00000000)              // Input pixel clock.
#define LTDC_PCPolarity_IIPC       LTDC_GCR_PCPOL                 // Inverted input pixel clock.

#define LTDC_DEPolarity_AL         ((U32)0x00000000)              // Data Enable, is active low.
#define LTDC_DEPolarity_AH         LTDC_GCR_DEPOL                 // Data Enable, is active high.

#define LTDC_VSPolarity_AL         ((U32)0x00000000)              // Vertical Synchronization is active low.
#define LTDC_VSPolarity_AH         LTDC_GCR_VSPOL                 // Vertical Synchronization is active high.

#define LTDC_HSPolarity_AL         ((U32)0x00000000)              // Horizontal Synchronization is active low.
#define LTDC_HSPolarity_AH         LTDC_GCR_HSPOL                 // Horizontal Synchronization is active high.

#define DMA2D_CR_TCIE              ((U32)0x00000200)              // Transfer Complete Interrupt Enable

// Common
#define __NVIC_PRIO_BITS           4                              // STM32F4XX uses 4 Bits for the Priority Levels

#define LTDC_IRQn                  88                             // LTDC global Interrupt
#define DMA2D_IRQn                 90                             // DMA2D global Interrupt

#define PLLSAION_BitNumber         0x1C

#define PERIPH_BB_BASE             ((U32)0x42000000)              // Peripheral base address in the bit-band region

#define RCC_OFFSET                 (RCC_BASE - PERIPH_BASE)
#define CR_OFFSET                  (RCC_OFFSET + 0x00)
#define CR_PLLSAION_BB             (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLSAION_BitNumber * 4))

#define FLAG_MASK                  ((U8)0x1F)

#define BUFFER_OFFSET              ((U32)0x130000)

#define DISABLE 0
#define ENABLE  1
#define RESET   0
#define SET     1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  U32 HSPolarity;             // Horizontal synchronization polarity.        Can be one value of @ref LTDC_HSPolarity
  U32 VSPolarity;             // Vertical synchronization polarity.          Can be one value of @ref LTDC_VSPolarity
  U32 DEPolarity;             // Data enable polarity.                       Can be one of value of @ref LTDC_DEPolarity
  U32 PCPolarity;             // Pixel clock polarity.                       Can be one of value of @ref LTDC_PCPolarity
  U32 HorizontalSync;         // Number of Horizontal synchronization width. Must range from 0x000 to 0xFFF.
  U32 VerticalSync;           // Number of Vertical synchronization heigh.   Must range from 0x000 to 0x7FF.
  U32 AccumulatedHBP;         // Accumulated horizontal back porch width.    Must range from LTDC_HorizontalSync to 0xFFF.
  U32 AccumulatedVBP;         // Accumulated vertical back porch heigh.      Must range from LTDC_VerticalSync to 0x7FF.
  U32 AccumulatedActiveW;     // Accumulated active width.                   Must range from LTDC_AccumulatedHBP to 0xFFF.
  U32 AccumulatedActiveH;     // Accumulated active heigh.                   Must range from LTDC_AccumulatedVBP to 0x7FF.
  U32 TotalWidth;             // Total width.                                Must range from LTDC_AccumulatedActiveW to 0xFFF.
  U32 TotalHeigh;             // Total heigh.                                Must range from LTDC_AccumulatedActiveH to 0x7FF.
  U32 BackgroundRedValue;     // Background red value.                       Must range from 0x00 to 0xFF.
  U32 BackgroundGreenValue;   // Background green value.                     Must range from 0x00 to 0xFF.
  U32 BackgroundBlueValue;    // Background blue value.                      Must range from 0x00 to 0xFF.
} LTDC_INIT_T;

typedef struct {
  U32 HorizontalStart;        // Window Horizontal Start Position.           Must range from 0x000 to 0xFFF.
  U32 HorizontalStop;         // Window Horizontal Stop Position.            Must range from 0x0000 to 0xFFFF.
  U32 VerticalStart;          // Window vertical Start Position.             Must range from 0x000 to 0xFFF.
  U32 VerticalStop;           // Window vaertical Stop Position.             Must range from 0x0000 to 0xFFFF.
  U32 PixelFormat;            // Pixel format.                               Can be one of value of @ref LTDC_Pixelformat
  U32 ConstantAlpha;          // Constant alpha used for blending.           Must range from 0x00 to 0xFF.
  U32 DefaultColorBlue;       // Default blue value.                         Must range from 0x00 to 0xFF.
  U32 DefaultColorGreen;      // Default green value.                        Must range from 0x00 to 0xFF.
  U32 DefaultColorRed;        // Default red value.                          Must range from 0x00 to 0xFF.
  U32 DefaultColorAlpha;      // Default alpha value.                        Must range from 0x00 to 0xFF.
  U32 BlendingFactor_1;       // Blending factor 1.                          Can be one of value of @ref LTDC_BlendingFactor1
  U32 BlendingFactor_2;       // Blending factor 2.                          Can be one of value of @ref LTDC_BlendingFactor2
  U32 CFBStartAdress;         // Color frame buffer address
  U32 CFBLineLength;          // Color frame buffer line length.             Must range from 0x0000 to 0x1FFF.
  U32 CFBPitch;               // Color frame buffer pitch in bytes.          Must range from 0x0000 to 0x1FFF.
  U32 CFBLineNumber;          // Number of line in frame buffer.             Must range from 0x000 to 0x7FF.
} LTDC_INIT_LAYER_T;

typedef struct {
  volatile U32 CR;            // Control Register                                  Address offset: 0x84
  volatile U32 WHPCR;         // Window Horizontal Position Configuration Register Address offset: 0x88
  volatile U32 WVPCR;         // Window Vertical Position Configuration Register   Address offset: 0x8C
  volatile U32 CKCR;          // Color Keying Configuration Register               Address offset: 0x90
  volatile U32 PFCR;          // Pixel Format Configuration Register               Address offset: 0x94
  volatile U32 CACR;          // Constant Alpha Configuration Register             Address offset: 0x98
  volatile U32 DCCR;          // Default Color Configuration Register              Address offset: 0x9C
  volatile U32 BFCR;          // Blending Factors Configuration Register           Address offset: 0xA0
  volatile U32 aRESERVED0[2]; // Reserved
  volatile U32 CFBAR;         // Color Frame Buffer Address Register               Address offset: 0xAC
  volatile U32 CFBLR;         // Color Frame Buffer Length Register                Address offset: 0xB0
  volatile U32 CFBLNR;        // ColorFrame Buffer Line Number Register            Address offset: 0xB4
  volatile U32 aRESERVED1[3]; // Reserved
  volatile U32 CLUTWR;        // CLUT Write Register                               Address offset: 0x144
} LTDC_LAYER_T;

typedef struct {
  volatile U32 aRESERVED0[2]; // Reserved, 0x00-0x04
  volatile U32 SSCR;          // Synchronization Size Configuration Register,      Address offset: 0x08
  volatile U32 BPCR;          // Back Porch Configuration Register,                Address offset: 0x0C
  volatile U32 AWCR;          // Active Width Configuration Register,              Address offset: 0x10
  volatile U32 TWCR;          // Total Width Configuration Register,               Address offset: 0x14
  volatile U32 GCR;           // Global Control Register,                          Address offset: 0x18
  volatile U32 aRESERVED1[2]; // Reserved
  volatile U32 SRCR;          // Shadow Reload Configuration Register,             Address offset: 0x24
  volatile U32 aRESERVED2[1]; // Reserved
  volatile U32 BCCR;          // Background Color Configuration Register,          Address offset: 0x2C
  volatile U32 aRESERVED3[1]; // Reserved
  volatile U32 IER;           // Interrupt Enable Register,                        Address offset: 0x34
  volatile U32 ISR;           // Interrupt Status Register,                        Address offset: 0x38
  volatile U32 ICR;           // Interrupt Clear Register,                         Address offset: 0x3C
  volatile U32 LIPCR;         // Line Interrupt Position Configuration Register,   Address offset: 0x40
  volatile U32 CPSR;          // Current Position Status Register,                 Address offset: 0x44
  volatile U32 CDSR;          // Current Display Status Register,                  Address offset: 0x48
} LTDC_T;

typedef struct {
  volatile U32 CR;            // Clock control register,                                  Address offset: 0x00
  volatile U32 PLLCFGR;       // PLL configuration register,                              Address offset: 0x04
  volatile U32 CFGR;          // Clock configuration register,                            Address offset: 0x08
  volatile U32 CIR;           // Clock interrupt register,                                Address offset: 0x0C
  volatile U32 AHB1RSTR;      // AHB1 peripheral reset register,                          Address offset: 0x10
  volatile U32 AHB2RSTR;      // AHB2 peripheral reset register,                          Address offset: 0x14
  volatile U32 AHB3RSTR;      // AHB3 peripheral reset register,                          Address offset: 0x18
  volatile U32 aRESERVED0;    // Reserved
  volatile U32 APB1RSTR;      // APB1 peripheral reset register,                          Address offset: 0x20
  volatile U32 APB2RSTR;      // APB2 peripheral reset register,                          Address offset: 0x24
  volatile U32 aRESERVED1[2]; // Reserved
  volatile U32 AHB1ENR;       // AHB1 peripheral clock register,                          Address offset: 0x30
  volatile U32 AHB2ENR;       // AHB2 peripheral clock register,                          Address offset: 0x34
  volatile U32 AHB3ENR;       // AHB3 peripheral clock register,                          Address offset: 0x38
  volatile U32 aRESERVED2;    // Reserved
  volatile U32 APB1ENR;       // APB1 peripheral clock enable register,                   Address offset: 0x40
  volatile U32 APB2ENR;       // APB2 peripheral clock enable register,                   Address offset: 0x44
  volatile U32 aRESERVED3[2]; // Reserved
  volatile U32 AHB1LPENR;     // AHB1 peripheral clock enable in low power mode register, Address offset: 0x50
  volatile U32 AHB2LPENR;     // AHB2 peripheral clock enable in low power mode register, Address offset: 0x54
  volatile U32 AHB3LPENR;     // AHB3 peripheral clock enable in low power mode register, Address offset: 0x58
  volatile U32 aRESERVED4;    // Reserved
  volatile U32 APB1LPENR;     // APB1 peripheral clock enable in low power mode register, Address offset: 0x60
  volatile U32 APB2LPENR;     // APB2 peripheral clock enable in low power mode register, Address offset: 0x64
  volatile U32 aRESERVED5[2]; // Reserved
  volatile U32 BDCR;          // Backup domain control register,                          Address offset: 0x70
  volatile U32 CSR;           // Clock control & status register,                         Address offset: 0x74
  volatile U32 aRESERVED6[2]; // Reserved
  volatile U32 SSCGR;         // Spread spectrum clock generation register,               Address offset: 0x80
  volatile U32 PLLI2SCFGR;    // PLLI2S configuration register,                           Address offset: 0x84
  volatile U32 PLLSAICFGR;    // PLLSAI configuration register,                           Address offset: 0x88
  volatile U32 DCKCFGR;       // Dedicated Clocks configuration register,                 Address offset: 0x8C
} RCC_T;

typedef struct {
  volatile U32 ISER[8];       // Offset: 0x000 (R/W)  Interrupt Set Enable Register
  volatile U32 aRESERVED0[24];
  volatile U32 ICER[8];       // Offset: 0x080 (R/W)  Interrupt Clear Enable Register
  volatile U32 aRSERVED1[24];
  volatile U32 ISPR[8];       // Offset: 0x100 (R/W)  Interrupt Set Pending Register
  volatile U32 aRESERVED2[24];
  volatile U32 ICPR[8];       // Offset: 0x180 (R/W)  Interrupt Clear Pending Register
  volatile U32 aRESERVED3[24];
  volatile U32 IABR[8];       // Offset: 0x200 (R/W)  Interrupt Active bit Register
  volatile U32 aRESERVED4[56];
  volatile U8  IP[240];       // Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide)
  volatile U32 aRESERVED5[644];
  volatile U32 STIR;          // Offset: 0xE00 ( /W)  Software Trigger Interrupt Register
} NVIC_T;

typedef struct {
  volatile U32 MODER;         // GPIO port mode register,               Address offset: 0x00
  volatile U32 OTYPER;        // GPIO port output type register,        Address offset: 0x04
  volatile U32 OSPEEDR;       // GPIO port output speed register,       Address offset: 0x08
  volatile U32 PUPDR;         // GPIO port pull-up/pull-down register,  Address offset: 0x0C
  volatile U32 IDR;           // GPIO port input data register,         Address offset: 0x10
  volatile U32 ODR;           // GPIO port output data register,        Address offset: 0x14
  volatile U16 BSRRL;         // GPIO port bit set/reset low register,  Address offset: 0x18
  volatile U16 BSRRH;         // GPIO port bit set/reset high register, Address offset: 0x1A
  volatile U32 LCKR;          // GPIO port configuration lock register, Address offset: 0x1C
  volatile U32 AFR[2];        // GPIO alternate function registers,     Address offset: 0x20-0x24
} GPIO_T;

typedef struct {
  volatile U32 CR;            // DMA2D Control Register,                         Address offset: 0x00
  volatile U32 ISR;           // DMA2D Interrupt Status Register,                Address offset: 0x04
  volatile U32 IFCR;          // DMA2D Interrupt Flag Clear Register,            Address offset: 0x08
  volatile U32 FGMAR;         // DMA2D Foreground Memory Address Register,       Address offset: 0x0C
  volatile U32 FGOR;          // DMA2D Foreground Offset Register,               Address offset: 0x10
  volatile U32 BGMAR;         // DMA2D Background Memory Address Register,       Address offset: 0x14
  volatile U32 BGOR;          // DMA2D Background Offset Register,               Address offset: 0x18
  volatile U32 FGPFCCR;       // DMA2D Foreground PFC Control Register,          Address offset: 0x1C
  volatile U32 FGCOLR;        // DMA2D Foreground Color Register,                Address offset: 0x20
  volatile U32 BGPFCCR;       // DMA2D Background PFC Control Register,          Address offset: 0x24
  volatile U32 BGCOLR;        // DMA2D Background Color Register,                Address offset: 0x28
  volatile U32 FGCMAR;        // DMA2D Foreground CLUT Memory Address Register,  Address offset: 0x2C
  volatile U32 BGCMAR;        // DMA2D Background CLUT Memory Address Register,  Address offset: 0x30
  volatile U32 OPFCCR;        // DMA2D Output PFC Control Register,              Address offset: 0x34
  volatile U32 OCOLR;         // DMA2D Output Color Register,                    Address offset: 0x38
  volatile U32 OMAR;          // DMA2D Output Memory Address Register,           Address offset: 0x3C
  volatile U32 OOR;           // DMA2D Output Offset Register,                   Address offset: 0x40
  volatile U32 NLR;           // DMA2D Number of Line Register,                  Address offset: 0x44
  volatile U32 LWR;           // DMA2D Line Watermark Register,                  Address offset: 0x48
  volatile U32 AMTCR;         // DMA2D AHB Master Timer Configuration Register,  Address offset: 0x4C
  volatile U32 RESERVED[236]; // Reserved, 0x50-0x3FF
  volatile U32 FGCLUT[256];   // DMA2D Foreground CLUT,                          Address offset:400-7FF
  volatile U32 BGCLUT[256];   // DMA2D Background CLUT,                          Address offset:800-BFF
} DMA2D_T;

typedef enum {
  GPIO_Mode_IN      = 0x00,   // Input Mode
  GPIO_Mode_OUT     = 0x01,   // Output Mode
  GPIO_Mode_AF      = 0x02,   // Alternate function Mode
  GPIO_Mode_AN      = 0x03    // Analog Mode
} GPIO_MODE_T;

typedef enum {
  GPIO_Speed_2MHz   = 0x00,
  GPIO_Speed_25MHz  = 0x01,
  GPIO_Speed_50MHz  = 0x02,
  GPIO_Speed_100MHz = 0x03
} GPIO_SPEED_T;

typedef enum {
  GPIO_OType_PP     = 0x00,
  GPIO_OType_OD     = 0x01
} GPIO_OTYPE_T;

typedef enum {
  GPIO_PuPd_NOPULL  = 0x00,
  GPIO_PuPd_UP      = 0x01,
  GPIO_PuPd_DOWN    = 0x02
} GPIO_PUPD_T;

typedef struct {
  volatile U32 GPIO_Pin;      // GPIO pins to be configured.
  GPIO_MODE_T  GPIO_Mode;     // Operating mode for the selected pins.
  GPIO_SPEED_T GPIO_Speed;    // Speed for the selected pins.
  GPIO_OTYPE_T GPIO_OType;    // Operating output type for the selected pins.
  GPIO_PUPD_T  GPIO_PuPd;     // Operating Pull-up/Pull down for the selected pins.
} GPIO_INIT_T;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// Array for speeding up nibble conversion for A4 bitmaps
//
static const U8 _aMirror[] = {
  0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
  0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1,
  0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82, 0x92, 0xA2, 0xB2, 0xC2, 0xD2, 0xE2, 0xF2,
  0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
  0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74, 0x84, 0x94, 0xA4, 0xB4, 0xC4, 0xD4, 0xE4, 0xF4,
  0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x85, 0x95, 0xA5, 0xB5, 0xC5, 0xD5, 0xE5, 0xF5,
  0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x86, 0x96, 0xA6, 0xB6, 0xC6, 0xD6, 0xE6, 0xF6,
  0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
  0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8,
  0x09, 0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79, 0x89, 0x99, 0xA9, 0xB9, 0xC9, 0xD9, 0xE9, 0xF9,
  0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A, 0x7A, 0x8A, 0x9A, 0xAA, 0xBA, 0xCA, 0xDA, 0xEA, 0xFA,
  0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B, 0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
  0x0C, 0x1C, 0x2C, 0x3C, 0x4C, 0x5C, 0x6C, 0x7C, 0x8C, 0x9C, 0xAC, 0xBC, 0xCC, 0xDC, 0xEC, 0xFC,
  0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D, 0x6D, 0x7D, 0x8D, 0x9D, 0xAD, 0xBD, 0xCD, 0xDD, 0xED, 0xFD,
  0x0E, 0x1E, 0x2E, 0x3E, 0x4E, 0x5E, 0x6E, 0x7E, 0x8E, 0x9E, 0xAE, 0xBE, 0xCE, 0xDE, 0xEE, 0xFE,
  0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F, 0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF,
};

static LTDC_LAYER_T * _apLayer[] = { LTDC_LAYER1, LTDC_LAYER2 };
static const U32      _aAddr[]   = { LCD_FRAME_BUFFER, LCD_FRAME_BUFFER + XSIZE_PHYS * YSIZE_PHYS * sizeof(U32) * NUM_VSCREENS * NUM_BUFFERS };

static int _aPendingBuffer[2] = { -1, -1 };
static int _aBufferIndex[GUI_NUM_LAYERS];
static int _axSize[GUI_NUM_LAYERS];
static int _aySize[GUI_NUM_LAYERS];
static int _aBytesPerPixels[GUI_NUM_LAYERS];

//
// Prototypes of DMA2D color conversion routines
//
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);

//
// Color conversion routines using DMA2D
//
DEFINE_DMA2D_COLORCONVERSION(M8888I, LTDC_Pixelformat_ARGB8888)
DEFINE_DMA2D_COLORCONVERSION(M888,   LTDC_Pixelformat_ARGB8888) // Internal pixel format of emWin is 32 bit, because of that ARGB8888
DEFINE_DMA2D_COLORCONVERSION(M565,   LTDC_Pixelformat_RGB565)
DEFINE_DMA2D_COLORCONVERSION(M1555I, LTDC_Pixelformat_ARGB1555)
DEFINE_DMA2D_COLORCONVERSION(M4444I, LTDC_Pixelformat_ARGB4444)

//
// Buffer for DMA2D color conversion, required because hardware does not support overlapping regions
//
static U32 _aBuffer[XSIZE_PHYS * sizeof(U32) * 3];

static U32 * _pBuffer_DMA2D = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 0];
static U32 * _pBuffer_FG    = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 1];
static U32 * _pBuffer_BG    = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 2];

//
// Array of color conversions for each layer
//
static const LCD_API_COLOR_CONV * _apColorConvAPI[] = {
  COLOR_CONVERSION_0,
#if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};

//
// Array of orientations for each layer
//
static const int _aOrientation[] = {
  ORIENTATION_0,
#if GUI_NUM_LAYERS > 1
  ORIENTATION_1,
#endif
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetPixelformat
*/
static U32 _GetPixelformat(int LayerIndex) {
  const LCD_API_COLOR_CONV * pColorConvAPI;

  if (LayerIndex >= GUI_COUNTOF(_apColorConvAPI)) {
    return 0;
  }
  pColorConvAPI = _apColorConvAPI[LayerIndex];
  if        (pColorConvAPI == GUICC_M8888I) {
    return LTDC_Pixelformat_ARGB8888;
  } else if (pColorConvAPI == GUICC_M888  ) {
    return LTDC_Pixelformat_RGB888;
  } else if (pColorConvAPI == GUICC_M565  ) {
    return LTDC_Pixelformat_RGB565;
  } else if (pColorConvAPI == GUICC_M1555I) {
    return LTDC_Pixelformat_ARGB1555;
  } else if (pColorConvAPI == GUICC_M4444I) {
    return LTDC_Pixelformat_ARGB4444;
  } else if (pColorConvAPI == GUICC_8666  ) {
    return LTDC_Pixelformat_L8;
  } else if (pColorConvAPI == GUICC_1616I ) {
    return LTDC_Pixelformat_AL44;
  } else if (pColorConvAPI == GUICC_88666I) {
    return LTDC_Pixelformat_AL88;
  }
  while (1); // Error
}

/*********************************************************************
*
*       _GetBytesPerLine
*/
static int _GetBytesPerLine(int LayerIndex, int xSize) {
  int BitsPerPixel, BytesPerLine;

  BitsPerPixel  = LCD_GetBitsPerPixelEx(LayerIndex);
  BytesPerLine = (BitsPerPixel * xSize + 7) / 8;
  return BytesPerLine;
}

/*********************************************************************
*
*       _GPIO_SetBits
*/
static void _GPIO_SetBits(GPIO_T * GPIOx, U16 GPIO_Pin) {
  GPIOx->BSRRL = GPIO_Pin;
}

/*********************************************************************
*
*       _GPIO_ResetBits
*/
static void _GPIO_ResetBits(GPIO_T * GPIOx, U16 GPIO_Pin) {
  GPIOx->BSRRH = GPIO_Pin;
}

/*********************************************************************
*
*       _GPIO_PinAFConfig
*/
static void _GPIO_PinAFConfig(GPIO_T * GPIOx, U16 GPIO_PinSource, U8 GPIO_AF) {
  U32 temp, temp_2;

  temp = ((U32)(GPIO_AF) << ((U32)((U32)GPIO_PinSource & (U32)0x07) * 4)) ;
  GPIOx->AFR[GPIO_PinSource >> 0x03] &= ~((U32)0xF << ((U32)((U32)GPIO_PinSource & (U32)0x07) * 4)) ;
  temp_2 = GPIOx->AFR[GPIO_PinSource >> 0x03] | temp;
  GPIOx->AFR[GPIO_PinSource >> 0x03] = temp_2;
}

/*********************************************************************
*
*       _GPIO_Init
*/
static void _GPIO_Init(GPIO_T * GPIOx, GPIO_INIT_T * GPIO_InitStruct) {
  U32 pinpos, pos, currentpin;

  //
  // GPIO Mode Configuration
  //
  for (pinpos = 0x00; pinpos < 0x10; pinpos++) {
    pos = ((U32)0x01) << pinpos;
    //
    // Get the port pins position
    //
    currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
    if (currentpin == pos) {
      GPIOx->MODER  &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
      GPIOx->MODER |= (((U32)GPIO_InitStruct->GPIO_Mode) << (pinpos * 2));
      if ((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF)) {
        //
        // Speed mode configuration
        //
        GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2));
        GPIOx->OSPEEDR |= ((U32)(GPIO_InitStruct->GPIO_Speed) << (pinpos * 2));
        //
        // Output mode configuration
        //
        GPIOx->OTYPER  &= ~((GPIO_OTYPER_OT_0) << ((U16)pinpos)) ;
        GPIOx->OTYPER |= (U16)(((U16)GPIO_InitStruct->GPIO_OType) << ((U16)pinpos));
      }
      //
      // Pull-up Pull down resistor configuration
      //
      GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((U16)pinpos * 2));
      GPIOx->PUPDR |= (((U32)GPIO_InitStruct->GPIO_PuPd) << (pinpos * 2));
    }
  }
}

/*********************************************************************
*
*       _RCC_AHB1PeriphClockCmd
*/
static void _RCC_AHB1PeriphClockCmd(U32 RCC_AHB1Periph, int NewState) {
  if (NewState != DISABLE) {
    RCC->AHB1ENR |=  RCC_AHB1Periph;
  } else {
    RCC->AHB1ENR &= ~RCC_AHB1Periph;
  }
}

/*********************************************************************
*
*       _RCC_APB2PeriphClockCmd
*/
static void _RCC_APB2PeriphClockCmd(U32 RCC_APB2Periph, int NewState) {
  if (NewState != DISABLE) {
    RCC->APB2ENR |= RCC_APB2Periph;
  } else {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/*********************************************************************
*
*       _RCC_PLLSAIConfig
*/
static void _RCC_PLLSAIConfig(U32 PLLSAIN, U32 PLLSAIQ, U32 PLLSAIR) {
  RCC->PLLSAICFGR = (PLLSAIN << 6) | (PLLSAIQ << 24) | (PLLSAIR << 28);
}

/*********************************************************************
*
*       _RCC_LTDCCLKDivConfig
*/
static void _RCC_LTDCCLKDivConfig(U32 RCC_PLLSAIDivR) {
  U32 tmpreg;

  tmpreg = RCC->DCKCFGR;
  tmpreg &= ~RCC_DCKCFGR_PLLSAIDIVR; // Clear PLLSAIDIVR[2:0] bits
  tmpreg |= RCC_PLLSAIDivR;          // Set PLLSAIDIVR values
  RCC->DCKCFGR = tmpreg;             // Store the new value
}

/*********************************************************************
*
*       _RCC_PLLSAICmd
*/
static void _RCC_PLLSAICmd(int NewState) {
  *(volatile U32 *)CR_PLLSAION_BB = (U32)NewState;
}

/*********************************************************************
*
*       _RCC_GetFlagStatus
*/
static int _RCC_GetFlagStatus(U8 RCC_FLAG) {
  U32 tmp, statusreg;
  int bitstatus;

  //
  // Get the RCC register index
  //
  tmp = RCC_FLAG >> 5;
  if (tmp == 1) {              /* The flag to check is in CR register */
    statusreg = RCC->CR;
  } else if (tmp == 2) {         /* The flag to check is in BDCR register */
    statusreg = RCC->BDCR;
  }  else {                      /* The flag to check is in CSR register */
    statusreg = RCC->CSR;
  }
  //
  // Get the flag position
  //
  tmp = RCC_FLAG & FLAG_MASK;
  if ((statusreg & ((U32)1 << tmp)) != (U32)RESET) {
    bitstatus = SET;
  } else {
    bitstatus = RESET;
  }
  //
  // Return the flag status
  //
  return bitstatus;
}

/*********************************************************************
*
*       _LCD_AF_GPIOConfig
*
* Purpose:
*   Pin configuration for display controller
*
*  LCD_TFT R0    - PI.15  LCD_TFT R1    - PJ.00  LCD_TFT R2    - PJ.01  LCD_TFT R3    - PJ.02
*  LCD_TFT R4    - PJ.03  LCD_TFT R5    - PJ.04  LCD_TFT R6    - PJ.05  LCD_TFT R7    - PJ.06
*  LCD_TFT G0    - PJ.07  LCD_TFT G1    - PJ.08  LCD_TFT G2    - PJ.09  LCD_TFT G3    - PJ.10
*  LCD_TFT G4    - PJ.11  LCD_TFT G5    - PK.00  LCD_TFT G6    - PK.01  LCD_TFT G7    - PK.02
*  LCD_TFT B0    - PJ.12  LCD_TFT B1    - PJ.13  LCD_TFT B2    - PJ.14  LCD_TFT B3    - PJ.15
*  LCD_TFT B4    - PK.03  LCD_TFT B5    - PK.04  LCD_TFT B6    - PK.05  LCD_TFT B7    - PK.06
*  LCD_TFT HSYNC - PI.12  LCD_TFT VSYNC - PI.13  LCD_TFT CLK   - PI.14
*  LCD_TFT DE    - PK.07  LCD_ENABLE    - PC.06  LCD_BACKLIGHT - PA.08
*/
static void _LCD_AF_GPIOConfig(void) {
  GPIO_INIT_T GPIO_InitStruct;

  //
  // Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks
  //
  _RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOJ | \
                         RCC_AHB1Periph_GPIOK | RCC_AHB1Periph_GPIOC | \
                         RCC_AHB1Periph_GPIOA,  ENABLE);
  //
  // GPIOI configuration
  //
  _GPIO_PinAFConfig(GPIOI, GPIO_PinSource12, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOI, GPIO_PinSource13, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOI, GPIO_PinSource14, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOI, GPIO_PinSource15, GPIO_AF_LTDC);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  _GPIO_Init(GPIOI, &GPIO_InitStruct);
  //
  // GPIOJ configuration
  //
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource0, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource1, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource2, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource3, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource4, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource5, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource6, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource7, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource8, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource9, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource10, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource11, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource12, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource13, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource14, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOJ, GPIO_PinSource15, GPIO_AF_LTDC);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                             GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                             GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                             GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  _GPIO_Init(GPIOJ, &GPIO_InitStruct);
  //
  // GPIOI configuration
  //
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource0, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource1, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource2, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource3, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource4, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource5, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource6, GPIO_AF_LTDC);
  _GPIO_PinAFConfig(GPIOK, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                             GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  _GPIO_Init(GPIOK, &GPIO_InitStruct);
  //
  // GPIOC configuration
  //
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  _GPIO_Init(GPIOC, &GPIO_InitStruct);
  //
  // GPIOA configuration
  //
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  _GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*********************************************************************
*
*       _LTDC_Init
*/
static void _LTDC_Init(LTDC_INIT_T * pInit) {
  U32 horizontalsync, accumulatedHBP, accumulatedactiveW, totalwidth, backgreen, backred;

  //
  // Sets Synchronization size
  //
  LTDC->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
  horizontalsync = (pInit->HorizontalSync << 16);
  LTDC->SSCR |= (horizontalsync | pInit->VerticalSync);
  //
  // Sets Accumulated Back porch
  //
  LTDC->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
  accumulatedHBP = (pInit->AccumulatedHBP << 16);
  LTDC->BPCR |= (accumulatedHBP | pInit->AccumulatedVBP);
  //
  // Sets Accumulated Active Width
  //
  LTDC->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
  accumulatedactiveW = (pInit->AccumulatedActiveW << 16);
  LTDC->AWCR |= (accumulatedactiveW | pInit->AccumulatedActiveH);
  //
  // Sets Total Width
  //
  LTDC->TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
  totalwidth = (pInit->TotalWidth << 16);
  LTDC->TWCR |= (totalwidth | pInit->TotalHeigh);
  LTDC->GCR &= (U32)GCR_MASK;
  LTDC->GCR |=  (U32)(pInit->HSPolarity | pInit->VSPolarity | pInit->DEPolarity | pInit->PCPolarity);
  //
  // Sets the background color value
  //
  backgreen = (pInit->BackgroundGreenValue << 8);
  backred = (pInit->BackgroundRedValue << 16);
  LTDC->BCCR &= ~(LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED);
  LTDC->BCCR |= (backred | backgreen | pInit->BackgroundBlueValue);
}

/*********************************************************************
*
*       _LTDC_LayerCmd
*/
static void _LTDC_LayerCmd(LTDC_LAYER_T * LTDC_Layerx, int NewState) {
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_LEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_LEN;
  }
  LTDC->SRCR = LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_LayerEnableColorKeying
*/
static void _LTDC_LayerEnableColorKeying(LTDC_LAYER_T * LTDC_Layerx, int NewState) {
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_COLKEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_COLKEN;
  }
  LTDC->SRCR = LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_LayerEnableLUT
*/
static void _LTDC_LayerEnableLUT(LTDC_LAYER_T * LTDC_Layerx, int NewState) {
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_CLUTEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_CLUTEN;
  }
  LTDC->SRCR = LTDC_SRCR_IMR;//LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_ReloadConfig
*/
static void _LTDC_ReloadConfig(U32 LTDC_Reload) {
  LTDC->SRCR = (U32)LTDC_Reload;
}

/*********************************************************************
*
*       _LTDC_Cmd
*/
static void _LTDC_Cmd(int NewState) {
  if (NewState != DISABLE) {
    LTDC->GCR |= (U32)LTDC_GCR_LTDCEN;
  } else {
    LTDC->GCR &= ~(U32)LTDC_GCR_LTDCEN;
  }
}

/*********************************************************************
*
*       _LTDC_ITConfig
*/
static void _LTDC_ITConfig(U32 LTDC_IT, int NewState) {
  if (NewState != DISABLE) {
    LTDC->IER |= LTDC_IT;
  } else {
    LTDC->IER &= (U32)~LTDC_IT;
  }
}

/*********************************************************************
*
*       _LTDC_SetLayerPos
*/
static void _LTDC_SetLayerPos(int LayerIndex, int xPos, int yPos) {
  int xSize, ySize;
  U32 HorizontalStart, HorizontalStop, VerticalStart, VerticalStop;

  xSize = LCD_GetXSizeEx(LayerIndex);
  ySize = LCD_GetYSizeEx(LayerIndex);
  HorizontalStart = xPos + HBP + 1;
  HorizontalStop  = xPos + HBP + xSize;
  VerticalStart   = yPos + VBP + 1;
  VerticalStop    = yPos + VBP + ySize;
  //
  // Horizontal start and stop position
  //
  _apLayer[LayerIndex]->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  _apLayer[LayerIndex]->WHPCR = (HorizontalStart | (HorizontalStop << 16));
  //
  // Vertical start and stop position
  //
  _apLayer[LayerIndex]->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  _apLayer[LayerIndex]->WVPCR  = (VerticalStart | (VerticalStop << 16));
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_SetLayerAlpha
*/
static void _LTDC_SetLayerAlpha(int LayerIndex, int Alpha) {
  //
  // Set constant alpha value
  //
  _apLayer[LayerIndex]->CACR &= ~(LTDC_LxCACR_CONSTA);
#if (GUI_USE_ARGB)
  _apLayer[LayerIndex]->CACR  = Alpha;
#else
  _apLayer[LayerIndex]->CACR  = 255 - Alpha;
#endif
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_IMR); // Reload immediately
}

/*********************************************************************
*
*       _LTDC_SetLUTEntry
*/
static void _LTDC_SetLUTEntry(int LayerIndex, U32 Color, int Pos) {
  U32 r, g, b, a;

  r = ( Color        & 0xff) << 16;
  g = ((Color >>  8) & 0xff) <<  8;
  b = ((Color >> 16) & 0xff);
  a = Pos << 24;
  _apLayer[LayerIndex]->CLUTWR &= ~(LTDC_LxCLUTWR_BLUE | LTDC_LxCLUTWR_GREEN | LTDC_LxCLUTWR_RED | LTDC_LxCLUTWR_CLUTADD);
  _apLayer[LayerIndex]->CLUTWR  = r | g | b | a;
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/*********************************************************************
*
*       _LTDC_LayerInit
*/
static void _LTDC_LayerInit(LTDC_LAYER_T * LTDC_Layerx, LTDC_INIT_LAYER_T * LTDC_Layer_InitStruct) {
  U32 whsppos, wvsppos, dcgreen, dcred, dcalpha, cfbp;

  //
  // Configures the horizontal start and stop position
  //
  whsppos = LTDC_Layer_InitStruct->HorizontalStop << 16;
  LTDC_Layerx->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  LTDC_Layerx->WHPCR = (LTDC_Layer_InitStruct->HorizontalStart | whsppos);
  //
  // Configures the vertical start and stop position
  //
  wvsppos = LTDC_Layer_InitStruct->VerticalStop << 16;
  LTDC_Layerx->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  LTDC_Layerx->WVPCR  = (LTDC_Layer_InitStruct->VerticalStart | wvsppos);
  //
  // Specifies the pixel format
  //
  LTDC_Layerx->PFCR &= ~(LTDC_LxPFCR_PF);
  LTDC_Layerx->PFCR = (LTDC_Layer_InitStruct->PixelFormat);
  //
  // Configures the default color values
  //
  dcgreen = (LTDC_Layer_InitStruct->DefaultColorGreen <<  8);
  dcred   = (LTDC_Layer_InitStruct->DefaultColorRed   << 16);
  dcalpha = (LTDC_Layer_InitStruct->DefaultColorAlpha << 24);
  LTDC_Layerx->DCCR &=  ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);
  LTDC_Layerx->DCCR = (LTDC_Layer_InitStruct->DefaultColorBlue | dcgreen | dcred | dcalpha);
  //
  // Specifies the constant alpha value
  //
  LTDC_Layerx->CACR &= ~(LTDC_LxCACR_CONSTA);
  LTDC_Layerx->CACR = (LTDC_Layer_InitStruct->ConstantAlpha);
  //
  // Specifies the blending factors
  //
  LTDC_Layerx->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
  LTDC_Layerx->BFCR = (LTDC_Layer_InitStruct->BlendingFactor_1 | LTDC_Layer_InitStruct->BlendingFactor_2);
  //
  // Configures the color frame buffer start address
  //
  LTDC_Layerx->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
  LTDC_Layerx->CFBAR = (LTDC_Layer_InitStruct->CFBStartAdress);
  //
  // Configures the color frame buffer pitch in byte
  //
  cfbp = (LTDC_Layer_InitStruct->CFBPitch << 16);
  LTDC_Layerx->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
  LTDC_Layerx->CFBLR  = (LTDC_Layer_InitStruct->CFBLineLength | cfbp);
  //
  // Configures the frame buffer line number
  //
  LTDC_Layerx->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
  LTDC_Layerx->CFBLNR  = (LTDC_Layer_InitStruct->CFBLineNumber);
}

/*********************************************************************
*
*       _NVIC_SetPriority
*/
static void _NVIC_SetPriority(int IRQn, U32 priority) {
  if(IRQn < 0) {
    while (1); // Not supported here, stop execution.
  } else {
    NVIC->IP[(U32)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); /* Set Priority for device specific Interrupts */
  }
}

/*********************************************************************
*
*       _NVIC_EnableIRQ
*/
static void _NVIC_EnableIRQ(int IRQn) {
  NVIC->ISER[(U32)((I32)IRQn) >> 5] = (U32)(1 << ((U32)((I32)IRQn) & (U32)0x1F)); /* Enable interrupt */
}

/*********************************************************************
*
*       _DMA2D_ITConfig
*/
static void _DMA2D_ITConfig(U32 DMA2D_IT, int NewState) {
  if (NewState != DISABLE) {
    DMA2D->CR |= DMA2D_IT;
  } else {
    DMA2D->CR &= (U32)~DMA2D_IT;
  }
}

/*********************************************************************
*
*       _DMA_ExecOperation
*/
static void _DMA_ExecOperation(void) {
  DMA2D->CR     |= 1;                               // Control Register (Start operation)
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }
}

/*********************************************************************
*
*       _DMA_Copy
*/
static void _DMA_Copy(int LayerIndex, const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00000000UL | (1 << 9);         // Control Register (Memory to memory and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = PixelFormat;                     // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_CopyRGB565
*/
static void _DMA_CopyRGB565(const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
  DMA2D->CR      = 0x00000000UL | (1 << 9);         // Control Register (Memory to memory and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = LTDC_Pixelformat_RGB565;         // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_Fill
*/
static void _DMA_Fill(int LayerIndex, void * pDst, int xSize, int ySize, int OffLine, U32 ColorIndex) {
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00030000UL | (1 << 9);         // Control Register (Register to memory and TCIE)
  DMA2D->OCOLR   = ColorIndex;                      // Output Color Register (Color to be used)
  //
  // Set up pointers
  //
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->OOR     = OffLine;                         // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->OPFCCR  = PixelFormat;                     // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_AlphaBlendingBulk
*/
static void _DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register
  DMA2D->BGOR    = 0;                               // Background Offset Register
  DMA2D->OOR     = 0;                               // Output Offset Register
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888;       // Foreground PFC Control Register (Defines the FG pixel format)
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888;       // Background PFC Control Register (Defines the BG pixel format)
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;       // Output     PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_MixColors
*
* Purpose:
*   Function for mixing up 2 colors with the given intensity.
*   If the background color is completely transparent the
*   foreground color should be used unchanged.
*/
static LCD_COLOR _DMA_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens) {
  U32 ColorFG, ColorBG, ColorDst;

  if ((BkColor & 0xFF000000) == 0xFF000000) {
    return Color;
  }
  ColorFG = Color   ^ 0xFF000000;
  ColorBG = BkColor ^ 0xFF000000;
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)&ColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)&ColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)&ColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(1 << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //

  //_DMA_ExecOperation();
  DMA2D->CR     |= 1;                               // Control Register (Start operation)
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }

  return ColorDst ^ 0xFF000000;
}

/*********************************************************************
*
*       _DMA_MixColorsBulk
*/
static void _DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_ConvertColor
*/
static void _DMA_ConvertColor(void * pSrc, void * pDst,  U32 PixelFormatSrc, U32 PixelFormatDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL | (1 << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = 0;                               // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = PixelFormatSrc;                  // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_DrawBitmapL8
*/
static void _DMA_DrawBitmapL8(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL | (1 << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = OffSrc;                          // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffDst;                          // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_L8;             // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | ySize;      // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_DrawBitmapA4
*/
static int _DMA_DrawBitmapA4(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  U8 * pRD;
  U8 * pWR;
  U32 NumBytes, Color, Index;

  //
  // Check size of conversion buffer
  //
  NumBytes = ((xSize + 1) & ~1) * ySize;
  if ((NumBytes > sizeof(_aBuffer)) || (NumBytes == 0)) {
    return 1;
  }
  //
  // Conversion (swapping nibbles)
  //
  pWR = (U8 *)_aBuffer;
  pRD = (U8 *)pSrc;
  do {
    *pWR++ = _aMirror[*pRD++];
  } while (--NumBytes);
  //
  // Get current drawing color (ABGR)
  //
  Index = LCD_GetColorIndex();
  Color = LCD_Index2Color(Index);
  //
  // Set up operation mode
  //
  DMA2D->CR = 0x00020000UL;
  //
  // Set up source
  //
  DMA2D->FGCOLR  = ((Color & 0xFF) << 16)  // Red
                 |  (Color & 0xFF00)       // Green
                 | ((Color >> 16) & 0xFF); // Blue
  DMA2D->FGMAR   = (U32)_aBuffer;
  DMA2D->FGOR    = 0;
  DMA2D->FGPFCCR = 0xA;                    // A4 bitmap
  DMA2D->NLR     = (U32)((xSize + OffSrc) << 16) | ySize;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->BGOR    = OffDst - OffSrc;
  DMA2D->BGPFCCR = PixelFormatDst;
  DMA2D->OMAR    = DMA2D->BGMAR;
  DMA2D->OOR     = DMA2D->BGOR;
  DMA2D->OPFCCR  = DMA2D->BGPFCCR;
  //
  // Execute operation
  //
  _DMA_ExecOperation();
  return 0;
}

/*********************************************************************
*
*       _DMA_LoadLUT
*/
static void _DMA_LoadLUT(LCD_COLOR * pColor, U32 NumItems) {
  DMA2D->FGCMAR  = (U32)pColor;                     // Foreground CLUT Memory Address Register
  //
  // Foreground PFC Control Register
  //
  DMA2D->FGPFCCR  = LTDC_Pixelformat_RGB888         // Pixel format
                  | ((NumItems - 1) & 0xFF) << 8;   // Number of items to load
  DMA2D->FGPFCCR |= (1 << 5);                       // Start loading
  //
  // Waiting not required here...
  //
}

/*********************************************************************
*
*       _InvertAlpha_SwapRB_CPY
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa. Result is located in destination buffer.
*
* Performance:
*   10.8 MPixel/s
*/
#if (GUI_USE_ARGB == 0)
static void _InvertAlpha_SwapRB_CPY(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
  while (NumItems >= 4) {
    Color = *(pColorSrc + 0);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 0) = Color;
    Color = *(pColorSrc + 1);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 1) = Color;
    Color = *(pColorSrc + 2);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 2) = Color;
    Color = *(pColorSrc + 3);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColorDst + 3) = Color;
    pColorSrc += 4;
    pColorDst += 4;
    NumItems -= 4;
  };
  while (NumItems--) {
    Color = *pColorSrc++;
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *pColorDst++ = Color;
  };
}

/*********************************************************************
*
*       _InvertAlpha_SwapRB_MOD
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa. Conversion is done in given buffer.
*
* Performance:
*   10.9 MPixel/s
*/
static void _InvertAlpha_SwapRB_MOD(LCD_COLOR * pColor, U32 NumItems) {
  U32 Color;
  while (NumItems >= 4) {
    Color = *(pColor + 0);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 0) = Color;
    Color = *(pColor + 1);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 1) = Color;
    Color = *(pColor + 2);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 2) = Color;
    Color = *(pColor + 3);
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *(pColor + 3) = Color;
    pColor += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColor;
    Color = ((Color << 24) >>  8)  // Red
          |  (Color & 0xFF00)      // Green
          | ((Color <<  8) >> 24)  // Blue
          | (~Color & 0xFF000000); // Alpha
    *pColor++ = Color;
  };
}

/*********************************************************************
*
*       _InvertAlpha_CPY
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the destination memory location.
*
* Performance:
*   17.5 MPixel/s
*/
static void _InvertAlpha_CPY(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;

  while (NumItems >= 4) {
    Color = *(pColorSrc + 0);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 0) = Color;
    Color = *(pColorSrc + 1);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 1) = Color;
    Color = *(pColorSrc + 2);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 2) = Color;
    Color = *(pColorSrc + 3);
    Color ^= 0xFF000000;      // Invert alpha
    *(pColorDst + 3) = Color;
    pColorSrc += 4;
    pColorDst += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColorSrc++;
    *pColorDst++ = Color ^ 0xFF000000; // Invert alpha
  };
}

/*********************************************************************
*
*       _InvertAlpha_MOD
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the given buffer.
*
* Performance:
*   18.0 MPixel/s
*/
static void _InvertAlpha_MOD(LCD_COLOR * pColor, U32 NumItems) {
  U32 Color;

  while (NumItems >= 4) {
    Color = *(pColor + 0);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 0) = Color;
    Color = *(pColor + 1);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 1) = Color;
    Color = *(pColor + 2);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 2) = Color;
    Color = *(pColor + 3);
    Color ^= 0xFF000000; // Invert alpha
    *(pColor + 3) = Color;
    pColor += 4;
    NumItems -= 4;
  }
  while (NumItems--) {
    Color = *pColor;
    Color ^= 0xFF000000; // Invert alpha
    *pColor++ = Color;
  };
}
#endif

/*********************************************************************
*
*       _DMA_AlphaBlending
*/
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
#if (GUI_USE_ARGB)
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(pColorFG, pColorBG, pColorDst, NumItems);
#else
  //
  // Invert alpha values
  //
  _InvertAlpha_CPY(pColorFG, _pBuffer_FG, NumItems);
  _InvertAlpha_CPY(pColorBG, _pBuffer_BG, NumItems);
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(_pBuffer_FG, _pBuffer_BG, pColorDst, NumItems);
  //
  // Invert alpha values
  //
  _InvertAlpha_MOD(pColorDst, NumItems);
#endif
}

/*********************************************************************
*
*       _DMA_Index2ColorBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given index values to 32 bit colors.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the color array needs to be converted after DMA2D has been used.
*/
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {
#if (GUI_USE_ARGB)
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_Pixelformat_ARGB8888, NumItems);
#else
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_Pixelformat_ARGB8888, NumItems);
  //
  // Convert colors from ARGB to ABGR and invert alpha values
  //
  _InvertAlpha_SwapRB_MOD(pColor, NumItems);
#endif
}

/*********************************************************************
*
*       _DMA_Color2IndexBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given 32 bit color array to index values.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the given color array needs to be converted before DMA2D can be used.
*/
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {
#if (GUI_USE_ARGB)
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pColor, pIndex, LTDC_Pixelformat_ARGB8888, PixelFormat, NumItems);
#else
  //
  // Convert colors from ABGR to ARGB and invert alpha values
  //
  _InvertAlpha_SwapRB_CPY(pColor, _pBuffer_DMA2D, NumItems);
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(_pBuffer_DMA2D, pIndex, LTDC_Pixelformat_ARGB8888, PixelFormat, NumItems);
#endif
}

/*********************************************************************
*
*       _LCD_MixColorsBulk
*/
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) {
#if (GUI_USE_ARGB)
  int y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
  for (y = 0; y < ySize; y++) {
    //
    // Use DMA2D for mixing up
    //
    _DMA_MixColorsBulk(pFG, pBG, pDst, Intens, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
#else
  int y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
  for (y = 0; y < ySize; y++) {
    //
    // Invert alpha values
    //
    _InvertAlpha_CPY(pFG, _pBuffer_FG, xSize);
    _InvertAlpha_CPY(pBG, _pBuffer_BG, xSize);
    //
    // Use DMA2D for mixing up
    //
    _DMA_MixColorsBulk(_pBuffer_FG, _pBuffer_BG, pDst, Intens, xSize);
    //
    // Invert alpha values
    //
    _InvertAlpha_MOD(pDst, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
#endif
}

/*********************************************************************
*
*       _LCD_DisplayOn
*/
static void _LCD_DisplayOn(void) {
  //
  // Enable LCD Backlight
  //
  _GPIO_SetBits(GPIOA, GPIO_Pin_8);
  _GPIO_SetBits(GPIOC, GPIO_Pin_6);
  //
  // Display On
  //
  _LTDC_Cmd(ENABLE); /* display ON */
}

/*********************************************************************
*
*       _LCD_DisplayOff
*/
static void _LCD_DisplayOff(void) {
  //
  // Disable LCD Backlight
  //
  _GPIO_ResetBits(GPIOA, GPIO_Pin_8);
  _GPIO_ResetBits(GPIOC, GPIO_Pin_6);
  //
  // Display Off
  //
  _LTDC_Cmd(DISABLE);
}

/*********************************************************************
*
*       _LCD_InitController
*/
static void _LCD_InitController(int LayerIndex) {
  LTDC_INIT_T       LTDC_InitStruct       = {0};
  LTDC_INIT_LAYER_T LTDC_Layer_InitStruct = {0};
  int xSize, ySize, BytesPerLine, BitsPerPixel, i;
  U32 Pixelformat, Color;
  static int Done;

  if (LayerIndex >= GUI_COUNTOF(_apLayer)) {
    return;
  }
  if (Done == 0) {
    Done = 1;
    //
    // Clock configuration
    //
    _RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);  // Enable LTDC Clock
    _RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); // Enable DMA2D Clock
    //
    // Configure the LCD Control pins
    //
    _LCD_AF_GPIOConfig();
    //
    // Configure PLLSAI prescalers for LCD:
    //   Enable Pixel Clock
    //   PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
    //   PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz
    //   PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz
    //   LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz
    //
    _RCC_PLLSAIConfig(192, 7, 3);
    _RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
    //
    // Enable PLLSAI Clock
    //
    _RCC_PLLSAICmd(ENABLE);
    //
    // Wait for PLLSAI activation
    //
    while(_RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
    //
    // Polarity configuration
    //
    LTDC_InitStruct.HSPolarity = LTDC_HSPolarity_AL;  // Horizontal synchronization polarity as active low
    LTDC_InitStruct.VSPolarity = LTDC_VSPolarity_AL;  // Vertical synchronization polarity as active low
    LTDC_InitStruct.DEPolarity = LTDC_DEPolarity_AL;  // Data enable polarity as active low
    LTDC_InitStruct.PCPolarity = LTDC_PCPolarity_IPC; // Pixel clock polarity as input pixel clock
    //
    // Configure R,G,B component values for LCD background color
    //
#if (GUI_USE_ARGB)
    LTDC_InitStruct.BackgroundRedValue   = (BK_COLOR >> 16) & 0xFF;
    LTDC_InitStruct.BackgroundGreenValue = (BK_COLOR >>  8) & 0xFF;
    LTDC_InitStruct.BackgroundBlueValue  = (BK_COLOR >>  0) & 0xFF;
#else
    LTDC_InitStruct.BackgroundRedValue   = (BK_COLOR >>  0) & 0xFF;
    LTDC_InitStruct.BackgroundGreenValue = (BK_COLOR >>  8) & 0xFF;
    LTDC_InitStruct.BackgroundBlueValue  = (BK_COLOR >> 16) & 0xFF;
#endif
    //
    // Timing configuration
    //
    LTDC_InitStruct.HorizontalSync     = HSW;                    // Horizontal synchronization width
    LTDC_InitStruct.VerticalSync       = VSW;                    // Vertical synchronization height
    LTDC_InitStruct.AccumulatedHBP     = HBP;                    // Accumulated horizontal back porch
    LTDC_InitStruct.AccumulatedVBP     = VBP;                    // Accumulated vertical back porch
    LTDC_InitStruct.AccumulatedActiveW = HBP + XSIZE_PHYS;       // Accumulated active width
    LTDC_InitStruct.AccumulatedActiveH = VBP + YSIZE_PHYS;       // Accumulated active height
    LTDC_InitStruct.TotalWidth         = HBP + XSIZE_PHYS + HFP; // Total width
    LTDC_InitStruct.TotalHeigh         = VBP + YSIZE_PHYS + VFP; // Total height
    _LTDC_Init(&LTDC_InitStruct);
    //
    // Enable line interrupt
    //
    _LTDC_ITConfig(LTDC_IER_LIE, ENABLE);
    _NVIC_SetPriority(LTDC_IRQn, 0);
    _NVIC_EnableIRQ(LTDC_IRQn);
    //
    // Enable DMA2D transfer complete interrupt
    //
    _DMA2D_ITConfig(DMA2D_CR_TCIE, ENABLE);
    _NVIC_SetPriority(DMA2D_IRQn, 0);
    _NVIC_EnableIRQ(DMA2D_IRQn);
    //
    // Clear transfer complete interrupt flag
    //
    DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
  }
  //
  // Layer configuration
  //
  if (LCD_GetSwapXYEx(LayerIndex)) {
    xSize = LCD_GetYSizeEx(LayerIndex);
    ySize = LCD_GetXSizeEx(LayerIndex);
  } else {
    xSize = LCD_GetXSizeEx(LayerIndex);
    ySize = LCD_GetYSizeEx(LayerIndex);
  }
  //
  // Windowing configuration
  //   Horizontal start = horizontal synchronization + Horizontal back porch = 43
  //   Vertical start   = vertical synchronization   + vertical back porch   = 12
  //   Horizontal stop  = Horizontal start + LCD_PIXEL_WIDTH  -1
  //   Vertical stop    = Vertical start   + LCD_PIXEL_HEIGHT -1
  //
  LTDC_Layer_InitStruct.HorizontalStart = HBP + 1;
  LTDC_Layer_InitStruct.HorizontalStop  = (xSize + HBP);
  LTDC_Layer_InitStruct.VerticalStart   = VBP + 1;
  LTDC_Layer_InitStruct.VerticalStop    = (ySize + VBP);
  //
  // Pixel Format configuration
  //
  Pixelformat = _GetPixelformat(LayerIndex);
  LTDC_Layer_InitStruct.PixelFormat = Pixelformat;
  //
  // Alpha constant (255 totally opaque)
  //
  LTDC_Layer_InitStruct.ConstantAlpha = 255;
  //
  // Default Color configuration (configure A, R, G, B component values)
  //
  LTDC_Layer_InitStruct.DefaultColorBlue  = 0;
  LTDC_Layer_InitStruct.DefaultColorGreen = 0;
  LTDC_Layer_InitStruct.DefaultColorRed   = 0;
  LTDC_Layer_InitStruct.DefaultColorAlpha = 0;
  //
  // Configure blending factors
  //
  BytesPerLine = _GetBytesPerLine(LayerIndex, xSize);
  LTDC_Layer_InitStruct.BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_InitStruct.BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
  LTDC_Layer_InitStruct.CFBLineLength    = BytesPerLine + 3;
  LTDC_Layer_InitStruct.CFBPitch         = BytesPerLine;
  LTDC_Layer_InitStruct.CFBLineNumber    = ySize;
  //
  // Input Address configuration
  //
  LTDC_Layer_InitStruct.CFBStartAdress = _aAddr[LayerIndex];
  _LTDC_LayerInit(_apLayer[LayerIndex], &LTDC_Layer_InitStruct);
  //
  // Enable LUT on demand
  //
  BitsPerPixel = LCD_GetBitsPerPixelEx(LayerIndex);
  if (BitsPerPixel <= 8) {
    //
    // Enable usage of LUT for all modes with <= 8bpp
    //
    _LTDC_LayerEnableLUT(_apLayer[LayerIndex], ENABLE);
  } else {
    //
    // Optional CLUT initialization for AL88 mode (16bpp)
    //
    if (_apColorConvAPI[LayerIndex] == GUICC_88666I) {
      _LTDC_LayerEnableLUT(_apLayer[LayerIndex], ENABLE);
      for (i = 0; i < 256; i++) {
        Color = LCD_API_ColorConv_8666.pfIndex2Color(i);
        _LTDC_SetLUTEntry(LayerIndex, Color, i);
      }
    }
  }
  //
  // Enable layer
  //
  _LTDC_LayerCmd(_apLayer[LayerIndex], ENABLE);
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/*********************************************************************
*
*       _GetBufferSize
*/
static U32 _GetBufferSize(int LayerIndex) {
  U32 BufferSize;

  BufferSize = _axSize[LayerIndex] * _aySize[LayerIndex] * _aBytesPerPixels[LayerIndex];
  return BufferSize;
}

/*********************************************************************
*
*       _LCD_CopyBuffer
*/
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) {
  U32 BufferSize, AddrSrc, AddrDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc    = _aAddr[LayerIndex] + BufferSize * IndexSrc;
  AddrDst    = _aAddr[LayerIndex] + BufferSize * IndexDst;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, _axSize[LayerIndex], _aySize[LayerIndex], 0, 0);
  _aBufferIndex[LayerIndex] = IndexDst; // After this function has been called all drawing operations are routed to Buffer[IndexDst]!
}

/*********************************************************************
*
*       _LCD_CopyRect
*/
static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) {
  U32 BufferSize, AddrSrc, AddrDst;
  int OffLine;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y1 * _axSize[LayerIndex] + x1) * _aBytesPerPixels[LayerIndex];
  OffLine = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, OffLine, OffLine);
}

/*********************************************************************
*
*       _LCD_FillRect
*/
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) {
  U32 BufferSize, AddrDst;
  int xSize, ySize;

  if (GUI_GetDrawMode() == GUI_DM_XOR) {
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
  } else {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = _GetBufferSize(LayerIndex);
    AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
    _DMA_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, _axSize[LayerIndex] - xSize, PixelIndex);
  }
}

/*********************************************************************
*
*       _LCD_DrawBitmap32bpp
*/
static void _LCD_DrawBitmap32bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmap16bpp
*/
static void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmap8bpp
*/
static void _LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_DrawBitmapL8((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);
}

/*********************************************************************
*
*       _LCD_DrawBitmap4bpp
*/
static int _LCD_DrawBitmap4bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Check if destination has direct color mode
  //
  if (PixelFormat > LTDC_Pixelformat_ARGB4444) {
    return 1;
  }
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine * 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  return _DMA_DrawBitmapA4((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);;
}

/*********************************************************************
*
*       _LCD_DrawMemdev16bpp
*/
static void _LCD_DrawMemdev16bpp(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 2) - xSize;
  OffLineDst = (BytesPerLineDst / 2) - xSize;
  _DMA_CopyRGB565(pSrc, pDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*********************************************************************
*
*       _LCD_GetpPalConvTable
*
* Purpose:
*   The emWin function LCD_GetpPalConvTable() normally translates the given colors into
*   index values for the display controller. In case of index based bitmaps without
*   transparent pixels we load the palette only to the DMA2D LUT registers to be
*   translated (converted) during the process of drawing via DMA2D.
*/
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex) {
#if (GUI_USE_ARGB)
  void (* pFunc)(void);
  int DoDefault = 0;

  //
  // Check if we have a non transparent device independent bitmap
  //
  if (pBitmap->BitsPerPixel == 8) {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) {
      if (pBitmap->pPal) {
        if (pBitmap->pPal->HasTrans) {
          DoDefault = 1;
        }
      } else {
        DoDefault = 1;
      }
    } else {
      DoDefault = 1;
    }
  } else {
    DoDefault = 1;
  }
  //
  // Default palette management for other cases
  //
  if (DoDefault) {
    //
    // Return a pointer to the index values to be used by the controller
    //
    return LCD_GetpPalConvTable(pLogPal);
  }
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT((U32 *)pLogPal->pPalEntries, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return _pBuffer_DMA2D;
#else
  void (* pFunc)(void);
  int DoDefault = 0;

  //
  // Check if we have a non transparent device independent bitmap
  //
  if (pBitmap->BitsPerPixel == 8) {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) {
      if (pBitmap->pPal) {
        if (pBitmap->pPal->HasTrans) {
          DoDefault = 1;
        }
      } else {
        DoDefault = 1;
      }
    } else {
      DoDefault = 1;
    }
  } else {
    DoDefault = 1;
  }
  //
  // Default palette management for other cases
  //
  if (DoDefault) {
    //
    // Return a pointer to the index values to be used by the controller
    //
    return LCD_GetpPalConvTable(pLogPal);
  }
  //
  // Convert palette colors from ARGB to ABGR
  //
  _InvertAlpha_SwapRB_CPY((U32 *)pLogPal->pPalEntries, _pBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT(_pBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return _pBuffer_DMA2D;
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
*       DMA2D_ISR_Handler
*
* Purpose:
*   Transfer-complete-interrupt of DMA2D
*/
void DMA2D_ISR_Handler(void) {
  DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
}

/*********************************************************************
*
*       LTDC_ISR_Handler
*
* Purpose:
*   End-Of-Frame-Interrupt for managing multiple buffering
*/
void LTDC_ISR_Handler(void) {
  U32 Addr;
  int i;

  LTDC->ICR = (U32)LTDC_IER_LIE;
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    if (_aPendingBuffer[i] >= 0) {
      //
      // Calculate address of buffer to be used  as visible frame buffer
      //
      Addr = _aAddr[i] + _axSize[i] * _aySize[i] * _aPendingBuffer[i] * _aBytesPerPixels[i];
      //
      // Store address into SFR
      //
      _apLayer[i]->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
      _apLayer[i]->CFBAR = Addr;
      //
      // Reload configuration
      //
      _LTDC_ReloadConfig(LTDC_SRCR_IMR);
      //
      // Tell emWin that buffer is used
      //
      GUI_MULTIBUF_ConfirmEx(i, _aPendingBuffer[i]);
      //
      // Clear pending buffer flag of layer
      //
      _aPendingBuffer[i] = -1;
    }
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
  int r = 0;

  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the display controller and put it into operation.
    //
    _LCD_InitController(LayerIndex);
    break;
  }
  case LCD_X_SETORG: {
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
    LCD_X_SETORG_INFO * p;

    p = (LCD_X_SETORG_INFO *)pData;
    _apLayer[LayerIndex]->CFBAR = _aAddr[LayerIndex] + p->yPos * _axSize[LayerIndex] * _aBytesPerPixels[LayerIndex];
    _LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
    break;
  }
  case LCD_X_SHOWBUFFER: {
    //
    // Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
    //
    LCD_X_SHOWBUFFER_INFO * p;

    p = (LCD_X_SHOWBUFFER_INFO *)pData;
    _aPendingBuffer[LayerIndex] = p->Index;
    break;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;

    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _LTDC_SetLUTEntry(LayerIndex, p->Color, p->Pos);
    break;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    _LCD_DisplayOn();
    break;
  }
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    _LCD_DisplayOff();
    break;
  }
  case LCD_X_SETVIS: {
    //
    // Required for setting the layer visibility which is passed in the 'OnOff' element of pData
    //
    LCD_X_SETVIS_INFO * p;

    p = (LCD_X_SETVIS_INFO *)pData;
    _LTDC_LayerCmd(_apLayer[LayerIndex], p->OnOff ? ENABLE : DISABLE);
    break;
  }
  case LCD_X_SETPOS: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETPOS_INFO * p;

    p = (LCD_X_SETPOS_INFO *)pData;
    _LTDC_SetLayerPos(LayerIndex, p->xPos, p->yPos);
    break;
  }
  case LCD_X_SETSIZE: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETSIZE_INFO * p;
    int xPos, yPos;

    GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
    p = (LCD_X_SETSIZE_INFO *)pData;
    if (LCD_GetSwapXYEx(LayerIndex)) {
      _axSize[LayerIndex] = p->ySize;
      _aySize[LayerIndex] = p->xSize;
    } else {
      _axSize[LayerIndex] = p->xSize;
      _aySize[LayerIndex] = p->ySize;
    }
    _LTDC_SetLayerPos(LayerIndex, xPos, yPos);
    break;
  }
  case LCD_X_SETALPHA: {
    //
    // Required for setting the alpha value which is passed in the 'Alpha' element of pData
    //
    LCD_X_SETALPHA_INFO * p;

    p = (LCD_X_SETALPHA_INFO *)pData;
    _LTDC_SetLayerAlpha(LayerIndex, p->Alpha);
    break;
  }
  case LCD_X_SETCHROMAMODE: {
    //
    // Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData
    //
    LCD_X_SETCHROMAMODE_INFO * p;

    p = (LCD_X_SETCHROMAMODE_INFO *)pData;
    _LTDC_LayerEnableColorKeying(_apLayer[LayerIndex], (p->ChromaMode != 0) ? ENABLE : DISABLE);
    break;
  }
  case LCD_X_SETCHROMA: {
    //
    // Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData
    //
    LCD_X_SETCHROMA_INFO * p;
    U32 Color;

    p = (LCD_X_SETCHROMA_INFO *)pData;
    Color = ((p->ChromaMin & 0xFF0000) >> 16) | (p->ChromaMin & 0x00FF00) | ((p->ChromaMin & 0x0000FF) << 16);
    _apLayer[LayerIndex]->CKCR = Color;
    _LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
    break;
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
  int i;
  U32 PixelFormat;

  //
  // At first initialize use of multiple buffers on demand
  //
  #if (NUM_BUFFERS > 1)
    for (i = 0; i < GUI_NUM_LAYERS; i++) {
      GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
    }
  #endif
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
  //
  // Set size of 1st layer
  //
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx (0, YSIZE_0, XSIZE_0);
    LCD_SetVSizeEx(0, YSIZE_0 * NUM_VSCREENS, XSIZE_0);
  } else {
    LCD_SetSizeEx (0, XSIZE_0, YSIZE_0);
    LCD_SetVSizeEx(0, XSIZE_0, YSIZE_0 * NUM_VSCREENS);
  }
  #if (GUI_NUM_LAYERS > 1)
    //
    // Set display driver and color conversion for 2nd layer
    //
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
    //
    // Set size of 2nd layer
    //
    if (LCD_GetSwapXYEx(1)) {
      LCD_SetSizeEx (1, YSIZE_0, XSIZE_1);
      LCD_SetVSizeEx(1, YSIZE_1 * NUM_VSCREENS, XSIZE_1);
    } else {
      LCD_SetSizeEx (1, XSIZE_1, YSIZE_1);
      LCD_SetVSizeEx(1, XSIZE_1, YSIZE_1 * NUM_VSCREENS);
    }
  #endif
  //
  // Setting up VRam address and remember pixel size
  //
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    LCD_SetVRAMAddrEx(i, (void *)(_aAddr[i]));                                                 // Setting up VRam address
    _aBytesPerPixels[i] = LCD_GetBitsPerPixelEx(i) >> 3;                                       // Remember pixel size
  }
  //
  // Setting up custom functions
  //
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    if (_aOrientation[i] == ROTATION_0) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);               // Set custom function for copying complete buffers (used by multiple buffering) using DMA2D
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT, (void(*)(void))_LCD_CopyRect);                   // Set custom function for copy recxtangle areas (used by GUI_CopyRect()) using DMA2D
      //
      // Set functions for direct color mode layers. Won't work with indexed color modes because of missing LUT for DMA2D destination
      //
      PixelFormat = _GetPixelformat(i);
      if (PixelFormat <= LTDC_Pixelformat_ARGB4444) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);                 // Set custom function for filling operations using DMA2D
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))_LCD_DrawBitmap8bpp);       // Set up custom drawing routine for index based bitmaps using DMA2D
      }
      //
      // Set up drawing routine for 16bpp bitmap using DMA2D
      //
      if (PixelFormat == LTDC_Pixelformat_RGB565) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))_LCD_DrawBitmap16bpp);     // Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565
      }
      //
      // Set up drawing routine for 32bpp bitmap using DMA2D
      //
      if (PixelFormat == LTDC_Pixelformat_ARGB8888) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))_LCD_DrawBitmap32bpp);     // Set up drawing routine for 32bpp bitmap using DMA2D. Makes only sense with ARGB8888
      }
    }
  }
  //
  // Set up custom color conversion using DMA2D, works only for direct color modes because of missing LUT for DMA2D destination
  //
  GUICC_M1555I_SetCustColorConv(_Color2IndexBulk_M1555I_DMA2D, _Index2ColorBulk_M1555I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB1555
  //GUICC_M565_SetCustColorConv  (_Color2IndexBulk_M565_DMA2D,   _Index2ColorBulk_M565_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB565 (does not speed up conversion, default method is slightly faster!)
  GUICC_M4444I_SetCustColorConv(_Color2IndexBulk_M4444I_DMA2D, _Index2ColorBulk_M4444I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB4444
  GUICC_M888_SetCustColorConv  (_Color2IndexBulk_M888_DMA2D,   _Index2ColorBulk_M888_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB888
  GUICC_M8888I_SetCustColorConv(_Color2IndexBulk_M8888I_DMA2D, _Index2ColorBulk_M8888I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB8888
  //
  // Set up custom alpha blending function using DMA2D
  //
  GUI_SetFuncAlphaBlending(_DMA_AlphaBlending);                                                // Set up custom alpha blending function using DMA2D
  //
  // Set up custom function for translating a bitmap palette into index values.
  // Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap
  //
  GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
  //
  // Set up custom function for mixing up single colors using DMA2D
  //
  GUI_SetFuncMixColors(_DMA_MixColors);
  //
  // Set up custom function for mixing up arrays of colors using DMA2D
  //
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);
  //
  // Set up custom function for drawing AA4 characters
  //
  GUI_AA_SetpfDrawCharAA4(_LCD_DrawBitmap4bpp);
  //
  // Set up custom function for drawing 16bpp memory devices
  //
  GUI_MEMDEV_SetDrawMemdev16bppFunc(_LCD_DrawMemdev16bpp);
}

/*************************** End of file ****************************/
