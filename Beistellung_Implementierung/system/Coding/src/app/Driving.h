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
#ifndef DRIVING_H
#define DRIVING_H

/* INCLUDES ***************************************************************************************/
#include "Types.h"
#include "service/LineSensor.h"


/* CONSTANTS **************************************************************************************/
#define SENSOR_WEIGHT_SCALE (1000) /**< Weight scale for weighted average */
#define CENTER_OF_LINE_POSITION (2000)
#define MAX_MOTOR_SPEED (70U)
#define FORWARD_SPEED (30U)
/* PID parameters */
#define PROP_DENOM (30)
#define PROP_NUM (3)
#define DERIV_DENOM (1000)
#define DERIV_NUM (0)

/* Speed factor depending on the desired mode */
#define SPEED_FACTOR_HIGH_SPEED (1.2)
#define SPEED_FACTOR_SAFETY (0.8)
#define SPEED_FACTOR_BALANCED (1.0)

/* MACROS *****************************************************************************************/
#define CALIB_NO_LINE(value)   ((value) < 500)
#define CALIB_OVER_LINE(value) ((value) > 700)

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** Stop immediately the driving of the robot.
*/
void Driving_stopDriving(void);

/** Drive directly forward (not following the line).
*/
void Driving_driveForward(void);

/** Contain the line following algorithm.
*/
void Driving_followLine(LineSensorValues * SensorValues);

void Driving_setParameters(Float32 speedFactor);

#endif /* DRIVING_H */
