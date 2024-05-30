/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/PID/PIDExample.c $
***************************************************************************************************/
/**
@addtogroup App
@{
@file       PIDExample.c

Line Follower algorithm.


@version    %$Id: PIDExample.c 285 2024-01-12 12:27:51Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/
 
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/** Scaling factor */
#define SENSOR_WEIGHT_SCALE (1000u)

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/**
 * Determine position of line under sensor.
 *
 * This function uses a weighted average to compute a logical line position. The value moves from
 * 0 to 4000 if the line moves from left to right of the robot.
 * https://en.wikipedia.org/wiki/Weighted_arithmetic_mean
 *
 *     0 * sensor1 + 1000 * sensor1 + 2000 * sensor2 + ... + 4000 * sensor4
 *     ----------------------------------------------------------------
 *                      sum(sensor0, ... , sensor4)
 *
 * @param sensorValues the current sensor values.
 * @return logical position between 0 and 4000.
 */
static UInt32 calculatePosition(const LineSensorValues *sensorValues);

/**
 * Controller loop to calculate motor speeds
 * 
 * @param[in]  posError  position error
 * @param[out] leftSpeed New speed for left motor
 * @param[out] rightSpeed New speed for right motor
 */
static void regulateSpeed(Int32 posError, UInt16 * leftSpeed, UInt16 * rightSpeed);

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/* INTERNAL FUNCTIONS *****************************************************************************/

static void regulateSpeed(Int32 error, UInt16 * leftSpeed, UInt16 * rightSpeed)
{
    /* PID controller */
    Int32 proportional = (error * gParam->proportional.numerator) / gParam->proportional.denominator;
    Int32 derivative   = ((error - gLastError) * gParam->derivative.numerator) / gParam->derivative.denominator;
    Int32 integral     = 0;  /* not needed */
    Int32 speedDifference = proportional + derivative + integral;

    Int32 left = gParam->maxMotorSpeed + speedDifference;
    Int32 right = gParam->maxMotorSpeed - speedDifference;

    if (left < 0)
    {
        left = 0;
    }
    if (right < 0)
    {
        right = 0;
    }

    if (left > gParam->maxMotorSpeed)
    {
        left = gParam->maxMotorSpeed;
    }
    if (right > gParam->maxMotorSpeed)
    {
        right = gParam->maxMotorSpeed;
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