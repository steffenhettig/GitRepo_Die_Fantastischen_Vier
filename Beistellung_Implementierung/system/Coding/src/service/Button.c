/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/Button.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Button.c

Button state provider.

For a detailed description see the detailed description in @ref Button.h

@version    %$Id: Button.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "Button.h"
#include "os/Task.h"
#include "os/SoftTimer.h"
#include "os/Scheduler.h"
#include "hal/Gpio.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/** Minimum time for a stable button input value */
#define BUTTON_DEBOUNCE_DELAY_MS (100u)

/* TYPES ******************************************************************************************/
/** List of available button debouncing states. */
typedef enum tag_ButtonDebounceState
{
    DEBOUNCE_STATE_INIT = 0,        /**< Initializing debouncing.       */
    DEBOUNCE_STATE_PRESSED_START,   /**< Pressed initiated.             */
    DEBOUNCE_STATE_PRESSED,         /**< Pressed.                       */
    DEBOUNCE_STATE_RELEASED_START,  /**< Release initiated.             */
    DEBOUNCE_STATE_RELEASED         /**< Released.                      */
} ButtonDebounceState;

/* PROTOTYPES *************************************************************************************/

/** Debounce task worker function.
 *
 * @param[in] data Instance data (unused).
 */
static void gDebounceTaskWorker (void * data);

/** Debounce one button.
 *
 * @param[in] id Button id.
 */
static void debounceButton (ButtonID id);

/** Enter a new state in the debounce state machine.
 *
 * @param[in] id Button id.
 * @param[in] newState New debounce state.
 */
static void enterDebounceState (ButtonID id, ButtonDebounceState newState);

/** Check if a button is physically pressed at the time of call.
 *
 * @param[in] id Button id
 */
static bool isHwButtonPressed (ButtonID id);

/** Set state of a button.
 *  This function is expected to be called from the debounce task only.
 *
 * @param[in] id The button id.
 * @param[in] state the new button state.
 */
static void setState (ButtonID id, ButtonState state);


/* VARIABLES **************************************************************************************/

/** Debounced state for each button. */
static ButtonState gButtonStates[BUTTON_ID_MAX];

/** Debounce task structure. */
static Task gDebounceTask;

/** Debounce state update timer. */
static SoftTimer gDebounceTimer;

/** debounce state machine state for each button. */
static ButtonDebounceState gDebounceState[BUTTON_ID_MAX];

 


/* EXTERNAL FUNCTIONS *****************************************************************************/

void Button_init(void)
{
    for (UInt8 i = 0; i < BUTTON_ID_MAX; ++i)
    {
        gButtonStates[i] = BUTTON_STATE_UNKNOWN;
        gDebounceState[i] = DEBOUNCE_STATE_INIT;
    }

    SoftTimer_init(&gDebounceTimer);
    SoftTimerHandler_register(&gDebounceTimer);
    SoftTimer_start(&gDebounceTimer, BUTTON_DEBOUNCE_DELAY_MS);

    Task_init(&gDebounceTask, gDebounceTaskWorker, TASK_STATE_RUNNING, NULL);
    Scheduler_addTask(&gDebounceTask);
    
}

ButtonState Button_getState(ButtonID id)
{
    ButtonState state = BUTTON_STATE_UNKNOWN;

    if (BUTTON_ID_MAX > id)
    {
        state = gButtonStates[id];

        if ( BUTTON_STATE_TRIGGERED == state)
        {
            gButtonStates[id] = BUTTON_STATE_UNKNOWN;
            enterDebounceState(id, DEBOUNCE_STATE_INIT);
        }
    }

    return state;
}

void setState(ButtonID id, ButtonState newState)
{
    if (BUTTON_ID_MAX > id)
    {
        /** Triggered is persistent until the button was read. */
        if (BUTTON_STATE_TRIGGERED != gButtonStates[id])
        {
            if ((BUTTON_STATE_PRESSED == gButtonStates[id]) &&  (BUTTON_STATE_RELEASED == newState))
            {
                gButtonStates[id] = BUTTON_STATE_TRIGGERED;
            }
            else
            {
                gButtonStates[id] = newState;
            }
        }
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void gDebounceTaskWorker(void * data)
{
    (void)data;

    if (SOFTTIMER_IS_EXPIRED(&gDebounceTimer))
    {
        SoftTimer_restart(&gDebounceTimer);

        for (UInt8 i = 0; i < BUTTON_ID_MAX; ++i)
        {
            debounceButton(i);
        }
    }
}

static bool isHwButtonPressed(ButtonID id)
{
    Gpio_State pinState = GPIO_STATE_UNKNOWN;
    UInt8      pinConfig;

    switch (id)
    {
        case BUTTON_ID_A:
            Gpio_read(GPIO_BUTTON_A, &pinState);
            break;
        case BUTTON_ID_B:
            Gpio_alloc(GPIO_BUTTON_B, &pinConfig);
            Gpio_read(GPIO_BUTTON_B, &pinState);
            Gpio_free(GPIO_BUTTON_B, pinConfig);
            break;
        case BUTTON_ID_C:
            Gpio_alloc(GPIO_BUTTON_C, &pinConfig);
            Gpio_read(GPIO_BUTTON_C, &pinState);
            Gpio_free(GPIO_BUTTON_C, pinConfig);
        default:
            break;
    }

    return GPIO_STATE_OFF == pinState;  /* Low active. */
}

static void enterDebounceState(ButtonID id, ButtonDebounceState newState)
{
    switch (newState)
    {
        case DEBOUNCE_STATE_PRESSED_START:
        case DEBOUNCE_STATE_RELEASED_START:
            break;

        case DEBOUNCE_STATE_PRESSED:
            setState(id, BUTTON_STATE_PRESSED);
            break;

        case DEBOUNCE_STATE_RELEASED:
            setState(id, BUTTON_STATE_RELEASED);
            break;

        case DEBOUNCE_STATE_INIT:
        default:
            break;
    }

    gDebounceState[id] = newState;
}

static void debounceButton(ButtonID id)
{
    bool isButtonPressed = isHwButtonPressed(id);

    switch (gDebounceState[id])
    {
        case DEBOUNCE_STATE_INIT:
            if (isButtonPressed)
            {
                enterDebounceState(id, DEBOUNCE_STATE_PRESSED_START);
            }
            else
            {
                enterDebounceState(id, DEBOUNCE_STATE_RELEASED_START);
            }
            break;

        case DEBOUNCE_STATE_PRESSED_START:
            if (isButtonPressed)
            {
                enterDebounceState(id, DEBOUNCE_STATE_PRESSED);
            }
            else
            {
                enterDebounceState(id, DEBOUNCE_STATE_RELEASED_START);
            }
            break;

        case DEBOUNCE_STATE_PRESSED:
            if (!isButtonPressed)
            {
                enterDebounceState(id, DEBOUNCE_STATE_RELEASED_START);
            }
            break;

        case DEBOUNCE_STATE_RELEASED_START:
            if (!isButtonPressed)
            {
                enterDebounceState(id, DEBOUNCE_STATE_RELEASED);
            }
            else
            {
                enterDebounceState(id, DEBOUNCE_STATE_PRESSED_START);
            }
            break;

        case DEBOUNCE_STATE_RELEASED:
            if (isButtonPressed)
            {
                enterDebounceState(id, DEBOUNCE_STATE_PRESSED_START);
            }
            break;

        default:
            break;
    }
}