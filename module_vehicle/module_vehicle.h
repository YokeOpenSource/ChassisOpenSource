#ifndef _MODULE_VEHICLE_H_
#define _MODULE_VEHICLE_H_
#include "stm32f4xx.h"

typedef struct
{
	int16_t motor_current[4]; // 0.1A
	int16_t rpm_set[4]; // revolutions per minutes
	int16_t rpm_get[4]; // revolutions per minutes
}Motor_data_t; 

typedef struct
{
	int16_t angular[3]; // mrad/s
	int16_t acc[3]; // unit: mm/s2
	int16_t yaw;  // unit : 1 degree
	int16_t pith; // unit : 1 degree
	int16_t roll; // unit : 1 degree
}Imu_data_t; 

typedef struct
{
	int32_t left_odom; // mm
	int32_t right_odom; // mm 
}Odom_data_t; 

typedef struct
{
	double line_speed;//union: m/s
	double angular_speed;//union: m/s
	double travel_speed;//union: m/s
}Speed_data_t; 

typedef struct
{
	uint8_t online_status;
	uint16_t offline_cnt; 
}Terminal_Online_check_data_t; 

typedef union
{
	/* data */
	uint8_t status_byte;
	struct 
	{
		uint8_t can_ctrl_terminal:1;
		uint8_t usart_ctrl_terminal:1;
		uint8_t remoter_ctrl_terminal:1;
	}terminal;
}Terminal_status_t; 



typedef struct
{
	Motor_data_t motor_data;
	Imu_data_t imu_data;
	Odom_data_t odom_data;
	Speed_data_t speed_data_fdb;
	Terminal_status_t terminal_status;
}Feedback_data_t; 


typedef struct
{
	Speed_data_t usart_ctrl_data;
	Speed_data_t can_ctrl_data;
	Speed_data_t remoter_ctrl_data;
}Conntrol_data_t;

typedef struct
{
	double wheel_radius; // m
	double rotate_radius;
	double motor_reduction_ration; 
	uint16_t encoder_resolution;// line per circle
	double max_line_speed;//m/s
	double max_angular_speed;//rad/s
	double max_travel_speed;//m/s
}Vehicle_param_t; 

typedef struct
{
	int16_t battery_volt; // 1V
	int16_t power; // percent
}Battery_data_t; 

typedef struct
{
	Battery_data_t battery_data;
	Feedback_data_t feedback_data;
	Conntrol_data_t control_data;
	Vehicle_param_t vehicle_param;
}VehicleData_t;

extern VehicleData_t vehicle_data;

void Vehicle_Ctrl_Switch(void);

void InitVehicleParam(void );

void VehicleCtrlLoopTask(void *pvParam);
#endif
