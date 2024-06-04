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
#include "RedetectTrack.h"

// #include "app/StateHandler.h"
#include "service/LineSensor.h"
#include "os/SoftTimer.h"
//#include "DriveControl.h"
//#include "TickTimer.h"


 /* CONSTANTS **************************************************************************************/
 #define TRESHOLD_LINE 500

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

    while (EV_NO_EVENT == retEvent)
    {   
      LineSensorValues sensorValues;
      LineSensor_read(&sensorValues);

      Driving_driveForward();

      if (SOFTTIMER_IS_EXPIRED(&gTimer))
      {
        retEvent =  EV_REDETECT_TRACK_TIMEOUT;
      }
      if ((TRESHOLD_LINE < sensorValues.value[LINESENSOR_LEFT]) || (TRESHOLD_LINE < sensorValues.value[LINESENSOR_MIDDLE_LEFT]) || (TRESHOLD_LINE < sensorValues.value[LINESENSOR_MIDDLE]) || (TRESHOLD_LINE < sensorValues.value[LINESENSOR_MIDDLE_RIGHT]) || (TRESHOLD_LINE < sensorValues.value[LINESENSOR_RIGHT]))
      {
        retEvent = EV_TRACK_REDETECTED;
      }

    }
    return retEvent;
}


/* INTERNAL FUNCTIONS *****************************************************************************/
