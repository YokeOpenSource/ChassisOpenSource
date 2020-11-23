#include "main.h"
#include "SR73_Radar.h"
#include "stmflash.h"

#define START_TASK_PRIO 1
#define START_STK_SIZE  128
TaskHandle_t START_Task_Handler;
void start_task(void *pvParam);

#define LED_TASK_PRIO 1
#define LED_STK_SIZE  50
TaskHandle_t LED_Task_Handler;

#define Sensor_TASK_PRIO 3
#define Sensor_STK_SIZE  250
TaskHandle_t Sensor_Task_Handler;

#define SerialHandle_TASK_PRIO 3
#define SerialHandle_STK_SIZE  250
TaskHandle_t SerialHandle_Task_Handler;

#define CANHandle_TASK_PRIO 3
#define CANHandle_STK_SIZE  250
TaskHandle_t CANHandle_Task_Handler;

#define RemoterControlLoop_TASK_PRIO 4
#define RemoterControlLoop_STK_SIZE  250
TaskHandle_t RemoterControlLoop_Task_Handler;

#define PIDControlLoop_TASK_PRIO 4
#define PIDControlLoop_STK_SIZE  512
TaskHandle_t PIDControlLoop_Task_Handler;

#define VehicleCtrlLoop_TASK_PRIO 3
#define VehicleCtrlLoop_STK_SIZE  512
TaskHandle_t VehicleCtrlLoop_Task_Handler;

#define SR73_TASK_PRIO 3
#define SR73_STK_SIZE  512
TaskHandle_t SR73_Task_Handler;

uint16_t pwm_ = 200,dir = 0;

/**
* @brief  motor test function
*@note pwm_ control the speed ,dir control the direction
*/
void Motor_Test(void)
{
    RELAY_ON;
	
	while(1)
	{
		for(char i = 1;i<=4;i++)
		{
			Motor_PWM_Ctrl(dir,pwm_,(Motor_ID_t)i);
		}	
	}
}
/**
* @brief  led control
*/
void RunningLEDandBeep( void  *pvParma)
{	
	uint16_t cnt_200_ms = 0;
	uint8_t low_battery_flag = 0;
	for(;;)
	{

		//when battery volt low than 10V ,set the warmming flag
		if(vehicle_data.battery_data.battery_volt <= 100)
		{
			low_battery_flag = 1;
			
			if(vehicle_data.battery_data.battery_volt <= 95)
			{
				low_battery_flag = 2;
			}
		}
		else
		{
			low_battery_flag = 0;
		}
		
		if(low_battery_flag == 0)
		{
			
			RELAY_ON;

			if(Radar_GetObstractStatus() == 1)
			{
				cnt_200_ms++;
				
				if(cnt_200_ms >= 2)
				{
					cnt_200_ms = 0;
					Beep_T;
				}				
			}
			else
			{
				cnt_200_ms = 0;
				Beep_OFF;
			}
		}
		else if(low_battery_flag == 1)
		{	
			cnt_200_ms++;
			if(cnt_200_ms >= 5)
			{
				cnt_200_ms = 0;
				Beep_T;
			}
			
		}
		else if(low_battery_flag == 2)
		{
			Beep_T;
			RELAY_OFF;
		}
		
		
		LED_GREEN_TOGGLE;
		vTaskDelay(200);
	}
    
}

extern double avoidance_diatance;
uint32_t dis_temp = 0;

void InitParam(void)
{
	STMFLASH_Read(ADDR_FLASH_SECTOR_11,&dis_temp,1);
	avoidance_diatance = dis_temp / 100.0f;
}
void PS2_SetInit(void);
/**
 * @brief the program entrance
 * @return int
 */
