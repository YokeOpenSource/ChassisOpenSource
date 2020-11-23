#include "RemoterCtrlTask.h"
#include "module_ps2_remoter.h"
#include "module_remoter.h"
#include "FreeRTOS.h"
#include "task.h"

double remoter_x,remoter_y,remoter_z;

uint8_t remoter_online = 0;

/**
* @brief   switch to different remoter according the connection status
* @param[in] none
* @return   none
*/
void Remoter_Switch()
{
	if(UpdatePSRemoterData())
	{
		remoter_online = 1;
		remoter_x = PS2_GetX_Percent();
		remoter_y = PS2_GetY_Percent();
		remoter_z = PS2_GetZ_Percent();
	}
	else
	{
		if(IsRC_RemoterOffline() == 0)
		{
			remoter_online = 1;
			remoter_x = GetX_Percent_FromHotRC();
			remoter_y = GetY_Percent_FromHotRC();
			remoter_z = GetZ_Percent_FromHotRC();
		}
		else
		{
			remoter_online = 0;
			remoter_x = 0;
			remoter_y = 0;
			remoter_z = 0;
		}
			
	}
}

/**
* @brief   remoter handle task
* @param[in] none
* @return   none
*/
void RemoterCtrlHandleTask(void * pvParam)
{
	for(;;)
	{
		Remoter_Switch();
		Remoter_Time_Out_Protectprotect(500);
		vTaskDelay(50);
	}
}

/**
* @brief   get the x axis control data
* @param[in] none
* @return   none
*/
double Remoter_GetX(void)
{
	return remoter_x;
}
/**
* @brief   get the y axis control data
* @param[in] none
* @return   none
*/
double Remoter_GetY(void)
{
	return remoter_y;
}

/**
* @brief   get the z axis control data
* @param[in] none
* @return   none
*/
double Remoter_GetZ(void)
{
	return remoter_z;
}

