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
File        : GUI_X_RTE.c
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "RTE_Components.h"
#include "cmsis_os.h"
#include "GUI.h"

/*********************************************************************
*
*       Global data
*/
volatile GUI_TIMER_TIME OS_TimeMS;


/*********************************************************************
*
*       GUI Tick (executed each ms)
*/

static void GUI_Tick (void const *argument) {
  OS_TimeMS++;
}

/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime()
*                 GUI_X_Delay(int)

  Some timing dependent routines require a GetTime
  and delay function. Default time unit (tick), normally is
  1 ms.
*/

GUI_TIMER_TIME GUI_X_GetTime(void) { 
  return OS_TimeMS; 
}

void GUI_X_Delay(int ms) {
  osDelay(ms);
}

/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/

osTimerDef(GUI_Timer, GUI_Tick);
osTimerId  GUI_TimerId;

#ifdef RTE_Graphics_Touchscreen
extern void GUI_TOUCH_Initialize(void);
extern void GUI_TOUCH_Exec(void);
static void GUI_TOUCH_Tick(void const *argument) { GUI_TOUCH_Exec(); };
osTimerDef (GUI_TOUCH_Timer, GUI_TOUCH_Tick);
osTimerId   GUI_TOUCH_TimerId;
#endif

#ifdef RTE_Graphics_Joystick
extern void GUI_JOYSTICK_Initialize(void);
extern void GUI_JOYSTICK_Exec(void);
static void GUI_JOYSTICK_Tick(void const *argument) { GUI_JOYSTICK_Exec(); };
osTimerDef (GUI_JOYSTICK_Timer, GUI_JOYSTICK_Tick);
osTimerId   GUI_JOYSTICK_TimerId;
#endif

void GUI_X_Init(void) {
  GUI_TimerId = osTimerCreate(osTimer(GUI_Timer), osTimerPeriodic, NULL);
  osTimerStart(GUI_TimerId, 1);
#ifdef RTE_Graphics_Touchscreen
  GUI_TOUCH_Initialize();
  GUI_TOUCH_TimerId = osTimerCreate(osTimer(GUI_TOUCH_Timer), osTimerPeriodic, NULL);
  osTimerStart(GUI_TOUCH_TimerId, 20);
#endif
#ifdef RTE_Graphics_Joystick
  GUI_JOYSTICK_Initialize();
  GUI_JOYSTICK_TimerId = osTimerCreate(osTimer(GUI_JOYSTICK_Timer), osTimerPeriodic, NULL);
  osTimerStart(GUI_JOYSTICK_TimerId, 50);
#endif
}

/*********************************************************************
*
*       GUI_X_ExecIdle
*
* Note:
*  Called if WM is in idle state
*/

void GUI_X_ExecIdle(void) {
  osDelay(1);
}

/*********************************************************************
*
*      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/

void GUI_X_Log     (const char *s) { GUI_USE_PARA(s); }
void GUI_X_Warn    (const char *s) { GUI_USE_PARA(s); }
void GUI_X_ErrorOut(const char *s) { GUI_USE_PARA(s); }

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()
*
* Note:
*   The following routines are required only if emWin is used in a
*   true multi task environment, which means you have more than one
*   thread using the emWin API.
*   In this case the
*                       #define GUI_OS 1
*  needs to be in GUIConf.h
*/

osMutexDef(GUI_Mutex);
osMutexId  GUI_MutexId;

void GUI_X_InitOS(void)    { GUI_MutexId = osMutexCreate(osMutex(GUI_Mutex)); }
void GUI_X_Unlock(void)    { osMutexRelease(GUI_MutexId); }
void GUI_X_Lock(void)      { osMutexWait   (GUI_MutexId, osWaitForever); }
U32  GUI_X_GetTaskId(void) { return ((U32)osThreadGetId()); }

/*********************************************************************
*
*      Event driving (optional with multitasking)
*
*                 GUI_X_WaitEvent()
*                 GUI_X_WaitEventTimed()
*                 GUI_X_SignalEvent()
*/

osThreadId GUI_ThreadId;

void GUI_X_WaitEvent(void) {
  GUI_ThreadId = osThreadGetId();
  osSignalWait(0x00000001, osWaitForever); 
}

void GUI_X_WaitEventTimed(int Period) {
  GUI_ThreadId = osThreadGetId();
  osSignalWait(0x00000001, Period); 
}

void GUI_X_SignalEvent(void) {
  if (GUI_ThreadId) {
    osSignalSet(GUI_ThreadId, 0x00000001);
  }
}

/*************************** End of file ****************************/
