/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       Error.c
 * 
 * This module contains the error handling process of the zumo robot. This includes the error entry 
 * and the error process.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "app/Error.h"
#include "app/ModeSwitch.h"
#include "app/StateHandler.h"
#include "service/Button.h"
#include "service/Buzzer.h"
#include "service/Display.h"
#include "service/DriveControl.h"

 /* CONSTANTS **************************************************************************************/
#define ERRORHANDLER_TEXTLENGTH (30)

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/


 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/
void Error_entry(Errors errorID)
{
    Display_clear();
    Buzzer_beep(BUZZER_ALARM);

    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0, DRIVE_CONTROL_FORWARD);

    Display_write("ERROR OCCURED        ", MAX_DISP_LENGTH);
    Display_gotoxy(0,5);

    switch (errorID)
    {
    case ER_REDETECT_LINE:
        Display_write("ERROR REDETECT LINE  ", MAX_DISP_LENGTH);
        break;


    case ER_STARTLINE_TIMER:
        Display_write("ERROR STARTLINE TIMER", MAX_DISP_LENGTH);
        break;


    case ER_RACETIME:
        Display_write("ERROR RACETIME       ", MAX_DISP_LENGTH);
        break;


    case ER_CALIBRATION:
        Display_write("ERROR CALIBRATION    ", MAX_DISP_LENGTH);
        break;


    case ER_PARAMETER:
        Display_write("ERROR PARAMETER      ", MAX_DISP_LENGTH);
        break;

    default:
       
        break;
    }
}


void Error_process(void)
{
    /* This method does actually nothing. */

    /* But since you're here... */

    /* Why don't programmers like dirty jokes? */




    /* Because they have too many uncontrolled variables! */
}
/* INTERNAL FUNCTIONS *****************************************************************************/