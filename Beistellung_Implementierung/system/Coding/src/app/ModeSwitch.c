/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       ModeSwitch.c
 * 
 * This module contains the process of switching between the driving modes.
 * It works as an addition to the Wait process.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "app/ModeSwitch.h"
#include "app/StateHandler.h"
#include "app/Driving.h"

 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/
static DrivingMode gDrivingMode = BALANCED_MODE;

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

Events ModeSwitch_process(void) {

    gDrivingMode++;
    if (BALANCED_MODE < gDrivingMode)
    {
        /* Set back to high speed mode */
        gDrivingMode = HIGHSPEED_MODE;
    }

    switch (gDrivingMode)
    {
    case HIGHSPEED_MODE:
        Driving_setParameters(SPEED_FACTOR_HIGH_SPEED);
        break;

    case SAFETY_MODE:
        Driving_setParameters(SPEED_FACTOR_SAFETY);
        break;

    case BALANCED_MODE:
        Driving_setParameters(SPEED_FACTOR_BALANCED);
        break;

    default:
        break;
    }

    return EV_MODE_SWITCH_SUCCESSFUL;
}

DrivingMode ModeSwitch_getMode(void)
{
    return gDrivingMode;
}