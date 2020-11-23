/**
 * @file module_tim2.c
 * @brief tim2 work function
 * @updata_note
 */

#include "module_tim2.h"
#include "module_task.h"

/**
* @brief    Timer2 interrupt function
*
* @param[in] none
*
* @return    input data
*
*/
unsigned int Timer2_Counter = 0;    //Timer2��ʱ����������(ms)
unsigned int W5500_Send_Delay_Counter = 0; //W5500������ʱ��������(ms)

void TIM7_IRQHandler( void )
{

    if( TIM_GetITStatus( TIM7, TIM_IT_Update ) != RESET )
    {
        TIM_ClearFlag( TIM7, TIM_FLAG_Update );

        //����1ms
        Timer2_Counter++;
        W5500_Send_Delay_Counter++;

//        hSCH_Update();

    }
}


