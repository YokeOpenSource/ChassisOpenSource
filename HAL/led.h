/**
 * @file led.h
 * @brief  led head file
 * @updata_note
 */

#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

/*new board*/
#define LED_GREEN_OFF  GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define LED_GREEN_ON   GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define LED_GREEN_TOGGLE  GPIOD->ODR ^= GPIO_Pin_2


void LED_Configuration( void );

#ifdef __cplusplus
}
#endif

#endif
