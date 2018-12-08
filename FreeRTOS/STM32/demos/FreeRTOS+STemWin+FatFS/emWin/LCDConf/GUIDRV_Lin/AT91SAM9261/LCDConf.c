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
Purpose     : Display driver configuration, initialization and touch support
----------------------------------------------------------------------
*/

#include "RTOS.h"
#include "GUI.h"
#include "TaskPrio.h"
#include "GUIDRV_Lin.h"

/*********************************************************************
*
*       Configuratoin defines
*
**********************************************************************
*/
#define LCD_BITSPERPIXEL  24 /* The values 8, 16 and 24 are supported by this configuration file */

#define LCD_MIRROR_X       0
#define LCD_MIRROR_Y       1
#define LCD_SWAP_XY        1

#define _XSIZE           240
#define _YSIZE           320

#define NUM_BUFFERS        3
#define NUM_VSCREENS       1

/*********************************************************************
*
*       Common defines and checks
*
**********************************************************************
*/
#define _VYSIZE             (_YSIZE * NUM_VSCREENS)

#ifndef   NUM_BUFFERS
  #define NUM_BUFFERS 1
#else
  #if (NUM_BUFFERS < 1)
    #error NUM_BUFFERS has to be at least 1!
  #elif (NUM_BUFFERS > 1)
    #if (_YSIZE < _VYSIZE)
      #error Multiple buffers AND virtual screens are not allowed!
    #endif
  #endif
#endif

//
// Color conversion
//
#if   (LCD_BITSPERPIXEL == 8)
  #define COLOR_CONVERSION GUICC_8666
#elif (LCD_BITSPERPIXEL == 16)
  #define COLOR_CONVERSION GUICC_555
#elif (LCD_BITSPERPIXEL == 24)
  #define COLOR_CONVERSION GUICC_888
#endif

//
// Virtual screen size
//
#ifndef _VXSIZE
  #define _VXSIZE _XSIZE
#endif
#ifndef _VYSIZE
  #define _VYSIZE _YSIZE
#endif

//
// Display driver
//
#if (LCD_SWAP_XY)
  #define LCD_XSIZE _YSIZE
  #define LCD_YSIZE _XSIZE
  #define LCD_VXSIZE _VYSIZE
  #define LCD_VYSIZE _VXSIZE
  #if   (LCD_BITSPERPIXEL == 8)
    #error Display rotation via driver currently not supported in 8bpp!
  #elif (LCD_BITSPERPIXEL == 16)
    #if   (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_OS_16
    #elif (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 1)
      #define DISPLAY_DRIVER GUIDRV_LIN_OSY_16
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_OSX_16
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 1)
      #error Currently not supported
    #endif
  #elif (LCD_BITSPERPIXEL == 24)
    #if   (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_OS_24
    #elif (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 1)
      #define DISPLAY_DRIVER GUIDRV_LIN_OSY_24
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_OSX_24
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 1)
      #error Currently not supported
    #endif
  #endif
#else
  #define LCD_XSIZE _XSIZE
  #define LCD_YSIZE _YSIZE
  #define LCD_VXSIZE _VXSIZE
  #define LCD_VYSIZE _VYSIZE
  #if   (LCD_BITSPERPIXEL == 8)
    #if (LCD_MIRROR_X || LCD_MIRROR_Y)
      #error Mirroring via driver currently not supported in 8bpp!
    #else
      #define DISPLAY_DRIVER GUIDRV_LIN_8
    #endif
  #elif (LCD_BITSPERPIXEL == 16)
    #if   (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_16
    #elif (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 1)
      #define DISPLAY_DRIVER GUIDRV_LIN_OY_16
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_OX_16
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 1)
      #define DISPLAY_DRIVER GUIDRV_LIN_OXY_16
    #endif
  #elif (LCD_BITSPERPIXEL == 24)
    #if   (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_24
    #elif (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 1)
      #define DISPLAY_DRIVER GUIDRV_LIN_OY_24
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 0)
      #define DISPLAY_DRIVER GUIDRV_LIN_OX_24
    #elif (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 1)
      #error Currently not supported
    #endif
  #endif
#endif

/****** Define Video RAM location ***********************************/
#if (LCD_BITSPERPIXEL > 16)
  #define USE_INTERNAL_RAM (0) // Set to 1 for internal RAM
#else
  #define USE_INTERNAL_RAM (1) // Set to 0 for external SDRAM
#endif

#define OFFSET_VRAM 0x04000000UL
#define VL_BRSTLN   2

/****** Video RAM ***************************************************/
#define NUM_DWORDS ((NUM_BUFFERS * _VXSIZE * _VYSIZE * (LCD_BITSPERPIXEL / 8)) / 4)

