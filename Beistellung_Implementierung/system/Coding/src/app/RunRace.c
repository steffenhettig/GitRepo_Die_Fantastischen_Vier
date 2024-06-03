/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       Vorlage.c
 *
 * Module description comes here.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>
#include "RunRace.h"

// #include "app/StateHandler.h"
#include "service/LineSensor.h"
//#include "DriveControl.h"
//#include "TickTimer.h"


 /* CONSTANTS **************************************************************************************/
 #define TRESHOLD_LINE 500

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

static void regulateSpeed(Int32 error, UInt16 * leftSpeed, UInt16 * rightSpeed);
static UInt32 calculatePosition(const LineSensorValues *sensorValues);
static UInt32 glastPostion = 0U;
static UInt32 gLastError = 0U;
const float gK_p = 0.25;
const UInt32 gK_d = 6;

 /* EXTERNAL FUNCTIONS *****************************************************************************/




// Implementation of the RunRace_Process method
static Events RunRace_process(void) 
{
    Events retEvent = EV_NO_EVENT;

    while (retEvent == EV_NO_EVENT)
    {   
      LineSensorValues sensorValues;
      LineSensor_read(&sensorValues);
      Driving_followLine(&sensorValues);

      if (0) /* Timer Condition is missing, replace 0*/
      {

      }
      if ((TRESHOLD_LINE < sensorValues.value[LINESENSOR_LEFT]) && (TRESHOLD_LINE < sensorValues.value[LINESENSOR_RIGHT])) /* if Endline detected */
      {
        retEvent =  EV_STARTENDLINE_DETECTED;
      }
      if ((TRESHOLD_LINE >= sensorValues.value[LINESENSOR_LEFT]) && (TRESHOLD_LINE >= sensorValues.value[LINESENSOR_MIDDLE_LEFT]) && (TRESHOLD_LINE >= sensorValues.value[LINESENSOR_MIDDLE]) && (TRESHOLD_LINE >= sensorValues.value[LINESENSOR_MIDDLE_RIGHT]) && (TRESHOLD_LINE >= sensorValues.value[LINESENSOR_RIGHT]))
      {
        retEvent = EV_TRACK_LOST;
      }

    }
    return retEvent;
}


/* INTERNAL FUNCTIONS *****************************************************************************/