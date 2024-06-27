/***************************************************************************************************
  (c) FantaVier
***************************************************************************************************/
/**
 * @file       StateHandler.c
 *
 * This module contains the state machine handling of the zumo robot. This includes the different
 * states and events which can occur during the whole racing process.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "app/StateHandler.h"
#include "app/Error.h"
#include "app/Init.h"
#include "app/ModeSwitch.h"
#include "app/RaceDone.h"
#include "app/RedetectTrack.h"
#include "app/RunRace.h"
#include "app/StartRace.h"
#include "app/Wait.h"
#include "app/Calibration.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static States gCurrentState = ST_INIT;
static Events gCurrentEvent = EV_NO_EVENT;
static Errors gErrorID = ER_NO_ERROR;

/* EXTERNAL FUNCTIONS *****************************************************************************/
void StateHandler_process(void)
{
    Events retEv;

    switch (gCurrentState)
    { 
    case ST_INIT:
        Init_process();
        gCurrentState = ST_CALIBRATION;
        Calibration_entry();

        break;

    case ST_WAIT:
        gCurrentEvent = Wait_process();

        if (EV_PUSH_BUTTON_B_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_MODE_SWITCH;
        }
        if (EV_PUSH_BUTTON_A_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_START_RACE;
            StartRace_entry();
        }
        if (EV_PUSH_BUTTON_C_PRESSED == gCurrentEvent)
        {
            gCurrentState = ST_CALIBRATION;
            Calibration_entry();
        }
        break;

    case ST_MODE_SWITCH:
        retEv = ModeSwitch_process();
        if (EV_MODE_SWITCH_SUCCESSFUL == retEv)
        {
            gCurrentState = ST_WAIT;
        }
        if (EV_MODE_SWITCH_FAILED == retEv)
        {
            gCurrentState = ST_ERROR;
            gErrorID = ER_PARAMETER;
            Error_entry(gErrorID);
        }
        break;

    case ST_CALIBRATION:
        retEv = Calibration_process();
        if (EV_CALIBRATION_SUCCESSFUL == retEv)
        {
            gCurrentState = ST_WAIT;
        }
        if (EV_CALIBRATION_FAILED == retEv)
        {
            gCurrentState = ST_ERROR;
            gErrorID = ER_CALIBRATION;
            Error_entry(gErrorID);
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
            Error_entry(gErrorID);
            break;

        default:
            break;
        } 

        break;

    case ST_START_RACE:
        retEv = StartRace_process();
        if (EV_STARTENDLINE_DETECTED == retEv)
        {
            gCurrentState = ST_RUN_RACE;
            RunRace_entry();
        }
        if (EV_STARTENDLINE_DETECTED_TIMEOUT == retEv)
        {
            gCurrentState = ST_ERROR;
            gErrorID = ER_STARTLINE_TIMER;
            Error_entry(gErrorID);
        }
        break;

    case ST_RACE_DONE:
        RaceDone_process();
        gCurrentState = ST_WAIT;
        break;

    case ST_ERROR:
        Error_process();
        break;

    case ST_REDETECT_TRACK:
        retEv = RedetectTrack_process();
        if (EV_TRACK_REDETECTED == retEv)
        {
            gCurrentState = ST_RUN_RACE;
        }
        if (EV_TRACK_REDETECTED_TIMEOUT == retEv)
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