/****** Addresses of sfrs and peripheral identifiers ****************/
#define _PIOA_BASE_ADDR  (0xFFFFF400)
#define _PIOB_BASE_ADDR  (0xFFFFF600)
#define _PIOC_BASE_ADDR  (0xFFFFF800)

#define _PMC_BASE_ADDR   (0xFFFFFC00)

#define _PIOA_ID          (2)
#define _PIOB_ID          (3)
#define _PIOC_ID          (4)
#define _PIO_PER_OFFS     (0x00)
#define _PIO_ODR_OFFS     (0x14)
#define _PIO_OER_OFFS     (0x10)
#define _PIO_SODR_OFFS    (0x30)    /* Set output data              */
#define _PIO_CODR_OFFS    (0x34)    /* Clear output data register   */
#define _PIO_ODSR_OFFS    (0x38)    /* output data status register  */
#define _PIO_PDSR_OFFS    (0x3c)    /* pin data status register     */

/****** SFRs and identifiers used for LED demo **********************/

#define _PMC_PCER (*(volatile unsigned int*) (_PMC_BASE_ADDR + 0x10))

/****** Assign LEDs to Ports ****************************************/

/****** Target specific configuration *******************************/

#define _LEDPORT_BASE_ADDR  (_PIOA_BASE_ADDR)
#define _LEDPORT_ID    (_PIOA_ID)
#define _LED0_BIT      (13) /* LED1 on AT91SAM9261-EK board, active low */
#define _LED1_BIT      (14) /* LED2 on AT91SAM9261-EK board, active low */

#define _LEDPORT_PER   (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_PER_OFFS))
#define _LEDPORT_OER   (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_OER_OFFS))
#define _LEDPORT_SODR  (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_SODR_OFFS))
#define _LEDPORT_CODR  (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_CODR_OFFS))
#define _LEDPORT_STATE (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_ODSR_OFFS))

#define _LEDMASK_ALL   ((1 << _LED0_BIT) | (1 << _LED1_BIT))

//
// Ports for external Ethernet controller
#define _PIOC_PER   (*(volatile unsigned int*) (_PIOC_BASE_ADDR + _PIO_PER_OFFS))
#define _PIOC_ODR   (*(volatile unsigned int*) (_PIOC_BASE_ADDR + _PIO_ODR_OFFS))
#define _PIOC_OER   (*(volatile unsigned int*) (_PIOC_BASE_ADDR + _PIO_OER_OFFS))
#define _PIOC_SODR  (*(volatile unsigned int*) (_PIOC_BASE_ADDR + _PIO_SODR_OFFS))
#define _PIOC_CODR  (*(volatile unsigned int*) (_PIOC_BASE_ADDR + _PIO_CODR_OFFS))

#define AT91C_TOUCHSCREEN_SPI_CLK        920000
#define AT91C_SPI_PCS2_DATAFLASH            0xB

#define AT91C_TOUCHSCREEN_TIMEOUT        5000000

/* SFR definitions */
#define AT91C_LCDC_BA1      (*(volatile U32 *)0x00600000) // (LCDC) DMA Base Address Register 1
#define AT91C_LCDC_FRMCFG   (*(volatile U32 *)0x00600018) // (LCDC) DMA Frame Configuration Register
#define AT91C_LCDC_DMACON   (*(volatile U32 *)0x0060001C) // (LCDC) DMA Control Register

#define AT91C_LCDC_LCDCON1  (*(volatile U32 *)0x00600800) // (LCDC) LCD Control 1 Register
#define AT91C_LCDC_LCDCON2  (*(volatile U32 *)0x00600804) // (LCDC) LCD Control 2 Register
#define AT91C_LCDC_TIM2     (*(volatile U32 *)0x0060080C) // (LCDC) LCD Timing Config 2 Register
#define AT91C_LCDC_LCDFRCFG (*(volatile U32 *)0x00600810) // (LCDC) LCD Frame Config Register
#define AT91C_LCDC_FIFO     (*(volatile U32 *)0x00600814) // (LCDC) LCD FIFO Register
#define AT91C_LCDC_MVAL     (*(volatile U32 *)0x00600818) // (LCDC) LCD Mode Toggle Rate Value Register
#define AT91C_LCDC_PWRCON   (*(volatile U32 *)0x0060083C) // (LCDC) Power Control Register
#define AT91C_LCDC_TIM1     (*(volatile U32 *)0x00600808) // (LCDC) LCD Timing Config 1 Register
#define AT91C_LCDC_CTRSTCON (*(volatile U32 *)0x00600840) // (LCDC) Contrast Control Register
#define AT91C_LCDC_CTRSTVAL (*(volatile U32 *)0x00600844) // (LCDC) Contrast Value Register
#define AT91C_LCDC_IER      (*(volatile U32 *)0x00600848) // (LCDC) LCD Interrupt Enable Register
#define AT91C_LCDC_IDR      (*(volatile U32 *)0x0060084C) // (LCDC) LCD Interrupt Disable Register
#define AT91C_LCDC_IMR      (*(volatile U32 *)0x00600850) // (LCDC) LCD Interrupt Mask Register
#define AT91C_LCDC_ISR      (*(volatile U32 *)0x00600854) // (LCDC) LCD Interrupt Status Register
#define AT91C_LCDC_ICR      (*(volatile U32 *)0x00600858) // (LCDC) LCD Interrupt Clear Register

