/**
 * @file module_beep.c
 * @updata_note
 */

#include "module_beep.h"
#include "beep.h"

/**
* @brief    Open the beep
*
* @param[in] none
*
* @return    none
*
*/
void OpenBeep( void )
{
    Beep_ON;
}
/**
* @brief    Close the beep
*
* @param[in] none
*
* @return    none
*
*/
void CloseBeep( void )
{
    Beep_OFF;
}

/**
* @brief    Toggle the beep
*
* @param[in] none
*
* @return    none
*
*/
void ToggleBeep( void )
{
    Beep_T;
}

