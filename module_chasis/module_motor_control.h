/**
 * @file module_motor_control.h
 * @brief  header file for motor control module
 * @updata_note
 */
#ifndef __MODULE_MOTOR_CONTROL_H__
#define __MODULE_MOTOR_CONTROL_H__

#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

enum 
{
	eMotor_Forward,
	eMotor_Backward
};

typedef enum
{
	eMotor1 = 1,
	eMotor2,
	eMotor3,
	eMotor4
}Motor_ID_t;


double Filter_LimitFilter( double target, double up_limit, double low_limit );
double Filter_AccFilterInNormal( double cal_value, double current, double threshold, double acc );
double Filter_AccFilterInEmergency( double cal_value, double current, double threshold, double acc );

signed short int Motor_MetersPerSecondToRPM( double m_move_velocity, float gear, double radius );
double Motor_RPMToMetersPerSecond( signed short int rmp, float gear, double radius );

	
void Motor_PWM_Ctrl(uint8_t dir,int16_t pwm,Motor_ID_t driver_number);
double *Get_Motor_Speed(void);


extern double motor_rmp_set[4];
extern double motor_rmp_get[4];

void PidControlLoopTask(void *pvParam);
void Motor_Pid_ParamInit(void);

#ifdef __cplusplus
}
#endif

#endif

