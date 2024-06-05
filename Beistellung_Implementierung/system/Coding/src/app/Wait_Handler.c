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
#include "service/Button.h"

 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

void Wait_process(void) {

    int gCurrentEvent = 100;

do 
{

    if (3 == Button_getState(0))
    {
        gCurrentEvent = 0;
    }
   
    
    if (3 == Button_getState(1))
    { 
        gCurrentEvent = 1;
    }


    if (3 == Button_getState(2))
    {
        gCurrentEvent = 2;
    }

} while (100 == gCurrentEvent);

   
    return;

};