int main( void )
{
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	//初始化蜂鸣器
    Beep_Configuration();
	
	delay_init(168);
	
	Beep_ON;
	delay_ms(100);
	Beep_OFF;
	
	//初始化PS遥控接口
	PS_Remoter_GPIO_Init();	
//	PS2_SetInit();
	delay_ms(500);

	//Bluetooth
    USART1_Configuration();
	//SBUS
    USART3_Configuration(); 
	//usart2 
    USART2_DMA_Configuration();
	//外部CAN网络接口
    CAN2_Configuration();
	//指示灯
    LED_Configuration();

	//继电器IO初始化
	Relay_Configuration();

	//编码IO初始化
	Encoder_Init();

	//init param
	Motor_Pid_ParamInit();
	InitVehicleParam();
	
   //imu init
	IIC_Init();
	MPU6050_initialize();
	DMP_Init();

	//初始化电机电流采样以及电源电压采集接口
    AD_Configuration();
	//电机PWM控制初始化	
	Motor_PWM_Configuration();
	
	InitParam();

	//电机开环测试
//	Motor_Test();
	
	RELAY_ON;
	
	
	xTaskCreate( (TaskFunction_t) start_task,
				 (const char *)  "start_task",
				 (uint16_t)      START_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  START_TASK_PRIO,
				 (TaskHandle_t *) &START_Task_Handler);
	
	vTaskStartScheduler();

	//never arrive here
    while( 1 )
    {	
		
    }
}
  
/**
* @brief create the task
*@note
*/
void start_task(void *pvParam)
{
	 taskENTER_CRITICAL(); 
	
	 xTaskCreate( (TaskFunction_t) RunningLEDandBeep,
				 (const char *)  "running_led_task",
				 (uint16_t)      LED_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  LED_TASK_PRIO,
				 (TaskHandle_t *) &LED_Task_Handler);
				 
	 xTaskCreate( (TaskFunction_t) Sensor_UpdateTask,
				 (const char *)  "sensor_update_task",
				 (uint16_t)      Sensor_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  Sensor_TASK_PRIO,
				 (TaskHandle_t *) &Sensor_Task_Handler);


	 xTaskCreate( (TaskFunction_t) SerialHandleTask,
				 (const char *)  "serial_handle_task",
				 (uint16_t)      SerialHandle_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  SerialHandle_TASK_PRIO,
				 (TaskHandle_t *) &SerialHandle_Task_Handler);
				 
	 xTaskCreate( (TaskFunction_t) CAN_HandleTask,
				 (const char *)  "can_handle_task",
				 (uint16_t)      CANHandle_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  CANHandle_TASK_PRIO,
				 (TaskHandle_t *) &CANHandle_Task_Handler);
				 
	 xTaskCreate( (TaskFunction_t) RemoterCtrlHandleTask,
				 (const char *)  "remoter_handle_task",
				 (uint16_t)       RemoterControlLoop_STK_SIZE,
				 (void *)         NULL,
			     (UBaseType_t )   RemoterControlLoop_TASK_PRIO,
				 (TaskHandle_t *) &RemoterControlLoop_Task_Handler);
				 
	xTaskCreate( (TaskFunction_t) PidControlLoopTask,
				 (const char *)  "PidControlLoop_task",
				 (uint16_t)      PIDControlLoop_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  PIDControlLoop_TASK_PRIO,
				 (TaskHandle_t *) &PIDControlLoop_Task_Handler);
				 
	xTaskCreate( (TaskFunction_t) VehicleCtrlLoopTask,
				 (const char *)  "VehicleCtrlLoop_Task",
				 (uint16_t)      VehicleCtrlLoop_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  VehicleCtrlLoop_TASK_PRIO,
				 (TaskHandle_t *) &VehicleCtrlLoop_Task_Handler);
				 

	xTaskCreate( (TaskFunction_t) Radar_CheckObstract,
				 (const char *)  "SR73 TASK",
				 (uint16_t)      SR73_STK_SIZE,
				 (void *)        NULL,
			     (UBaseType_t )  SR73_TASK_PRIO,
				 (TaskHandle_t *) &SR73_Task_Handler);
				 
				 
    vTaskDelete(START_Task_Handler);
				 
    taskEXIT_CRITICAL();
}

