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


/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
States gCurrentState;
Events gCurrentEvent;
Errors gErrorID;

/* EXTERNAL FUNCTIONS *****************************************************************************/
Events Calibration_process(void);
Events ModeSwitch_process(void);
Events StartRace_process(void);
Events RunRace_process(void);
Events RedetectTrack_process(void);
void Init_process(void);
void Wait_process(void);
void RaceDone_process(void);
void Error_process(void);

/* INTERNAL FUNCTIONS *****************************************************************************/


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
        if(EV_PUSH_BUTTON_B_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_MODE_SWITCH;
        }
        if(EV_PUSH_BUTTON_A_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_START_RACE;
        }
        if(EV_PUSH_BUTTON_C_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_CALIBRATION;
        }
        break;

    case ST_MODE_SWITCH:
        if(EV_MODE_SWITCH_SUCCESSFUL == ModeSwitch_process())//---------hier Rückgabewerte: mode_switch_successful
        {
            gCurrentState = ST_WAIT;
        }
        else
        {
            gCurrentState = ST_ERROR;
        }
        break;

    case ST_CALIBRATION:
        if(EV_CALIBRATION_SUCCESSFUL == Calibration_process())//---------hier Rückgabewerte: calibration_successful
        {
            gCurrentState = ST_WAIT;
        }
        else
        {
            gCurrentState = ST_ERROR;
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

            default:
                gCurrentState = ST_ERROR;
                break;
        }     
        break;

    case ST_START_RACE:
        if(EV_STARTENDLINE_DETECTED == StartRace_process())//---------hier Rückgabewerte: EV_STARTLINE_DETECTED
        {
            gCurrentState = ST_RUN_RACE;
        }
        else
        {
            gCurrentState = ST_ERROR;
        }
        break;

    case ST_RACE_DONE:
        RaceDone_process();//---------
      break;

    case ST_ERROR:
        Error_process();//---------
        break;

    case ST_REDETECT_TRACK:
        if(EV_TRACK_REDETECTED == RedetectTrack_process())//---------hier Rückgabewerte: EV_TRACK_REDETECTED
        {
            gCurrentState = ST_RUN_RACE;
        }
        else
        {
            gCurrentState = ST_ERROR;
        }
        break;

    default:
        break;
  }
}
