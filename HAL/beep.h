/**
 * @file beep.h
 * @brief  beep head file
 * @updata_note
 */

#ifndef __BEEP_H__
#define __BEEP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#define Beep_ON  GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define Beep_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define Beep_T  GPIOC->ODR ^= GPIO_Pin_4

	

	

	
void Beep_Configuration( void );


#ifdef __cplusplus
}
#endif

#endif
