/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/Task.h $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       Task.h

Enter short description here.

Enter detailed description here.

@version    %$Id: Task.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef TASK_H
#define TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/


/* MACROS *****************************************************************************************/

/** Get state of  a given task.
 *
 * @param[in] task Pointer to task structure.
 */
#define TASK_GET_STATE(task)            (task)->state

/** Set state of given task.
 *
 * @param[in] task Pointer to task structure.
 * @param[in] newState New task state.
 */
#define TASK_SET_STATE(task, newState)  (task)->state = (newState)

/** Execute the task work cycle function.
 * @param[in] task Pointer to task structure.
 */
#define TASK_EXECUTE(task)              (task)->workCallback((task->instanceData))

/* TYPES ******************************************************************************************/

/** Statuses of the task. */
typedef enum
{
    TASK_RET_FAIL    = 0,   /**< Status: fail.      */
    TASK_RET_SUCCESS        /**< Status: success.   */
} Task_Ret;

/** Possible Task States.
 */
typedef enum tag_TaskState
{
    TASK_STATE_SUSPENDED = 0,   /**< Task is not allowed to run. */
    TASK_STATE_RUNNING          /**< Task is allowed to run.     */
} TaskState;

/** Type of task work cycle function.
 */
typedef void (*TaskWorkCallback)(void * instanceData);

/** Task Structure.
 *
 */
typedef struct tag_Task
{
    TaskState        state;          /**< Task status.                           */
    void *           instanceData;   /**< Task context, passed to work function. */
    TaskWorkCallback workCallback;   /**< Task work cycle function.              */
} Task;

/* PROTOTYPES *************************************************************************************/

/** Initialize a task structure.
 *
 * @param[in] task The task structure to initialize.
 * @param[in] callback The task work cycle function.
 * @param[in] state The task state.
 * @param[in] data Task context data that gets passed to the work cycle function.
 *
 * @return Task_Ret Status of operation.
 */
extern Task_Ret Task_init (Task * task, TaskWorkCallback callback, TaskState state, void * data);

#ifdef __cplusplus
}
#endif

#endif  /* TASK_H */