/**
 * @file usart3.c
 * @brief usart3 configuration function
 * @updata_note
 * @note for S-BUSÒ£¿ØÆ÷
 */

#include "usart3.h"
#include "module_remoter.h"

unsigned char sbus_rx_buffer[25];

/**
* @brief    Configurate the USART3
*
* @param[in] none
*
* @return    none
*
*/
void USART3_Configuration( void )
{
    USART_InitTypeDef usart3;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );

    GPIO_PinAFConfig( GPIOC, GPIO_PinSource11, GPIO_AF_USART3 );

    gpio.GPIO_Pin = GPIO_Pin_11 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOC, &gpio );

    USART_DeInit( USART3 );
    usart3.USART_BaudRate = 100000;   //SBUS 100K baudrate
    usart3.USART_WordLength = USART_WordLength_8b;
    usart3.USART_StopBits = USART_StopBits_2;
    usart3.USART_Parity = USART_Parity_Even;
    usart3.USART_Mode = USART_Mode_Rx;
    usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init( USART3, &usart3 );

    USART_Cmd( USART3, ENABLE );
	
    USART_DMACmd( USART3, USART_DMAReq_Rx, ENABLE );
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);  // Ê¹ÄÜIDLEÖÐ¶Ï

    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &nvic );

    DMA_DeInit( DMA1_Stream1 );
    dma.DMA_Channel = DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr = ( uint32_t ) & ( USART3->DR );
    dma.DMA_Memory0BaseAddr = ( uint32_t )sbus_rx_buffer;
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_BufferSize = SBUS_Data_length;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_Mode_Normal;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init( DMA1_Stream1, &dma );

    DMA_Cmd( DMA1_Stream1, ENABLE );
}




