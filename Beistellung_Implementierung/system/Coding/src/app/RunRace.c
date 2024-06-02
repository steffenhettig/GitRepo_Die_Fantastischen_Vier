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

/* --> Schleife fehlt mit den jeweiligen Events als Rückgabewerte */
static Events RunRace_process(void) 
{
    LineSensorValues SensorValues;
    /* Vernüpfung mit Hardware fehlt --> Werte einlesen */

    Events RetEvent = EV_NO_EVENT;

    LineSensor_read(&SensorValues);
    UInt32 position = calculatePosition(&SensorValues);
    UInt32 error = position - CENTER_OF_LINE_POSITION;

    UInt16 leftSpeed;
    UInt16 rightSpeed;

    regulateSpeed(error, &leftSpeed, &rightSpeed);

    /* Vernüpfung mit Hardware fehlt --> Ausgeben auf die Motoren */


    return RetEvent;
}


/* INTERNAL FUNCTIONS *****************************************************************************/

static void regulateSpeed(Int32 error, UInt16 * leftSpeed, UInt16 * rightSpeed)
{
    /* PID controller */
    Int32 proportional = (Int32)(error * gK_p);
    Int32 derivative   = (error - gLastError) * gK_d;
    Int32 integral     = 0;  /* not needed */
    Int32 speedDifference = proportional + derivative + integral;

    Int32 left = MAX_MOTOR_SPEED + speedDifference;
    Int32 right = MAX_MOTOR_SPEED - speedDifference;

    if (left < 0)
    {
        left = 0;
    }
    if (right < 0)
    {
        right = 0;
    }

    if (left > MAX_MOTOR_SPEED)
    {
        left = MAX_MOTOR_SPEED;
    }

    if (right > MAX_MOTOR_SPEED)
    {
        right = MAX_MOTOR_SPEED;
    }

    *leftSpeed = left;
    *rightSpeed = right;

    gLastError = error;
}

static UInt32 calculatePosition(const LineSensorValues *sensorValues)
{
    UInt32 position = 0u;
    UInt32 sum = 0u;
    UInt32 weight = 0u;
    bool foundLine = false;

    for (UInt8 sensor = 0; sensor < LINESENSOR_COUNT; ++sensor)
    {
        UInt32 val = sensorValues->value[sensor];

        if (CALIB_OVER_LINE(val))    /* <-- Was ist das für eine Funktion?? */
        {
            foundLine = true;
        }

        position += val * weight;
        sum += val;

        weight += SENSOR_WEIGHT_SCALE;
    }

    if (!foundLine)
    {
        position = glastPostion;
    }
    else
    {
        /* build weighted average */
        position /= sum;
        glastPostion = position;
    }

    return position;
}

// Create and initialize RunRace_Handler instance
//RunRace_Handler RunRace_Handler_instance = {
//    .RunRace_Process = RunRace_Process
//};