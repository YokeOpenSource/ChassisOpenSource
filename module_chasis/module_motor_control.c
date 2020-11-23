#include "module_motor_control.h"
#include "pwm.h"
#include "pid.h"
#include "encoder.h"
#include "FreeRTOS.h"
#include "task.h"

#define Pi 3.141592654
double motor_rmp_get[4];
double motor_rmp_set[4];
pid_t pid_param[4];
int32_t encoder_data[4];

/**
* @brief  车速转换成电机RPM数据
 * @param[in] m_move_velocity: 车速
 * @param[in] gear : 减速比
 * @param[in] diameter: 轮半径
 * @return  rpm
 */
signed short int Motor_MetersPerSecondToRPM( double m_move_velocity, float gear, double radius )
{
    signed short int out_rpm;

    /*计算该速度的输出rpm*/
    out_rpm = m_move_velocity * 60.0 * gear / ( Pi * radius * 2 );

    return out_rpm;
}

/**
* @brief  RPM转换成车速数据
* @param[in] rmp: 电机rpm数据
 * @param[in] gear : 减速比
 * @param[in] diameter: 轮半径
 * @return  车速
 */
double Motor_RPMToMetersPerSecond( signed short int rmp, float gear, double radius )
{
    double velocity ;

    /*将rmp 计算为m/s*/
    velocity = rmp * ( Pi * radius * 2 ) / gear / 60.0;

    return velocity;
}


/**
 * @brief  set the motor ctrl pwm duty cycle
 * @param[in] motor dir 
 * @param[in] pwm value, range 0 - 100 ,fre 10k
 * @param[in] motor id ,range 1-4
 * @retval  none
 */
void Motor_PWM_Ctrl(uint8_t dir,int16_t pwm,Motor_ID_t driver_number)
{
	switch(driver_number)
	{
		case eMotor1:
		    if(dir == eMotor_Forward)
			{
				TIM1->CCR1 = pwm  ;
				MOTOR1_PWM_SWITCH_H;
			}
			else
			{
				TIM1->CCR1 =  pwm ;
				MOTOR1_PWM_SWITCH_L;			
			}
			break;
			
		case eMotor2:
		    if(dir == eMotor_Forward)
			{
				TIM1->CCR4 =  pwm ;
				MOTOR2_PWM_SWITCH_H;
			}
			else
			{
				TIM1->CCR4 =  pwm ;
				MOTOR2_PWM_SWITCH_L;			
			}
		    break;
			
		case eMotor3:
		    if(dir == eMotor_Forward)
			{
				TIM8->CCR4 =  pwm ;
				MOTOR3_PWM_SWITCH_H;
			}
			else
			{
				TIM8->CCR4 =  pwm ;
				MOTOR3_PWM_SWITCH_L;				
			}
			break;
			
		case eMotor4:
		    if(dir == eMotor_Forward)
			{
				TIM8->CCR3 =  pwm ;
				MOTOR4_PWM_SWITCH_H;
			}
			else
			{
				TIM8->CCR3 =  pwm ;
				MOTOR4_PWM_SWITCH_L;			
			}
			break;
			
		default:
			break;
		
	}
}

/**
* @brief   return the rpm data
* @param[in] none
* @return    none
*/
double *Get_Motor_Speed(void)
{
	for(char i= 0;i<4;i++)
	{
		encoder_data[i]  = Get_Encoder_CNT(i+1);
		motor_rmp_get[i] =  encoder_data[i]  * 20.0f * 60 / 4.0f / 11.0f / 90.0f * 2.107f;
	}
	
	return motor_rmp_get;
}

/**
* @brief   init the pid parameter 
* @param[in] none
* @return    none
*/
void Motor_Pid_ParamInit(void)
{
	for(char i = 0;i < 4;i++)
	{
		PID_struct_init(&pid_param[i],DELTA_PID,500,500,0.0F,0.12F,0.0F);	
	}
}

/**
* @brief   pid calc
* @param[in] none
* @return    input data
*/
static void Motor_PID_Ctrl(void)
{
	for(char i = 0;i < 4; i++)
	{
		int16_t set = 0 ,get = 0 ;
		set = motor_rmp_set[i];
		get = motor_rmp_get[i];
		
		pid_calc(&pid_param[i],set,get);
		
		if(set == 0 && get == 0)
		{
			pid_param[i].iout = 0;
		}
		
		if(pid_param[i].out > 0)
		{
			Motor_PWM_Ctrl(0,pid_param[i].out,(Motor_ID_t)(i+1));
		}
	    else if(pid_param[i].out < 0)
		{
			Motor_PWM_Ctrl(1,-pid_param[i].out,(Motor_ID_t)(i+1));
		}	
		else
		{
			Motor_PWM_Ctrl(0,0,(Motor_ID_t)(i+1));
		}
	}	
}

/**
* @brief  pid control task 
* @param[in] none
* @return    none
*/
void PidControlLoopTask(void *pvParam)
{
	for(;;)
	{
		Motor_PID_Ctrl();
		vTaskDelay(10);
	}
}


