/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/LineSensor.h $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       LineSensor.h

Functions to control the 5 Line sensors on the Pololu Zumo.


@version    %$Id: LineSensor.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef LINESENSOR_H
#define LINESENSOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/** Maximum sensor read delay. */
#define LINESENSOR_READ_TIMEOUT_US    (2000u)

/** Number of sensors. */
#define LINESENSOR_COUNT            (5u)

#define LINESENSOR_LEFT             (0u)   /**< leftmost sensor */
#define LINESENSOR_MIDDLE_LEFT      (1u)   /**< middle left sensor */
#define LINESENSOR_MIDDLE           (2u)   /**< middle sensor */
#define LINESENSOR_MIDDLE_RIGHT     (3u)   /**< middle right sensor */
#define LINESENSOR_RIGHT            (4u)   /**< rightmost sensor */

/** Normalized value range. */
#define LINESENSOR_NORMALIZED_RANGE   (1000u)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/**
 * Type holding line sensor values.
 */
typedef struct tag_LineSensorValues
{
    UInt16 value[LINESENSOR_COUNT];      /*< Line detect confidence in percent.   */
    bool   calibrated[LINESENSOR_COUNT]; /*< indicate if result is calibrated.    */
} LineSensorValues;

/* PROTOTYPES *************************************************************************************/

/**
 * Initialize module.
 */
extern void LineSensor_init (void);

/** Start calibrating the LineSensors.
*/
extern void LineSensor_startCalibration (void);

/** Stop calibrating the LineSensors.
*/
extern void LineSensor_stopCalibration (void);

/* Get status of calibrations.
 * @return  bool status.
 * @retval  true
 *              All sensors calibrated succesfully.
 * @retval  false
 *              Not all sensors calibrated yet.
 */
extern bool LineSensor_getCalibrationState (void);

/** Read the LineSensors.
*
* @param[out] sensorValues The sensor values from the LineSensor.
*/
extern void LineSensor_read (LineSensorValues * sensorValues);

/** Turn Emitters on. */
extern void LineSensor_enableEmitter (void);

/** Turn Emitters off */
extern void LineSensor_disableEmitter (void);

#endif  /* LINESENSOR_H */