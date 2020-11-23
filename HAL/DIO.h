/**
 * @file DIO.h
 * @brief  DIO head file
 * @updata_note
 */

#ifndef __DIO_H__
#define __DIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#define RELAY_ON      GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define RELAY_OFF     GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define RELAY_TOGGLE  GPIOC->ODR ^= GPIO_Pin_13

void Relay_Configuration(void);	


#ifdef __cplusplus
}
#endif

#endif
