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
File        : LCD_X_I2C.c
Purpose     : Port routines, I2C-bus protocol by bit banging
              - Clock stretching supported
              - Multiple masters supported
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "LCD_X_I2C.h"

/*********************************************************************
*
*       Defines: Low level access, needs to be modified
*
**********************************************************************
*/
#include "AT91SAM7X512.h"

#define DATA_LINE      (1 << 21)            /* PA21, J8-15 */

#define CONTROL_LINES ((1 << 20) | /* SCL,     PA20, J8-14 */  \
                       (1 << 22))  /* RST,     PA22, J8-16 */

#define I2C_CLR_SDA()     PIOA_CODR = DATA_LINE
#define I2C_SET_SDA()     PIOA_SODR = DATA_LINE
#define I2C_CLR_SCL()     PIOA_CODR = (1 << 20)
#define I2C_SET_SCL()     PIOA_SODR = (1 << 20)
#define I2C_CLR_RESET()   PIOA_CODR = (1 << 22)
#define I2C_SET_RESET()   PIOA_SODR = (1 << 22)
#define I2C_READ_SDA()   (PIOA_PDSR & DATA_LINE)
#define I2C_READ_SCL()   (PIOA_PDSR & (1 << 20))
#define I2C_SDA_DIR_IN()  PIOA_ODR  = DATA_LINE; \
                          PIOA_OWDR = DATA_LINE
#define I2C_SDA_DIR_OUT() PIOA_OER  = DATA_LINE; \
                          PIOA_OWER = DATA_LINE
#define I2C_SCL_DIR_IN()  PIOA_ODR  = (1 << 20); \
                          PIOA_OWDR = (1 << 20)
#define I2C_SCL_DIR_OUT() PIOA_OER  = (1 << 20); \
                          PIOA_OWER = (1 << 20)

#define NOP() asm("NOP");asm("NOP");asm("NOP");

/*********************************************************************
*
*       Defines: High level access, normally no modification required
*
**********************************************************************
*/
#define CLEAR_SDA()       I2C_SDA_DIR_OUT(); \
                          I2C_CLR_SDA();
#define CLEAR_SCL()       I2C_SCL_DIR_OUT(); \
                          I2C_CLR_SCL();


#define I2C_SLAVE_WRITE ((_SlaveAddr << 1) + 0)
#define I2C_SLAVE_READ  ((_SlaveAddr << 1) + 1)

#define MODE_R 1
#define MODE_W 2

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static unsigned char _SlaveAddr;
static unsigned char _AccessMode;

static int _Started;

/*********************************************************************
*
*       _StartCond
*/
static int _StartCond(void) {
  int r, Cnt;

  r = 0;
  //
  // If already started, do a restart cond
  //
  if (_Started) {
    //
    // Set SDA to 1
    //
    I2C_SDA_DIR_IN();
    NOP();
    I2C_SCL_DIR_IN();
    for (Cnt = 1000; Cnt && (I2C_READ_SCL() == 0); Cnt--);
    //
    // Repeated start setup time, minimum 4.7us
    //
    NOP();
  }
  I2C_SDA_DIR_IN();
  if (I2C_READ_SDA() == 0) {
    r = 1; // Error
  }
  //
  // SCL is high, set SDA from 1 to 0.
  //
  CLEAR_SDA();
  NOP();
  CLEAR_SCL();
  _Started = 1;
  return r;
}

/*********************************************************************
*
*       _StopCond
*/
static int _StopCond(void){
  int r, Cnt;

  //
  // Set SDA to 0
  //
  CLEAR_SDA()
  NOP();
  //
  // Clock stretching
  //
  I2C_SCL_DIR_IN();
  for (Cnt = 1000; Cnt && (I2C_READ_SCL() == 0); Cnt--);
  r = Cnt ? 0 : 1;
  //
  // Stop bit setup time, minimum 4us
  //
  NOP();
  //
  // SCL is high, set SDA from 0 to 1
  //
  I2C_SDA_DIR_IN();
  if (I2C_READ_SDA() == 0) {
    r = 1; // Error
  }
  NOP();
  _Started = 0;
  return r;
}

/*********************************************************************
*
*       _WriteBit
*
* Purpose:
*   Write a bit to I2C bus
*/
static int _WriteBit(int Bit) {
  int r, Cnt;

  if (Bit) {
    I2C_SDA_DIR_IN();
  } else {
    CLEAR_SDA()
  }
  NOP();
  I2C_SCL_DIR_IN();
  for (Cnt = 1000; Cnt && (I2C_READ_SCL() == 0); Cnt--);
  r = Cnt ? 0 : 1;
  //
  // SCL is high, now data is valid
  // If SDA is high, check that nobody else is driving SDA
  //
  if (Bit) {
    I2C_SDA_DIR_IN();
    if (I2C_READ_SDA() == 0) {
      r = 1; // Error
    }
  }
  NOP();
  CLEAR_SCL();
  return r;
}

/*********************************************************************
*
*       _ReadBit
*
* Purpose:
*   Read a bit from I2C bus
*/
static int _ReadBit(int * pError) {
  int Bit, Cnt;

  //
  // Let the slave drive data
  //
  I2C_SDA_DIR_IN();
  NOP();
  I2C_SCL_DIR_IN();
  for (Cnt = 1000; Cnt && (I2C_READ_SCL() == 0); Cnt--);
  *pError |= Cnt ? 0 : 1;
  //
  // SCL is high, now data is valid
  //
  I2C_SDA_DIR_IN();
  Bit = I2C_READ_SDA() >> 21;
  NOP();
  CLEAR_SCL();
  return Bit;
}

