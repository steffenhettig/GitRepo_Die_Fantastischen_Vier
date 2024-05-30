/***************************************************************************************************
(c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
$URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/branches/%23OLED/system/Coding/src/service/LineSensor.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       LineSensor.c

Functions to control the 5 Line sensors on the Pololu Zumo.

@version    %$Id: LineSensor.c 316 2024-03-04 14:38:23Z dembinsky $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <avr/pgmspace.h>

#include "LineSensor.h"
#include "os/Task.h"
#include "os/SoftTimer.h"
#include "os/Scheduler.h"
#include "hal/Gpio.h"
#include "hal/TickTimer.h"
#include "service/Display.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Calibration data for a line sensor. */
typedef struct tag_LineSensorCalibrationData
{
    UInt16 minVal; /**< Minimum values obtained during calibration.     */
    UInt16 maxVal; /**< Maximum values obtained during calibration.     */
    UInt16 range; /**< Calibration range.                               */
} LineSensorCalibrationData;

/* PROTOTYPES *************************************************************************************/

/** Calibration task worker function.
 *
 * @param[in] data Instance data (unused).
 */
static void calibrationTaskWorker (void * data);

/** Read raw (uncalibrated) sensor values.
 *  Reading is done by charcing the sensors for ~10us, then measure the time until the drop
 *  to low level again. A darker ground means a slower voltage drop.
 * @param[out] data sensor data value container.
 */
static void readSensorValuesRaw(LineSensorValues * data);


/** Reset calibration data
 */
static void resetCalibration(void);

/* VARIABLES **************************************************************************************/

/** Line signals in read mode. */
static const Gpio_ID readPins[LINESENSOR_COUNT] PROGMEM =
{
    GPIO_LINE_IN_DOWN1,
    GPIO_LINE_IN_DOWN2,
    GPIO_LINE_IN_DOWN3,
    GPIO_LINE_IN_DOWN4,
    GPIO_LINE_IN_DOWN5
};

/** Line signals in write mode. */
static const Gpio_ID writePins[LINESENSOR_COUNT] PROGMEM =
{
    GPIO_LINE_OUT_DOWN1,
    GPIO_LINE_OUT_DOWN2,
    GPIO_LINE_OUT_DOWN3,
    GPIO_LINE_OUT_DOWN4,
    GPIO_LINE_OUT_DOWN5
};

/** Calibration min/max values for each sensor. */
static LineSensorCalibrationData gCalibrationData[LINESENSOR_COUNT];

/** Calibration task structure. */
static Task gCalibrationTask;

/** Calibration poll intervall. */
static SoftTimer gCalibrationTimer;

/* EXTERNAL FUNCTIONS *****************************************************************************/

extern void LineSensor_init(void)
{
    Task_init(&gCalibrationTask, calibrationTaskWorker, TASK_STATE_SUSPENDED, NULL);
    Scheduler_addTask(&gCalibrationTask);
    SoftTimer_init(&gCalibrationTimer);
    SoftTimerHandler_register(&gCalibrationTimer);
}

extern void LineSensor_read(LineSensorValues *sensorValues)
{
#if 0
    char line[LINESENSOR_COUNT+1];
#endif
    readSensorValuesRaw(sensorValues);

    /* Normalize values based on calibration data. */
    for (UInt8 i = 0u; i < LINESENSOR_COUNT; ++i)
    {
        const LineSensorCalibrationData *caldata = &gCalibrationData[i];

        if (0u != caldata->range)
        {
            Int32 longVal = sensorValues->value[i] - caldata->minVal;

            if (0 > longVal)
            {
                longVal = 0;
            }
            else if (caldata->maxVal < longVal)
            {
                longVal = caldata->maxVal;
            }

            longVal *= LINESENSOR_NORMALIZED_RANGE;
            longVal /= caldata->range;

            sensorValues->value[i]      = (UInt16)longVal;
            sensorValues->calibrated[i] = true;
#if 0
            line[i] = '1';
#endif
        }
        else
        {
            /* Uncalibrated sensor does not produce anything. */
            sensorValues->value[i]      = 0u;
            sensorValues->calibrated[i] = false;
#if 0
            line[i] = '0';
#endif
        }
    }

#if 0
    line[LINESENSOR_COUNT] = 0;
    Display_gotoxy(0, 1);
    Display_write(line, sizeof(line));
#endif
}

