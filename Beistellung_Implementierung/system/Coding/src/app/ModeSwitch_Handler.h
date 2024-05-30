/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       MODESWITCH_HANDLER.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef MODESWITCH_HANDLER_H
#define VMODESWITCH_HANDLER_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

// Definition of the ModeSwitch_Handler class
typedef struct {
    // Methoden
    void (*ModeSwitch_SwitchToNextParameterSet)(void);
} ModeSwitch_Handler;

/* PROTOTYPES *************************************************************************************/

// External declaration of the ModeSwitch_Handler instance
extern ModeSwitch_Handler ModeSwitch_Handler_instance;

#endif /* MODESWITCH_HANDLER_H */