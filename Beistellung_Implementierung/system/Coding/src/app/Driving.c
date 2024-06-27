/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Driving.c
 *
 * This module contains the driving process of the zumo robot. This includes mainly the following
 * of the line algorithm (PID). Moreover it provides methods to stop the robot and to drive forward.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "Driving.h"
#include "service/DriveControl.h"

 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/
/** Contain the process of regulating speed due to PID controller.
* @param error control error
* @param leftSpeed left motor speed
* @param rightSpeed right motor speed
*/
static void regulateSpeed(Int32 error, UInt16 * leftSpeed, UInt16 * rightSpeed);

/** Contain the process of calculating position due to a weighted average.
* @param sensorValues current sensor values of the line sensors
* @return current position of the robot
*/
static UInt32 calculatePosition(const LineSensorValues *sensorValues);

 /* VARIABLES **************************************************************************************/
static UInt32 gLastPostion = 0U;
static UInt32 gLastError = 0U;
const float gK_p = 0.25;
const UInt32 gK_d = 6;
static Float32 gSpeedFactor = SPEED_FACTOR_BALANCED;

 /* EXTERNAL FUNCTIONS *****************************************************************************/
void Driving_stopDriving(void)
{
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0, DRIVE_CONTROL_FORWARD);
}
void Driving_driveForward(void)
{
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, FORWARD_SPEED, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, FORWARD_SPEED, DRIVE_CONTROL_FORWARD);  
}

void Driving_followLine(LineSensorValues * SensorValues) 
{
    UInt32 position = calculatePosition(SensorValues);
    //UInt32 error = position - CENTER_OF_LINE_POSITION;
    UInt32 error = position - ((LINESENSOR_COUNT - 1) * SENSOR_WEIGHT_SCALE / 2);

    UInt16 leftSpeed;
    UInt16 rightSpeed;

    regulateSpeed(error, &leftSpeed, &rightSpeed);

    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, leftSpeed * gSpeedFactor, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, rightSpeed * gSpeedFactor, DRIVE_CONTROL_FORWARD);
}

void Driving_setParameters(Float32 speedFactor)
{
    gSpeedFactor = speedFactor;
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void regulateSpeed(Int32 error, UInt16 * leftSpeed, UInt16 * rightSpeed)
{
    /* PID controller */
    Int32 proportional = (Int32)((error * PROP_NUM) / PROP_DENOM);
    Int32 derivative   = (error - gLastError) * DERIV_NUM / DERIV_DENOM;
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

UInt32 calculatePosition(const LineSensorValues * sensorValues)
{
    UInt32 position = 0u;
    UInt32 sum = 0u;
    UInt32 weight = 0u;
    bool foundLine = false;

    for (UInt8 sensor = 0; sensor < LINESENSOR_COUNT; ++sensor)
    {
        UInt32 val = sensorValues->value[sensor];

        if (CALIB_OVER_LINE(val))
        {
            foundLine = true;
        }

        position += val * weight;
        sum += val;

        weight += SENSOR_WEIGHT_SCALE;
    }

    if (!foundLine)
    {
        position = gLastPostion;
    }
    else
    {
        /* build weighted average */
        position /= sum;
        gLastPostion = position;
    }

    return position;
}