#define AT91C_PIOA_PER      (*(volatile U32 *)0xFFFFF400) // (PIOA) PIO Enable Register
#define AT91C_PIOA_OER      (*(volatile U32 *)0xFFFFF410) // (PIOA) Output Enable Register
#define AT91C_PIOA_ODR      (*(volatile U32 *)0xFFFFF414) // (PIOA) Output Disable Registerr
#define AT91C_PIOA_CODR     (*(volatile U32 *)0xFFFFF434) // (PIOA) Clear Output Data Register
#define AT91C_PIOA_ASR      (*(volatile U32 *)0xFFFFF470) // (PIOA) Select A Register
#define AT91C_PIOA_BSR      (*(volatile U32 *)0xFFFFF474) // (PIOA) Select B Register
#define AT91C_PIOA_PDR      (*(volatile U32 *)0xFFFFF404) // (PIOA) PIO Disable Register

#define AT91C_PIOB_BSR      (*(volatile U32 *)0xFFFFF674) // (PIOB) Select B Register
#define AT91C_PIOB_PDR      (*(volatile U32 *)0xFFFFF604) // (PIOB) PIO Disable Register

#define AT91C_SPI0_PTCR     (*(volatile U32 *)0xFFFC8120) // (PDC_SPI0) PDC Transfer Control Register
#define AT91C_SPI0_TCR      (*(volatile U32 *)0xFFFC810C) // (PDC_SPI0) Transmit Counter Register
#define AT91C_SPI0_RPR      (*(volatile U32 *)0xFFFC8100) // (PDC_SPI0) Receive Pointer Register
#define AT91C_SPI0_TPR      (*(volatile U32 *)0xFFFC8108) // (PDC_SPI0) Transmit Pointer Register
#define AT91C_SPI0_RCR      (*(volatile U32 *)0xFFFC8104) // (PDC_SPI0) Receive Counter Register

#define AT91C_PMC_SCER      (*(volatile U32 *)0xFFFFFC00) // (PMC) System Clock Enable Register
#define AT91C_PMC_PCER      (*(volatile U32 *)0xFFFFFC10) // (PMC) Peripheral Clock Enable Register

#define AT91C_SPI0_SR       (*(volatile U32 *)0xFFFC8010) // (SPI0) Status Register
#define AT91C_SPI0_CR       (*(volatile U32 *)0xFFFC8000) // (SPI0) Control Register
#define AT91C_SPI0_CSR2     (*(volatile U32 *)0xFFFC8038) // (SPI0) Chip Select Register
#define AT91C_SPI0_RDR      (*(volatile U32 *)0xFFFC8008) // (SPI0) Receive Data Register
#define AT91C_SPI0_MR       (*(volatile U32 *)0xFFFC8004) // (SPI0) Mode Register

#define AT91C_PIOC_PPUDR    (*(volatile U32 *)0xFFFFF860) // (PIOC) Pull-up Disable Register
#define AT91C_PIOC_PPUER    (*(volatile U32 *)0xFFFFF864) // (PIOC) Pull-up Enable Register

#define AT91C_PDC_RXTEN     ((unsigned int) 0x1 <<  0)    // (PDC) Receiver Transfer Enable
#define AT91C_PDC_RXTDIS    ((unsigned int) 0x1 <<  1)    // (PDC) Receiver Transfer Disable
#define AT91C_PDC_TXTEN     ((unsigned int) 0x1 <<  8)    // (PDC) Transmitter Transfer Enable
#define AT91C_PDC_TXTDIS    ((unsigned int) 0x1 <<  9)    // (PDC) Transmitter Transfer Disable

#define AT91C_ID_SPI0       ((unsigned int) 12)           // Serial Peripheral Interface 0

