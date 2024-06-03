/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       RUNRACE.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef RUNRACE_H
#define RUNRACE_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/*   Nur, damit es sich Compilieren lässt --> kommt wieder raus --> ist ja schon in StateHandler.h   */
typedef enum {
    EV_NO_EVENT,
    EV_CALIBRATION_SUCCESSFUL,
    EV_MODE_SWITCH_SUCCESSFUL,
    EV_STARTENDLINE_DETECTED,
    EV_TRACK_LOST,
    EV_TRACK_REDETECTED,
    EV_PUSH_BUTTON_A_PRESSED,
    EV_PUSH_BUTTON_B_PRESSED,
    EV_PUSH_BUTTON_C_PRESSED,
}Events;

// Definition of the RunRace_Handler class
/*
typedef struct {
    // Methoden
    void (*RunRace_Process)(void);
} RunRace_Handler;*/

/* PROTOTYPES *************************************************************************************/

// External declaration of the RunRace_Handler instance
//extern RunRace_Handler RunRace_Handler_instance;

#endif /* RUNRACE_HANDLER_H */
