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

#include "app/StateHandler.h"
#include "app/Driving.h"
/* CONSTANTS **************************************************************************************/
#define MAX_REDETECT_TRACK_TIME 5000
/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/


/** Redetects the track when lost
* @param void
*/
Events RedetectTrack_process(void);

/* PROTOTYPES *************************************************************************************/

#endif /* RUNRACE_HANDLER_H */