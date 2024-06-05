
/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       EndRace.h
 *
 * Module description comes here.
 */
/**************************************************************************************************/
#ifndef ENDRACE_HANDLER_H
#define ENDRACE_HANDLER_H

/* INCLUDES ***************************************************************************************/
#include "Common/Types.h"
#include "app/StartRace.h" 
#include "service/Led.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
typedef struct {
    Events (*EndRace_Process)(void);
} EndRace_Handler;

/* PROTOTYPES *************************************************************************************/

#endif /* ENDRACE_HANDLER_H */

// External declaration of the EndRace_Handler instance
extern EndRace_Handler EndRace_Handler_instance;



