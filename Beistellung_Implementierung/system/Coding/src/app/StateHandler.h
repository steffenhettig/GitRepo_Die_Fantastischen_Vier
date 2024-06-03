/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       StateHandler.h
 *
 * Module description comes here.
 */
/**************************************************************************************************/
#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

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
    EV_MODE_SWITCH_SUCCESSFUL,
    EV_STARTENDLINE_DETECTED,
    EV_TRACK_LOST,
    EV_TRACK_REDETECTED,
    EV_PUSH_BUTTON_A_PRESSED,
    EV_PUSH_BUTTON_B_PRESSED,
    EV_PUSH_BUTTON_C_PRESSED,
}Events;

typedef enum {
    ER_PARAMETER,
    ER_STARTLINE_TIMER,
    ER_RACETIME,
    ER_REDETECT_LINE,
    ER_CALIBRATION,
}Errors;

/** contains the state machine handling of the zumo
* @param void
*/
void StateHandler_process(void);

/* PROTOTYPES *************************************************************************************/

#endif /* STATE_HANDLER_H */
