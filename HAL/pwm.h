/**
 * @file pwm.h
 * @brief  pwm header file  
 */
 
#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define MOTOR1_PWM_SWITCH_H    GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define MOTOR1_PWM_SWITCH_L    GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define MOTOR2_PWM_SWITCH_H    GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define MOTOR2_PWM_SWITCH_L    GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define MOTOR3_PWM_SWITCH_H    GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define MOTOR3_PWM_SWITCH_L    GPIO_ResetBits(GPIOC,GPIO_Pin_2)

#define MOTOR4_PWM_SWITCH_H    GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define MOTOR4_PWM_SWITCH_L    GPIO_ResetBits(GPIOA,GPIO_Pin_5)	

void PWM_Configuration(void);
void Motor_PWM_Configuration(void);
#ifdef __cplusplus
}
#endif

#endif

