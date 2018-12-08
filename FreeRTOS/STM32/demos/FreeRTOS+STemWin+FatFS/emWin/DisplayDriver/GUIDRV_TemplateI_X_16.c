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
File        : GUIDRV_TemplateI_X_16.c
Purpose     : Non optimized drawing routines of GUIDRV_TemplateI (16bpp)
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>

#include "GUI_Private.h"
#include "LCD_SIM.h"

#include "GUIDRV_TemplateI_Private.h"

#if (!defined(WIN32) | defined(LCD_SIMCONTROLLER))

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Existing optimizations
//
#define OPTIMIZATION      0
#define OPTIMIZATION_OX   0
#define OPTIMIZATION_OY   0
#define OPTIMIZATION_OXY  0
#define OPTIMIZATION_OS   0
#define OPTIMIZATION_OSX  0
#define OPTIMIZATION_OSY  0
#define OPTIMIZATION_OSXY 0

//
// Offset calculation
//
#define XY2OFF(pContext, x, y) (U32)((U32)y * pContext->BytesPerLine + x)

/*********************************************************************
*
*       Static code: Common for all
*
**********************************************************************
*/
/*********************************************************************
*
*       _OnInitHook
*/
static void _OnInitHook(DRIVER_CONTEXT * pContext) {
  GUI_USE_PARA(pContext);
  //
  // TBD: Add specific code required on initialization like memory allocation for a cache
  //
}

/*********************************************************************
*
*       _GetDevFunc
*/
static void (* _GetDevFunc(GUI_DEVICE ** ppDevice, int Index))(void) {
  switch (Index) {
  case PRIVATE_DEVFUNC_ONINITHOOK:
    return (void (*)(void))_OnInitHook;
  }
  return GUIDRV__TemplateI_GetDevFunc(ppDevice, Index);
}

/*********************************************************************
*
*       _GetDevProp
*/
static I32 _GetDevProp(GUI_DEVICE * pDevice, int Index) {
  switch (Index) {
  case LCD_DEVCAP_BITSPERPIXEL: return 16;
  case LCD_DEVCAP_NUMCOLORS:    return 32768;
  }
  return GUIDRV__TemplateI_GetDevProp(pDevice, Index);
}

/*********************************************************************
*
*       _GetDevData
*/
static void * _GetDevData(GUI_DEVICE * pDevice, int Index) {
  GUI_USE_PARA(pDevice);
  #if GUI_SUPPORT_MEMDEV
    switch (Index) {
    case LCD_DEVDATA_MEMDEV:
      return (void *)&GUI_MEMDEV_DEVICE_16;
    }
  #else
    GUI_USE_PARA(Index);
  #endif
  return NULL;
}

/*********************************************************************
*
*       _SetPixelIndex
*/
static void _SetPixelIndex(GUI_DEVICE * pDevice, int x, int y, int PixelIndex) {
  #ifdef WIN32
    LCDSIM_SetPixelIndex(x, y, PixelIndex, pDevice->LayerIndex);
  #else
    //
    // TBD: Add 'SetPixel' code
    //
    GUI_USE_PARA(pDevice);
    GUI_USE_PARA(x);
    GUI_USE_PARA(y);
    GUI_USE_PARA(PixelIndex);
  #endif
}

/*********************************************************************
*
*       _GetPixelIndex
*/
static unsigned int _GetPixelIndex(GUI_DEVICE * pDevice, int x, int y) {
  unsigned int PixelIndex;
  
  #ifdef WIN32
    PixelIndex = LCDSIM_GetPixelIndex(x, y, pDevice->LayerIndex);
  #else
    //
    // TBD: Add 'GetPixel' code
    //
    GUI_USE_PARA(pDevice);
    GUI_USE_PARA(x);
    GUI_USE_PARA(y);
    PixelIndex = 0;
  #endif
  return PixelIndex;
}

/*********************************************************************
*
*       Orientation dependent code and data
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDRV_TemplateI_##EXT##
*/
#define DEFINE_GUI_DEVICE_API(EXT)                     \
const GUI_DEVICE_API GUIDRV_TEMPLATE_I_##EXT##_API = { \
  DEVICE_CLASS_DRIVER,                                 \
  GUIDRV__NoOpt_DrawBitmap,                            \
  GUIDRV__NoOpt_DrawHLine,                             \
  GUIDRV__NoOpt_DrawVLine,                             \
  GUIDRV__NoOpt_FillRect,                              \
  _GetPixelIndex_##EXT,                                \
  _SetPixelIndex_##EXT,                                \
  GUIDRV__NoOpt_XorPixel,                              \
  GUIDRV__TemplateI_SetOrg,                            \
  _GetDevFunc,                                         \
  _GetDevProp_##EXT,                                   \
  _GetDevData,                                         \
  GUIDRV__TemplateI_GetRect,                           \
};

/*********************************************************************
*
*       The actual drivers...
*/
#if (OPTIMIZATION      == 0)
  DEFINE_FUNCTIONS(     16, LOG2PHYS_X,      LOG2PHYS_Y,      0, 0, 0)
#endif                                                        
#if (OPTIMIZATION_OX   == 0)                                 
  DEFINE_FUNCTIONS(  OX_16, LOG2PHYS_X_OX,   LOG2PHYS_Y_OX,   1, 0, 0)
#endif                                                        
#if (OPTIMIZATION_OY   == 0)                                  
  DEFINE_FUNCTIONS(  OY_16, LOG2PHYS_X_OY,   LOG2PHYS_Y_OY,   0, 1, 0)
#endif                                                        
#if (OPTIMIZATION_OXY  == 0)                                  
  DEFINE_FUNCTIONS( OXY_16, LOG2PHYS_X_OXY,  LOG2PHYS_Y_OXY,  1, 1, 0)
#endif                                                        
#if (OPTIMIZATION_OS   == 0)                                  
  DEFINE_FUNCTIONS(  OS_16, LOG2PHYS_X_OS,   LOG2PHYS_Y_OS,   0, 0, 1)
#endif                                                        
#if (OPTIMIZATION_OSX  == 0)                                  
  DEFINE_FUNCTIONS( OSX_16, LOG2PHYS_X_OSX,  LOG2PHYS_Y_OSX,  1, 0, 1)
#endif                                                        
#if (OPTIMIZATION_OSY  == 0)                                  
  DEFINE_FUNCTIONS( OSY_16, LOG2PHYS_X_OSY,  LOG2PHYS_Y_OSY,  0, 1, 1)
#endif                                                        
#if (OPTIMIZATION_OSXY == 0)                                  
  DEFINE_FUNCTIONS(OSXY_16, LOG2PHYS_X_OSXY, LOG2PHYS_Y_OSXY, 1, 1, 1)
#endif

#else

void GUIDRV_TemplateI_X_16_C(void);   // Avoid empty object files
void GUIDRV_TemplateI_X_16_C(void) {}

#endif

/*************************** End of file ****************************/
