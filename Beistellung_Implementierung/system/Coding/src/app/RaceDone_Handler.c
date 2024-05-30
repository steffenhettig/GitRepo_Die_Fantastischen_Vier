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
#include "RaceDone_Handler.h"
#include "DriveControl.h"
#include "TickTimer.h"
#include "Buzzer.h"
#include "Display.h"

 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/


static void RaceDone_Process(void) {

    //The DriveControl, TickTimer, Buzzerand Display functions must be called up here / Variables Return parameters still need to be added
   
    printf("Finishing the race...\n");
}

// Create and initialize RaceDone_Handler instance
RaceDone_Handler RaceDone_Handler_instance = {
    .RaceDone_Process = RaceDone_Process
};