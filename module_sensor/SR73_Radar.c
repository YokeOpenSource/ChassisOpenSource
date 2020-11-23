#include "SR73_Radar.h"
#include "FreeRTOS.h"
#include "task.h"
#include "math.h"

//雷达数据保存结构图变量
Radar_Object_Data_t object_data;

//障碍物检测标志
uint8_t obstract_flag = 0;
uint16_t obstract_cnt = 0;
//radar在线检测变量
uint16_t  radar_online_flag  = 0;
uint8_t   radar_offline_cnt   = 0;

//雷达默认避障距离：0.5m ，如果FLASH修改过，开机会读取并更新该变量
double avoidance_diatance = 0.5;

/**
* @brief  解析SR73雷达的CAN数据
*@note
* @param [in]  CAN报文数据指针 
* @param [out] 
*/
void SR73_Radar_MessageDispatch(CanRxMsg *can_rx)
{
	switch(can_rx->StdId)
	{
		case 0x60a:
			/*< 获取目标数量*/
			object_data.target_num = can_rx->Data[0];
			break;
		
		case 0x60b:
			/*< 刷新在线检测变量*/
		    radar_offline_cnt = 0;
			radar_online_flag = 1;
			/*< 解析目标数据*/
			object_data.object_id =  (can_rx->Data[0]) ;
			object_data.dislong   = ((can_rx->Data[1] << 5) | (can_rx->Data[2] >> 3))*0.2 -500 ;
			object_data.dislat    =(((can_rx->Data[2] & 0x7) << 8) | can_rx->Data[3])* 0.2 -204.6 ;
			object_data.vrelong   = ((can_rx->Data[4] << 2) | (can_rx->Data[5] >> 6))*0.25 -128 ;
			object_data.vrelat    =(((can_rx->Data[5] & 0x3f) << 3) | (can_rx->Data[6] >> 5))*0.25- 64;
			object_data.dynprop   = (can_rx->Data[6] & 0x7);
			object_data.RCS = can_rx->Data[7] * 0.5 - 64;
		
			/*< 换算出目标的角度和径向距离*/
			object_data.angle = atan(object_data.dislat / object_data.dislong) / 3.14 * 180;
     		object_data.radius = sqrt(object_data.dislong * object_data.dislong + object_data.dislat * object_data.dislat);
			
			/*< 检测到障碍物，置位标志*/
			if(object_data.radius <= avoidance_diatance  && obstract_flag == 0)
			{
				obstract_flag = 1;
			}
			
			if(object_data.radius <= avoidance_diatance  && obstract_flag == 1)
			{
				obstract_cnt = 0;
			}
			break;
		
		default:
			break;
	}
}



/**
* @brief  解析SR28F雷达的CAN数据
*@note
* @param [in]  CAN报文数据指针 
* @param [out] 
*/
void SR28F_Radar_MessageDispatch(CanRxMsg *can_rx)
{
	switch(can_rx->StdId)
	{		
		case 0x70C:
			/*< 刷新在线检测变量*/
		    radar_offline_cnt = 0;
			radar_online_flag = 1;
			/*< 解析目标数据*/
			object_data.object_id =  (can_rx->Data[0]) ;
			object_data.RCS = can_rx->Data[1] * 0.5 - 50;
		
			object_data.angle = can_rx->Data[4] - 90;
     		object_data.radius = (can_rx->Data[2] << 8 |  can_rx->Data[3] ) * 0.01;
			object_data.target_speed  =  (((can_rx->Data[5] & 0x07) << 8) | can_rx->Data[6]) * 0.05 - 35;
			
			/*< 检测到障碍物，置位标志*/
			if(object_data.radius <= avoidance_diatance  && obstract_flag == 0)
			{
				obstract_flag = 1;
			}

			if(object_data.radius <= avoidance_diatance  && obstract_flag == 1)
			{
				obstract_cnt = 0;
			}
			break;
		
		default:
			break;
	}
}

/**
* @brief  获取是否检测到障碍物
*@note
* @param [in]  none
* @param [out] none
*/
uint8_t Radar_GetObstractStatus(void)
{
	if(radar_online_flag == 1)
	{
		if(obstract_flag == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
/**
* @brief 检测雷达是否已连接
*@note
* @param [in]  none
* @param [out] none
*/
void CheckRadar_ConnectStatus(void)
{
	radar_offline_cnt++;
	if(radar_offline_cnt >= 50)
	{
		radar_offline_cnt = 50;
		radar_online_flag = 0;
	}
}

/**
* @brief 雷达处理线程
*@note
* @param [in]  none
* @param [out] none
*/
void Radar_CheckObstract(void * param)
{
	for(;;)
	{
		if(obstract_flag == 1)
		{
			obstract_cnt++;
			if(obstract_cnt >= 20)
			{
				obstract_cnt = 0;
				obstract_flag = 0;
			}		
		}
		
		CheckRadar_ConnectStatus();
		
		vTaskDelay(10);
	}
}




