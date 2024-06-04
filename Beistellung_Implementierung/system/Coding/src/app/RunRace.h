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

#include "app/StateHandler.h"
#include "os/SoftTimer.h"
/* CONSTANTS **************************************************************************************/
#define MAX_LAP_TIME 20000
/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/*   Nur, damit es sich Compilieren lässt --> kommt wieder raus --> ist ja schon in StateHandler.h   */

/** contains the Race-Process
* @param void
* @param Events: report an event back to StateHandler
*/
Events RunRace_process(void);

#endif /* RUNRACE_HANDLER_H */
