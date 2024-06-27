/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2020   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/branches/%23OLED/system/Coding/src/hal/Gpio.c $
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       Gpio.c

Abstraction of the GPIOs.

This module abstracts the GPIOs of the hardware.

@version    %$Id: Gpio.c 313 2024-02-25 12:18:59Z nheitz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "hal/Gpio.h"

/* CONSTANTS **************************************************************************************/

#define OPT_INPUT           (0x00u)   /**< Pin direction input is bit 0 == 0.  */
#define OPT_OUTPUT          (0x01u)   /**< Pin direction output is bit 0 == 1. */

#define OPT_TRISTATE        (0x00u)   /**< Input pullup disable is bit 1 == 0. */
#define OPT_PULLUP          (0x02u)   /**< Input pullup enable is bit 1 == 1.  */

#define OPT_LOW             (0x00u)   /**< Output low  is bit 2 == 1.          */
#define OPT_HIGH            (0x04u)   /**< Output high is  bit 2 == 0.         */
#define OPT_INIT            (0x08u)   /**< Use config for HAL gpio init        */



/* MACROS *****************************************************************************************/
#define IS_OUTPUT(options)  (((options) & OPT_OUTPUT) == OPT_OUTPUT)
#define IS_PULLUP(options)  (((options) & OPT_PULLUP) == OPT_PULLUP)
#define IS_SET(options)     (((options) & OPT_HIGH) == OPT_HIGH)
#define IS_INIT(options)     (((options) & OPT_INIT) == OPT_INIT)

/* TYPES ******************************************************************************************/

/** GPIO ports */
typedef enum tag_Port
{
    GPIO_Port_B = 0,        /**< GPIO port B. */
    GPIO_Port_C,            /**< GPIO port C. */
    GPIO_Port_D,            /**< GPIO port D. */
    GPIO_Port_E,            /**< GPIO port E. */
    GPIO_Port_F             /**< GPIO port F. */
} Port;

/** Internal GPIO table. */
typedef struct tag_GPIO_PortConfig
{
    Gpio_ID id;              /**< ID of the GPIO pin.               */
    UInt8   dd;              /**< Data direction register pin mask. */
    UInt8   pin;             /**< Port register pin mask.           */
    Port    port;            /**< Port the pin is on.               */
    UInt8   options;         /**< Pin option bit mask.              */

} GPIO_PortConfig;

/* PROTOTYPES *************************************************************************************/

/** Configure pin.
 * @param[in] id Gpio ID.
 * @param[in] options options bits.
 */
static void setConfig(Gpio_ID id, UInt8 options);

/* VARIABLES **************************************************************************************/

