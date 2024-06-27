/***************************************************************************************************
(c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
$URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/DriveControl.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       DriveControl.c

DriveControl.


Quadrature encoder transitions are often detected by monitoring both encoder channels directly.
However, since transitions on the Zumo’s encoders can occur at high frequencies (several thousand
per second) when its motors are running, it is necessary to use the AVR’s pin change interrupts or
external interrupts to read the encoders. To reduce the required number of interrupt pins, the
Zumo 32U4 main board XORs together both channels of each encoder and connects the resulting signal
to an interrupt pin, while channel B of each encoder is connected to a non-interrupt pin.

Example: Forward movement (A rises/falls before B):
                         ___     ___
                   A : _|   |___|   |___
                     :     ___     ___
                   B : ___|   |___|   |_
                     :   _   _   _   _
             A XOR B : _| |_| |_| |_| |_

The XORed signal and the channel B signal can be used to reconstruct the channel A signal by
simply XORing them again.

@version    %$Id: DriveControl.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <util/atomic.h>

#include "DriveControl.h"
#include "hal/Gpio.h"
#include "hal/Irq.h"
#include "hal/Pwm.h"

/* CONSTANTS **************************************************************************************/

/** Aproximation of wheel cirumference based on diameter  (d * pi). */
#define WHEEL_CIRCUMFERENCE_MM ((DRIVE_CONTROL_WHEEL_DIAMETER_MM * 3141l) / 1000l)

/** How many encoder steps is one wheel revolution. */
#define WHEEL_REVOLUTION_STEPS (DRIVE_CONTROL_ENCODER_STEPS_PER_REVOLUTION * DRIVE_CONTROL_GEAR_RATIO)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Encoder State. */
typedef struct tag_MotorEncoder
{
    DriveControlMotorID             id;         /**< Id of encoder.                     */
    volatile MotorEncoderCounter    counter;    /**< Pulse counter.                     */
    volatile bool                   lastA;      /**< Former encoder A signal state.     */
    volatile bool                   lastB;      /**< Former encoder B signal state.     */
} MotorEncoder;

/* PROTOTYPES *************************************************************************************/

/** Interrupt handler for left encoder pulses. */
static void onLeftEncoderInterrupt (void);

/** Interrupt handler for right encoder pulses. */
static void onRightEncoderInterrupt (void);

/** Get encoder signal state for given motor ID.
 *
 * @param[in] id The motor ID for selecting the encoder instance.
 * @param[out] signalA State of encoder signalA.
 * @param[out] signalB State of encoder signalB.
 */
static void getEncoderState (DriveControlMotorID id, volatile bool * signalA, volatile bool * signalB);

/** Upodate encoder for given motor ID, called from Interrupt Handler!
 *
 * @param[in] id The motor ID for selecting the encoder instance.
 */
static void updateEncoder (DriveControlMotorID id);

/** Initialize an encoder.
 *
 * @param[in] encoder The encoder data to initialize.
 * @param[in] id The encoder id.
 */
static void MotorEncoder_init (MotorEncoder * encoder, DriveControlMotorID id);

/** Reset encoder state.
*
* @param[in] encoder The encoder data.
*/
static void MotorEncoder_reset (MotorEncoder * encoder);

/** Get encoder count.
*
* @param[in] encoder The encoder data.
*
* @return The encoder counter
*/
static MotorEncoderCounter MotorEncoder_get (MotorEncoder * encoder);

/* VARIABLES **************************************************************************************/

/** Encoder states. */
static MotorEncoder gEncoder[DRIVE_CONTROL_MOTOR_MAX];

/* EXTERNAL FUNCTIONS *****************************************************************************/

extern void DriveControl_init(void)
{
    Irq_setCallback(IRQ_ENCODER_LEFT, onLeftEncoderInterrupt);
    Irq_setCallback(IRQ_ENCODER_RIGHT, onRightEncoderInterrupt);

    Irq_enable(IRQ_ENCODER_LEFT);
    Irq_enable(IRQ_ENCODER_RIGHT);

    /* Reset counters after enabling interrupts in case anything spurious happend during enable. */
    for (int i = 0; i < DRIVE_CONTROL_MOTOR_MAX; ++i)
    {
        MotorEncoder_init(&gEncoder[i], i);
    }
}

