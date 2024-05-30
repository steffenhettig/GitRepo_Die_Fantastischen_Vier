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
#include "RunRace_Handler.h"
#include "DriveControl.h"
#include "TickTimer.h"


 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

// Implementation of the RunRace_Process method
static void RunRace_Process(void) {
    
    //The TickTimerand DriveControl functions must be called up here / Variables Return parameters still need to be added

    printf("Running the race...\n");
}

// Create and initialize RunRace_Handler instance
RunRace_Handler RunRace_Handler_instance = {
    .RunRace_Process = RunRace_Process
};