/** Pololu Zumo U324 pin assignment. */
static const GPIO_PortConfig portConfig[] PROGMEM =
{
    { GPIO_OLED_DC,         (1<<DDB0), (1<<PORTB0), GPIO_Port_B, OPT_OUTPUT | OPT_LOW},
    { GPIO_OLED_MOSI,       (1<<DDD5), (1<<PORTD5), GPIO_Port_D, OPT_OUTPUT | OPT_LOW},
    { GPIO_OLED_RS,         (1<<DDD2), (1<<PORTD2), GPIO_Port_D, OPT_OUTPUT | OPT_LOW | OPT_INIT},
    { GPIO_OLED_SCK,        (1<<DDD3), (1<<PORTD3), GPIO_Port_D, OPT_OUTPUT | OPT_LOW | OPT_INIT},

    { GPIO_LED_RED,         (1<<DDB0), (1<<PORTB0), GPIO_Port_B, OPT_OUTPUT | OPT_HIGH | OPT_INIT },
    { GPIO_LED_YELLOW,      (1<<DDC7), (1<<PORTC7), GPIO_Port_C, OPT_OUTPUT | OPT_LOW  | OPT_INIT },
    { GPIO_LED_GREEN,       (1<<DDD5), (1<<PORTD5), GPIO_Port_D, OPT_OUTPUT | OPT_HIGH | OPT_INIT },

    { GPIO_BUTTON_A,        (1<<DDB3), (1<<PORTB3), GPIO_Port_B, OPT_INPUT  | OPT_PULLUP | OPT_INIT },
    { GPIO_BUTTON_B,        (1<<DDD5), (1<<PORTD5), GPIO_Port_D, OPT_INPUT  | OPT_TRISTATE },
    { GPIO_BUTTON_C,        (1<<DDB0), (1<<PORTB0), GPIO_Port_B, OPT_INPUT  | OPT_TRISTATE },

    { GPIO_MOTOR_PWM_L,     (1<<DDB6), (1<<PORTB6), GPIO_Port_B, OPT_OUTPUT | OPT_LOW | OPT_INIT },
    { GPIO_MOTOR_PWM_R,     (1<<DDB5), (1<<PORTB5), GPIO_Port_B, OPT_OUTPUT | OPT_LOW | OPT_INIT },
    { GPIO_MOTOR_DIR_L,     (1<<DDB2), (1<<PORTB2), GPIO_Port_B, OPT_OUTPUT | OPT_LOW | OPT_INIT },
    { GPIO_MOTOR_DIR_R,     (1<<DDB1), (1<<PORTB1), GPIO_Port_B, OPT_OUTPUT | OPT_LOW | OPT_INIT },

    { GPIO_ENCODER_B_L,     (1<<DDE2), (1<<PORTE2), GPIO_Port_E, OPT_INPUT  | OPT_PULLUP | OPT_INIT },
    { GPIO_ENCODER_XOR_L,   (1<<DDB4), (1<<PORTB4), GPIO_Port_B, OPT_INPUT  | OPT_PULLUP | OPT_INIT },
    { GPIO_ENCODER_B_R,     (1<<DDF0), (1<<PORTF0), GPIO_Port_F, OPT_INPUT  | OPT_PULLUP | OPT_INIT },
    { GPIO_ENCODER_XOR_R,   (1<<DDE6), (1<<PORTE6), GPIO_Port_E, OPT_INPUT  | OPT_PULLUP | OPT_INIT },

    { GPIO_LINE_OUT_DOWN1,  (1<<DDF7), (1<<PORTF7), GPIO_Port_F, OPT_OUTPUT | OPT_HIGH | OPT_INIT },
    { GPIO_LINE_OUT_DOWN2,  (1<<DDF5), (1<<PORTF5), GPIO_Port_F, OPT_OUTPUT | OPT_HIGH | OPT_INIT },
    { GPIO_LINE_OUT_DOWN3,  (1<<DDF4), (1<<PORTF4), GPIO_Port_F, OPT_OUTPUT | OPT_HIGH | OPT_INIT },
    { GPIO_LINE_OUT_DOWN4,  (1<<DDD4), (1<<PORTD4), GPIO_Port_D, OPT_OUTPUT | OPT_HIGH | OPT_INIT },
    { GPIO_LINE_OUT_DOWN5,  (1<<DDD6), (1<<PORTD6), GPIO_Port_D, OPT_OUTPUT | OPT_HIGH | OPT_INIT },

    { GPIO_LINE_IN_DOWN1,   (1<<DDF7), (1<<PORTF7), GPIO_Port_F, OPT_INPUT  | OPT_TRISTATE },
    { GPIO_LINE_IN_DOWN2,   (1<<DDF5), (1<<PORTF5), GPIO_Port_F, OPT_INPUT  | OPT_TRISTATE },
    { GPIO_LINE_IN_DOWN3,   (1<<DDF4), (1<<PORTF4), GPIO_Port_F, OPT_INPUT  | OPT_TRISTATE },
    { GPIO_LINE_IN_DOWN4,   (1<<DDD4), (1<<PORTD4), GPIO_Port_D, OPT_INPUT  | OPT_TRISTATE },
    { GPIO_LINE_IN_DOWN5,   (1<<DDD6), (1<<PORTD6), GPIO_Port_D, OPT_INPUT  | OPT_TRISTATE },
    { GPIO_LINE_EMITTER,    (1<<DDB7), (1<<PORTB7), GPIO_Port_B, OPT_OUTPUT | OPT_LOW | OPT_INIT },

    { GPIO_BUZZER,          (1<<DDD7), (1<<PORTD7), GPIO_Port_D, OPT_OUTPUT | OPT_LOW | OPT_INIT }
};

