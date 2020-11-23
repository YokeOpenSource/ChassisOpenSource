/**
 * @file led.c
 * @brief led configuration function
 * @updata_note
 */

#include "led.h"

/**
* @brief    Configurate the Led
*
* @param[in] none
*
* @return    none
*
*/
void LED_Configuration( void )
{
    GPIO_InitTypeDef gpio;
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );

    gpio.GPIO_Pin = GPIO_Pin_2 ;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init( GPIOD, &gpio );

    LED_GREEN_OFF;
}