#define AT91C_SPI_RXBUFF    ((unsigned int) 0x1 <<  6)    // (SPI) RXBUFF Interrupt

#define TS_DLYBS                 (10UL << 16)
#define TS_DLYBCT                (40UL << 24)

#define ADS_CTRL_SWITCH_SHIFT     4UL                    // Address setting

#define ADS_CTRL_PD0              (1UL << 0)             // PD0
#define ADS_CTRL_PD1              (1UL << 1)             // PD1
#define ADS_CTRL_START            (1UL << 7)             // Start Bit

#define AT91C_PA6_SPI0_NPCS3      (1UL <<  6)            //  SPI0 Peripheral Chip Select 3
#define AT91C_PA3_SPI0_NPCS0      (1UL <<  3)            //  SPI0 Peripheral Chip Select 0
#define AT91C_PA0_SPI0_MISO       (1UL <<  0)            //  SPI0 Master In Slave
#define AT91C_PA2_SPI0_SPCK       (1UL <<  2)            //  SPI0 Serial Clock
#define AT91C_PA1_SPI0_MOSI       (1UL <<  1)            //  SPI0 Master Out Slave
#define AT91C_PA28_SPI0_NPCS2     (1UL << 28)            //  SPI0 Peripheral Chip Select 2

#define AT91C_SPI_MSTR            (0x1UL <<  0)          // (SPI) Master/Slave Mode
#define AT91C_SPI_SPIDIS          (0x1UL <<  1)          // (SPI) SPI Disable
#define AT91C_SPI_MODFDIS         (0x1UL <<  4)          // (SPI) Mode Fault Detection
#define AT91C_SPI_SWRST           (0x1UL <<  7)          // (SPI) SPI Software reset
#define AT91C_SPI_PCS             (0xFUL << 16)          // (SPI) Peripheral Chip Select

#define AT91C_SPI_NCPHA           (0x1UL  <<  1)         // (SPI) Clock Phase
#define AT91C_SPI_DLYBS           (0xFFUL << 16)         // (SPI) Delay Before SPCK
#define AT91C_SPI_DLYBCT          (0xFFUL << 24)         // (SPI) Delay Between Consecutive Transfers

#define AT91C_SPI_SPIEN           (0x1UL <<  0)          // (SPI) SPI Enable

#define AT91C_LCDC_EOFIE          (0x1UL <<  2)           // (LCDC) End of frame interrupt enable
#define AT91C_LCDC_LSTLNIE        (0x1UL <<  1)           // (LCDC) Last line interrupt enable

/* LCD interrupt */
#define LCD_INTMASK               AT91C_LCDC_LSTLNIE

/* PLA frequency */
#define MAIN_OSC_CLK (18432000UL)
#define MUL_PLLA          (109UL)
#define DIV_PLLA           (10UL)
#define PLLA_FREQ    ((MAIN_OSC_CLK * MUL_PLLA) / DIV_PLLA)

/* Further display controller values */
#define VL_PIXCLOCK  4965000
#define VL_HSP       1
#define VL_VSP       2
#define VL_TFT       1
#define VL_HPW       5
#define VL_BLW       1
#define VL_ELW       33
#define VL_VPW       1
#define VL_BFW       1
#define VL_EFW       0
#if   (LCD_BITSPERPIXEL == 8)
  #define VL_BPIX    3
  #define SHIFT      0
#elif (LCD_BITSPERPIXEL == 16)
  #define VL_BPIX    4
  #define SHIFT      0
#elif (LCD_BITSPERPIXEL == 24)
  #define VL_BPIX    5
  #define SHIFT      1
#else
#error LCD_BITSPERPIXEL value is either unknown or not allowed
#endif

#define AT91C_MASTER_CLOCK_FOR_I2S (PLLA_FREQ / 2)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#ifdef __ICCARM__
  static U32 _aVRAM[NUM_DWORDS];
#endif
#ifdef __CC_ARM
  static U32 _aVRAM[NUM_DWORDS];
#endif

static OS_STACKPTR int _Stack[256];         // Task stacks
static OS_TASK         _TCB;                // Task-control-blocks

static int             _PendingBuffer;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       AT91F_PIOC_CfgPullup
*/
static void AT91F_PIOC_CfgPullup(unsigned int pullupEnable) {
  AT91C_PIOC_PPUDR = ~pullupEnable;
  AT91C_PIOC_PPUER = pullupEnable;
}

/*********************************************************************
*
*       AT91F_PDC_DisableTx
*/
static void AT91F_PDC_DisableTx(void) {
  AT91C_SPI0_PTCR = AT91C_PDC_TXTDIS;
}

