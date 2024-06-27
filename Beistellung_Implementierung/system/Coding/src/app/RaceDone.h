/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       RaceDone.h
 *
 * This module contains the finishing process of the zumo robot. This includes the stopping of the
 * robot, the displaying of the elapsed time and the notification of the user.
 */
/**************************************************************************************************/
#ifndef RACEDONE_H
#define RACEDONE_H

/* INCLUDES ***************************************************************************************/
#include "Common/Types.h"
#include "app/StartRace.h" 
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** Contain the finishing process of the zumo robot.
 * 
 * @return Report the current event of the calibration process back to StateHandler.
 */
Events RaceDone_process(void);


#endif /* RACEDONE_H */


