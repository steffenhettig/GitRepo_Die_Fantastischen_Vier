/**
@addtogroup app
@{
@file       StartRace_Handler.c

Handles the complete start Race process

On Entry it shall wait 3s
then the system will start driving until it reaches the start line

if the starline is detected it'll jump to the run Race state and start time messurement

if the startline is not detected it'll jump in the error state

@version    %$Id: StartRace_Handler.c Buehler
* @}
 *************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>
#include "app/StartRace_Handler.h"
#include "service/LineSensor.h"
#include "service/DriveControl.h"
#include "os/SoftTimer.h"
#include "hal/TickTimer.h"
#include "service/Buzzer.h"
#include "service/Button.h"
#include "app/StateHandler.h"
#include "app/Driving.h"

 /* CONSTANTS **************************************************************************************/
#define STARTTIME 3u
#define STARTLINETIME 3u

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/
StartRace_Timemeassure gRaceTime;
 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/


/**
 * Implementation of the StartRace_Process method
*/
static Events StartRace_Process(void) 
{

  SoftTimer StartTimer;
  SoftTimer StartLineTimer;
  LineSensorValues SensorValues;
  Events ReturnValue;
  Bool ErrorOccured = false;
  Bool StartLineDetected = true;
  
  //**********Entry: Wait 3s*****************
  //Start Timer 
  if (SOFTTIMER_IS_EXPIRED(&StartTimer))
  {
      SoftTimer_start(&StartTimer, STARTTIME);   
  }
  
  while (!SOFTTIMER_IS_EXPIRED(&StartTimer))
  {
    //do nothing 
    //wait 3s
  }

  //Unregister Timer
  if (SOFTTIMER_RET_SUCCESS != SoftTimerHandler_unRegister(&StartTimer))
  {
    //Jump in Error
    ErrorOccured = true;
  }

  //*************Start driving and search for starline*************************************

  //Start Starlinetimer
  if (SOFTTIMER_IS_EXPIRED(&StartLineTimer))
  {
    SoftTimer_start(&StartLineTimer, STARTLINETIME);  
  }
  
  //Start driving

  //Driving_driveForward()

  DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, START_SPEED, DRIVE_CONTROL_FORWARD);
  DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, START_SPEED, DRIVE_CONTROL_FORWARD);

  //Drive until the startline is detected or the timer is expired
  while (!ErrorOccured && !StartLineDetected)
  {

    //Checks if timer is expired
    if (SOFTTIMER_IS_EXPIRED(&StartLineTimer))
    {
      //Timer is expired
      ErrorOccured = true;
    }

    LineSensor_read(&SensorValues);

    /*Checks if startline is detected
    * Startline is detected, if the middle sensor and the sensor on the ouside are detecting a line
    * The sensors in between shall not see a line!
    * 
    * Starline:  -|- 
    * notice the gap between those two lines 
    */
    if (CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_LEFT])
                  && CALIBRATION_HANDLER_NO_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE_LEFT])
                  && CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE])
                  && CALIBRATION_HANDLER_NO_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE_RIGHT])
                  && CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_RIGHT]))
    {

      StartLineDetected = true;

    }


  }

/***************Startline is detected or timer is expired***********************************/

  //Stop Startlinetimer and checks for succes
  if (SOFTTIMER_RET_SUCCESS != SoftTimer_Stop(&StartLineTimer))
  {
    //Jump in Error State
    ErrorOccured = true;
  }
      
  //Free StartLineTimer
  if (SOFTTIMER_RET_SUCCESS != SoftTimerHandler_unRegister(&StartLineTimer))
  {
    //Jump in Error
    ErrorOccured = true;
  }


/************************EXIT of Start Race State******************************************************
 * There are to different outputs based on the Varibales ErrorOccured and StartLineDetected 
 * 
 * If ErrorOccured=ture and/Or StarlineDetected=false the programm will jump into the error state
 * 
 * if StartLineDetected = true AND ErrorOccured = false the program will start the exit logic:
 * -Play Buzzer
 * -Start Timer for time meassurement
*/

  if (StartLineDetected && !ErrorOccured)
  {

    //Notify User with buzzer
    Buzzer_beep(BUZZER_NOTIFY);

    //Set return Vallue
    ReturnValue = EV_STARTENDLINE_DETECTED;

    /*Start Timemessure
    * Time messurement is started done by saving the current vallue of the tick Counter
    * At the end of the race 
    * 
    */
    gRaceTime.StartTime = TickTimer_get();
    


  }
  else
  {
    // to Error State
    ReturnValue = EV_NO_EVENT;
  }


  return ReturnValue;   
}


// Create and initialize StartRace_Handler instance
StartRace_Handler StartRace_Handler_instance = {
    .StartRace_Process = StartRace_Process
};