/*********************************************************************
*
*       AT91F_PDC_DisableRx
*/
static void AT91F_PDC_DisableRx(void) {
  AT91C_SPI0_PTCR = AT91C_PDC_RXTDIS;
}

/*********************************************************************
*
*       AT91F_PDC_SetTx
*/
static void AT91F_PDC_SetTx(volatile unsigned char * address, unsigned int bytes) {
  AT91C_SPI0_TPR = (unsigned int) address;
  AT91C_SPI0_TCR = bytes;
}

/*********************************************************************
*
*       AT91F_PDC_SetRx
*/
static void AT91F_PDC_SetRx(volatile unsigned char * address, unsigned int bytes) {
  AT91C_SPI0_RPR = (unsigned int) address;
  AT91C_SPI0_RCR = bytes;
}

/*********************************************************************
*
*       AT91F_PDC_EnableTx
*/
static void AT91F_PDC_EnableTx(void) {
  AT91C_SPI0_PTCR = AT91C_PDC_TXTEN;
}

/*********************************************************************
*
*       AT91F_PDC_EnableRx
*/
static void AT91F_PDC_EnableRx(void) {
  AT91C_SPI0_PTCR = AT91C_PDC_RXTEN;
}

/*********************************************************************
*
*       AT91F_PIO_CfgInput
*/
static void AT91F_PIO_CfgInput(unsigned int inputEnable) {
  AT91C_PIOA_ODR = inputEnable;
  AT91C_PIOA_PER = inputEnable;
}

/*********************************************************************
*
*       AT91F_PIOA_CfgPeriph
*/
static void AT91F_PIOA_CfgPeriph(unsigned int periphAEnable, unsigned int periphBEnable) {
  AT91C_PIOA_ASR = periphAEnable;
  AT91C_PIOA_BSR = periphBEnable;
  AT91C_PIOA_PDR = (periphAEnable | periphBEnable);
}

/*********************************************************************
*
*       AT91F_PMC_EnablePeriphClock
*/
static void AT91F_PMC_EnablePeriphClock(unsigned int periphIds) {
  AT91C_PMC_PCER = periphIds;
}

/*********************************************************************
*
*       AT91F_SPI0_CfgPMC
*/
static void AT91F_SPI0_CfgPMC(void) {
  AT91F_PMC_EnablePeriphClock(((unsigned int) 1 << AT91C_ID_SPI0));
}

/*********************************************************************
*
*       AT91F_TSC_SPI_SendRecv
*/
static unsigned int AT91F_TSC_SPI_SendRecv (unsigned char bCmd) {
  volatile unsigned int dStatus;
  volatile unsigned int uResult;
  volatile unsigned int uTimeout;
  volatile unsigned char bResult[64];
  int Loop;
  U8 * pResult;

  pResult = (U8 *)((U32)(&bResult[0] + 31) & ~0x1fUL);
  pResult[0] = bCmd;
  pResult[1] = 0;
  pResult[2] = 0;
  uResult = uTimeout = 0;
  OS_ARM_DCACHE_CleanRange(pResult, 0x20);
  //
  // Send Command and data through the SPI
  //
  AT91F_PDC_DisableRx();
  AT91F_PDC_SetRx(pResult, 3);
  AT91F_PDC_DisableTx();
  AT91F_PDC_SetTx(pResult, 3);
  AT91F_PDC_EnableRx();
  AT91F_PDC_EnableTx();
  do {
    dStatus = AT91C_SPI0_SR;
    uTimeout++;
    Loop  = ((dStatus & AT91C_SPI_RXBUFF) != AT91C_SPI_RXBUFF) ? 1 : 0;
    Loop &= (uTimeout < AT91C_TOUCHSCREEN_TIMEOUT) ? 1 : 0;
  } while (Loop);
  OS_ARM_DCACHE_InvalidateRange(pResult, 0x20);
  AT91F_PDC_DisableTx();
  AT91F_PDC_DisableRx();	
  uResult = ((unsigned int)pResult[1] << 8) | ((unsigned int)pResult[2]);
  uResult = uResult >> 3;
  return uResult;
}

