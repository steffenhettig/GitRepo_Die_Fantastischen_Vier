/***************************************************************************************************
/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       ModeSwitch.c
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

#include "app/ModeSwitch.h"
#include "app/StateHandler.h"
#include "service/Button.h"
#include "service/Buzzer.h"
#include "service/Display.h"



 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

typedef enum tag_ModeStatus
{
    NO_SELECTION = 0,    /**< No Selection   */
    HIGHSPEED_MODE,      /**< Highspeed Mode */
    SAFETY_MODE,         /**< Safety Mode    */
    BALANCED_MODE,       /**< Balanced Mode  */

} ModeStatus;

#define MODESWITCH_TEXTLENGTH 30
#define ONE_SECOND 1

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

 int gCounterVariableButton = 0;

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

Events ModeSwitch_process(void) {

    Display_clear();
    Display_write("Switch the Mode with Button B",MODESWITCH_TEXTLENGTH);
    

    sleep(ONE_SECOND);
    

    Display_write("You have 5 Sec after the Beep",MODESWITCH_TEXTLENGTH);
    

    Buzzer_beep(BUZZER_NOTIFY);


    time_t start_time, current_time;
    double elapsed_time;

    start_time = time(NULL); // Startzeit speichern

    while (TRUE) {
        current_time = time(NULL);
        elapsed_time = difftime(current_time, start_time);  /**< Determination of elapsed time */

        if (BUTTON_STATE_TRIGGERED == Button_getState(BUTTON_ID_B))
        {

        gCounterVariableButton ++;
        
        }

        if (elapsed_time >= 5)   /**< If 5 seconds have passed, end the loop */
        {                            
            break;
        }
      
        sleep(ONE_SECOND);       /**<Wait a second */ 
    }


switch (gCounterVariableButton)
    {
    case HIGHSPEED_MODE:
        Display_write("Highspeed Mode is selected",MODESWITCH_TEXTLENGTH);
        
        GlobalVelocity = 0;              
        gCounterVariableButton = 0;
        break;

    case SAFETY_MODE:
        Display_write("Safety Mode is selected",MODESWITCH_TEXTLENGTH);
        
        GlobalVelocity = -200;
        gCounterVariableButton = 0;
        break;

    case BALANCED_MODE:
        Display_write("Balanced Mode is selected",MODESWITCH_TEXTLENGTH);

        GlobalVelocity = -100;
        gCounterVariableButton = 0;
        break;

    default:
        Display_write("No mode is selected",MODESWITCH_TEXTLENGTH);
        GlobalVelocity = -400;
        gCounterVariableButton = 0;
        break;
    }


    if(-400 == GlobalVelocity)
    {
       return EV_MODE_SWITCH_FAILED;
    }
    else
    {
       return EV_MODE_SWITCH_SUCCESSFUL;
    }
    

};
