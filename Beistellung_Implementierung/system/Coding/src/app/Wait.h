/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Wait.h
 *
 * This module contains the waiting process bevor start. 
 * With button A you can start the race.
 * With button B you can switch between the driving modes.
 * With button C you can start the calibration process.
 */
 /**************************************************************************************************/
#ifndef WAIT_H
#define WAIT_H

/* INCLUDES ***************************************************************************************/
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** Contain the waiting process (state: ready/ idle).
* @return Report the current event of the calibration process back to StateHandler.
*/
Events Wait_process(void);

#endif /* WAIT_H */
