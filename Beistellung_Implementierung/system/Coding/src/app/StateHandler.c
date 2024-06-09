/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       StateHandler.c
 *
 * Module description comes here.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "app/StateHandler.h"

#include "app/Calibration.h"
//#include "app/Error.h"
//#include "app/Init.h"
//#include "app/ModeSwitch.h"
//#include "app/RaceDone.h"
#include "app/RedetectTrack.h"
#include "app/RunRace.h"
#include "app/StartRace.h"
//#include "app/Wait.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
States gCurrentState;
Events gCurrentEvent;
Errors gErrorID;

/* EXTERNAL FUNCTIONS *****************************************************************************/


void StateHandler_process(void)
{
    switch (gCurrentState)
    { 
    case ST_INIT: 
        Init_process(); //---------
        gCurrentState = ST_CALIBRATION;
        break;

    case ST_WAIT:
        Wait_process(); //---------
        if (EV_PUSH_BUTTON_B_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_MODE_SWITCH;
        }
        if (EV_PUSH_BUTTON_A_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_START_RACE;
        }
        if (EV_PUSH_BUTTON_C_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_CALIBRATION;
        }
        break;

    case ST_MODE_SWITCH:
        if (EV_MODE_SWITCH_SUCCESSFUL == ModeSwitch_process())//---------hier Rückgabewerte: mode_switch_successful
        {
            gCurrentState = ST_WAIT;
        }
        if (EV_MODE_SWITCH_FAILED == ModeSwitch_process())
        {
            gCurrentState = ST_ERROR;
            gErrorID = ER_PARAMETER;
        }
        break;

    case ST_CALIBRATION:
        if (EV_CALIBRATION_SUCCESSFUL == Calibration_process())//---------hier Rückgabewerte: calibration_successful
        {
            gCurrentState = ST_WAIT;
        }
        if (EV_CALIBRATION_FAILED == Calibration_process())
        {
            gCurrentState = ST_ERROR;
            gErrorID = ER_CALIBRATION;
        }
        break;

    case ST_RUN_RACE: 
        switch (RunRace_process())
        {
            case EV_STARTENDLINE_DETECTED:
                gCurrentState = ST_RACE_DONE;
                break;

            case EV_TRACK_LOST:
                gCurrentState = ST_REDETECT_TRACK;
                break;

            case EV_LAPTIME_TIMEOUT:
                gCurrentState = ST_ERROR;
                gErrorID = ER_RACETIME;
                break;

            default:
                break;
        }     
        break;

    case ST_START_RACE:
        if (EV_STARTENDLINE_DETECTED == StartRace_process())//---------hier Rückgabewerte: EV_STARTLINE_DETECTED
        {
            gCurrentState = ST_RUN_RACE;
        }
        if (EV_STARTENDLINE_DETECTED_TIMEOUT == StartRace_process())
        {
            gCurrentState = ST_ERROR;
            gErrorID = ER_STARTLINE_TIMER;
        }
        break;

    case ST_RACE_DONE:
        RaceDone_process();//---------
      break;

    case ST_ERROR:
        Error_process(gErrorID);//---------
        break;

    case ST_REDETECT_TRACK:
        if(EV_TRACK_REDETECTED == RedetectTrack_process())//---------hier Rückgabewerte: EV_TRACK_REDETECTED
        {
            gCurrentState = ST_RUN_RACE;
        }
        if(EV_TRACK_REDETECTED_TIMEOUT == RedetectTrack_process())
        {
            gCurrentState = ST_ERROR;
            gErrorID = ER_REDETECT_LINE;
        }
        break;

    default:
        break;
  }
}

/* INTERNAL FUNCTIONS *****************************************************************************/
