/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Calibration.h
 *
 * This module calibrates the line sensors. Starting with the right sensor, the robot turns left 
 * until the left sensor sees the line. Continuing with the left sensor, the robot turns right.
 * And finally the robot centers on the line.
 */
/**************************************************************************************************/
#ifndef CALIBRATION_H
#define CALIBRATION_H

/* INCLUDES ***************************************************************************************/
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/


/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/**
 * This function contains the calibration process.
 * 
 * @return Report the current event of the calibration process back to StateHandler.
 */
Events Calibration_process(void);

/** Contain the entry method of the Calibration module.
 * 
 */
void Calibration_entry(void);

#endif /* CALIBRATION_H */