/* EXTERNAL FUNCTIONS *****************************************************************************/

Gpio_Ret Gpio_init(void)
{
    /* Initialize all defined pins. */

    for (UInt8 idx = 0; idx < sizeof(portConfig)/sizeof(portConfig[0]); ++idx)
    {
        UInt8 id = pgm_read_byte(&portConfig[idx].id);
        UInt8 options = pgm_read_byte(&portConfig[idx].options);

        if (IS_INIT(options))
        {
            setConfig(id, options);
        }
    }

    return GPIO_RET_OK;
}

Gpio_Ret Gpio_write(Gpio_ID id, Gpio_State state)
{
    UInt8 pin = pgm_read_byte(&portConfig[id].pin);

    switch (pgm_read_byte(&portConfig[id].port))
    {
        case GPIO_Port_B:
        {
            if (state == GPIO_STATE_ON)
            {
                PORTB |= pin;
            }
            else
            {
                PORTB &= ~pin;
            }
            break;
        }

        case GPIO_Port_C:
        {
            if (state == GPIO_STATE_ON)
            {
                PORTC |= pin;
            }
            else
            {
                PORTC &= ~pin;
            }
            break;
        }

        case GPIO_Port_D:
        {
            if (state == GPIO_STATE_ON)
            {
                PORTD |= pin;
            }
            else
            {
                PORTD &= ~pin;
            }
            break;
        }

        case GPIO_Port_E:
        {
            if (state == GPIO_STATE_ON)
            {
                PORTE |= pin;
            }
            else
            {
                PORTE &= ~pin;
            }
            break;
        }

        case GPIO_Port_F:
        {
            if (state == GPIO_STATE_ON)
            {
                PORTF |= pin;
            }
            else
            {
                PORTF &= ~pin;
            }
            break;
        }

        default:
            return GPIO_RET_ERROR;
    }

    return GPIO_RET_OK;
}

Gpio_Ret Gpio_read(Gpio_ID id, Gpio_State *state)
{
    UInt8 pin = pgm_read_byte(&portConfig[id].pin);

    *state = GPIO_STATE_UNKNOWN;

    switch (pgm_read_byte(&portConfig[id].port))
    {
        case GPIO_Port_B:
        {
            if (PINB & pin)
            {
                *state = GPIO_STATE_ON;
            }
            else
            {
                *state = GPIO_STATE_OFF;
            }
            break;
        }

        case GPIO_Port_C:
        {
            if (PINC & pin)
            {
                *state = GPIO_STATE_ON;
            }
            else
            {
                *state = GPIO_STATE_OFF;
            }
            break;
        }

        case GPIO_Port_D:
        {
            if (PIND & pin)
            {
                *state = GPIO_STATE_ON;
            }
            else
            {
                *state = GPIO_STATE_OFF;
            }
            break;
        }

        case GPIO_Port_E:
        {
            if (PINE & pin)
            {
                *state = GPIO_STATE_ON;
            }
            else
            {
                *state = GPIO_STATE_OFF;
            }
            break;
        }

        case GPIO_Port_F:
        {
            if (PINF & pin)
            {
                *state = GPIO_STATE_ON;
            }
            else
            {
                *state = GPIO_STATE_OFF;
            }
            break;
        }

        default:
            return GPIO_RET_ERROR;
    }

    return GPIO_RET_OK;
}

