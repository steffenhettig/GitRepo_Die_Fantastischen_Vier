/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Calibration.h
 *
 * Calibrates the robot's line sensor
 */
/**************************************************************************************************/
#ifndef CALIBRATION_H
#define CALIBRATION_H

/* INCLUDES ***************************************************************************************/
#include "Common/Types.h"
#include "os/ErrorHandler.h"
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/
#define CALIB_TIMEOUT_MS 6000u /**< Calibration Timeout time in milliseconds used to handle lost of track or staing to long in one state of the internal statemachine*/
#define CALIB_OVER_LINE_THRESHOLD 180U
#define CALIB_NO_LINE_THRESHOLD 90U
#define CALIB_BETWEEN_LINE_UP_THRESHOLD 400U
#define CALIB_BETWEEN_LINE_DOWN_THRESHOLD 100U
#define CALIBRATION_SPEED_CALIBRATION 18U  

/* MACROS *****************************************************************************************/


/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/**
 * This function performs the calibration process (do function of State) and manages any errors that 
 * may occur during the calibration by updating the error handler message and return the EV_ERROR Event.
 *
 * @param[out] currentErrorHandlerMsg A pointer to an ErrorHandlerErrorCode 
 *                                       that will be updated with any error 
 *                                       codes encountered during the calibration process.
 *
 * @return FsmRobotEvent The event that the FSM (Finite State Machine) should 
 *                        handle next, based on the internal results of the Calibrate State.
 */

Events CalibrationXXX_process(void);

/** This function will be used to initalize the internal static variables*/
void CalibrateState_initVariables(void);

/** This will update the current selected parameterset pointer*/
//void CalibrateState_updateParameters(void);

#endif /* CALIBRATION_H */