/*********************************************************************
*
*       _RequestValues
*/
static void _RequestValues(U32 * pxVal, U32 * pyVal) {
  U8 bCmd;

  //
  // power-up
  //
  bCmd = (1 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START | ADS_CTRL_PD0 | ADS_CTRL_PD1 ;
  AT91F_TSC_SPI_SendRecv(bCmd);
  //
  // Get X position
  //
  bCmd = (1 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START | ADS_CTRL_PD0 | ADS_CTRL_PD1 ;
  *pxVal = AT91F_TSC_SPI_SendRecv(bCmd);
  //
  // Get Y position
  //
  bCmd = (5 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START | ADS_CTRL_PD0 | ADS_CTRL_PD1;
  *pyVal = AT91F_TSC_SPI_SendRecv(bCmd);
  //
  // power-down
  //
  bCmd = (1 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START ;
  AT91F_TSC_SPI_SendRecv(bCmd);
  //
  // power-down
  //
  bCmd = (5 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START ;
  AT91F_TSC_SPI_SendRecv(bCmd);
}

/*********************************************************************
*
*       _StoreState
*/
static void _StoreState(int x, int y) {
  int xPos, yPos;
  int MirrorX;
  int MirrorY;
  int SwapXY;
  int xSize, ySize;
  MirrorX = LCD_GetMirrorX();
  MirrorY = LCD_GetMirrorY();
  SwapXY  = LCD_GetSwapXY();
  if (SwapXY) {
    xSize   = LCD_GetYSize();
    ySize   = LCD_GetXSize();
  } else {
    xSize   = LCD_GetXSize();
    ySize   = LCD_GetYSize();
  }
  if        ((MirrorX == 0) && (MirrorY == 0) && (SwapXY == 0)) {
    xPos = x;
    yPos = y;
  } else if ((MirrorX == 0) && (MirrorY == 0) && (SwapXY == 1)) {
    xPos = ySize - 1 - y;
    yPos = xSize - 1 - x;
  } else if ((MirrorX == 0) && (MirrorY == 1) && (SwapXY == 0)) {
    xPos = x;
    yPos = ySize - 1 - y;
  } else if ((MirrorX == 0) && (MirrorY == 1) && (SwapXY == 1)) {
    xPos = ySize - 1 - y;
    yPos = x;
  } else if ((MirrorX == 1) && (MirrorY == 0) && (SwapXY == 0)) {
    xPos = xSize - 1 - x;
    yPos = y;
  } else if ((MirrorX == 1) && (MirrorY == 0) && (SwapXY == 1)) {
    xPos = y;
    yPos = xSize - 1 - x;
  } else if ((MirrorX == 1) && (MirrorY == 1) && (SwapXY == 0)) {
    xPos = xSize - 1 - x;
    yPos = ySize - 1 - y;
  } else if ((MirrorX == 1) && (MirrorY == 1) && (SwapXY == 1)) {
    xPos = y;
    yPos = x;
  }
  GUI_TOUCH_StoreState(xPos, yPos);
}

/*********************************************************************
*
*       _ExecTouch
*/
static void _ExecTouch(void) {
  U32 xVal, yVal;

  while (1) {
    OS_Delay(50);
    _RequestValues(&xVal, &yVal);
    if (xVal > 4000) {
      GUI_TOUCH_StoreState(-1, -1);
    } else {
      int xPos;
      int yPos;
      int SwapXY;

      SwapXY = LCD_GetSwapXY();
      if (SwapXY) {
        xPos = (xVal - 180) * LCD_GetYSize() / (3880 - 180);
        yPos = (yVal - 180) * LCD_GetXSize() / (3880 - 180);
      } else {
        xPos = (xVal - 180) * LCD_GetXSize() / (3880 - 180);
        yPos = (yVal - 180) * LCD_GetYSize() / (3880 - 180);
      }
      _StoreState(xPos, yPos);
    }
  }
}

/*********************************************************************
*
*       _InitTouch
*/
static void _InitTouch(void) {
  AT91F_PIOC_CfgPullup(0xFFFFFFFF);
  AT91F_PIO_CfgInput(AT91C_PA6_SPI0_NPCS3);	// Avoids conflicts between PA3 and PA6 when booting on dataflash card!!!
  AT91F_PIO_CfgInput(AT91C_PA3_SPI0_NPCS0);	// Avoids conflicts between PA3 and PA6 when booting on dataflash card!!!
  //
  // Open PIO for SPI0
  //
  AT91F_PIOA_CfgPeriph(((unsigned int)AT91C_PA0_SPI0_MISO   ) |
                       ((unsigned int)AT91C_PA2_SPI0_SPCK   ) |
                       ((unsigned int)AT91C_PA1_SPI0_MOSI   ),
                       ((unsigned int)AT91C_PA28_SPI0_NPCS2 )); // Peripheral B
  //
  // Enables the SPI0 Clock
  //
  AT91F_SPI0_CfgPMC();
  AT91C_SPI0_CR = AT91C_SPI_SPIDIS;
  //
  // Reset SPI0
  //
  AT91C_SPI0_CR = AT91C_SPI_SWRST;
  //
  // Configure SPI0 in Master Mode with no CS selected
  //
  AT91C_SPI0_MR = AT91C_SPI_MSTR | AT91C_SPI_MODFDIS | AT91C_SPI_PCS;
  //
  // Configure CS2
  //
  AT91C_SPI0_CSR2 =  (AT91C_SPI_NCPHA)              |
                     (AT91C_SPI_DLYBS  & TS_DLYBS)  |
                     (AT91C_SPI_DLYBCT & TS_DLYBCT) |
                    ((AT91C_MASTER_CLOCK_FOR_I2S / AT91C_TOUCHSCREEN_SPI_CLK) << 8);
  //
  // Choose NCS2
  //
  AT91C_SPI0_MR &= 0xFFF0FFFF;
  AT91C_SPI0_MR |= ((AT91C_SPI_PCS2_DATAFLASH << 16) & AT91C_SPI_PCS);
  //
  // SPI_Enable
  //
  AT91C_SPI0_CR = AT91C_SPI_SPIEN;
  //
  // Setup and start software timer for touch execute
  //
  OS_CREATETASK(&_TCB, "TOUCH_Task", _ExecTouch, TASKPRIO_TOUCH, _Stack);
}

/*********************************************************************
*
*       _LCD_On
*/
static void _LCD_On(void) {
  AT91C_LCDC_PWRCON = (1 << 0) | 0x0C;
}

/*********************************************************************
*
*       _LCD_Off
*/
static void _LCD_Off(void) {
  AT91C_LCDC_PWRCON = (0 << 0) | 0x0C;
}

/*********************************************************************
*
*       _LCD_SetLUTEntry
*/
static void _LCD_SetLUTEntry(unsigned Pos, unsigned long Color) {
  U32 r, g, b;
  r = (Color & 0x0000ff) >>  0;
  g = (Color & 0x00ff00) >>  8;
  b = (Color & 0xff0000) >> 16;
  *(((volatile U32 *)0x00600C00) + Pos) = (r >> 3 | ((g & 0xf8) << 2) | ((b & 0xf8) << 7));
}

/*********************************************************************
*
*       _LCD_ISR_EndOfFrame
*/
static void _LCD_ISR_EndOfFrame(void) {
  U32 Addr;

  if (AT91C_LCDC_ISR & LCD_INTMASK) {
    AT91C_LCDC_ICR |= LCD_INTMASK;
  }
  if (_PendingBuffer >= 0) {
    Addr = ((U32)&_aVRAM[0] + _XSIZE * _YSIZE * _PendingBuffer * (LCD_BITSPERPIXEL >> 3));
    AT91C_LCDC_BA1 = Addr;
    GUI_MULTIBUF_Confirm(_PendingBuffer);
    _PendingBuffer = -1;
  }
}

/*********************************************************************
*
*       _LCD_InitController
*
* Function description:
*   Initializes the display controller
*
* Notes       : (1) - LCD frame buffer must be in internal RAM.
*                     since AHB master configuration can not be set.
*                     Eg. When accessing NAND flash, data corruption
*                     will occur.
*
*                     This is not required with a stand alone GUI demo.
*/
static int _LCD_InitController(unsigned LayerIndex) {
  if (LayerIndex == 0) {
    _PendingBuffer = -1;
    //
    // Initialize touch screen
    //
    _InitTouch();
    //
    // Initialize port pins
    //
    AT91C_PIOB_PDR      = 0x1F9F9F9E;
    AT91C_PIOB_BSR      = 0x1F800000;
    AT91C_PIOA_PER      = (1 << 12);
    AT91C_PIOA_OER      = (1 << 12);
    AT91C_PIOA_CODR     = (1 << 12);
    //
    // Enable clock
    //
    AT91C_PMC_SCER      = (1 << 17);
    //
    // Initialize LCDC registers
    //
    AT91C_LCDC_PWRCON   = 0x0C;
    AT91C_LCDC_DMACON   = 0;
    AT91C_LCDC_DMACON   = (1 << 1);
  #if (LCD_BITSPERPIXEL == 24)
    AT91C_LCDC_LCDCON1  = ((((PLLA_FREQ / 2) / VL_PIXCLOCK + 1) / 4) - 1) << 12;;
  #else
    AT91C_LCDC_LCDCON1  = ((((PLLA_FREQ / 2) / VL_PIXCLOCK + 1) / 2) - 1) << 12;
  #endif
    AT91C_LCDC_LCDCON2  = ((U32)(1uL << 31)
                        | 2
                        | (1 << 15))
                        | ((U32)(VL_BPIX << 5));
    AT91C_LCDC_TIM1     = ((U32)((VL_VPW - 1) << 16))
                        | ((U32)(VL_BFW << 8))
                        | ((U32)VL_EFW);
    AT91C_LCDC_TIM2     = ((U32)((VL_ELW - 1) << 21))
                        | ((U32)((VL_HPW - 1) << 8))
                        | ((U32)(VL_BLW - 1));
    AT91C_LCDC_LCDFRCFG = ((U32)((_XSIZE - 1) << 21))
                        | ((U32)(_YSIZE - 1) << (SHIFT));
    AT91C_LCDC_FIFO     = 512 - (2 * VL_BRSTLN + 3);
    AT91C_LCDC_MVAL     = 0;
    AT91C_LCDC_IDR      = ~0UL;
    AT91C_LCDC_CTRSTCON = 3
                        | (1 << 2)
                        | (1 << 3);
    AT91C_LCDC_CTRSTVAL = 0xDA;
    AT91C_LCDC_FRMCFG   = ((U32)((VL_BRSTLN - 1) << 24))
                        | ((U32)_XSIZE * (U32)_YSIZE * (U32)(1 << VL_BPIX)) / 32;
    AT91C_LCDC_DMACON   = (1 << 0);
    //
    // Install interrupt handler for EndOfFrame-interrupt
    //
    OS_ARM_EnableISR(21);
    OS_ARM_InstallISRHandler(21, _LCD_ISR_EndOfFrame);
    AT91C_LCDC_IER = LCD_INTMASK;
  }
  return 0;
}

/*********************************************************************
*
*       _LCD_CopyBuffer
*/
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) {
  U32 BufferSize, BaseAddr, AddrSrc, AddrDst;

  GUI_USE_PARA(LayerIndex);
  BufferSize = ((_XSIZE * _YSIZE * LCD_BITSPERPIXEL) >> 3);
  BaseAddr   = ((U32)&_aVRAM[0] + OFFSET_VRAM);
  AddrSrc    = BaseAddr + BufferSize * IndexSrc;
  AddrDst    = BaseAddr + BufferSize * IndexDst;
  GUI_MEMCPY((void *)AddrDst, (void *)AddrSrc, BufferSize);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_DisplayDriver
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * p) {
  GUI_USE_PARA(LayerIndex);
  switch (Cmd) {
  case LCD_X_SETVRAMADDR: {
      LCD_X_SETVRAMADDR_INFO * pData;
      pData = (LCD_X_SETVRAMADDR_INFO *)p;
      AT91C_LCDC_BA1 = (U32)pData->pVRAM;
    }
    break;
  case LCD_X_SETORG: {
      LCD_X_SETORG_INFO * pData;
      LCD_X_SETVRAMADDR_INFO Info;

      pData = (LCD_X_SETORG_INFO *)p;
      Info.pVRAM = (void *)((U32)&_aVRAM[0] + pData->yPos * _XSIZE * (LCD_BITSPERPIXEL >> 3));
      LCD_X_DisplayDriver(LayerIndex, LCD_X_SETVRAMADDR, &Info);
    }
    break;
  case LCD_X_SHOWBUFFER: {
      LCD_X_SHOWBUFFER_INFO * pData;

      pData = (LCD_X_SHOWBUFFER_INFO *)p;
      //_aPendingBuffer[LayerIndex] = pData->Index;
      _PendingBuffer = pData->Index;
    }
    break;
  case LCD_X_INITCONTROLLER:
    return _LCD_InitController(LayerIndex);
  case LCD_X_ON:
    _LCD_On();
    break;
  case LCD_X_OFF:
    _LCD_Off();
    break;
  case LCD_X_SETLUTENTRY: {
      LCD_X_SETLUTENTRY_INFO * pData;
      pData = (LCD_X_SETLUTENTRY_INFO *)p;
      _LCD_SetLUTEntry(pData->Pos, pData->Color);
    }
    break;
  }
  return 0;
}

/*********************************************************************
*
*       LCD_X_Config
*/
void LCD_X_Config(void) {
  //
  // Initialize multibuffering
  //
  GUI_MULTIBUF_Config(NUM_BUFFERS);
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Custom defined copy function
  //
  LCD_SetDevFunc(0, LCD_DEVFUNC_COPYBUFFER, (void (*)())_LCD_CopyBuffer);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx    (0, LCD_XSIZE, LCD_YSIZE);
  LCD_SetVSizeEx   (0, LCD_VXSIZE, LCD_VYSIZE);
  LCD_SetVRAMAddrEx(0, (void *)((U32)&_aVRAM[0] + OFFSET_VRAM)); // Note (1)
}

/*************************** End of file ****************************/
