/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Error.c
 * 
 * This module contains the error handling process of the zumo robot. This includes the error entry 
 * and the error process.
 */
 /**************************************************************************************************/
#ifndef ERROR_H
#define ERROR_H

/* INCLUDES ***************************************************************************************/
#include "Common/Types.h"
#include "app/Calibration.h"
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/
        
/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** Contain the entry of the error handling.
* @param Errors: receives the error ID
*/

void Error_entry(Errors errorID);
/** Contain the error handling of the zumo.
* @param Errors: receives the error ID
*/
void Error_process(void);

#endif /* ERROR_H */
