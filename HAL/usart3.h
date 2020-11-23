/**
 * @file usart3.h
 * @brief  usart3 head file     
 * @updata_note 
 */
 
#ifndef __USART3_H__
#define __USART3_H__

#ifdef __cplusplus
extern "C" {
#endif
    
#include "stm32f4xx.h"

void USART3_Configuration(void);
    
extern unsigned char sbus_rx_buffer[25];

#ifdef __cplusplus
}
#endif

#endif
