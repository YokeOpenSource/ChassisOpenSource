/**
 * @file usart6.h
 * @brief  usart6 head file
 */

#ifndef __USART6_H__
#define __USART6_H__

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

void USART6_Init(void);
void USART6_DMA_Tx_Init(void);
void USART6_DMA_Rx_Init(void);
void USART6_Configuration(void);
void USART6_Tx_IRQ_Callback(void);
void USART6_Rx_IRQ_Callback(void);
void USART6_IRQHandler(void);
void DMA2_Stream1_IRQHandler(void);
void DMA2_Stream6_IRQHandler(void);
unsigned int USART6_Send(unsigned char *data, unsigned short len);
unsigned int USART6_Recv(unsigned char *data, unsigned short len);
unsigned int USART6_Read(unsigned char *data, unsigned short len);
unsigned char USART6_At( unsigned short offset);
void USART6_Drop( unsigned short LenToDrop);
unsigned int USART6_GetDataCount( void );
bool USART6_isFull( void );



#ifdef __cplusplus
}
#endif

#endif
