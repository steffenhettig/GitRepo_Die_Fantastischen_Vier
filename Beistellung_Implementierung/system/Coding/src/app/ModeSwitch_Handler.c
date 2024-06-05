/***************************************************************************************************
/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       ModeSwitch.c
 * After setting the global variable gModeStatus to status code 0, the ModeSwitch runs 
 * through a loop in which the button states of buttons A, B, C are queried. 
 * If the status code changes, the loop is exited and the return to the StateHandler follows.
 * The global variable is queried in StartRace.c using a switch case and a different mode is executed accordingly.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/ModeSwitch.h"
#include "service/Button.h"

 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/
int gModeStatus = 0;
 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

void ModeSwitch_process(void) {

do 
{

    if (3 == Button_getState(0))
    {
        gModeStatus = 1;
    }
   
    
    if (3 == Button_getState(1))
    { 
        gModeStatus = 2;
    }


    if (3 == Button_getState(2))
    {
        gModeStatus = 3;
    }

} while (0 == gModeStatus);
   
    return;
};
