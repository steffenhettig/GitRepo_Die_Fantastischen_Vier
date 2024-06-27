/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       RunRace.c
 *
 * This module contains the driving process of the zumo robot.
 * It monitors the lap time and triggers the redect track process if the track is lost.
 * Additionally it monitors if the endline is detected.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "RunRace.h"
#include "app/StateHandler.h"
#include "service/Display.h"
#include "app/StartRace.h"
#include "hal/TickTimer.h"
#include "app/Driving.h"
#include "service/DriveControl.h"

 /* CONSTANTS **************************************************************************************/
 #define TIME_MIN_US (500000LLU)
 #define TIME_MAX_TRACK_LOST (150000ULL)
 #define TRACK_LOST_ZERO (0ULL)

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/
static SoftTimer gTimer;
static UInt64 gTrackLost = TRACK_LOST_ZERO;
extern StartRace_Timemeasure gRaceTime;


 /* EXTERNAL FUNCTIONS *****************************************************************************/
Events RunRace_process(void) 
{
    Events retEvent = EV_NO_EVENT;

    LineSensorValues sensorValues;
    LineSensor_read(&sensorValues);
    Driving_followLine(&sensorValues);

    if (SOFTTIMER_IS_EXPIRED(&gTimer))
    {
        retEvent =  EV_LAPTIME_TIMEOUT;
    }
     /* if Endline detected */
    if (CALIB_OVER_LINE(sensorValues.value[LINESENSOR_LEFT]) && CALIB_OVER_LINE(sensorValues.value[LINESENSOR_RIGHT]) && (TIME_MIN_US < TickTimer_get() - gRaceTime.StartTime))
    {
        retEvent =  EV_STARTENDLINE_DETECTED;
    }
     /* if track lost */
    if (CALIB_NO_LINE(sensorValues.value[LINESENSOR_LEFT]) && CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE_LEFT]) && CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE]) && CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE_RIGHT]) && CALIB_NO_LINE(sensorValues.value[LINESENSOR_RIGHT]))
    {
        if (TRACK_LOST_ZERO == gTrackLost)
        {
            gTrackLost = TickTimer_get();
        }
        else if (TickTimer_get() - gTrackLost > TIME_MAX_TRACK_LOST)
        {
            gTrackLost = TRACK_LOST_ZERO;
            retEvent = EV_TRACK_LOST;
        }
    }
    else
    {
        gTrackLost = TRACK_LOST_ZERO;
    }

    return retEvent;
}

void RunRace_entry(void)
{
    Display_clear();
    SoftTimer_init(&gTimer);
    SoftTimerHandler_register(&gTimer);
    SoftTimer_start(&gTimer, MAX_LAP_TIME);
}

/* INTERNAL FUNCTIONS *****************************************************************************/
