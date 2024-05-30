/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/Buzzer.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Buzzer.c

Enter short description here.

For a detailed description see the detailed description in @ref Buzzer.h.

@version    %$Id: Buzzer.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "service/Buzzer.h"
#include "hal/Pwm.h"
#include "os/Task.h"
#include "os/SoftTimer.h"
#include "os/Scheduler.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/** Sound volume in percent. */
#define BUZZER_VOLUME_PERCENT       (75u)

/** Notify sound duration. */
#define BUZZER_NOTIFY_DURATION_MS   (1000u)

/** Alarm sound part duration. */
#define BUZZER_ALARM_DURATION_MS    (333u)

/* TYPES ******************************************************************************************/
/** List of available buzzer states. */
typedef enum tag_BuzzerState
{
    BUZZER_STATE_INIT,              /**< Initial buzzer state.  */
    BUZZER_STATE_ALARM_BEEP1,       /**< Buzzer state BEEP1.    */
    BUZZER_STATE_ALARM_PAUSE,       /**< Buzzer state PAUSE.    */
    BUZZER_STATE_ALARM_BEEP2,       /**< Buzzer state BEEP2.    */
    BUZZER_STATE_NOTIFY             /**< Buzzer state NOTIFY.   */
} BuzzerState;

/* PROTOTYPES *************************************************************************************/

/** Buzzer task worker function.
 *
 * @param[in] data Instance data (unused).
 */
static void gBuzzerTaskWorker (void * data);

/** Enter a new state in the state machine.
 *
 * @param[in] newState new state.
 */
static void enterState (BuzzerState newState);

/* VARIABLES **************************************************************************************/

/** Buzzer task structure. */
static Task gBuzzerTask;

/** Buzzer state update timer. */
static SoftTimer gBuzzerTimer;

/** state of buzzer state machine. */
static BuzzerState gState;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void Buzzer_init(void)
{
    gState = BUZZER_STATE_INIT;

    SoftTimer_init(&gBuzzerTimer);
    SoftTimerHandler_register(&gBuzzerTimer);

    Task_init(&gBuzzerTask, gBuzzerTaskWorker, TASK_STATE_RUNNING, NULL);
    Scheduler_addTask(&gBuzzerTask);
}

void Buzzer_beep(BuzzerID id)
{
    switch (id)
    {
        case BUZZER_NOTIFY:
            enterState(BUZZER_STATE_NOTIFY);
            break;

        case BUZZER_ALARM:
            enterState(BUZZER_STATE_ALARM_BEEP1);
            break;
        default:
            break;
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void gBuzzerTaskWorker(void * data)
{
    (void)data;

    if (SOFTTIMER_IS_EXPIRED(&gBuzzerTimer))
    {
        switch (gState)
        {
            case BUZZER_STATE_INIT:
                SoftTimer_restart(&gBuzzerTimer);
                break;

            case BUZZER_STATE_ALARM_BEEP1:
                enterState(BUZZER_STATE_ALARM_PAUSE);
                break;

            case BUZZER_STATE_ALARM_PAUSE:
                enterState(BUZZER_STATE_ALARM_BEEP2);
                break;

            case BUZZER_STATE_NOTIFY:
            case BUZZER_STATE_ALARM_BEEP2:
            default:
                enterState(BUZZER_STATE_INIT);
                break;
        }
    }
}

static void enterState(BuzzerState newState)
{
    switch (newState)
    {
        case BUZZER_STATE_INIT:
            SoftTimer_Stop(&gBuzzerTimer);
            Pwm_setDutyCycle(PWM_BUZZER_ALARM, 0);
            break;

        case BUZZER_STATE_ALARM_BEEP1:
        case BUZZER_STATE_ALARM_BEEP2:
            SoftTimer_start(&gBuzzerTimer, BUZZER_ALARM_DURATION_MS);
            Pwm_setDutyCycle(PWM_BUZZER_ALARM, BUZZER_VOLUME_PERCENT);
            break;

        case BUZZER_STATE_ALARM_PAUSE:
            SoftTimer_start(&gBuzzerTimer, BUZZER_ALARM_DURATION_MS);
            Pwm_setDutyCycle(PWM_BUZZER_ALARM, 0);
            break;

        case BUZZER_STATE_NOTIFY:
            SoftTimer_start(&gBuzzerTimer, BUZZER_NOTIFY_DURATION_MS);
            Pwm_setDutyCycle(PWM_BUZZER_NOTIFY, BUZZER_VOLUME_PERCENT);
            break;

        default:
            break;
    }

    gState = newState;
}