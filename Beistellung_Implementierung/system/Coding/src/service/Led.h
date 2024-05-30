/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/Led.h $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Led.h

Functions to control the leds on the Pololu Zumo.

The functions are:
- Turn on a certain led.
- Turn off a certain led.
- Start a moving light, which contains all 3 leds, for a certain time.

@version    %$Id: Led.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Statuses of the module. */
typedef enum
{
    LED_RET_OK = 0,   /**< Ok.                              */
    LED_RET_ERROR     /**< Not otherwise specified error.   */
} Led_Ret;

/** Contains all available leds. */
typedef enum
{
    LED_RED = 1,                         /**< Red Led.      */
    LED_GREEN,                           /**< Green Led.    */
    LED_YELLOW,                          /**< Yellow Led.   */
    LED_COUNT                            /**< LED max.      */
} Led_Led;

/* PROTOTYPES *************************************************************************************/

/**
 * Initialize module.
 */
extern void Led_init (void);

/**
 * Turns on the given led.
 *
 * @param[in] led The name of the led.
 */
extern void Led_switchOn (Led_Led led);

/**
 * Turns off the given led.
 *
 * @param[in] led The name of the led.
 */
extern void Led_switchOff (Led_Led led);

#endif  /* LED_H */