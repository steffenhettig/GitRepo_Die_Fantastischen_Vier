/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Calibration/CalibrationExample.c $
***************************************************************************************************/
/**
@addtogroup App
@{
@file       
@version    %$Id: CalibrationExample.c 286 2024-01-12 12:32:10Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/
#define CALIB_SPEED 33u       /**< Motor speed while calibrating */
#define CALIB_SPEED_SLOW 25u  /**< Motor speed while centering on line  */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/** Calibration state. */
typedef enum tag_CalibrationState
{
    CALIBRATION_STATE_INIT,                             /**< Initial calibration state.           */
    CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR,     /**< State TURN_RIGHT_UNTIL_LEFT_SENSOR.  */
    CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR,     /**< State TURN_LEFT_UNTIL_RIGHT_SENSOR.  */
    CALIBRATION_STATE_CENTER_ON_LINE,                   /**< State CENTER_ON_LINE.                */
    CALIBRATION_STATE_FINISHED,                         /**< State FINISHED.                      */
    CALIBRATION_STATE_TIMEOUT                           /**< State TIMEOUT.                       */
} CalibrationState;

/* PROTOTYPES *************************************************************************************/

/** Reaction to processCycle event.
 * 
 * Handles internal state machine processing.
 *
 * @param[in] pState Pointer to context state.
 */
static void onProcessCycleEvent (StateHandlerStatePtr pState);

/* VARIABLES **************************************************************************************/

/** Calibration state of local state machine */
static CalibrationState gState;

/** Timer used by calibration steps. */
static SoftTimer gTimer;

/* EXTERNAL FUNCTIONS *****************************************************************************/

/* INTERNAL FUNCTIONS *****************************************************************************/

static void onProcessCycleEvent(StateHandlerStatePtr pState)
{
    LineSensorValues values;

    switch (gState)
    {
        case CALIBRATION_STATE_INIT:
            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                SoftTimer_start(&gTimer, 5000u);
                LineSensor_startCalibration();
            }
            break;

        case CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIB_SPEED, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIB_SPEED, DRIVE_CONTROL_BACKWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TIMEOUT;
            }

            LineSensor_read(&values);
            if (true == (values.calibrated[LINESENSOR_LEFT] && CALIB_OVER_LINE(values.value[LINESENSOR_LEFT])))
            {
                SoftTimer_restart(&gTimer);
                gState = CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR;
            }
            break;

        case CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIB_SPEED, DRIVE_CONTROL_BACKWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIB_SPEED, DRIVE_CONTROL_FORWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TIMEOUT;
            }

            LineSensor_read(&values);
            if (true == (values.calibrated[LINESENSOR_RIGHT] &&  CALIB_OVER_LINE(values.value[LINESENSOR_RIGHT])))
            {
                if (!LineSensor_getCalibrationState())
                {
                    /* restart sequence, some sensors not yet calibrated. */
                    gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                }
                else 
                {
                    SoftTimer_restart(&gTimer);
                    gState = CALIBRATION_STATE_CENTER_ON_LINE;
                }
            }
            break;

        case CALIBRATION_STATE_CENTER_ON_LINE:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIB_SPEED_SLOW, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIB_SPEED_SLOW, DRIVE_CONTROL_BACKWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TIMEOUT;
            }

            LineSensor_read(&values);

            /* stop if only middle sensor sees a line */

            if (CALIB_NO_LINE(values.value[LINESENSOR_LEFT]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_MIDDLE_LEFT]) &&
                CALIB_OVER_LINE(values.value[LINESENSOR_MIDDLE]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_MIDDLE_RIGHT]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_RIGHT]))
            {
                DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0u, DRIVE_CONTROL_FORWARD);
                DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0u, DRIVE_CONTROL_BACKWARD);

                SoftTimer_Stop(&gTimer);
                if (SOFTTIMER_RET_SUCCESS != SoftTimerHandler_unRegister(&gTimer))
                {
                    ErrorState_enter(pState, ERRORHANDLER_CALIBRATE_TIMER_UNINIT_FAIL);
                }

                gState = CALIBRATION_STATE_FINISHED;
            }
            break;

        case CALIBRATION_STATE_TIMEOUT:

            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0u, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0u, DRIVE_CONTROL_BACKWARD);
            LineSensor_stopCalibration();

            ErrorState_enter(pState, ERRORHANDLER_CALIBRATE_TIMEOUT);
            break;

        case CALIBRATION_STATE_FINISHED:
            LineSensor_stopCalibration();
            SoftTimerHandler_unRegister(&gTimer);

            ReadyState_enter(pState);
            break;
    }
}

