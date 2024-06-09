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

/** contains the calibration handling of the zumo
* @param void
* @param Events: report an event back to StateHandler
*/
Events Calibration_process(void);

/** contains the parameter change algorithm of the zumo
* @param void
* @param Events: report an event back to StateHandler
*/
Events ModeSwitch_process(void);

/** contains the start handling of the zumo
* @param void
* @param Events: report an event back to StateHandler
*/
Events StartRace_process(void);

/** contains the Race-Process
* @param void
* @param Events: report an event back to StateHandler
*/
Events RunRace_process(void);

/** redetect the track when lost (gap handling)
* @param void
* @param Events: report an event back to StateHandler
*/
Events RedetectTrack_process(void);

/** calls the init methods of the used modules
* @param void
*/
void Init_process(void);

/** contains the waiting process (state: ready/ idle)
* @param void
*/
void Wait_process(void);

/** contains the end race handling of the zumo
* @param void
*/
void RaceDone_process(void);

/** contains the error handling of the zumo
* @param void
* @param Errors: receives the error ID
*/
void Error_process(Errors errorID);

/* PROTOTYPES *************************************************************************************/

#endif /* STATE_HANDLER_H */
