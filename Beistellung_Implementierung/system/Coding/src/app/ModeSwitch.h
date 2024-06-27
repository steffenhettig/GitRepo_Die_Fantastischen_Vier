/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       ModeSwitch.h
 *
 * This module contains the process of switching between the driving modes.
 * It works as an addition to the Wait process.
 */
 /**************************************************************************************************/

#ifndef MODESWITCH_H
#define MODESWITCH_H

/* INCLUDES ***************************************************************************************/
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/
/* VARIABLES **************************************************************************************/
/* MACROS *****************************************************************************************/
/* TYPES ******************************************************************************************/
typedef enum
{
    NO_SELECTION = 0,    /**< No Selection   */
    HIGHSPEED_MODE,      /**< Highspeed Mode */
    SAFETY_MODE,         /**< Safety Mode    */
    BALANCED_MODE,       /**< Balanced Mode  */
} DrivingMode;
/* PROTOTYPES *************************************************************************************/

/** Set the driving mode of the zumo robot.
* @return Report the current event of the calibration process back to StateHandler.
*/
Events ModeSwitch_process(void);

/** Get the current driving mode of the zumo robot.
* @return Report the current driving mode.
*/
DrivingMode ModeSwitch_getMode(void);

#endif /* MODESWITCH_H */
