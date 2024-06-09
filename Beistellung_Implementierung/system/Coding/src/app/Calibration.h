/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @addtogroup app
 * @{
 * @file       CALIBRATION.h
 *
 * The Calibration_Handler handles the calibration process of the 5 Line Sensors.
 * If the process is started the system has to be placed over the Line
 * The System will start a sweeping motion over the Line until all Sensors are calibrated
 *
 *
 * @version    %$Id: Calibration.h Buehler
 * @}
 */
 /**************************************************************************************************/
#ifndef CALIBRATION_H
#define CALIBRATION_H

/* INCLUDES ***************************************************************************************/
#include "app/StateHandler.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/** Checks if a Line is detected under the given sensor*/
#define CALIBRATION_HANDLER_LINE_DETECTED(SensorValue) \
    (SensorValue>=CALIBRATION_LINE_DETECTED_LIMIT)

/** Checks if no is Line detected under the given sensor */
#define CALIBRATION_HANDLER_NO_LINE_DETECTED(SensorValue) \
    (SensorValue<=CALIBRATION_NO_LINE_DETECTED_LIMIT)


/* TYPES ******************************************************************************************/

//Those values may be changed with the parameter sets
#define CALIBRATION_MAX_CALIBRATIONTIME 5000U   /**< Maximum of time for every calibration step */
#define CALIBRATION_SPEED_CALIBRATION 33U       /**< Motorspeed during the calibrationprocess in % */
#define CALIBRATION_SPEED_CENTERING 25U         /**< Motorspeed while centering in % */
#define CALIBRATION_NO_LINE_DETECTED_LIMIT 25U  /**< Max confidenc a line is under the sensor to determ no line is detected  */
#define CALIBRATION_LINE_DETECTED_LIMIT 75U     /**< Min confidence a line is under the sensor */

/** Definition of the Calibration Handler class */
typedef struct {

    // Methoden
    Events (*Calibration_Process)(void);

} Calibration_Handler;


/* PROTOTYPES *************************************************************************************/

/** Instance to start the calibration process */
extern Calibration_Handler Calibration_Handler_instance;

#endif /* CALIBRATION_HANDLER_H */
