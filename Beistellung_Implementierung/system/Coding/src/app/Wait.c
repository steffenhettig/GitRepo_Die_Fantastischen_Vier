/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Wait.c
 *
 * This module contains the waiting process bevor start. 
 * With button A you can start the race.
 * With button B you can switch between the driving modes.
 * With button C you can start the calibration process.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/Wait.h"
#include "app/StateHandler.h"
#include "app/ModeSwitch.h"
#include "service/Button.h"
#include "service/Display.h"


 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

Events Wait_process(void) 
{
    Events retEv = EV_NO_EVENT;
    DrivingMode retDrMod = ModeSwitch_getMode();
    
    Display_gotoxy(0, 3);

    switch (retDrMod)
    {
    case HIGHSPEED_MODE:
        Display_clearLine();
        Display_write("Highspeed Mode       ", MAX_DISP_LENGTH);
        break;

    case SAFETY_MODE:
        Display_clearLine();
        Display_write("Safety Mode          ", MAX_DISP_LENGTH);
        break;

    case BALANCED_MODE:
        Display_clearLine();
        Display_write("Balanced Mode        ", MAX_DISP_LENGTH);
        break;

    default:
        
        break;
    }

    if (BUTTON_STATE_PRESSED == Button_getState(BUTTON_ID_A))
    {
        retEv = EV_PUSH_BUTTON_A_PRESSED;
    }
   
    
    if (BUTTON_STATE_PRESSED == Button_getState(BUTTON_ID_B))
    { 
        retEv = EV_PUSH_BUTTON_B_PRESSED;
    }


    if (BUTTON_STATE_PRESSED == Button_getState(BUTTON_ID_C))
    {
        retEv = EV_PUSH_BUTTON_C_PRESSED;
    }

   
    return retEv;
}
