/**
 * @file usart1.c
 * @brief usart1 configuration function
 * @updata_note
 */

#include "usart1.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;        
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif

/**
* @brief    Configurate the usart1
*
* @param[in] none
*
* @return    none
*
*/
void USART1_Configuration( void )
{
    USART_InitTypeDef usart1;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );

    GPIO_PinAFConfig( GPIOA, GPIO_PinSource9, GPIO_AF_USART1 );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource10, GPIO_AF_USART1 );

    gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOA, &gpio );

    usart1.USART_BaudRate = 115200;
    usart1.USART_WordLength = USART_WordLength_8b;
    usart1.USART_StopBits = USART_StopBits_1;
    usart1.USART_Parity = USART_Parity_No;
    usart1.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init( USART1, &usart1 );

    USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
    USART_Cmd( USART1, ENABLE );

    nvic.NVIC_IRQChannel = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &nvic );


}

void USART1_SendChar( unsigned char b )
{
    while( USART_GetFlagStatus( USART1, USART_FLAG_TC ) == RESET );
    USART_SendData( USART1, b );
}


void USART1_IRQHandler( void )
{

	char get_char;
	
    if( USART_GetITStatus( USART1, USART_IT_RXNE ) != RESET )
    {
        get_char = USART_ReceiveData( USART1 );

        USART_ClearITPendingBit( USART1, USART_IT_RXNE );

    }
}
