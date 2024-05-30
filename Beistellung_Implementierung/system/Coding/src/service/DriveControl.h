/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/DriveControl.h $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       DriveControl.h

Functions to control motor movements.

@version    %$Id: DriveControl.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef DRIVECONTROL_H
#define DRIVECONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/** Diameter of installed wheels. */
#define DRIVE_CONTROL_WHEEL_DIAMETER_MM             (38l)

/** Encoder steps per revolution when counting all edges. */
#define DRIVE_CONTROL_ENCODER_STEPS_PER_REVOLUTION  (12l)

/** Gear ratio of motors  The 75:1 Pololu Motor has actually 75.81:1 ratio. */
#define DRIVE_CONTROL_GEAR_RATIO                    (76l)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Contains all available motors. */
typedef enum tag_DriveControlMotorID
{
    DRIVE_CONTROL_MOTOR_LEFT,            /**< Left motor id.                      */
    DRIVE_CONTROL_MOTOR_RIGHT,           /**< Right motor id.                     */
    DRIVE_CONTROL_MOTOR_MAX              /**< Upper limit range for enumeration.  */
} DriveControlMotorID;

/** Possible directions. */
typedef enum tag_DriveControlDirection
{
    DRIVE_CONTROL_FORWARD,            /**< Forward movement.  */
    DRIVE_CONTROL_BACKWARD,           /**< Backward movement. */
} DriveControl_Direction;

/** Type for the encoder counter. */
typedef Int32 MotorEncoderCounter;

/* PROTOTYPES *************************************************************************************/

/**
 * Initialize module.
 */
extern void DriveControl_init (void);

/** Drive in  a given direction.
*
* @param[in] motorID The motor ID of the motor to drive.
* @param[in] speed The speed with which the motor should drive in percent.
* @param[in] direction The direction in which the motor should drive.
*
*/
extern void DriveControl_drive (DriveControlMotorID motorID, UInt8 speed, DriveControl_Direction direction);

/** Get the current distance.
*
* @return The current distance in millimeter.
*/
extern Int32 DriveControl_getMileage (void);

/** Reset mileage counter to zero.
*
*/
extern void DriveControl_resetMileage (void);

#endif  /* DRIVECONTROL_H */