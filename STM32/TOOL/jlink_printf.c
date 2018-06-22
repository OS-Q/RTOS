/*
 * 通过JLink重定向printf到PC
 * 
 * JLink配置
 * Trace页
 * 1、Enable打勾
 * 2、Core Clock 72.000000 MHZ
 * 3、ITM Stimulus Ports -> Enable: 0x00000001, Privilege: 0x00000000
 * 其他默认即可
 */

#pragma import(__use_no_semihosting_swi)
#include <stdio.h>
#include "stm32f10x_it.h"

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

volatile int32_t ITM_RxBuffer;

struct __FILE { int handle; /* Add whatever you need here */ };
    FILE __stdout;
    FILE __stdin;
    
int fputc(int ch, FILE *f) 
{
    if (DEMCR & TRCENA) 
    {
        while (ITM_Port32(0) == 0);
        ITM_Port8(0) = ch;
    }
    return(ch);
}


int fgetc(FILE *f)
{
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
}

int ferror(FILE *f)
{
    /* Your implementation of ferror */
    return EOF;
}

void _ttywrch(int c)
{
		FILE *f = 0;
    fputc(c, f);
}

int __backspace()
{
    return 0;
}
void _sys_exit(int return_code)
{
label:
    goto label;  /* endless loop */
}
