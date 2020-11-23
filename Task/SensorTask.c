#include "SensorTask.h"
#include "module_ad.h"
#include "module_vehicle.h"
#include "MPU6050.h"
#include "usart1.h"
#include "module_motor_control.h"
#include "FreeRTOS.h"
#include "task.h"

#define SENSOR_DEBUG 

/**
* @brief  update the motor current and battery volt data
*@note
* @param [in]  none
* @param [out] none
*/
static void Update_ADC(void)
{
	for(char i = 0;i < 4;i++)
	{
		vehicle_data.feedback_data.motor_data.motor_current[i] = Get_Motor_Current_Vol(i+1) * 100;
	}
	
	vehicle_data.battery_data.battery_volt = Get_Power_Vol() * 10 + 5;
	vehicle_data.battery_data.power = (Get_Power_Vol() - 10.5f) / 2.0f * 100;
	
	if(vehicle_data.battery_data.power >= 100)
		vehicle_data.battery_data.power = 100;
	else if(vehicle_data.battery_data.power <= 0)
		vehicle_data.battery_data.power = 0;
	
#ifdef SENSOR_DEBUG
	printf("M1:%d\tM2:%d\tM3:%d\tM4:%d\r\n",vehicle_data.feedback_data.motor_data.motor_current[0]\
											,vehicle_data.feedback_data.motor_data.motor_current[0]\
											,vehicle_data.feedback_data.motor_data.motor_current[0]\
											,vehicle_data.feedback_data.motor_data.motor_current[0]);
#endif
}


/**
* @brief  update the imu data
*@note
* @param [in]  none
* @param [out] none
*/
static void Update_IMU(void)
{
	float pitch,yaw,roll;
	extern double accel_conv[3],gyro_conv[3];
	
	if(Read_DMP(&pitch,&roll,&yaw) == 0)
	{
		vehicle_data.feedback_data.imu_data.pith = pitch * 1000;
		vehicle_data.feedback_data.imu_data.yaw  = yaw * 1000;
		vehicle_data.feedback_data.imu_data.roll = roll * 1000;
		
		for(char i = 0; i < 3; i++)
		{
			vehicle_data.feedback_data.imu_data.angular[i] =  gyro_conv[i] * 1000;
			vehicle_data.feedback_data.imu_data.acc[i] =  accel_conv[i] * 1000;
		}
			
#ifdef SENSOR_DEBUG		
		printf("yaw:%f \tpitch:%f \troll:%f\r\n",yaw,pitch,roll);	
#endif		
	}
}

/**
* @brief  update the motion data
*@note
* @param [in]  none
* @param [out] none
*/
static void Update_Motion(void)
{
	int16_t  left_wheel_rpm_avr,right_wheel_rpm_avr;
	double wheel_speed_l,wheel_speed_r;
	double *rpm_get;
	
	rpm_get = Get_Motor_Speed();
	
	left_wheel_rpm_avr  = (rpm_get[0]+ rpm_get[3]) / 2;
	right_wheel_rpm_avr = (rpm_get[1]+ rpm_get[2]) / 2;
	
	wheel_speed_l = Motor_RPMToMetersPerSecond(left_wheel_rpm_avr,1,vehicle_data.vehicle_param.wheel_radius);
	wheel_speed_r = Motor_RPMToMetersPerSecond(right_wheel_rpm_avr,1,vehicle_data.vehicle_param.wheel_radius);
	
	vehicle_data.feedback_data.speed_data_fdb.line_speed    = (wheel_speed_r - wheel_speed_l) / 2.0f * 1000;
	vehicle_data.feedback_data.speed_data_fdb.angular_speed = (wheel_speed_l + wheel_speed_r) / 2.0f / vehicle_data.vehicle_param.rotate_radius *1000;
	
	vehicle_data.feedback_data.odom_data.left_odom -=  wheel_speed_l * 0.05f * 1000;
	vehicle_data.feedback_data.odom_data.right_odom +=  wheel_speed_r * 0.05f *1000;
	
}


/**
* @brief  update the sensor date ,period is 20ms
*@note
* @param [in]  
* @param [in]  
* @param [out] 
*/
void Sensor_UpdateTask(void * pvParam)
{
	for(;;)
	{
		Update_ADC();
		Update_IMU();
		Update_Motion();
		vTaskDelay(50);
	}
}


