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
File        : GUI_SPY_X_StartServer.c
Purpose     : Start of the emWinSPY server in WIN32 simulation.
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>

#include "IP.h"       // BSD socket interface
#include "RTOS.H"     // embOS header
#include "GUI.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PORT 2468 // emWinSPY-Port

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

//
// embOS Stack area of the server
//
static OS_STACKPTR int _StackSPYServer[1000];

//
// embOS Task-control-block of the server
//
static OS_TASK _SPYServer_TCB;

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _ListenAtTcpAddr
*
* Starts listening at the given TCP port.
*/
static int _ListenAtTcpAddr(unsigned short Port) {
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(Port);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  listen(sock, 1);
  return sock;
}

/*********************************************************************
*
*       _Send / _Recv
*
*  Function description:
*    Functions for sending and receiving data used by emWinSPY
*
*  Return value:
*    In case of success both functions should return the number
*    of successfully transmitted bytes, otherwise -1.
*/
static int _Send(const U8 * buf, int len, void * pConnectInfo) {
  int r;

  r = send((long)pConnectInfo, (const char *)buf, len, 0);
  return r;
}

static int _Recv(U8 * buf, int len, void * pConnectInfo) {
    int r;

  r = recv((long)pConnectInfo, (char *)buf, len, 0);
  return r;
}

/*********************************************************************
*
*       _ServerTask
*
*  Function description:
*    This task  waits for an incoming connection. If a connection
*    has been established it calls GUI_SPY_Process() which should
*    return if the connection is closed.
*/
static void _ServerTask(void) {
  static struct sockaddr_in Addr;
  int s, Sock, AddrLen;

  //
  // Loop until we get a socket into listening state
  //
  do {
    s = _ListenAtTcpAddr(PORT);
    if (s != -1) {
      break;
    }
    OS_Delay(100); // Try again
  } while (1);
  //
  // Loop once per client and create a thread for the actual server
  //
  while (1) {
    //
    // Wait for an incoming connection
    //
    AddrLen = sizeof(Addr);
    if ((Sock = accept(s, (struct sockaddr*)&Addr, &AddrLen)) == SOCKET_ERROR) {
      continue; // Error
    }
    //
    // Execute emWinSPY
    //
    GUI_SPY_Process(_Send, _Recv, (void *)Sock);
    //
    // Close the connection
    //
    closesocket(Sock);
    memset(&Addr, 0, sizeof(struct sockaddr_in));
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_SPY_X_StartServer
*/
int GUI_SPY_X_StartServer(void) {
  //
  // Create task for SPY Server
  //
  OS_CREATETASK(&_SPYServer_TCB, "SPY Server", _ServerTask, 230, _StackSPYServer);
  //
  // O.k., server has been started
  //
  return 0;
}

/*************************** End of file ****************************/
