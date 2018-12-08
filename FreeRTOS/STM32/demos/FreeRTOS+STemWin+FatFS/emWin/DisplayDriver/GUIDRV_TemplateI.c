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
File        : GUIDRV_TemplateI.c
Purpose     : Display driver
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
#if defined(WIN32) && defined(LCD_SIMCONTROLLER)
  //
  // TBD: Add prototypes of simulation routines
  //
#endif

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitOnce
*
* Purpose:
*   Allocates a fixed block for the context of the driver
*
* Return value:
*   0 on success, 1 on error
*/
static int _InitOnce(GUI_DEVICE * pDevice) {
  if (pDevice->u.pContext == NULL) {
    pDevice->u.pContext = GUI_ALLOC_GetFixedBlock(sizeof(DRIVER_CONTEXT));
    GUI__memset((U8 *)pDevice->u.pContext, 0, sizeof(DRIVER_CONTEXT));
  }
  if (pDevice->u.pContext) {
    return 0; // Ok
  } else {
    return 1; // Error
  }
}

/*********************************************************************
*
*       _SetVSize
*/
static void _SetVSize(GUI_DEVICE * pDevice, int xSize, int ySize) {
  DRIVER_CONTEXT * pContext;

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    pContext->vxSize = xSize;
    pContext->vySize = ySize;
  }
}

/*********************************************************************
*
*       _SetSize
*/
static void _SetSize(GUI_DEVICE * pDevice, int xSize, int ySize) {
  DRIVER_CONTEXT * pContext;

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    pContext->xSize = xSize;
    pContext->ySize = ySize;
    if (pContext->vxSize == 0) {
      pContext->vxSize = pContext->xSize;
    }
    if (pContext->vySize == 0) {
      pContext->vySize = pContext->ySize;
    }
  }
}

/*********************************************************************
*
*       _Check_XXX_BitBus
*/
static void _Check_XXX_BitBus(GUI_DEVICE * pDevice) {
  //
  // TBD: Check if all required port routines are available
  //
  GUI_USE_PARA(pDevice);
}

/*********************************************************************
*
*       Static code: Initialization
*
**********************************************************************
*/
/*********************************************************************
*
*       _Init
*/
static int _Init(GUI_DEVICE * pDevice) {
  DRIVER_CONTEXT * pContext;
  T_ONINITHOOK pfOnInitHook;
  int xSize;

  if (_InitOnce(pDevice)) {
    GUI_DEBUG_ERROROUT("GUIDRV_TemplateI.c:\nNot enough memory available in function _Init().");
  }
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    //
    // Calculate BytesPerLine and BitsPerPixel
    //
    if (pDevice->pDeviceAPI->pfGetDevProp(pDevice, LCD_DEVCAP_SWAP_XY)) {
      xSize = pContext->ySize;
    } else {
      xSize = pContext->xSize;
    }
    pContext->BitsPerPixel = (int)pDevice->pDeviceAPI->pfGetDevProp(pDevice, LCD_DEVCAP_BITSPERPIXEL);
    pContext->BytesPerLine = (xSize * pContext->BitsPerPixel + 7) / 8;
    //
    // Driver specific initialization
    //
    pfOnInitHook = (T_ONINITHOOK)pDevice->pDeviceAPI->pfGetDevFunc(&pDevice, PRIVATE_DEVFUNC_ONINITHOOK);
    if (pfOnInitHook) {
      pfOnInitHook(pContext);
    }
    //
    // Controller specific initialization
    //
    if (pContext->pfInit) {
      pContext->pfInit(pDevice);
    } else {
      GUI_DEBUG_ERROROUT("GUIDRV_TemplateI.c:\nNo display controller selected in function _Init().");
    }
  }
  return LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_INITCONTROLLER, NULL);
}

/*********************************************************************
*
*       Private code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDRV__TemplateI_SetOrg
*/
void GUIDRV__TemplateI_SetOrg(GUI_DEVICE * pDevice, int x, int y) {
  LCD_X_SETORG_INFO Data = {0};

  #ifdef WIN32
    LCDSIM_SetOrg(x, y, pDevice->LayerIndex);
  #else
    Data.xPos = x;
    Data.yPos = y;
    LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETORG, (void *)&Data);
  #endif
}

/*********************************************************************
*
*       GUIDRV__TemplateI_GetDevFunc
*/
void (* GUIDRV__TemplateI_GetDevFunc(GUI_DEVICE ** ppDevice, int Index))(void) {
  GUI_USE_PARA(ppDevice);
  switch (Index) {
  case LCD_DEVFUNC_SET_VSIZE:
    return (void (*)(void))_SetVSize;
  case LCD_DEVFUNC_SET_SIZE:
    return (void (*)(void))_SetSize;
  case LCD_DEVFUNC_INIT:
    return (void (*)(void))_Init;
  }
  return NULL;
}

/*********************************************************************
*
*       GUIDRV__TemplateI_GetDevProp
*/
I32 GUIDRV__TemplateI_GetDevProp(GUI_DEVICE * pDevice, int Index) {
  DRIVER_CONTEXT * pContext;

  pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
  switch (Index) {
  case LCD_DEVCAP_XSIZE:
    return pContext->xSize;
  case LCD_DEVCAP_YSIZE:
    return pContext->ySize;
  case LCD_DEVCAP_VXSIZE:
    return pContext->vxSize;
  case LCD_DEVCAP_VYSIZE:
    return pContext->vySize;
  case LCD_DEVCAP_XMAG:
    return 1;
  case LCD_DEVCAP_YMAG:
    return 1;
  }
  return -1;
}

/*********************************************************************
*
*       GUIDRV__TemplateI_GetRect
*/
void GUIDRV__TemplateI_GetRect(GUI_DEVICE * pDevice, LCD_RECT * pRect) {
  DRIVER_CONTEXT * pContext;

  pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
  pRect->x0 = 0;
  pRect->y0 = 0;
  pRect->x1 = pContext->vxSize - 1;
  pRect->y1 = pContext->vySize - 1;
}

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
//
// TBD: Add functions to be used for function pointers of MANAGE_VMEM_API
//

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDRV_TemplateI_Config
*/
void GUIDRV_TemplateI_Config(GUI_DEVICE * pDevice, CONFIG_TEMPLATE_I * pConfig) {
  DRIVER_CONTEXT * pContext;

  if (pDevice) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    if (pContext) {
      //
      // TBD: Process items of config structure
      //
      GUI_USE_PARA(pConfig);
    }
  }
}

/*********************************************************************
*
*       GUIDRV_TemplateI_SetBus_XXX
*/
void GUIDRV_TemplateI_SetBus_XXX(GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API) {
  DRIVER_CONTEXT * pContext;

  if (pDevice) {
    pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;
    if (pContext) {
      //
      // Get function pointers
      //
      pContext->HW_API = *pHW_API;
      //
      // Set routine for checking interface routines
      //
      pContext->pfCheck = _Check_XXX_BitBus;
      //
      // TBD: Set up function pointers of MANAGE_VMEM_API
      //
    }
  }
}


#else

void GUIDRV_TemplateI_C(void);   // Avoid empty object files
void GUIDRV_TemplateI_C(void) {}

#endif

/*************************** End of file ****************************/
