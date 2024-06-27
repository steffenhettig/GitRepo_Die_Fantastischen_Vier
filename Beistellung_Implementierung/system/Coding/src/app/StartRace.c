/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file    StartRace.c
 *
 * This module contains the starting process of the zumo robot. This includes the waiting time of 3s
 * and the driving forward until the startline is detected.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/StartRace.h"
#include "common/Types.h"
#include "service/LineSensor.h"
#include "service/DriveControl.h"
#include "os/SoftTimer.h"
#include "hal/TickTimer.h"
#include "service/Buzzer.h"
#include "app/StateHandler.h"
#include "app/Driving.h"
#include "app/Calibration.h"

/* CONSTANTS **************************************************************************************/
#define STARTTIME (3000U) /**< Time to wait before starting */
#define STARTLINETIME (2U)
#define START_RACE_SPEED (20U)
#define START_RACE_OVER_LINE_THRESHOLD (180U)
#define START_RACE_NO_LINE_THRESHOLD (90U)
 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/
StartRace_Timemeasure gRaceTime;
static SoftTimer gStartTimer;

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/


/**
 * Implementation of the StartRace_Process method
*/
Events StartRace_process(void) 
{
    static Bool driveForwardFlag = false;

    LineSensorValues sensorValues;
    Events retVal = EV_NO_EVENT;

    /* wait for 3s */
    if ((!driveForwardFlag) && SOFTTIMER_IS_EXPIRED(&gStartTimer))
    {
        driveForwardFlag = true;
        SoftTimer_Stop(&gStartTimer);

        if (SOFTTIMER_RET_SUCCESS != SoftTimerHandler_unRegister(&gStartTimer))
        {
            retVal = EV_STARTENDLINE_DETECTED_TIMEOUT;
        }
    }

    /* Start driving forward */
    if (driveForwardFlag)
    {
        LineSensor_read(&sensorValues);
        Driving_followLine(&sensorValues);

        /* Checks if startline is detected */
        if (CALIB_OVER_LINE(sensorValues.value[LINESENSOR_LEFT]) && CALIB_OVER_LINE(sensorValues.value[LINESENSOR_RIGHT])) /* if Startline detected */
        {
            driveForwardFlag = false;

            /* Notify User with buzzer */
            Buzzer_beep(BUZZER_NOTIFY);

            /* Set return Value. */
            retVal = EV_STARTENDLINE_DETECTED;

            /* Start lap time measurement. */
            gRaceTime.StartTime = TickTimer_get();
        }
    }

    return retVal;
}

void StartRace_entry(void)
{
    SoftTimer_init(&gStartTimer);
    SoftTimerHandler_register(&gStartTimer);
    SoftTimer_start(&gStartTimer, STARTTIME);
    LineSensor_enableEmitter();
}