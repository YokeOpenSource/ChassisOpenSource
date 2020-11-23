/**
 * @file beep.c
 * @brief beep configuration function
 */

#include "beep.h"

/**
* @brief    Configurate the Beep
*
* @param[in] none
*
* @return    none
*
*/
void Beep_Configuration( void )
{
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );

    gpio.GPIO_Pin = GPIO_Pin_4;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init( GPIOC, &gpio );
}







