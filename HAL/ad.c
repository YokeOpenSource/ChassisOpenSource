/**
 * @file ad.c
 * @brief ad configuration function
 * @updata_note
 */

#include "ad.h"


__IO short ADC_ConvertedValue[5] = {0};

/**
* @brief    Configurate the AD module
*
* @param[in] none
*
* @return    none
*
*/
void AD_Configuration( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
	
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
	
    /*---------------------------------DMA----------------------------------*/
    // ADC1,DMA2，Stream 0，Channel 0
    RCC_AHB1PeriphClockCmd( ADC_DMA_CLK, ENABLE );
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_ADDR;
    DMA_InitStructure.DMA_Memory0BaseAddr = ( u32 )ADC_ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 5;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_Channel = ADC_DMA_CHANNEL;
    DMA_Init( ADC_DMA_STREAM, &DMA_InitStructure );

    DMA_Cmd( ADC_DMA_STREAM, ENABLE );

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;// 独立ADC模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; // 时钟为fpclk x分频
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;// 禁止DMA直接访问模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  // 采样时间间隔
    ADC_CommonInit( &ADC_CommonInitStructure );

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b; // ADC 分辨率
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; // 禁止扫描模式，多通道采集才需要
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // 连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止外部边沿触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 5;   //转换通道
    ADC_Init( ADC1, &ADC_InitStructure );

    // 配置 ADC 通道转换顺序和采样时间周期
    ADC_RegularChannelConfig( ADC1, ADC_Channel_9, 1,
                              ADC_SampleTime_3Cycles );
    ADC_RegularChannelConfig( ADC1, ADC_Channel_15, 2,
                              ADC_SampleTime_3Cycles );
    ADC_RegularChannelConfig( ADC1, ADC_Channel_11, 3,
                              ADC_SampleTime_3Cycles );
    ADC_RegularChannelConfig( ADC1, ADC_Channel_10, 4,
                              ADC_SampleTime_3Cycles );
    ADC_RegularChannelConfig( ADC1, ADC_Channel_4, 5,
                              ADC_SampleTime_3Cycles );							  

    /*--------------------------------------------------------------------------------------*/
    //Enable DMA request after last transfer (Single-ADC mode)
    ADC_DMARequestAfterLastTransferCmd( ADC1, ENABLE );
    //Enable ADC DMA
    ADC_DMACmd( ADC1, ENABLE );
    /*---------------------------------------------------------------------------------------*/

    //Enable ADC
    ADC_Cmd( ADC1, ENABLE );

    //Begin ADC
    ADC_SoftwareStartConv( ADC1 );

}





