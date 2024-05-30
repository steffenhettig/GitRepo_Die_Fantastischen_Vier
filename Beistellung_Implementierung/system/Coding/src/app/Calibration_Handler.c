/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @addtogroup app
 * @{
 * @file       Calibration_Handler.c
 *
 * Functions to calibrate the 5 Line sensors on the Pololu Zumo. For a detailed discription look at Calibration_Handler.h
 *
 * @version    %$Id: Calibration_Handler.c Buehler
 * @}
 */
 /**************************************************************************************************/

 /* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "app/Calibration_Handler.h"
#include "service/Button.h"
#include "service/LineSensor.h"
#include "os/SoftTimer.h"
#include "service/DriveControl.h"
#include "service/Display.h"


 /* CONSTANTS **************************************************************************************/

 /* MACROS *****************************************************************************************/

 /* TYPES ******************************************************************************************/

/** Textlength used for to display the calibration state*/
#define CALIBRATION_TEXTLENGTH 15

 /* PROTOTYPES *************************************************************************************/

 /* VARIABLES **************************************************************************************/

/** All Calibration States*/
typedef enum CALIBRATION_HANDLER_CALIBRATION_STATE
{
    CALIBRATION_STATE_INIT,                         /**<State Calibration init */
    CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR, /**<State Calibrating the left sensors */
    CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR, /**<State Calibrating the right sensors */
    CALIBRATION_STATE_CENTER_ON_LINE,               /**<State Recentering the system */
    CALIBRATION_STATE_CHECKING,                    /**<State Checking if every sensor is calibrated and the system is centerd on the line*/
    CALIBRATION_STATE_CALIBRATION_FINISHED,         /**<State Calibration done sucessfully */
    CALIBRATION_STATE_CALIBRATION_TIMEOUT           /**<State Calibration Timeout */
} CalibrationState;

static CalibrationState gState; /**< current Calibration State*/

/** timer used by calibration steps for Timeout controll */
static SoftTimer gTimer; 

 /* EXTERNAL FUNCTIONS *****************************************************************************/

 /* INTERNAL FUNCTIONS *****************************************************************************/

/**
* @brief Calibrates the Sensors
* 
* Function called by the Calibration handler. 
* This function contains the logic and the implementation for the diferent calibration states
*
*/
static void Calibration_Process(void) 
{
    LineSensorValues SensorValues;
    SoftTimer_Ret TimerReturn;

    switch (gState)
    {
        case CALIBRATION_STATE_INIT:
        
            /** Clear Display and write current Calibration progress*/
            Display_clear();/**< Maybe needs to get removed and put in the first transition to calib*/
            Display_write("Calibration process 1 of 6 ", CALIBRATION_TEXTLENGTH);


            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                SoftTimer_start(&gTimer, CALIBRATION_MAX_CALIBRATIONTIME);
                LineSensor_startCalibration();

                //Update display
                Display_clearLine();
                Display_write("Calibration process 2 of 6 ", CALIBRATION_TEXTLENGTH);
            }
     
            break;

        case CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR:

            
        
            //Turns unitil the left sensor is over the line and calibrated
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_BACKWARD);
        
            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_CALIBRATION_TIMEOUT;
                
            }

            LineSensor_read(&SensorValues);
            if ((TRUE == SensorValues.calibrated[LINESENSOR_LEFT]) && (CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_LEFT])))
            {
                //Left sensor is calibrated and over the Line
                gState = CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR;
                SoftTimer_restart(&gTimer);

                Display_clearLine();
                Display_write("Calibration process 3 of 6 ", CALIBRATION_TEXTLENGTH);
            }
            break;

        case CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_BACKWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_FORWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_CALIBRATION_TIMEOUT;
                
            }
           
            LineSensor_read(&SensorValues);
            if ((TRUE == SensorValues.calibrated[LINESENSOR_RIGHT]) && (CALIBRATION_HANDLER_LINE_DETECTED(SensorValues.value[LINESENSOR_RIGHT])))
            {
                gState = CALIBRATION_STATE_CENTER_ON_LINE;
                SoftTimer_restart(&gTimer);

                Display_clearLine();
                Display_write("Calibration process 4 of 6 ", CALIBRATION_TEXTLENGTH);
            }

            break;

        case CALIBRATION_STATE_CENTER_ON_LINE:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIBRATION_SPEED_CALIBRATION, DRIVE_CONTROL_BACKWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_CALIBRATION_TIMEOUT; 
            }

            LineSensor_read(&SensorValues);

            /* stop and next state if robot is centered*/
            if (CALIBRATION_HANDLER_NO_LINE_DETECTED(SensorValues.value[LINESENSOR_LEFT])
                && NO_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE_LEFT])
                && LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE])
                && NO_LINE_DETECTED(SensorValues.value[LINESENSOR_MIDDLE_RIGHT])
                && NO_LINE_DETECTED(SensorValues.value[LINESENSOR_RIGHT]))
            {
                DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0U, DRIVE_CONTROL_FORWARD);
                DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT,0U, DRIVE_CONTROL_FORWARD);
                gState = CALIBRATION_STATE_CHECKING;

                Display_clearLine();
                Display_write("Calibration process 5 of 6 ", CALIBRATION_TEXTLENGTH);
            }
            break;

        case CALIBRATION_STATE_CHECKING:


            if (!LineSensor_getCalibrationState)
            {
                /*stop softtimer and check if it was succesfull*/
                if (SOFTTIMER_RET_SUCCESS != SoftTimer_Stop(&gTimer))
                {
                    //Softtimer couldn't be stoped
                    //Jump in Error State
                    
                }


                gState = CALIBRATION_STATE_CALIBRATION_FINISHED;

                Display_clearLine();
                Display_write("Calibration finished", CALIBRATION_TEXTLENGTH);
        
            }
            else
            {
                /*restart calibration if one sensor is not calibrated*/
                gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                SoftTimer_restart(&gTimer);


                Display_clearLine();
                Display_write("Calibration progress 2 of 6", CALIBRATION_TEXTLENGTH);
            }
            break;

        case CALIBRATION_STATE_CALIBRATION_FINISHED:

            /* unregister softtimer and check if it was sucessfull*/
            if (SOFTTIMER_RET_SUCCESS != SoftTimerHandler_unRegister(&gTimer))
            {
                //Softtimer couldn't be stoped
                //Jump in error State
            }


            //Enter State Wait
            break;

        case CALIBRATION_STATE_CALIBRATION_TIMEOUT:
            // Enter State Error
            break;
    }
}

/** Create and initialize Calibration_Handler instance */
Calibration_Handler Calibration_Handler_instance = {
    .Calibration_Process = Calibration_Process
};
