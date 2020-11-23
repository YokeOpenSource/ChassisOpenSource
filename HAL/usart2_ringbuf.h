/**
 * @file usart6.h
 * @brief  usart6 head file
 * @updata_note
 */

#ifndef __USART2_RINGBUF_H__
#define __USART2_RINGBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "stdio.h"
#include "stm32f4xx.h"
#include <stdarg.h>
#include <string.h>
#include "ringbuffer.h"
#include <stdbool.h>


void USART2_DMA_Configuration(void);

unsigned int USART2_Send(unsigned char *data, unsigned short len);
unsigned int USART2_Recv(unsigned char *data, unsigned short len);
unsigned int USART2_Read(unsigned char *data, unsigned short len);
unsigned char USART2_At( unsigned short offset);
void USART2_Drop( unsigned short LenToDrop);
unsigned int USART2_GetDataCount( void );
bool USART2_isFull( void );



#ifdef __cplusplus
}
#endif

#endif
