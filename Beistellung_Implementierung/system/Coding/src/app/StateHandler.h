/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       StateHandler.h
 *
 * This module contains the state machine handling of the zumo robot. This includes the different
 * states and events which can occur during the whole racing process.
 */
/**************************************************************************************************/
#ifndef STATEHANDLER_H
#define STATEHANDLER_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/
#define MAX_DISP_LENGTH (22U)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
typedef enum {
    ST_INIT,
    ST_WAIT,
    ST_MODE_SWITCH,
    ST_CALIBRATION,
    ST_RUN_RACE,
    ST_START_RACE,
    ST_RACE_DONE,
    ST_ERROR,
    ST_REDETECT_TRACK,
}States;

typedef enum {
    EV_NO_EVENT,
    EV_CALIBRATION_SUCCESSFUL,
    EV_CALIBRATION_FAILED,
    EV_MODE_SWITCH_SUCCESSFUL,
    EV_MODE_SWITCH_FAILED,
    EV_STARTENDLINE_DETECTED,
    EV_STARTENDLINE_DETECTED_TIMEOUT,
    EV_TRACK_LOST,
    EV_TRACK_REDETECTED,
    EV_TRACK_REDETECTED_TIMEOUT,
    EV_LAPTIME_TIMEOUT,
    EV_REDETECT_TRACK_TIMEOUT,
    EV_PUSH_BUTTON_A_PRESSED,
    EV_PUSH_BUTTON_B_PRESSED,
    EV_PUSH_BUTTON_C_PRESSED,
}Events;

typedef enum {
    ER_NO_ERROR,
    ER_PARAMETER,
    ER_STARTLINE_TIMER,
    ER_RACETIME,
    ER_REDETECT_LINE,
    ER_CALIBRATION,
}Errors;

/* PROTOTYPES *************************************************************************************/
/** Contain the state machine handling of the zumo robot.
* @param void
*/
void StateHandler_process(void);

#endif /* STATEHANDLER_H */
