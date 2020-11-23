#ifndef _SPI_H
#define _SPI_H
#include "stm32f4xx.h"


#define DI  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)

#define DO_H  GPIO_SetBits(GPIOC,GPIO_Pin_10)
#define DO_L  GPIO_ResetBits(GPIOC,GPIO_Pin_10)


#define CLK_H  GPIO_SetBits(GPIOC,GPIO_Pin_15)
#define CLK_L  GPIO_ResetBits(GPIOC,GPIO_Pin_15)

#define CS_H  GPIO_SetBits(GPIOC,GPIO_Pin_12)
#define CS_L  GPIO_ResetBits(GPIOC,GPIO_Pin_12)

void PS_Remoter_GPIO_Init(void);

#endif
