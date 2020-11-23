#include "SerialCtrlTask.h"
#include "usart6.h"
#include "usart2_ringbuf.h"
#include "delay.h"
#include "module_vehicle.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stmflash.h"

uint8_t serial_temp_buffer[50];

Terminal_Online_check_data_t serial_connect_check;

SerialCtrl_Frame_t serial_ctrl_data;

SerialRadarParam_Frame_t radar_param_data;

double usart_angular_speed_set,usart_line_speed_set;

/**
* @brief  get the one byte check sum 
*@note
* @param [in] buff ,the data need to be check 
* @param [in] len ,the data len
* @param [out] check result
*/
static uint8_t GetCheckSum(uint8_t *buff,uint8_t len)
{
	uint8_t sum = 0;
	for(char i = 0;i < len;i++)
	{
		sum += buff[i];
	}
	
	return sum ;
}

/**
* @brief  get the two byte check sum 
*@note
* @param [in] buff ,the data need to be check 
* @param [in] len ,the data len
* @param [out] check result
*/
static uint16_t GetCheckSum16(uint8_t *buff,uint8_t len)
{
	uint16_t sum = 0;
	for(char i = 0;i < len;i++)
	{
		sum += buff[i];
	}
	
	return sum ;
}

extern double avoidance_diatance  ;
/**
* @brief  dispatch the data of serial
*@note
* @param [in]  none 
* @param [out] none
*/
static void DispatchSerial(void)
{
    unsigned char rec_lens;
    
    rec_lens = USART2_GetDataCount();

    if(rec_lens >= 12 || USART2_isFull())
    {
        if(	USART2_At(0)  == 0xFE 
	     && USART2_At(1)  == 0xEF )
        {
			uint8_t data_len = 0;
			data_len = USART2_At(2);
			
			if( data_len > rec_lens)
			{
				 USART2_Drop(rec_lens);
				 return ;
			}
			
            
            USART2_Recv(serial_temp_buffer, data_len+4);
                      
            if(serial_temp_buffer[data_len + 3] == \
                GetCheckSum(serial_temp_buffer,data_len + 3))
            {   
				serial_connect_check.offline_cnt = 0;
				serial_connect_check.online_status = 1;
				vehicle_data.feedback_data.terminal_status.terminal.usart_ctrl_terminal = 1;
				
				if(serial_temp_buffer[3]== 0x01)
				{
					memcpy(&serial_ctrl_data,serial_temp_buffer,sizeof(SerialCtrl_Frame_t));
					usart_line_speed_set    = serial_ctrl_data.line_speed / 1000.0f;
					usart_angular_speed_set = serial_ctrl_data.angular_speed / 1000.0f;
				}	
				else if(serial_temp_buffer[3]== 0x02)
				{
					memcpy(&radar_param_data,serial_temp_buffer,sizeof(SerialRadarParam_Frame_t));
					
					double temp = radar_param_data.avoidance_dis / 100.0f;
					if(temp <= 0.2)
					{
						temp = 0.2;
					}
					
					avoidance_diatance = temp;
	
					if(radar_param_data.save_flag == 1)
					{
						uint32_t data_write = radar_param_data.avoidance_dis;
						
						if(data_write <= 20)
						{
							data_write = 20;
						}
						else if(data_write >= 10000)
							data_write = 10000;
						
						STMFLASH_Write(ADDR_FLASH_SECTOR_11,&data_write,1);
					}	
				}		
            }
            else 
            {
                USART2_Drop(1);
            }
        }
        else
        {
            USART2_Drop(1);
        }
    }
	else
	{
		for(;;)
		{
			if(USART2_GetDataCount() > 2)
			{
				if(	USART2_At(0)  == 0xFE \
					&& USART2_At(1)  == 0xEF)     
				 {
					break;
				 }
				else 
				{
					USART2_Drop(1);
				}
			}
			else break;
		}	
	}
}

/**
* @brief  check the serial connect status
*@note
* @param [in]  none 
* @param [out] none
*/
static void CheckSerial_ConnectStatus_20ms(void)
{
	serial_connect_check.offline_cnt++;
	if(serial_connect_check.offline_cnt >= 25)
	{
		serial_connect_check.offline_cnt = 10;
		serial_connect_check.online_status = 0;
		vehicle_data.feedback_data.terminal_status.terminal.usart_ctrl_terminal = 0;
	}
}

/**
* @brief  send the feedback data by serial
*@note
* @param [in]  none 
* @param [out] none
*/
void UsartSendFdbMessage(void)
{
	SerialFdb_Frame_t frame;
	
	frame.header = 0xEFFE;
	frame.uid =  0x01;
	frame.len = sizeof(SerialFdb_Frame_t) - 6;
	
	frame.line_speed    = vehicle_data.feedback_data.speed_data_fdb.line_speed;
	frame.angular_speed = vehicle_data.feedback_data.speed_data_fdb.angular_speed;
	frame.odom_l = vehicle_data.feedback_data.odom_data.left_odom;
	frame.odom_r = vehicle_data.feedback_data.odom_data.right_odom;
	frame.yaw = vehicle_data.feedback_data.imu_data.yaw;
	
	for(char i = 0;i < 3;i++)
	{
		frame.gyro[i] = vehicle_data.feedback_data.imu_data.angular[i];
		frame.acc[i]  = vehicle_data.feedback_data.imu_data.acc[i];
	}
	
	frame.battery_vlot = vehicle_data.battery_data.battery_volt;
	
	frame.avoidance_dis = avoidance_diatance * 100;
	
	frame.checksum = GetCheckSum16((uint8_t *)&frame,sizeof(SerialFdb_Frame_t) - 2);
	
	USART2_Send((uint8_t *)&frame,sizeof(SerialFdb_Frame_t));
}

/**
* @brief  dispatch the serial data and send fdb task
*@note
* @param [in]  none 
* @param [out] none
*/
void SerialHandleTask(void *pvParam)
{
	uint8_t cnt_5ms = 0;
	
	for(;;)
	{
		DispatchSerial();
		vTaskDelay(5);
		cnt_5ms++;
		if(cnt_5ms >= 4)
		{
			cnt_5ms = 0;
			UsartSendFdbMessage();
			CheckSerial_ConnectStatus_20ms();
		}
	}
}
