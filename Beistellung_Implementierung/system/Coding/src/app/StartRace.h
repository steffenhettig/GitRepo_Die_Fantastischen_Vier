/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file    StartRace.h
 *
 * This module contains the starting process of the zumo robot. This includes the waiting time of 3s
 * and the driving forward until the startline is detected.
 */
/**************************************************************************************************/
#ifndef STARTRACE_H
#define STARTRACE_H

/* INCLUDES ***************************************************************************************/
#include "Common/Types.h"
#include "app/Calibration.h"
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/
#define START_SPEED 33U /**< Motorspeed while Searching for the Startline in % */
/* MACROS *****************************************************************************************/

        
/* TYPES ******************************************************************************************/
/* Contains the start and the Stoptime of the Tick Timer */
typedef struct StartRace_Start_and_Stoptime
{
  UInt64 StartTime; /**< is set by "StartRace.C"*/
  UInt64 StopTime;  /**< is set by "EndRace.C"*/
} StartRace_Timemeasure;

/* PROTOTYPES *************************************************************************************/
/** Contain the starting process of the zumo.
* @return Report the current event of the calibration process back to StateHandler.
*/
Events StartRace_process(void);

/** Contain the entry method of the StartRace module.
 * 
 */
void StartRace_entry(void);

#endif /* STARTRACE_H */
