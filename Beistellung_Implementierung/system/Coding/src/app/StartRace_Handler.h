/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       STARTRACE_HANDLER.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef STARTRACE_HANDLER_H
#define STARTRACE_HANDLER_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

// Definition of the StartRace_Handler class
typedef struct {
    // Methoden
    void (*StartRace_Process)(void);
} StartRace_Handler;

/* PROTOTYPES *************************************************************************************/

// External declaration of the StartRace_Handler instance
extern StartRace_Handler StartRace_Handler_instance;

#endif /* VORLAGE_H */