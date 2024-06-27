/***************************************************************************************************
(c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
$URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/Led.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Led.c

LED service module.

@version    %$Id: Led.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "./Led.h"
#include "hal/Gpio.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/


/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

extern void Led_init(void)
{
    /* All off initially. */
    Led_switchOff(GPIO_LED_RED);
    Led_switchOff(GPIO_LED_YELLOW);
    Led_switchOff(GPIO_LED_GREEN);
}

extern void Led_switchOff(Led_Led led)
{
    /* Note: Red and green are low-active,  yellow on high. */
    switch (led)
    {
        case LED_RED:
            GPIO_SET(GPIO_LED_RED);
            break;

        case LED_YELLOW:
            GPIO_CLEAR(GPIO_LED_YELLOW);
            break;

        case LED_GREEN:
            GPIO_SET(GPIO_LED_GREEN);
            break;

        default:
            break;
    }
}

extern void Led_switchOn(Led_Led led)
{
    /* Note: Red and green are low-active,  yellow on high. */
    switch (led)
    {
        case LED_RED:
            GPIO_CLEAR(GPIO_LED_RED);
            break;

        case LED_YELLOW:
            GPIO_SET(GPIO_LED_YELLOW);
            break;

        case LED_GREEN:
            GPIO_CLEAR(GPIO_LED_GREEN);
            break;

        default:
            break;
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/

