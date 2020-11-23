#include "module_vehicle.h"
#include "module_motor_control.h"
#include "math.h"
#include "stdio.h"
#include "RemoterCtrlTask.h"
#include "module_remoter.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SR73_Radar.h"

//set wheel radius
#define MAILUN_WHEEL_RIDUS 0.045F
#define NORMAL_WHEEL_RIDUS 0.05F

double wheel_ridus = NORMAL_WHEEL_RIDUS;

VehicleData_t vehicle_data;

/**
* @brief   init the vehicle parameter 
* @param[in] none
* @return    input data
*/
void InitVehicleParam(void )
{
	vehicle_data.vehicle_param.encoder_resolution = 11;
	vehicle_data.vehicle_param.max_angular_speed = 1.5f;
	vehicle_data.vehicle_param.max_line_speed = 0.56f;
	vehicle_data.vehicle_param.max_travel_speed = 0.56f;
	vehicle_data.vehicle_param.motor_reduction_ration = 90;
	vehicle_data.vehicle_param.rotate_radius = 0.2f;
	vehicle_data.vehicle_param.wheel_radius = NORMAL_WHEEL_RIDUS;
}

/**
* @brief  according the speed calc the rpm data
* @param[in] none
* @return    none
*/
void Calc_RPM(double line_speed,double spin_speed,double traverse_speed)
{
    double set_rpm[4];
	
	set_rpm[0] =  line_speed + spin_speed *  vehicle_data.vehicle_param.rotate_radius + traverse_speed;
	set_rpm[1] = -line_speed + spin_speed *  vehicle_data.vehicle_param.rotate_radius + traverse_speed;
	set_rpm[2] = -line_speed + spin_speed *  vehicle_data.vehicle_param.rotate_radius - traverse_speed;
	set_rpm[3] =  line_speed + spin_speed *  vehicle_data.vehicle_param.rotate_radius - traverse_speed;

	for(char i = 0; i < 4 ; i++)
		set_rpm[i] = Motor_MetersPerSecondToRPM(set_rpm[i],1,wheel_ridus);
	
	for(char i = 0; i < 4 ; i++)
	{
		motor_rmp_set[i] = set_rpm[i];
	}
	
}

double x,y,z;
extern uint8_t isDisable_Avoidance;
/**
* @brief   update the speed from remoter 
* @param[in] none
* @return   none
*/
void Remoter_Ctrl_Loop(void)
{
	if(Radar_GetObstractStatus() == 0)
	{
		x =   Remoter_GetX() * vehicle_data.vehicle_param.max_line_speed ;
		y =   Remoter_GetY() * vehicle_data.vehicle_param.max_travel_speed ;
		z =  -Remoter_GetZ() * vehicle_data.vehicle_param.max_angular_speed ;		
	}
	else
	{
		if(isDisable_Avoidance == 0)
		{
	       double temp = Remoter_GetX() * vehicle_data.vehicle_param.max_line_speed;
		   x = temp > 0 ? 0:temp;		
		   y = 0;
		   z = 0;
		}
		else
		{
		    x =   Remoter_GetX() * vehicle_data.vehicle_param.max_line_speed ;
			y =   Remoter_GetY() * vehicle_data.vehicle_param.max_travel_speed ;
			z =  -Remoter_GetZ() * vehicle_data.vehicle_param.max_angular_speed ;
		}

	}

	
	Calc_RPM(-x,z,y);
}




extern double usart_line_speed_set,usart_angular_speed_set;
extern double can_line_speed_set,can_angular_speed_set;
/**
* @brief   update the speed from can bus or usart 
* @param[in] none
* @return   none
*/
void CAN_Usart_Ctrl_Loop(void)
{
	if(vehicle_data.feedback_data.terminal_status.terminal.can_ctrl_terminal == 1)
	{
		x =  can_line_speed_set ;
//		y =  GetY_Percent_FromHotRC() * MAX_TRAVEL_SPEED ;
		z =  can_angular_speed_set ;	
	}
	else if (vehicle_data.feedback_data.terminal_status.terminal.usart_ctrl_terminal == 1)
	{
		x =  usart_line_speed_set ;
//		y =  GetY_Percent_FromHotRC() * MAX_TRAVEL_SPEED ;
		z =  usart_angular_speed_set ;
	}
	else
	{
		x = 0;
		y = 0;
		z = 0;		
	}
	
	Calc_RPM(-x,z,0);
}

void Vehicle_Stop(void)
{
	Calc_RPM(0,0,0);
}

extern uint8_t remoter_online ;

/**
* @brief   vehicle control task
* @param[in] none
* @return   none
*/
void VehicleCtrlLoopTask(void *pvParam)
{
	for(;;)
	{
		if(remoter_online == 1 )
		{
			Remoter_Ctrl_Loop();
		}
		else
		{
			CAN_Usart_Ctrl_Loop();
		}	
			
		vTaskDelay(20);
	}
}