Gpio_Ret Gpio_alloc(Gpio_ID id, UInt8 * config)
{
    const GPIO_PortConfig * cfg = &portConfig[id];

    UInt8 ddrx = 0u;
    UInt8 portx = 0u;

    UInt8 pin = pgm_read_byte(&cfg->pin);
    UInt8 dd  = pgm_read_byte(&cfg->dd);

    /* Save current pind state into config. */
    switch (pgm_read_byte(&cfg->port))
    {
        case GPIO_Port_B:
        {
            ddrx = DDRB;
            portx = PORTB;
            break;
        }

        case GPIO_Port_C:
        {
            ddrx = DDRC;
            portx = PORTC;
            break;
        }

        case GPIO_Port_D:
        {
            ddrx = DDRD;
            portx = PORTD;
            break;
        }

        case GPIO_Port_E:
        {
            ddrx = DDRE;
            portx = PORTE;
            break;
        }

        case GPIO_Port_F:
        {
            ddrx = DDRF;
            portx = PORTF;
            break;
        }

        default:
            break;
    }

    *config  = 0u;

    if (ddrx & dd)
    {
        *config |= OPT_OUTPUT;

        if (portx & pin)
        {
            *config |= OPT_HIGH;
        }
    }
    else
    {
        if (portx & pin)
        {
            *config |= OPT_PULLUP;
        }
    }

    /* Configure pin to usage indicated by id. */
    setConfig(id, pgm_read_byte(&cfg->options));

    return GPIO_RET_OK;
}

Gpio_Ret Gpio_free(Gpio_ID id, UInt8 config)
{
    setConfig(id, config);
    return GPIO_RET_OK;
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void setConfig(Gpio_ID id, UInt8 options)
{
    const GPIO_PortConfig * cfg = &portConfig[id];

    bool pinIsOutput = IS_OUTPUT(options);
    bool pinIsSet     = IS_SET(options);
    bool pinIsPullup  = IS_PULLUP(options);

    UInt8 pin = pgm_read_byte(&cfg->pin);
    UInt8 dd  = pgm_read_byte(&cfg->dd);

    switch (pgm_read_byte(&cfg->port))
    {
        case GPIO_Port_B:
        {
            if (pinIsOutput)
            {
                DDRB |= dd;

                if (pinIsSet)
                {
                    PORTB |= pin;
                }
                else
                {
                    PORTB &= ~pin;
                }
            }
            else
            {
                DDRB &= ~dd;

                if (pinIsPullup)
                {
                    PORTB |= pin;
                }
                else
                {
                    PORTB &= ~pin;
                }
            }
            break;
        }
        case GPIO_Port_C:
        {
            if (pinIsOutput)
            {
                DDRC |= dd;

                if (pinIsSet)
                {
                    PORTC |= pin;
                }
                else
                {
                    PORTC &= ~pin;
                }
            }
            else
            {
                DDRC &= ~dd;

                if (pinIsPullup)
                {
                    PORTC |= pin;
                }
                else
                {
                    PORTC &= ~pin;
                }
            }
            break;
        }
        case GPIO_Port_D:
        {
            if (pinIsOutput)
            {
                DDRD |= dd;

                if (pinIsSet)
                {
                    PORTD |= pin;
                }
                else
                {
                    PORTD &= ~pin;
                }
            }
            else
            {
                DDRD &= ~dd;

                if (pinIsPullup)
                {
                    PORTD |= pin;
                }
                else
                {
                    PORTD &= ~pin;
                }
            }
            break;
        }
        case GPIO_Port_E:
        {
            if (pinIsOutput)
            {
                DDRE |= dd;

                if (pinIsSet)
                {
                    PORTE |= pin;
                }
                else
                {
                    PORTE &= ~pin;
                }
            }
            else
            {
                DDRE &= ~dd;

                if (pinIsPullup)
                {
                    PORTE |= pin;
                }
                else
                {
                    PORTE &= ~pin;
                }
            }
            break;
        }
        case GPIO_Port_F:
        {
            if (pinIsOutput)
            {
                DDRF |= dd;

                if (pinIsSet)
                {
                    PORTF |= pin;
                }
                else
                {
                    PORTF &= ~pin;
                }
            }
            else
            {
                DDRF &= ~dd;

                if (pinIsPullup)
                {
                    PORTF |= pin;
                }
                else
                {
                    PORTF &= ~pin;
                }
            }
            break;
        }
    }
}