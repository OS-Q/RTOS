
#include "cmsis_os.h"                               // CMSIS RTOS header file
#include "GUI.h"

/*----------------------------------------------------------------------------
 *      GUIThread: Thread to run emwin Demos or Templates
 *---------------------------------------------------------------------------*/
 
void GUIThread (void const *argument);              // thread function
osThreadId tid_GUIThread;                           // thread id
osThreadDef (GUIThread, osPriorityIdle, 1, 1024);   // thread object

int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if(!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {

  while (1) {
    MainTask();  
    osThreadYield();
  }
}

