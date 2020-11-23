/**
 * @file usart1.h
 * @brief  usart1 head file
 */

#ifndef __USART1_H__
#define __USART1_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdio.h>


void USART1_Configuration( void );
void USART1_SendChar( unsigned char b );
#define RxData_Buff_USART1_Length 150
#define RxData_USART1_length 99

#ifdef __cplusplus
}
#endif

#endif
