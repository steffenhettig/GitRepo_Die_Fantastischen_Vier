/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       RACEDONE_HANDLER.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef RACEDONE_HANDLER_H
#define RACEDONE_HANDLER_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

// Definition of the RaceDone_Handler class
typedef struct {
    // Methoden
    void (*RaceDone_Process)(void);
} RaceDone_Handler;

/* PROTOTYPES *************************************************************************************/

// External declaration of the RaceDone_Handler instance
extern RaceDone_Handler RaceDone_Handler_instance;

#endif /* RACEDONE_HANDLER_H */