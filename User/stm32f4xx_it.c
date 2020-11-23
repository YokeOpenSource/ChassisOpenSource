#include "stm32f4xx_it.h"
#include "main.h"
#include "CanCtrlTask.h"
#include "SR73_Radar.h"

extern void TimingDelay_Decrement( void );

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler( void )
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler( void )
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while( 1 )
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler( void )
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while( 1 )
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler( void )
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while( 1 )
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler( void )
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while( 1 )
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler( void )
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler( void )
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler( void )
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler( void )
//{
//	
//}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles CAN2 receive Handler.
  * @param  None
  * @retval None
  */

void CAN2_RX0_IRQHandler( void )
{

    CanRxMsg rx_message;
    if( CAN_GetITStatus( CAN2, CAN_IT_FMP0 ) != RESET )
    {
        CAN_ClearITPendingBit( CAN2, CAN_IT_FF0 );
        CAN_Receive( CAN2, CAN_FIFO0, &rx_message );

		DispatchCANMessage(&rx_message); 
		
		SR73_Radar_MessageDispatch(&rx_message);
		
		SR28F_Radar_MessageDispatch(&rx_message);
    }
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
