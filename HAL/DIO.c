/**
 * @file DIO.c
 * @brief DIO configuration function
 */

#include "DIO.h"
/**
* @brief    Configurate the io for relay control
*
* @param[in] none
*
* @return    none
*
*/
void Relay_Configuration(void)
{
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );

    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init( GPIOC, &gpio );
}
