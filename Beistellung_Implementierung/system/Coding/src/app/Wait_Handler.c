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
#include "Wait_Handler.h"
#include "Button.h"


 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

// Implementation of the Wait_NotifyStateHandler method
static void Wait_NotifyStateHandler(void) {

    //The Button function must be called up here / Variables Return parameters still need to be added

    printf("Notifying Scheduler from Wait_Handler...\n");

}

// Create and initialize Wait_Handler instance
Wait_Handler Wait_Handler_instance = {
    .Wait_NotifyStateHandler = Wait_NotifyStateHandler
};