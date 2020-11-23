/**
 * @file module_ad.c
 * @brief ad work function
 * @updata_note
 */

#include "module_ad.h"
#include "ad.h"

/**
* @brief    Get the voltage of AD channel 1
*
* @param[in] none
*
* @return    The voltage of AD channel 1
*
*/
double Get_Power_Vol( void )
{

    return ( ( double )ADC_ConvertedValue[0] ) * 3.3 / 2.0 * 20 / 1024;
}

/**
* @brief    Get the voltage of AD channel 2
*
* @param[in] none
*
* @return    The voltage of AD channel 2
*
*/
double Get_Motor_Current_Vol( uint8_t driver_number )
{
	double volt = 0;
	volt = ( ( double )ADC_ConvertedValue[driver_number] ) * 3300.0f / 25.0f / 1024.0f;
    return  volt / 250;
}



