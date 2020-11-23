/**
 * @file module_ad.h
 * @brief  module_ad head file
 */

#ifndef __MODULE_AD_H__
#define __MODULE_AD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

double Get_Power_Vol( void );
	
double Get_Motor_Current_Vol( uint8_t driver_number );

#ifdef __cplusplus
}
#endif

#endif

