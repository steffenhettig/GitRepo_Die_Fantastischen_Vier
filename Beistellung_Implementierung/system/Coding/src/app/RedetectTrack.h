/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       RedetectTrack.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef REDETECTTRACK_H
#define REDETECTTRACK_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/*   Nur, damit es sich Compilieren lässt --> kommt wieder raus --> ist ja schon in StateHandler.h   */
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

/** Redetects the track when lost
* @param void
*/
Events RedetectTrack_process(void);

/* PROTOTYPES *************************************************************************************/

#endif /* RUNRACE_HANDLER_H */
