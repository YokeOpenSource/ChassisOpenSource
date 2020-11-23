#include "CanCtrlTask.h"
#include "module_vehicle.h"
#include "FreeRTOS.h"
#include "task.h"

Terminal_Online_check_data_t can_connect_check;
double can_line_speed_set,can_angular_speed_set;

/**
* @brief  feedback the vel data by can bus
*@note
* @param [in] none 
* @param [in] none
* @param [out] none
*/
static void VelocityFeedback_ToController(void)
{
    static CanTxMsg tx_msg;
	static uint8_t cnt = 0;

    tx_msg.StdId = 0x001;
//	tx_msg.IDE = CAN_Id_Standard;

    tx_msg.DLC = 0x08;

	
    tx_msg.Data[0] = (int16_t)(vehicle_data.feedback_data.speed_data_fdb.line_speed) >> 8;
    tx_msg.Data[1] = (int16_t)(vehicle_data.feedback_data.speed_data_fdb.line_speed);

    tx_msg.Data[2] = (int16_t)(vehicle_data.feedback_data.speed_data_fdb.angular_speed) >> 8;
    tx_msg.Data[3] = (int16_t)(vehicle_data.feedback_data.speed_data_fdb.angular_speed) ;

    tx_msg.Data[4] = 0x00;
    tx_msg.Data[5] = 0x00;

    tx_msg.Data[6] = 0;

    tx_msg.Data[7] = cnt++;

    CAN_Transmit( CAN2, &tx_msg );
}
/**
* @brief  feedback the odom data by can bus
*@note
* @param [in] none 
* @param [in] none
* @param [out] none
*/
static void OdomFeedback_ToController(void)
{
    static CanTxMsg tx_msg;

    tx_msg.StdId = 0x002;

    tx_msg.DLC = 0x08;


    tx_msg.Data[0] = (int32_t)(vehicle_data.feedback_data.odom_data.left_odom) >> 24;
    tx_msg.Data[1] = (int32_t)(vehicle_data.feedback_data.odom_data.left_odom) >> 16;
    tx_msg.Data[2] = (int32_t)(vehicle_data.feedback_data.odom_data.left_odom) >> 8;
    tx_msg.Data[3] = (int32_t)(vehicle_data.feedback_data.odom_data.left_odom);

    tx_msg.Data[4] = (int32_t)(vehicle_data.feedback_data.odom_data.right_odom) >> 24;
    tx_msg.Data[5] = (int32_t)(vehicle_data.feedback_data.odom_data.right_odom) >> 16;
    tx_msg.Data[6] = (int32_t)(vehicle_data.feedback_data.odom_data.right_odom) >> 8;
    tx_msg.Data[7] = (int32_t)(vehicle_data.feedback_data.odom_data.right_odom);
	
    CAN_Transmit( CAN2, &tx_msg );
}
/**
* @brief  feedback the imu data by can bus
*@note
* @param [in] none 
* @param [in] none
* @param [out] none
*/
static void IMUFeedback_ToController(void)
{
    static CanTxMsg tx_msg;

    tx_msg.StdId = 0x004;

    tx_msg.DLC = 0x04;
	
    tx_msg.Data[0] = vehicle_data.feedback_data.imu_data.yaw >> 24;
    tx_msg.Data[1] = vehicle_data.feedback_data.imu_data.yaw >> 16;
    tx_msg.Data[2] = vehicle_data.feedback_data.imu_data.yaw >> 8;
    tx_msg.Data[3] = vehicle_data.feedback_data.imu_data.yaw;
	
    CAN_Transmit( CAN2, &tx_msg );
}
/**
* @brief  dispatch the can ctrl data
*@note
* @param [in] none 
* @param [in] none
* @param [out] none
*/
void DispatchCANMessage(CanRxMsg *can_rx)
{
	if(can_rx->StdId == 0x010)
	{
		can_connect_check.offline_cnt = 0;
		can_connect_check.online_status = 1;
		vehicle_data.feedback_data.terminal_status.terminal.can_ctrl_terminal = 1;
		
		can_line_speed_set    = (short)(can_rx->Data[0] << 8 |can_rx->Data[1]) / 1000.0f;
		can_angular_speed_set = (short)(can_rx->Data[2] << 8 |can_rx->Data[3]) / 1000.0f;
	}	
}

/**
* @brief  check the can bus connect status
*@note
* @param [in] none 
* @param [in] none
* @param [out] none
*/
void CAN_ConnectCheck_20ms(void)
{
	 can_connect_check.offline_cnt++;
	
	if(can_connect_check.offline_cnt >= 25)
	{
		can_connect_check.offline_cnt = 10;
		can_connect_check.online_status = 0;
		vehicle_data.feedback_data.terminal_status.terminal.can_ctrl_terminal = 0;
	}
}

/**
* @brief can handle task
*@note
* @param [in] none 
* @param [in] none
* @param [out] none
*/
void CAN_HandleTask(void *pvParam)
{
	for(;;)
	{
		VelocityFeedback_ToController();
		vTaskDelay(1);
		OdomFeedback_ToController();
		vTaskDelay(1);
		IMUFeedback_ToController();
		vTaskDelay(18);
		CAN_ConnectCheck_20ms();			
	}
}


