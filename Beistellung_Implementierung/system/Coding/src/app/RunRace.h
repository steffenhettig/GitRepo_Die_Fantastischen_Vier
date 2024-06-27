/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       RunRace.h
 *
 * This module contains the driving process of the zumo robot.
 * It monitors the lap time and triggers the redect track process if the track is lost.
 * Additionally it monitors if the endline is detected.
 */
/**************************************************************************************************/
#ifndef RUNRACE_H
#define RUNRACE_H

/* INCLUDES ***************************************************************************************/
#include "service/LineSensor.h"
#include "app/StateHandler.h"
#include "os/SoftTimer.h"
/* CONSTANTS **************************************************************************************/
#define MAX_LAP_TIME (20000) /**< maximum lap time (20 seconds) */
/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Contain the race process including monitoring lap time, endline detection and triggering a
 *  redetect track process if the track is lost.
 * 
 * @return Report the current event of the calibration process back to StateHandler.
 */
Events RunRace_process(void);

/** Contain the entry method of the RunRace module.
 * 
 */
void RunRace_entry(void);

#endif /* RUNRACE_H */
