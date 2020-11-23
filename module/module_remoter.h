/**
 * @file module_dio.h
 * @brief  module_dio head file
 * @updata_note
 */

#ifndef __MODULE_REMOTER_H__
#define __MODULE_REMOTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define BASE_TIME_PROTECT 50

#define SBUS_Data_length 25

extern unsigned short  sbus_channel_temp[25];
short int Remoter_GetCurrentVlaue( char channel );
double Remoter_GetPercent( char channel );
void Remoter_Time_Out_Protectprotect( unsigned short protect_time );

double GetX_Percent_FromHotRC(void);
double GetY_Percent_FromHotRC(void);
double GetZ_Percent_FromHotRC(void);
uint8_t IsRC_RemoterOffline(void);
uint16_t GetSwitchDValue(void);

#ifdef __cplusplus
}
#endif

#endif

