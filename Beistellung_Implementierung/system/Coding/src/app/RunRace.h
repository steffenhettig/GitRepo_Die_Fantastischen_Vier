/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       RUNRACE.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef RUNRACE_H
#define RUNRACE_H

/* INCLUDES ***************************************************************************************/
#include "service/LineSensor.h"
#include "os/SoftTimer.h"
/* CONSTANTS **************************************************************************************/
#define MAX_LAP_TIME 20000
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
    EV_LAPTIME_TIMEOUT,
    EV_PUSH_BUTTON_A_PRESSED,
    EV_PUSH_BUTTON_B_PRESSED,
    EV_PUSH_BUTTON_C_PRESSED,
}Events;

/** contains the Race-Process
* @param void
* @param Events: report an event back to StateHandler
*/
Events RunRace_process(void);

#endif /* RUNRACE_HANDLER_H */
