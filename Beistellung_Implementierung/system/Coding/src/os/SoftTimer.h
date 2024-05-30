/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/SoftTimer.h $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       SoftTimer.h

SoftTimer definiton

A soft timer is  a countdown Object that can be in state running or stopped. The countdown value is
decremented in running state during every hardware timer tick until zero is reached to indicate an
expired timer.

@version    %$Id: SoftTimer.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef SOFTTIMER_H
#define SOFTTIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/** Check if timer is expired */
#define SOFTTIMER_IS_EXPIRED(pSoftTimer) \
    ((0 == (pSoftTimer)->counter) && (SOFT_TIMER_RUNNING == (pSoftTimer)->state))

/* TYPES ******************************************************************************************/

/** Statuses of the soft timer api. */
typedef enum
{
    SOFTTIMER_RET_SUCCESS = 0,          /**< Operation succesful.                           */
    SOFTTIMER_RET_FAIL,                 /**< Operation failed.                              */
    SOFTTIMER_RET_NOMORETIMERS,         /**< Maximum number of registered timers reached.   */
    SOFTTIMER_RET_UNKNOWNTIMER,         /**< Timer is not known.                            */
    SOFTTIMER_RET_ALREADYREGISTERED,    /**< Timer is already registered.                   */
    SOFTTIMER_RET_NOTREGISTERED         /**< Timer is not registered.                       */
} SoftTimer_Ret;

/** SoftTimer states.
 */
typedef enum tag_SoftTimerState
{
    SOFT_TIMER_UNREGISTERED  = 0,   /**< State unregistered.    */
    SOFT_TIMER_STOPPED,             /**< State stopped.         */
    SOFT_TIMER_RUNNING              /**< State running.         */
} SoftTimerState;

/** SoftTimer data structure.
 */
typedef struct tag_SoftTimer
{
    SoftTimerState state;     /**< SoftTimer state.           */
    volatile UInt16 counter;  /**< The countdown counter.     */
    UInt16 threshHold;        /**< The countdown start value. */
} SoftTimer;


/* PROTOTYPES *************************************************************************************/

/** Initialize a soft timer structure.
 * @param[in] pSoftTimer  A soft timer structure.
 */
extern void SoftTimer_init (SoftTimer* pSoftTimer);

/** Start a soft timer with a countdown value.
 *
 * @param[in] pSoftTimer  A soft timer structure.
 * @param[in] thresHold The start value of the count down.
 */
extern SoftTimer_Ret SoftTimer_start (SoftTimer* pSoftTimer, UInt16 thresHold);

/** Update a soft timer (interrupt context!).
 *
 * @param[in] pSoftTimer  A soft timer structure.
 */
extern SoftTimer_Ret SoftTimer_Stop (SoftTimer* pSoftTimer);

/** Update a soft timer (interrupt context!).
 *
 * @param[in] pSoftTimer  A soft timer structure.
 */
extern void SoftTimer_Update (SoftTimer* pSoftTimer);

/** Update a soft timer with threshHold from former start call.
 *
 * @param[in] pSoftTimer  A soft timer structure.
 */
extern SoftTimer_Ret SoftTimer_restart (SoftTimer* pSoftTimer);

/** Get current countdown value of a soft timer.
 *
 * @param[in] pSoftTimer  A soft timer structure.
 */
extern UInt16 SoftTimer_get (SoftTimer* pSoftTimer);

/** Initialize the soft timer handler.
 */
extern void SoftTimerHandler_init (void);

/** Register a soft timer with the handler.
 *
 * @param[in] pSoftTimer  A soft timer structure.
 */
extern SoftTimer_Ret SoftTimerHandler_register (SoftTimer * pSoftTimer);

/** De-Register a soft timer from the handler.
 *
 * @param[in] pSoftTimer  A soft timer structure.
 */
extern SoftTimer_Ret SoftTimerHandler_unRegister (SoftTimer * pSoftTimer);

/** Update all registered soft timer (interrupt context!). */
extern void SoftTimerHandler_update (void);

/** Get the current value of the tick Counter in micro seconds since startup.
 * @return tick counter value in microseconds.
 */
extern UInt64 SoftTimer_getTimestampMs (void);

#ifdef __cplusplus
}
#endif

#endif  /* SOFTTIMER_H */