extern void DriveControl_drive(DriveControlMotorID motorID, UInt8 speed, DriveControl_Direction direction)
{
    Gpio_State gpioDirValue = direction ? GPIO_STATE_ON : GPIO_STATE_OFF;

    switch (motorID)
    {
        case DRIVE_CONTROL_MOTOR_LEFT:
            Gpio_write(GPIO_MOTOR_DIR_L, gpioDirValue);
            Pwm_setDutyCycle(PWM_MOTOR_LEFT, speed);
            break;

        case DRIVE_CONTROL_MOTOR_RIGHT:
            Gpio_write(GPIO_MOTOR_DIR_R, gpioDirValue);
            Pwm_setDutyCycle(PWM_MOTOR_RIGHT, speed);
            break;

        default:
            break;
    }

}

extern Int32 DriveControl_getMileage(void)
{
    Int32 leftSteps  = MotorEncoder_get(&gEncoder[DRIVE_CONTROL_MOTOR_LEFT]);
    Int32 rightSteps = MotorEncoder_get(&gEncoder[DRIVE_CONTROL_MOTOR_RIGHT]);

    Int32 averageSteps = (leftSteps + rightSteps) >> 1;
    Int32 distance;

    /* (encoderSteps * WHEEL_CIRCUMFERENCE) / (MOTOR_STEPS_PER_REVOLUTION * GEAR_RATIO)
     */
    distance = (averageSteps * WHEEL_CIRCUMFERENCE_MM ) / WHEEL_REVOLUTION_STEPS;

    return distance;
}

extern void DriveControl_resetMileage(void)
{
    for (int i = 0; i < DRIVE_CONTROL_MOTOR_MAX; ++i)
    {
        MotorEncoder_reset(&gEncoder[i]);
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void MotorEncoder_init(MotorEncoder * encoder, DriveControlMotorID id)
{
    encoder->id = id;
    MotorEncoder_reset(encoder);
}

static void MotorEncoder_reset(MotorEncoder * encoder)
{
    getEncoderState(encoder->id, &encoder->lastA, &encoder->lastB);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        encoder->counter = 0l;
    }
}

static MotorEncoderCounter MotorEncoder_get(MotorEncoder * encoder)
{
    Int32 result;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        result = encoder->counter;
    }

    return result;
}

static void getEncoderState(DriveControlMotorID id, volatile bool * signalA, volatile bool * signalB)
{
    Gpio_State pinState;

    Gpio_read((id == DRIVE_CONTROL_MOTOR_LEFT) ? GPIO_ENCODER_B_L : GPIO_ENCODER_B_R, &pinState);
    *signalB = (GPIO_STATE_ON == pinState);

    Gpio_read((id == DRIVE_CONTROL_MOTOR_LEFT) ? GPIO_ENCODER_XOR_L : GPIO_ENCODER_XOR_R, &pinState);
    *signalA = (GPIO_STATE_ON == pinState) ^ (*signalB);
}

static void updateEncoder(DriveControlMotorID id)
{
    bool signalA;
    bool signalB;
    MotorEncoder * encoder = &gEncoder[id];

    getEncoderState(id, &signalA, &signalB);

    if (signalA ^ encoder->lastB)  /* Forward movement. */
    {
        ++encoder->counter;
    }

    if (signalB ^ encoder->lastA)  /* Backward movement. */
    {
        --encoder->counter;
    }

    encoder->lastA = signalA;
    encoder->lastB = signalB;
}

static void onLeftEncoderInterrupt()
{
    updateEncoder(DRIVE_CONTROL_MOTOR_LEFT);
}

static void onRightEncoderInterrupt()
{
    updateEncoder(DRIVE_CONTROL_MOTOR_RIGHT);
}
