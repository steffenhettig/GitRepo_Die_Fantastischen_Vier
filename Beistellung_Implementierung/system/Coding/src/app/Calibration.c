/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Calibration.c
 *
 * This module calibrates the line sensors. Starting with the right sensor, the robot turns left 
 * until the left sensor sees the line. Continuing with the left sensor, the robot turns right.
 * And finally the robot centers on the line.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include <util/delay.h>

#include "app/Calibration.h"
#include "Common/Types.h"
#include "app/Driving.h"
#include "os/SoftTimer.h"
#include "service/DriveControl.h"
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
#define CALIBRATION_SPEED_CALIBRATION (30U) /**< Calibration speed for turning from right to left and left to right */
#define CALIBRATION_SPEED_CALIBRATION2 (25U) /**< Calibration speed for centering on the line */
#define CALIB_TIME (5000U)
#define DELAY_TIME (200)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
typedef enum tag_CalibrationState
{
    CALIBRATION_STATE_INIT,                         /**< Initial calibration state.           */
    CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR, /**< State TURN_RIGHT_UNTIL_LEFT_SENSOR.  */
    CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR, /**< State TURN_LEFT_UNTIL_RIGHT_SENSOR.  */
    CALIBRATION_STATE_CENTER_ON_LINE,               /**< State CENTER_ON_LINE.                */
    CALIBRATION_STATE_FINISHED,                     /**< State FINISHED.                      */
    CALIBRATION_STATE_TIMEOUT                       /**< State TIMEOUT.                       */
} CalibrationState;

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/** Calibration state of local state machine */
static CalibrationState gState = CALIBRATION_STATE_INIT;

/** Timer used by calibration steps. */
static SoftTimer gTimer;

/** current ErrorHandlerErrorCode of the calibration state.*/
static Errors gErrorID;

/** Current FSMRobotEvent for the finite state machine of the robot. */
static Events gRetEvent;

/* EXTERNAL FUNCTIONS *****************************************************************************/
Events Calibration_process(void)
{
    gRetEvent = EV_NO_EVENT;
    LineSensorValues sensorValues;

    switch (gState)
    {
    case CALIBRATION_STATE_INIT:
        if (SOFTTIMER_IS_EXPIRED(&gTimer))
        {
            gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
            SoftTimer_start(&gTimer, CALIB_TIME);
            LineSensor_startCalibration();
        }
        break;

    case CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR:
        DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_FORWARD);
        DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_BACKWARD);
        if (SOFTTIMER_IS_EXPIRED(&gTimer))
        {
            gState = CALIBRATION_STATE_TIMEOUT;
        }

        LineSensor_read(&sensorValues);

        if (sensorValues.calibrated[LINESENSOR_LEFT] && (CALIB_OVER_LINE(sensorValues.value[LINESENSOR_LEFT])))
        {
            _delay_ms(DELAY_TIME);
            SoftTimer_restart(&gTimer);
            gState = CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR;
        }
        break;

    case CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR:
        DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_BACKWARD);
        DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_FORWARD);

        if (SOFTTIMER_IS_EXPIRED(&gTimer))
        {
            gState = CALIBRATION_STATE_TIMEOUT;
        }

        LineSensor_read(&sensorValues);

        if (sensorValues.calibrated[LINESENSOR_RIGHT] && (CALIB_OVER_LINE(sensorValues.value[LINESENSOR_RIGHT])))
        {
            if (!LineSensor_getCalibrationState())
            {
                /* restart sequence, some sensors not yet calibrated. */
                gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
            }
            else
            {
                _delay_ms(DELAY_TIME);
                SoftTimer_restart(&gTimer);
                gState = CALIBRATION_STATE_CENTER_ON_LINE;
            }
        }
        break;

    case CALIBRATION_STATE_CENTER_ON_LINE:
        DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIBRATION_SPEED_CALIBRATION2, DRIVE_CONTROL_FORWARD);
        DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIBRATION_SPEED_CALIBRATION2, DRIVE_CONTROL_BACKWARD);

        if (SOFTTIMER_IS_EXPIRED(&gTimer))
        {
            gState = CALIBRATION_STATE_TIMEOUT;
        }

        LineSensor_read(&sensorValues);

        /* stop if only middle sensor sees a line */
        if ((CALIB_NO_LINE(sensorValues.value[LINESENSOR_LEFT])) 
        && (CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE_LEFT]))
        && (CALIB_OVER_LINE(sensorValues.value[LINESENSOR_MIDDLE]))
        && (CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE_RIGHT]))
        && (CALIB_NO_LINE(sensorValues.value[LINESENSOR_RIGHT])))
        {
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0u, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0u, DRIVE_CONTROL_BACKWARD);

            SoftTimer_Stop(&gTimer);
            if (SOFTTIMER_RET_SUCCESS != SoftTimerHandler_unRegister(&gTimer))
            {
                gErrorID = ER_CALIBRATION;
                gState = CALIBRATION_STATE_TIMEOUT;
                gRetEvent = EV_CALIBRATION_FAILED;
            }
            gState = CALIBRATION_STATE_FINISHED;
        }
        break;

    case CALIBRATION_STATE_TIMEOUT:
        DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0u, DRIVE_CONTROL_FORWARD);   /* Set speed of left motor to zero. This stops the motor. */
        DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0u, DRIVE_CONTROL_BACKWARD); /* Set speed of right motor to zero. This stops the motor. */
        LineSensor_stopCalibration();
        gErrorID = ER_CALIBRATION;
        gRetEvent = EV_CALIBRATION_FAILED;
        break;

    case CALIBRATION_STATE_FINISHED:
        LineSensor_stopCalibration();
        gErrorID = ER_NO_ERROR;
        gRetEvent = EV_CALIBRATION_SUCCESSFUL;
        gState = CALIBRATION_STATE_INIT;

        break;

    default:
        break;
    }
    return gRetEvent;
}
void Calibration_entry(void)
{
    gState = CALIBRATION_STATE_INIT;
        SoftTimer_init(&gTimer);
        gErrorID = 0;
        gRetEvent = EV_NO_EVENT;
        SoftTimerHandler_register(&gTimer);
        SoftTimer_start(&gTimer, 0U);
}

/* INTERNAL FUNCTIONS *****************************************************************************/