/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/SoftTimer.c $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       SoftTimer.c

SoftTimer implementation.
For a detailed description see the detailed description in @ref SoftTimer.h.

@version    %$Id: SoftTimer.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "os/SoftTimer.h"
#include "hal/TickTimer.h"

/* CONSTANTS **************************************************************************************/

/** Maximum number of soft timers */
#define SOFT_TIMER_MAX_TIMER (10u)

#ifdef TARGET_NATIVE
    #define ATOMIC_BLOCK(argument)
#else
    #include <util/atomic.h>
#endif

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/** Storage for soft timers. */
static SoftTimer * gSoftTimer[SOFT_TIMER_MAX_TIMER];

/* EXTERNAL FUNCTIONS *****************************************************************************/

void SoftTimer_init(SoftTimer* pSoftTimer)
{
    pSoftTimer->state = SOFT_TIMER_UNREGISTERED;
    pSoftTimer->counter = 0u;
    pSoftTimer->threshHold = 0u;
}

SoftTimer_Ret SoftTimer_start(SoftTimer* pSoftTimer, UInt16 thresHold)
{
    SoftTimer_Ret result = SOFTTIMER_RET_NOTREGISTERED;

    if (SOFT_TIMER_UNREGISTERED != pSoftTimer->state)
    {
        pSoftTimer->threshHold = thresHold;
        pSoftTimer->counter = thresHold;
        pSoftTimer->state = SOFT_TIMER_RUNNING;

        result = SOFTTIMER_RET_SUCCESS;
    }

    return result;
}

SoftTimer_Ret SoftTimer_Stop(SoftTimer* pSoftTimer)
{
    SoftTimer_Ret result = SOFTTIMER_RET_NOTREGISTERED;

    if (SOFT_TIMER_UNREGISTERED != pSoftTimer->state)
    {
        pSoftTimer->state = SOFT_TIMER_STOPPED;
        result = SOFTTIMER_RET_SUCCESS;
    }

    return result;
}

void SoftTimer_Update(SoftTimer* pSoftTimer)
{
    if ((SOFT_TIMER_RUNNING == pSoftTimer->state) && (0u != pSoftTimer->counter))
    {
        --pSoftTimer->counter;
    }
}

SoftTimer_Ret SoftTimer_restart(SoftTimer* pSoftTimer)
{
    return SoftTimer_start(pSoftTimer, pSoftTimer->threshHold);
}

UInt16 SoftTimer_get(SoftTimer * pSoftTimer)
{
    UInt16 result;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        result = pSoftTimer->counter;
    }

    return result;
}

void SoftTimerHandler_init()
{
    UInt8 index = 0u;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        for (index = 0u; index < SOFT_TIMER_MAX_TIMER; ++index)
        {
            gSoftTimer[index] = NULL;
        }
    }
}

SoftTimer_Ret SoftTimerHandler_register(SoftTimer * pNewSoftTimer)
{
    UInt8 slot = SOFT_TIMER_MAX_TIMER;
    UInt8 index = 0u;
    SoftTimer_Ret result = SOFTTIMER_RET_NOMORETIMERS;

    for (index = 0u; index < SOFT_TIMER_MAX_TIMER; ++index)
    {
        SoftTimer * pTimer = gSoftTimer[index];
        if (pTimer == pNewSoftTimer)
        {
            result = SOFTTIMER_RET_ALREADYREGISTERED;
            break;    /* Already present. */
        }
        if (NULL == pTimer)
        {
            slot = index;
        }
    }

    if (SOFT_TIMER_MAX_TIMER == index)
    {
        if (SOFT_TIMER_MAX_TIMER != slot)
        {
            pNewSoftTimer->state = SOFT_TIMER_STOPPED;

            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                gSoftTimer[slot] = pNewSoftTimer;
            }

            result = SOFTTIMER_RET_SUCCESS;
        }
    }

    return result;
}

SoftTimer_Ret SoftTimerHandler_unRegister(SoftTimer * pSoftTimerToRemove)
{
    SoftTimer_Ret result = SOFTTIMER_RET_UNKNOWNTIMER;
    UInt8 index = 0u;

    for (index = 0u; index < SOFT_TIMER_MAX_TIMER; ++index)
    {
        SoftTimer * pTimer = gSoftTimer[index];

        if (pTimer == pSoftTimerToRemove)
        {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                gSoftTimer[index] = NULL;
            }
            result = SOFTTIMER_RET_SUCCESS;
            break;
        }
    }

    return result;
}

void SoftTimerHandler_update()
{
    UInt8 index = 0u;

    for (index = 0u; index < SOFT_TIMER_MAX_TIMER; ++index)
    {
        SoftTimer * pTimer = gSoftTimer[index];

        if ((NULL != pTimer) &&  (0u != pTimer->counter) && (SOFT_TIMER_RUNNING == pTimer->state))
        {
            --pTimer->counter;
        }
    }
}

UInt64 SoftTimer_getTimestampMs()
{
    return TickTimer_get() / 1000u;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
