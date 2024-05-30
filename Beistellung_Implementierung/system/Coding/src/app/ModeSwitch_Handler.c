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
#include "ModeSwitch_Handler.h"
#include "Button.h"

 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

// Implementation of the ModeSwitch_SwitchToNextParameterSet method
static void ModeSwitch_SwitchToNextParameterSet(void) {

    //The Button function must be called up here / Variables Return parameters still need to be added

    printf("Switching to the next parameter set...\n");
}

// Create and initialize ModeSwitch_Handler instance
ModeSwitch_Handler ModeSwitch_Handler_instance = {
    .ModeSwitch_SwitchToNextParameterSet = ModeSwitch_SwitchToNextParameterSet
};