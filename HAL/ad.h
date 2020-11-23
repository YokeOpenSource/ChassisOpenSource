/**
 * @file ad.h
 * @brief  ad head file
 * @updata_note
 */

#ifndef __AD_H__
#define __AD_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"


// ADC DR Register
#define ADC_DR_ADDR    ((u32)ADC1+0x4c)

// ADC DMA
#define ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define ADC_DMA_CHANNEL  DMA_Channel_0
#define ADC_DMA_STREAM   DMA2_Stream0

extern __IO short ADC_ConvertedValue[5];

void AD_Configuration( void );


#ifdef __cplusplus
}
#endif

#endif
