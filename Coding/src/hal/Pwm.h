/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/Pwm.h $
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       Pwm.h

This module provides PWM output support.


@version    %$Id: Pwm.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef PWM_H
#define PWM_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/**< ID for the different PWM channels. */
typedef enum tag_PwmID
{
    PWM_MOTOR_RIGHT = 0u,   /**< PWM for Right motor.           */
    PWM_MOTOR_LEFT,         /**< PWM for LEFT motor.            */
    PWM_BUZZER_NOTIFY,      /**< PWM for buzzer signal NOTIFY.  */
    PWM_BUZZER_ALARM        /**< PWM for buzzer signal ALARM.   */
} PwmID;

/* PROTOTYPES *************************************************************************************/

/** Initializes a PWM generator module. */
extern void Pwm_init (void);

/** Set the duty cycle of a pwm output.
*
* @param[in] id The id of the PWM generator.
* @param[in] percent The duty cycle of the pwm. Allowed values are 0 - 100.
                     Where 0 deactivates the pwm and 100 creates a constant output.
*/
extern void Pwm_setDutyCycle (PwmID id, UInt8 percent);

#ifdef __cplusplus
}
#endif

#endif  /* PWM_H */