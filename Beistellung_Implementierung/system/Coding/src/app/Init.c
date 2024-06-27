/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Init.c
 *
 * This module initializes the system and displays the team name.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "app/Init.h"
#include "service/Display.h"
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
void Init_process(void)
{
    Display_clear();
    Display_gotoxy(0, 0);
    Display_write("Team: FantaVier      ", MAX_DISP_LENGTH);
}

/* INTERNAL FUNCTIONS *****************************************************************************/
