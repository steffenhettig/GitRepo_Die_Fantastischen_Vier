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
#include "StartRace_Handler.h"
#include "DriveControl.h"
#include "TickTimer.h"
#include "Buzzer.h"
#include "Button.h"

 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/


// Implementation of the StartRace_Process method
static void StartRace_Process(void) {
    
    // The TickTimer, Buzzer, DriveControl and Button functions must be called up here / Variables Return parameters still need to be added

    printf("Starting the race...\n");
}

// Create and initialize StartRace_Handler instance
StartRace_Handler StartRace_Handler_instance = {
    .StartRace_Process = StartRace_Process
};