extern void LineSensor_startCalibration(void)
{
    resetCalibration();

    SoftTimer_start(&gCalibrationTimer, 10u);
    TASK_SET_STATE(&gCalibrationTask, TASK_STATE_RUNNING);
    LineSensor_enableEmitter();
}

extern void LineSensor_stopCalibration(void)
{
    SoftTimer_Stop(&gCalibrationTimer);
    TASK_SET_STATE(&gCalibrationTask, TASK_STATE_SUSPENDED);
    LineSensor_disableEmitter();
}

extern bool LineSensor_getCalibrationState(void)
{
  
     bool result = true;
  

    for (UInt8 i = 0; i < LINESENSOR_COUNT; ++i)
    {
        if (0u == gCalibrationData[i].range)
        {
            char txt[] = "0";
 
            result = false;
            txt[0] = '0' + i;
            Display_gotoxy(0, 0);
            Display_write(txt, 1);
            Display_gotoxy(0, 1);
            Display_write("CALIB", 5);
            break;
        }
    }


    return result;
}

extern void LineSensor_enableEmitter(void)
{
    GPIO_SET(GPIO_LINE_EMITTER);
    TICKTIMER_DELAY_US(200u);
}

extern void LineSensor_disableEmitter(void)
{
    GPIO_CLEAR(GPIO_LINE_EMITTER);
    TICKTIMER_DELAY_US(200u);
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void readSensorValuesRaw(LineSensorValues * data)
{
    UInt8 pin;
    UInt8 dummyState;
    UInt64 startTimeUs;
    UInt64 delay;
    bool reloop;

    /* Prepare for reading. */
    for (pin = 0u; pin < LINESENSOR_COUNT; ++pin)
    {
        /* Init to max discharge delay. */
        data->value[pin] = LINESENSOR_READ_TIMEOUT_US;
        data->calibrated[pin] = false;

        /* Switch all line pins into write mode for charging. */
        Gpio_alloc(pgm_read_byte(&writePins[pin]), &dummyState);
    }

    TICKTIMER_DELAY_US(10u);   /* Charge sensors for at least 10 micro seconds. */

    /* Switch all line pins into read mode for discharging. */
    for (pin = 0; pin < LINESENSOR_COUNT; ++pin)
    {
        Gpio_alloc(pgm_read_byte(&readPins[pin]), &dummyState);
    }

    startTimeUs = TickTimer_get();

    /** Keep reading pins to measure time until they drop to zero. */
    do
    {
        delay = TickTimer_get() - startTimeUs;
        reloop = false;

        for (pin = 0; pin < LINESENSOR_COUNT; ++pin)
        {
            Gpio_State state;
            Gpio_read(pgm_read_byte(&readPins[pin]), &state);

            if (GPIO_STATE_OFF == state)
            {
                if (delay < data->value[pin] )
                {
                    data->value[pin] = delay;
                }

            }
            else
            {
                reloop = true; /* At least one sensor is not drained. */
            }
        }
    }
    while (reloop && (delay < LINESENSOR_READ_TIMEOUT_US));
}

static void calibrationTaskWorker(void *data)
{
    (void)data;

    LineSensorValues values;

    if (SOFTTIMER_IS_EXPIRED(&gCalibrationTimer))
    {
        SoftTimer_restart(&gCalibrationTimer);

        readSensorValuesRaw(&values);

        for (UInt8 i = 0; i < LINESENSOR_COUNT; ++i)
        {
            LineSensorCalibrationData *caldata = &gCalibrationData[i];

            if (values.value[i] < caldata->minVal)
            {
                caldata->minVal = values.value[i];
            }

            if (values.value[i] > caldata->maxVal)
            {
                caldata->maxVal = values.value[i];
            }

            /* Accept calibration if we've seen at least 25% of possible range. */
            if (caldata->maxVal > caldata->minVal)
            {
                UInt16 range = caldata->maxVal - caldata->minVal;
            
                if (range >= (LINESENSOR_READ_TIMEOUT_US / 4u))
                {
                    caldata->range = (UInt16)range;
                }
            }
        }
    }
}

static void resetCalibration(void)
{
    for (UInt8 idx = 0; idx < LINESENSOR_COUNT; ++idx)
    {
        gCalibrationData[idx].minVal = LINESENSOR_READ_TIMEOUT_US;
        gCalibrationData[idx].maxVal = 0u;
        gCalibrationData[idx].range  = 0u;
    }
}