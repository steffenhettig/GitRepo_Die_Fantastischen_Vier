/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       Vorlage.c
 *
 * Module description comes here.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>
#include "Calibration_Handler.h"
#include "Button.h"
#include "LineSensor.h"


 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

// Implementation of the Calibration_Process method
static void Calibration_Process(void) {

    //The functions for LineSensorand Button must be called up here / Variables Return parameters still need to be added
   
    printf("Performing calibration process...\n");
}

// Create and initialize Calibration_Handler instance
Calibration_Handler Calibration_Handler_instance = {
    .Calibration_Process = Calibration_Process
};
