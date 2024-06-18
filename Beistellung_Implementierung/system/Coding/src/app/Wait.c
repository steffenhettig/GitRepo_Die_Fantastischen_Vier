/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       Wait.c
 *
 * After setting the global event variable gCurrent to status code 100, the WaitHandler runs 
 * through a loop in which the button states of buttons A, B, C are queried. 
 * If the status code changes, the loop is exited and the return to the StateHandler follows.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/Wait.h"
#include "app/StateHandler.h"
#include "service/Button.h"


 /* CONSTANTS **************************************************************************************/

#define STATUSCODE 100

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

//    int gCurrentEvent = 100;  /**<Global variable from StateHandler with current StatusCode 100*/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

Events Wait_process(void) {

    Events retEv = EV_NO_EVENT;

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
