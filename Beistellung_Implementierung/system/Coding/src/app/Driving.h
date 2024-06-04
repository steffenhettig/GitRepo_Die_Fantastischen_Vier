/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       Driving.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef DRIVING_H
#define DRIVING_H

/* INCLUDES ***************************************************************************************/
#include "service/LineSensor.h"


/* CONSTANTS **************************************************************************************/
#define SENSOR_WEIGHT_SCALE 1000
#define CENTER_OF_LINE_POSITION 2000
#define MAX_MOTOR_SPEED 400U
#define CALIB_OVER_LINE(value) ((value) > 500)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** stops driving
* @param void
*/
void Driving_stopDriving(void);

/** drives directly forward with max speed
* @param void
*/
void Driving_driveForward(void);

/** contains the line following algorithm
* @param void
*/
void Driving_followLine(LineSensorValues * SensorValues);

/* PROTOTYPES *************************************************************************************/

#endif /* DRIVING_H */
