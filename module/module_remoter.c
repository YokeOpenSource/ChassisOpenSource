/**
 * @file module_remoter.c
 * @brief remoter work function
 * @updata_note
 */

#include "usart3.h"
#include "module_remoter.h"
#include "main.h"

uint16_t  sbus_channel_temp[25] = {992, 992, 32, 992, 192, 192}; //  temp sbus decode channel data

unsigned short remoter_time_out_counter = 0;

#define REMOTER_MC6C 1
#define REMOTER_HT_6A 2

#define REMOTER_TYPE REMOTER_HT_6A

#if (REMOTER_TYPE == REMOTER_HT_6A)
	#define REMOTER_MID_VALUE           992
	#define REMOTER_HALF_RANGE          560.0
	#define REMOTER_DEADBAND 0.01
#elif (REMOTER_TYPE == REMOTER_MC6C)
	#define REMOTER_MID_VALUE           1004
	#define REMOTER_HALF_RANGE          792.0
	#define REMOTER_DEADBAND 0.05
#endif




unsigned char remoter_offline_flag = 0;

/**
 * @brief return the channel value
 * @param[in]
 * @return
 */
short int Remoter_GetCurrentVlaue( char channel )
{
    return sbus_channel_temp[channel];
}

/**
 * @brief  get percent of x axis
 * @param[in]
 * @return
 */
double GetX_Percent_FromHotRC(void)
{
    double result = ( sbus_channel_temp[1] - REMOTER_MID_VALUE ) / REMOTER_HALF_RANGE;
    
    if(  result <= REMOTER_DEADBAND && result >= -REMOTER_DEADBAND  )
    {
        result = 0 ;
    }	
	
	return result;
}
/**
 * @brief  get percent of y axis
 * @param[in]
 * @return
 */
double GetY_Percent_FromHotRC(void)
{
    double result = ( sbus_channel_temp[0] - REMOTER_MID_VALUE ) / REMOTER_HALF_RANGE;
    
    if(  result <= REMOTER_DEADBAND && result >= -REMOTER_DEADBAND  )
    {
        result = 0 ;
    }	
	
	return result;
}

/**
 * @brief  get percent of z axis
 * @param[in]
 * @return
 */
double GetZ_Percent_FromHotRC(void)
{
#if (REMOTER_TYPE == REMOTER_HT_6A)
    double result = ( sbus_channel_temp[3] - REMOTER_MID_VALUE ) / REMOTER_HALF_RANGE;
#elif (REMOTER_TYPE == REMOTER_MC6C)
	    double result = -( sbus_channel_temp[0] - REMOTER_MID_VALUE ) / REMOTER_HALF_RANGE;
#endif    
    if(  result <= REMOTER_DEADBAND && result >= -REMOTER_DEADBAND  )
    {
        result = 0 ;
    }	
	
	return result;
}

/**
 * @brief  get the value  of swd
 * @param[in]
 * @return
 */
uint16_t GetSwitchDValue(void)
{
	return sbus_channel_temp[5];
}

/**
 * @brief   获取不同遥控器通道的百分比
 * @param[in] 通道号
 * @return
 */
double Remoter_GetPercent( char channel )
{
    double result = ( sbus_channel_temp[channel] - REMOTER_MID_VALUE ) / REMOTER_HALF_RANGE;
    
    if(  result <= 0.003 && result >= -0.003  )
    {
        result = 0 ;
    }
    
    return result;
}