/*********************************************************************
*
*       _WriteByte
*
* Purpose:
*   Write a byte to I2C bus. Return 0 if ack by the slave.
*/
static int _WriteByte(unsigned char Byte, int Start, int Stop) {
  unsigned Bit;
  int NAck, r;

  r = 0;
  if (Start) {
    r = _StartCond();
  }
  for (Bit = 0; Bit < 8; Bit++) {
    _WriteBit((Byte & 0x80) != 0);
    Byte <<= 1;
  }
  NAck = _ReadBit(&r);
  if (Stop) {
    r |= _StopCond();
  }
  return NAck | r;
}

/*********************************************************************
*
*       _ReadByte
*
* Purpose:
*   Read a byte from I2C bus
*/
static unsigned char _ReadByte(int NAck, int Stop, int * pError) {
  unsigned char Byte;
  unsigned Bit;

  for (Bit = 0, Byte = 0; Bit < 8; Bit++) {
    Byte = (Byte << 1) | _ReadBit(pError);
  }
  _WriteBit(NAck);
  if (Stop) {
    *pError |= _StopCond();
  }
  return Byte;
}

/*********************************************************************
*
*       _StartRead
*/
static int _StartRead(int * pStart) {
  int r;

  r = 0;
  if (_AccessMode != MODE_R) {
    r  = _StartCond();
    r |= _WriteByte(I2C_SLAVE_READ, *pStart, 0);
    _AccessMode = MODE_R;
    *pStart = 0;
  }
  return r;
}

/*********************************************************************
*
*       _StartWrite
*/
static int _StartWrite(int * pStart) {
  int r;

  r = 0;
  if (_AccessMode != MODE_W) {
    r  = _StartCond();
    r |= _WriteByte(I2C_SLAVE_WRITE, *pStart, 0);
    _AccessMode = MODE_W;
    *pStart = 0;
  }
  return r;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_I2C_Read
*
* Purpose:
*   Reads one byte of data.
*
* Parameters:
*   Start: Must me set to 1 if bus communication starts, otherwise 0
*   Stop:  Must me set to 1 if bus communication should end after the read operation, otherwise 0
*/
int LCD_X_I2C_Read(unsigned char * pData, int Start, int Stop) {
  int r;

  r = _StartRead(&Start);
  *pData = _ReadByte(Stop, Stop, &r);
  return r;
}

/*********************************************************************
*
*       LCD_X_I2C_ReadM
*
* Purpose:
*   Reads multiple bytes of data.
*
* Parameters:
*   Start: Must me set to 1 if bus communication starts, otherwise 0
*   Stop:  Must me set to 1 if bus communication should end after the read operation, otherwise 0
*/
int LCD_X_I2C_ReadM(unsigned char * pData, int NumItems, int Start, int Stop) {
  int r;

  r = _StartRead(&Start);
  if (NumItems > 1) {
    while (--NumItems) {
      *pData++ = _ReadByte(0, 0, &r);
    }
  }
  *pData = _ReadByte(Stop, Stop, &r);
  return r;
}

/*********************************************************************
*
*       LCD_X_I2C_Write
*
* Purpose:
*   Writes one byte of data.
*
* Parameters:
*   Start: Must me set to 1 if bus communication starts, otherwise 0
*   Stop:  Must me set to 1 if bus communication should end after the write operation, otherwise 0
*/
int LCD_X_I2C_Write(unsigned char Data, int Start, int Stop) {
  int r;

  r = _StartWrite(&Start);
  if (r == 0) {
    r |= _WriteByte(Data, Start, Stop);
  }
  return r;
}

/*********************************************************************
*
*       LCD_X_I2C_WriteM
*
* Purpose:
*   Writes multiple bytes of data.
*
* Parameters:
*   Start: Must me set to 1 if bus communication starts, otherwise 0
*   Stop:  Must me set to 1 if bus communication should end after the write operation, otherwise 0
*/
int LCD_X_I2C_WriteM(unsigned char * pData, int NumItems, int Start, int Stop) {
  int r;

  r = _StartWrite(&Start);
  if (NumItems > 1) {
    while (--NumItems) {
      r |= _WriteByte(*pData++, Start, 0);
      Start = 0;
    }
  }
  r |= _WriteByte(*pData, Start, Stop);
  return r;
}

/*********************************************************************
*
*       LCD_X_I2C_Init
*
* Purpose:
*   This routine should be called from the application program
*   to set the port pins to their initial values:
*
* Parameters:
*   SlaveAddr: Address used to be able to access the slave
*/
void LCD_X_I2C_Init(unsigned char SlaveAddr) {
  //
  // Set up slave address to be used
  //
  _SlaveAddr = SlaveAddr;
  //
  // Enable clock for PIOA, required for reading
  //
  PMC_PCER  |= (1 << PIOA_ID);            // Peripheral Clock Enable Register
  PIOA_PER   = DATA_LINE | CONTROL_LINES; // PIO Enable Register
  PIOA_ASR   = DATA_LINE;                 // Peripheral A Select Register
  PIOA_OER   = CONTROL_LINES;             // Output Enable Register
  PIOA_OWER  = CONTROL_LINES;             // Output Write Enable Register
  //
  // Reset, active high
  //
  I2C_SET_RESET();
  I2C_SET_SDA();
  I2C_SET_SCL();
  I2C_CLR_RESET();
}

/*************************** End of file ****************************/
