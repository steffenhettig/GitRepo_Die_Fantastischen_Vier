/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2020   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/Scheduler.h $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       Scheduler.h

Cooperative Round-Robin Scheduler.

A simple task scheduler that manages a fixed number of Tasks. Scheduling is
cooperative, meaning task execution is done by calling a work cycle function
that must return into the scheduler as fast as possible. Tasks in running
state are called round-robion sequentially.

@version    %$Id: Scheduler.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef SCHEDULER_H
#define SCHEDULER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"
#include "Task.h"

/* CONSTANTS **************************************************************************************/

/** Maximum number of tasks that can be handled by the scheduler. */
#define SCHEDULER_MAX_TASKS (10)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Statuses of the scheduler */
typedef enum
{
    SCHEDULER_RET_SUCCESS = 0,     /**< Operation succesful.                */
    SCHEDULER_RET_FAIL,            /**< Operation failed.                   */
    SCHEDULER_RET_NOMORETASKS,     /**< Maximum number of tasks reached.    */
    SCHEDULER_RET_TASKADDEDTWICE,  /**< Tried to add a task more then once. */
    SCHEDULER_RET_UNKNOWNTASK      /**< Task was not added.                 */
} Scheduler_Ret;

/* PROTOTYPES *************************************************************************************/

/**  Initialize the scheduler module.
  *
  */
extern Scheduler_Ret Scheduler_init (void);

/** Execute one scheduler cycle.
  * Call the work function of all running tasks sequentially.
  */
extern void Scheduler_execute (void);

/** Add a task to the scheduler.
 * @param[in] task Pointer to task structure.
 *
 * @return true if successful, false if no more space or task already added.
 */
extern Scheduler_Ret Scheduler_addTask (Task * task);

/** Remove a task from the scheduler.
 * @param[in] task Pointer to task structure.
 *
 * @return true if successful, falls if task not found.
 */
extern Scheduler_Ret Scheduler_removeTask (Task * task);

#ifdef __cplusplus
}
#endif

#endif  /* SCHEDULER_H */