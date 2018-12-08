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
File        : GUI_X_Joystick.c
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "Board_Joystick.h"

/*********************************************************************
*
*       Joystick support
*
*/

#ifdef  JOYSTICK_ROTATE
#define BUTTON_LEFT   JOYSTICK_DOWN
#define BUTTON_RIGHT  JOYSTICK_UP
#define BUTTON_UP     JOYSTICK_LEFT
#define BUTTON_DOWN   JOYSTICK_RIGHT
#define BUTTON_CENTER JOYSTICK_CENTER
#else
#define BUTTON_LEFT   JOYSTICK_LEFT
#define BUTTON_RIGHT  JOYSTICK_RIGHT
#define BUTTON_UP     JOYSTICK_UP
#define BUTTON_DOWN   JOYSTICK_DOWN
#define BUTTON_CENTER JOYSTICK_CENTER
#endif

void GUI_JOYSTICK_Initialize (void) {
  Joystick_Initialize();
}

void GUI_JOYSTICK_Exec (void) {
  GUI_PID_STATE state;
  static  U8    hold = 0;
  static  U8    prevkeys = 0;
          U32   keys;
          I32   diff, max;

  // Read Joystick keys
  keys = Joystick_GetState();

  // Dynamic pointer acceleration
  if (keys == prevkeys) {
    if (hold < (40+3)) hold++;
    diff = (hold > 3) ? hold - 3 : 0; 
  } else {
    hold = 0;
    diff = 1;
  }

  // Change State if keys are pressed or have changed
  if (keys || (keys != prevkeys)) {
    GUI_PID_GetState(&state);
    if (keys & BUTTON_LEFT) {
      state.x -= diff;
      if (state.x < 0) state.x = 0;
    }
    if (keys & BUTTON_RIGHT) {
      state.x += diff;
      max = LCD_GetXSize() - 1;
      if (state.x > max) state.x = max; 
    }
    if (keys & BUTTON_UP) {
      state.y -= diff;
      if (state.y < 0) state.y = 0;
    }
    if (keys & BUTTON_DOWN) {
      state.y += diff;
      max = LCD_GetYSize() - 1;
      if (state.y > max) state.y = max; 
    } 
    state.Pressed = (keys & BUTTON_CENTER) ? 1 : 0;
    GUI_PID_StoreState(&state);
    prevkeys = keys;
  }
}

/*************************** End of file ****************************/
