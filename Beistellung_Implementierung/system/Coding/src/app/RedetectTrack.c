/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       RedetectTrack.c
 *
 * Module description comes here.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/RedetectTrack.h"
#include "service/LineSensor.h"
#include "os/SoftTimer.h"


 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/


 /* EXTERNAL FUNCTIONS *****************************************************************************/
Events RedetectTrack_process(void) 
{
    Events retEvent = EV_NO_EVENT;
    static SoftTimer gTimer;
    SoftTimer_start(&gTimer, MAX_REDETECT_TRACK_TIME);
  
    LineSensorValues sensorValues;
    LineSensor_read(&sensorValues);

    Driving_driveForward();

    if (SOFTTIMER_IS_EXPIRED(&gTimer))
    {
      retEvent =  EV_REDETECT_TRACK_TIMEOUT;
    }
    if (CALIB_OVER_LINE(sensorValues.value[LINESENSOR_LEFT]) || CALIB_OVER_LINE(sensorValues.value[LINESENSOR_MIDDLE_LEFT]) || CALIB_OVER_LINE(sensorValues.value[LINESENSOR_MIDDLE]) || CALIB_OVER_LINE(sensorValues.value[LINESENSOR_MIDDLE_RIGHT]) || CALIB_OVER_LINE(sensorValues.value[LINESENSOR_RIGHT]))
    {
      retEvent = EV_TRACK_REDETECTED;
    }

    return retEvent;
}


/* INTERNAL FUNCTIONS *****************************************************************************/
