/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/branches/%23OLED/system/Coding/src/hal/Gpio.h $
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       Gpio.h

Abstraction of the GPIOs.

This module abstracts the GPIOs of the hardware.


@version    %$Id: Gpio.h 313 2024-02-25 12:18:59Z nheitz $
* @}
***************************************************************************************************/
#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/** Set the GPIO pin.
 *
 *  @param[in] id The id of the GPIO.
 */
#define GPIO_CLEAR(id)  Gpio_write((id), GPIO_STATE_OFF)

/** Clear the GPIO pin.
 *
 *  @param[in] id The id of the GPIO.
 */
#define GPIO_SET(id)    Gpio_write((id), GPIO_STATE_ON)

/* TYPES ******************************************************************************************/

/** Statuses of the module. */
typedef enum
{
    GPIO_RET_OK = 0,         /**< Ok.                               */
    GPIO_RET_ERROR           /**< Not otherwise specified error.    */
} Gpio_Ret;

/** Statuses of a GPIO. */
typedef enum
{
    GPIO_STATE_UNKNOWN = -1,    /**< GPIO state is undefined.  */
    GPIO_STATE_ON,              /**< GPIO state is on.         */
    GPIO_STATE_OFF              /**< GPIO state is off.        */
} Gpio_State;

/**  GPIO pin Ids. */
typedef enum tag_GPIO_ID
{
    GPIO_OLED_DC = 0,
    GPIO_OLED_MOSI,
    GPIO_OLED_RS,
    GPIO_OLED_SCK,

    GPIO_LED_RED,           /**< GPIO of the red LED.                   */
    GPIO_LED_YELLOW,        /**< GPIO of the yellow LED.                */
    GPIO_LED_GREEN,         /**< GPIO of the green LED.                 */

    GPIO_BUTTON_A,          /**< GPIO of button A.                      */
    GPIO_BUTTON_B,          /**< GPIO of button B.                      */
    GPIO_BUTTON_C,          /**< GPIO of button C.                      */

    GPIO_MOTOR_PWM_L,       /**< Left motor PWM.                        */
    GPIO_MOTOR_PWM_R,       /**< Right motor PWM.                       */
    GPIO_MOTOR_DIR_L,       /**< Left motor direction.                  */
    GPIO_MOTOR_DIR_R,       /**< Right motor direction.                 */

    GPIO_ENCODER_B_L,       /**< Left Motor encoder B.                  */
    GPIO_ENCODER_XOR_L,     /**< Left Motor encoder XOR.                */
    GPIO_ENCODER_B_R,       /**< Right Motor encoder B.                 */
    GPIO_ENCODER_XOR_R,     /**< Right Motor encoder XOR.               */

    GPIO_LINE_OUT_DOWN1,    /**< Line Sensor 1 write mode.              */
    GPIO_LINE_OUT_DOWN2,    /**< Line Sensor 2 write mode.              */
    GPIO_LINE_OUT_DOWN3,    /**< Line Sensor 3 write mode.              */
    GPIO_LINE_OUT_DOWN4,    /**< Line Sensor 4 write mode.              */
    GPIO_LINE_OUT_DOWN5,    /**< Line Sensor 5 write mode.              */

    GPIO_LINE_IN_DOWN1,     /**< Line Sensor 1 read mode.               */
    GPIO_LINE_IN_DOWN2,     /**< Line Sensor 2 read mode.               */
    GPIO_LINE_IN_DOWN3,     /**< Line Sensor 3 read mode.               */
    GPIO_LINE_IN_DOWN4,     /**< Line Sensor 4 read mode.               */
    GPIO_LINE_IN_DOWN5,     /**< Line Sensor 5 read mode.               */
    GPIO_LINE_EMITTER,      /**< IR emitters for line sensors.          */

    GPIO_BUZZER             /**< Buzzer PWM pin.                        */
} Gpio_ID;

/* PROTOTYPES *************************************************************************************/

/**
 * Initialize Gpio pins to a defined state.
 *
 * Function is expected to internally have access
 * to a table with the default state for every pin.
 */
extern  Gpio_Ret Gpio_init (void);

/**
 * Writes the status of the GPIO.
 *
 *
 * @param[in] id The id of the GPIO.
 * @param[in] state The status to be written.
 *
 * @return GPIO_Ret The status of the operation.
 */
extern Gpio_Ret Gpio_write (Gpio_ID id, Gpio_State state);

/**
 * Reads the status of the GPIO.
 *
 *
 * @param[in] id The id of the GPIO.
 * @param[out] state The status to be written.
 *
 * @return GPIO_Ret The status of the operation.
 */
extern Gpio_Ret Gpio_read (Gpio_ID id, Gpio_State *state);

/** Allocate a multiplexed pin for a different usage.
 *
 * This function must be used if a multiplexed pin is
 * temporarily used for another purpose given by id.
 * The pin associated hardware state is restored by calling
 * Gpio_free().
 *
 * @param[in] id The id of the GPIO.
 * @param[out] config An opaque value for restoring pin state in Gpio_free()
 */
extern Gpio_Ret Gpio_alloc (Gpio_ID id, UInt8 * config);

/** Restore a multiplexed pin after usage.
 *
 * This function must restores the pin associated hardware
 * to the state returned from a former Gpio_alloc()
 *
 * @param[in] id The id of the GPIO.
 * @param[in] config An opaque value provided by Gpio_alloc().
 */
extern Gpio_Ret Gpio_free (Gpio_ID id, UInt8 config);

#ifdef __cplusplus
}
#endif

#endif  /* GPIO_H */