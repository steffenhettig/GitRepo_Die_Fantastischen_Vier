/***************************************************************************************************
/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       ErrorHandler.c
 * After setting the global variable gModeStatus to status code 0, the ModeSwitch runs 
 * through a loop in which the button states of buttons A, B, C are queried. 
 * If the status code changes, the loop is exited and the return to the StateHandler follows.
 * The global variable is queried in StartRace.c using a switch case and a different mode is executed accordingly.
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "app/ErrorHandler.h"

#include "app/ModeSwitch.h"
#include "app/StateHandler.h"
#include "service/Button.h"
#include "service/Buzzer.h"
#include "service/Display.h"
#include "service/DriveControl.h"



 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

#define ERRORHANDLER_TEXTLENGTH 30

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/



void Error_process(Errors errorID)
{

    Display_clear();

    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0, DRIVE_CONTROL_FORWARD);
 
    Buzzer_beep(BUZZER_ALARM);

    Display_write("ERROR HANDLER CALLED", ERRORHANDLER_TEXTLENGTH);


    switch (errorID)
    {
    case ER_REDETECT_LINE:
        Display_write("ERROR REDETECT LINE",ERRORHANDLER_TEXTLENGTH);
        Display_write("Restart Required",ERRORHANDLER_TEXTLENGTH);
        break;


    case ER_STARTLINE_TIMER:
        Display_write("ERROR STARTLINE TIMER",ERRORHANDLER_TEXTLENGTH);
        Display_write("Restart Required",ERRORHANDLER_TEXTLENGTH);
     
        break;


    case ER_RACETIME:
        Display_write("ERROR RACETIME",ERRORHANDLER_TEXTLENGTH);
        Display_write("Restart Required",ERRORHANDLER_TEXTLENGTH);
       
        break;


    case ER_CALIBRATION:
        Display_write("ERROR CALIBRATION",ERRORHANDLER_TEXTLENGTH);
        Display_write("Restart Required",ERRORHANDLER_TEXTLENGTH);
       
        break;


    case ER_PARAMETER:
        Display_write("ERROR PARAMETER",ERRORHANDLER_TEXTLENGTH);
        Display_write("Restart Required",ERRORHANDLER_TEXTLENGTH);
       
        break;


    default:
        Display_write("ERRORHANDLER CALLED NO DATA",ERRORHANDLER_TEXTLENGTH);
        Display_write("Restart Required",ERRORHANDLER_TEXTLENGTH);
       
        break;
    }
 

}
