/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/Pwm.c $
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       Pwm.c

This module provides PWM output support.

@version    %$Id: Pwm.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include  <avr/io.h>

#include "Pwm.h"

/* CONSTANTS **************************************************************************************/

/** Frequency of alarm tone. */
#define PWM_BUZZER_ALARM_FREQ       (500u)

/** Frequency of notify tone. */
#define PWM_BUZZER_NOTIFY_FREQ      (1000u)

/** Percentage maximum value. */
#define PERCENTAGE_MAXIMUM          (100u)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/**
 * Initialize the motor PWM.
 */
static void initMotorPwm (void);

/**
 * Initialize the buzzer PWM.
 */
static void initBuzzerPwm (void);

/**
 * Set the buffer frequency.
 */
static void setBuzzerFrequency (UInt16 freq, UInt16 volume);

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

void Pwm_init(void)
{
    initMotorPwm();
    initBuzzerPwm();
}

void Pwm_setDutyCycle(PwmID id, UInt8 percent)
{
    if (percent > PERCENTAGE_MAXIMUM)
    {
        percent = PERCENTAGE_MAXIMUM;
    }

    switch (id)
    {
        case PWM_MOTOR_RIGHT:
            OCR1A = percent << 2;   /* Scale is 0..400. */
            break;

        case PWM_MOTOR_LEFT:
            OCR1B = percent << 2;   /* Scale is 0..400. */
            break;

        case PWM_BUZZER_ALARM:
            setBuzzerFrequency(PWM_BUZZER_ALARM_FREQ, percent);
            break;

        case PWM_BUZZER_NOTIFY:
            setBuzzerFrequency(PWM_BUZZER_NOTIFY_FREQ, percent);
            break;

        default:
            break;
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void initMotorPwm(void)
{
    /* Timer 1
     * PWM frequency: 16MHz / 2 (phase-correct) / 400 (top) = 20kHz.
     *
     * Right Motor PWM is Timer1a, left Timer1b.
     */
    TCCR1A = (1<<COM1A1)|(1<<COM1B1); /* Clear OC1A/OC1B on match, set at TOP. */
    TCCR1B = (1<<WGM13)|(1<<CS10);  /* PWM, Phase and Frequency Correct, no prescaling. */

    ICR1 = 400u;
    OCR1A = 0u;      /* DutyCycle 0%. */
    OCR1B = 0u;      /* DutyCycle 0%. */
}

static void initBuzzerPwm(void)
{
    TCCR4A = 0x00;  /* Bits 7 and 6 clear: normal port op., OC4A disconnected.
                     * Bits 5 and 4 clear: normal port op., OC4B disconnected.
                     * Bit 3 clear: no force output compare for channel A.
                     * Bit 2 clear: no force output compare for channel B.
                     * Bit 1 clear: disable PWM for channel A.
                     * Bit 0 clear: disable PWM for channel B.
                     */

    TCCR4B = 0x04;  /* Bit 7 clear: disable PWM inversion.
                     * Bit 6 clear: no prescaler reset.
                     * Bits 5 and 4 clear: dead time prescaler 1.
                     * Bit 3 clear, 2 set, 1-0 clear: timer clock = CK/8.
                     */

    TCCR4C = 0x09;  /* Bits 7 and 6 clear: normal port op., OC4A disconnected.
                     * Bits 5 and 4 clear: normal port op., OC4B disconnected.
                     * Bit 3 set, 2 clear: clear OC4D on comp match when upcounting,
                     *                     set OC4D on comp match when downcounting.
                     * Bit 1 clear: no force output compare for channel D.
                     * Bit 0 set: enable PWM for channel 4.
                     */

    TCCR4D = 0x01;  /* Bit 7 clear: disable fault protection interrupt.
                     * Bit 6 clear: disable fault protection mode.
                     * Bit 5 clear: disable fault protection noise canceler.
                     * Bit 4 clear: falling edge triggers fault.
                     * Bit 3 clear: disable fault protection analog comparator.
                     * Bit 2 clear: fault protection interrupt flag.
                     * Bit 1 clear, 0 set: select waveform generation mode,
                     *    phase- and frequency-correct PWM, TOP = OCR4C,
                     *    OCR4D set at BOTTOM, TOV4 flag set at BOTTOM.
                     */

    TC4H = 0;      /* 0% duty cycle: top 2 bits... */
    OCR4D = 0;     /* And bottom 8 bits. */
}

static void setBuzzerFrequency(UInt16 freq, UInt16 volume)
{
    UInt32 top = ((F_CPU/2) + (freq >> 1))/ freq;
    UInt8 dividerExponent = 0;

    while (top > 1023)
    {
        ++dividerExponent;
        top = ((F_CPU/2 >> dividerExponent) + (freq >> 1))/ freq;
    }

    TCCR4B = (TCCR4B & 0xF0) | (dividerExponent + 1); /* Select timer 4 clock prescaler: divider = 2^n if CS4 = n+1. */
    TC4H = top >> 8;                                  /* Set timer 4 pwm frequency: top 2 bits... */
    OCR4C = top;                                      /* And bottom 8 bits.   */

    /* Set duty cycle  0 (volume in range from 0..15). */

    volume = (volume * 15) / 100;

    top >>= 16-volume;
    TC4H = top >> 8;         /* Upper 2 bits. */
    OCR4D = top;
}
