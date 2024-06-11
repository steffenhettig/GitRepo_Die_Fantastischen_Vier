
/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       RaceDone.c
 *
 * Module description comes here.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/RaceDone.h"
#include "service/LineSensor.h"
#include "service/DriveControl.h"
#include "hal/TickTimer.h"
#include "service/Buzzer.h"
#include "service/Led.h"
#include "app/StateHandler.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
extern StartRace_Timemeassure gRaceTime;
/* INTERNAL FUNCTIONS *****************************************************************************/

/**
 * Implementation of the RaceDone_Process method
 */
static Events RaceDone_process(void) 
{
    LineSensorValues SensorValues;
    Events ReturnValue;
    Bool StopLineDetected = false;

    // Read the sensor values to check for stop line
    LineSensor_read(&SensorValues);

    /* Checks if stop line is detected
    * Stop line is detected if the middle sensor and the sensor on the outside are detecting a line
    * The sensors in between shall not see a line!
    * 
    * Stopline:  -|- 
    * notice the gap between those two lines 
    */
    if (CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_LEFT])
                  && CALIBRATION_HANDLER_NO_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE_LEFT])
                  && CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE])
                  && CALIBRATION_HANDLER_NO_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE_RIGHT])
                  && CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_RIGHT]))
    {
        StopLineDetected = true;
    }

    if (StopLineDetected)
    {
        // Stop all movements
        DriveControl_stop(DRIVE_CONTROL_MOTOR_LEFT);
        DriveControl_stop(DRIVE_CONTROL_MOTOR_RIGHT);

        // Stop time measurement
        gRaceTime.StopTime = TickTimer_get();

        // Notify with a notification sound
        Buzzer_beep(BUZZER_NOTIFY);

        // Display the measured time on the OLED
        UInt64 elapsedTime = gRaceTime.StopTime - gRaceTime.StartTime;
        char displayString[32];
        snprintf(displayString, sizeof(displayString), "Time: %llu ms", elapsedTime);
        Led_displayString(displayString);

        // Set return value
        ReturnValue = EV_STARTENDLINE_DETECTED;
    }
    else
    {
        // No stop line detected, return no event
        ReturnValue = EV_NO_EVENT;
    }

    return ReturnValue;   
}
