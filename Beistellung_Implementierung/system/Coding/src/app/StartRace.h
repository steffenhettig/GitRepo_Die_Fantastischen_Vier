/**
@addtogroup app
@{
@file       StartRace_Handler.h

Handles the complete start Race process

On Entry it shall wait 3s
then the system will start driving until it reaches the start line

if the starline is detected it'll jump to the run Race state and start time messurement

if the startline is not detected it'll jump in the error state

@version    %$Id: StartRace_Handler.h Buehler
* @}
 /**************************************************************************************************/
#ifndef STARTRACE_HANDLER_H
#define STARTRACE_HANDLER_H

/* INCLUDES ***************************************************************************************/
#include "Common/Types.h"
#include "app/Calibration.h"
#include "app/StateHandler.h"

/* CONSTANTS **************************************************************************************/
#define START_SPEED 33U /**< Motorspeed while Searching for the Startline in % */
/* MACROS *****************************************************************************************/

        
/* TYPES ******************************************************************************************/

/**
 * Contains the start and the Stoptime of the Tick Timer
 */
typedef struct StartRace_Start_and_Stoptime
{

  UInt64 StartTime; /**< is set by "StarRace.C"*/
  UInt64 StopTime;  /**< is set by "EndRace.C"*/

} StartRace_Timemeassure;

// Definition of the StartRace_Handler class
typedef struct {
    // Methoden
    Events (*StartRace_Process)(void);
} StartRace_Handler;

/* PROTOTYPES *************************************************************************************/

// External declaration of the StartRace_Handler instance
extern StartRace_Handler StartRace_Handler_instance;

#endif /* VORLAGE_H */
