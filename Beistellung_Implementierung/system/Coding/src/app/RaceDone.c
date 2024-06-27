/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       RaceDone.c
 *
 * This module contains the finishing process of the zumo robot. This includes the stopping of the
 * robot, the displaying of the elapsed time and the notification of the user.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/RaceDone.h"
#include "service/LineSensor.h"
#include "service/DriveControl.h"
#include "service/Display.h"
#include "hal/TickTimer.h"
#include "service/Buzzer.h"
#include "app/StateHandler.h"
#include "app/Driving.h"
/* CONSTANTS **************************************************************************************/
#define MS_DIVIDER (1000U) /**< Divider to convert the time from us to ms */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
extern StartRace_Timemeasure gRaceTime;

/* EXTERNAL FUNCTIONS *****************************************************************************/

/* INTERNAL FUNCTIONS *****************************************************************************/

Events RaceDone_process(void) 
{
    Events retVal = EV_STARTENDLINE_DETECTED;

    /* Stop all movements */
    Driving_stopDriving();

    /* Stop time measurement */
    gRaceTime.StopTime = TickTimer_get();

    /* Notify with a notification sound */
    Buzzer_beep(BUZZER_NOTIFY);

    /* Display the measured time on the display */
    UInt32 elapsedTime = (gRaceTime.StopTime - gRaceTime.StartTime) / MS_DIVIDER;
    char displayString[MAX_DISP_LENGTH] = {};
    snprintf(displayString, sizeof(displayString), "Time: %lu ms", elapsedTime);
    Display_gotoxy(0U, 0U);
    Display_write(displayString, sizeof(displayString));

    return retVal;   
}