/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       CALIBRATION_HANDLER.h
 *
 * Module description comes here.
 */
 /**************************************************************************************************/
#ifndef CALIBRATION_HANDLER_H
#define CALIBRATION_HANDLER_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

// Definition of the Calibration_Handler class
typedef struct {
    // Methoden
    void (*Calibration_Process)(void);
} Calibration_Handler;

/* PROTOTYPES *************************************************************************************/

// External declaration of the Calibration_Handler instance
extern Calibration_Handler Calibration_Handler_instance;

#endif /* CALIBRATION_HANDLER_H */