/**
* @brief    S-BUS receive interrupt
* @param[in] none
* @return    input data
*/
void RemoterDataDecoder( void )
{
	if(sbus_rx_buffer[0] == 0x0f && sbus_rx_buffer[24] == 0)
	{
		sbus_channel_temp[0] = (sbus_rx_buffer[1]| (sbus_rx_buffer[2] << 8)) & 0x07ff;
		sbus_channel_temp[1] = ((sbus_rx_buffer[2] >> 3) | (sbus_rx_buffer[3] << 5)) & 0x07ff;
		sbus_channel_temp[2] = ((sbus_rx_buffer[3] >> 6) | (sbus_rx_buffer[4] << 2) | (sbus_rx_buffer[5] << 10)) & 0x07ff;
		sbus_channel_temp[3] = ((sbus_rx_buffer[5] >> 1) | (sbus_rx_buffer[6] << 7)) & 0x07ff;
		sbus_channel_temp[4] = ((sbus_rx_buffer[6] >> 4) | (sbus_rx_buffer[7] << 4)) & 0x07ff;
		sbus_channel_temp[5] = ((sbus_rx_buffer[7] >> 7) | (sbus_rx_buffer[8] << 1)| (sbus_rx_buffer[9] << 9)) & 0x07ff;
		sbus_channel_temp[6] = ((sbus_rx_buffer[9] >> 2) | (sbus_rx_buffer[10] << 6)) & 0x07ff;
		sbus_channel_temp[7] = ((sbus_rx_buffer[10]>> 5) | (sbus_rx_buffer[11] << 3)) & 0x07ff;
		sbus_channel_temp[8] = (sbus_rx_buffer[12]| (sbus_rx_buffer[13] << 8)) & 0x07ff;
		sbus_channel_temp[9] = ((sbus_rx_buffer[13] >> 3) | (sbus_rx_buffer[14] << 5)) & 0x07ff;
		sbus_channel_temp[10] = ((sbus_rx_buffer[14] >> 6) | (sbus_rx_buffer[15] << 2) | (sbus_rx_buffer[16] << 10)) & 0x07ff;
		sbus_channel_temp[11] = ((sbus_rx_buffer[16] >> 1) | (sbus_rx_buffer[17] << 7)) & 0x07ff;
		sbus_channel_temp[12] = ((sbus_rx_buffer[17] >> 4) | (sbus_rx_buffer[18] << 4)) & 0x07ff;
		sbus_channel_temp[13] = ((sbus_rx_buffer[18] >> 7) | (sbus_rx_buffer[19] << 1)| (sbus_rx_buffer[20] << 9)) & 0x07ff;
		sbus_channel_temp[14] = ((sbus_rx_buffer[20] >> 2) | (sbus_rx_buffer[21] << 6)) & 0x07ff;
		sbus_channel_temp[15] = ((sbus_rx_buffer[21] >> 5) | (sbus_rx_buffer[22] << 3)) & 0x07ff;
		sbus_channel_temp[16] = (sbus_rx_buffer[23] &0X80)>>7 ;
		sbus_channel_temp[17] = (sbus_rx_buffer[23] &0X40)>>6;
		sbus_channel_temp[18] = (sbus_rx_buffer[23] &0X20)>>5;
		sbus_channel_temp[19] = (sbus_rx_buffer[23]&0X10)>>4;

	}
}

/**
* @brief    usart3 interrupt handle function ,reset the cnt of dma
* @param[in] none
* @return    input data
*/
void USART3_IRQHandler(void)
{
   unsigned int len = 0;

    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(USART3);  // 清USART_IT_IDLE标志

        DMA_Cmd(DMA1_Stream1, DISABLE);
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);

        len = SBUS_Data_length - DMA_GetCurrDataCounter(DMA1_Stream1);

        DMA_SetCurrDataCounter(DMA1_Stream1, SBUS_Data_length);
        DMA_Cmd(DMA1_Stream1, ENABLE);
		
		RemoterDataDecoder();	
    }
}

/**
* @brief   check the remoter is offline ?
* @param[in] none
* @return    input data
*/
void Remoter_Time_Out_Protectprotect( unsigned short protect_time )
{
#if (REMOTER_TYPE == REMOTER_HT_6A)
	if(sbus_channel_temp[2] == 32)
#elif (REMOTER_TYPE == REMOTER_MC6C)
	if(sbus_channel_temp[2] == 41)
#endif
	{
		remoter_time_out_counter++;
	}
	else{
		remoter_time_out_counter = 0;
		remoter_offline_flag  = 0;
	}
	
    if( remoter_time_out_counter >= protect_time / BASE_TIME_PROTECT )
    {
		remoter_offline_flag = 1;
    }
}
/**
* @brief   get the remoter connect status
* @param[in] none
* @return    input data
*/
uint8_t IsRC_RemoterOffline(void)
{
	return remoter_offline_flag;
}
