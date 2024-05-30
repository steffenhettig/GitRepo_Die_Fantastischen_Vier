/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/Scheduler.c $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       Scheduler.c

Simple task scheduler

For a detailed description see the detailed description in @ref Scheduler.h.

@version    %$Id: Scheduler.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "os/Scheduler.h"
#include "os/Task.h"

#ifndef TARGET_NATIVE
#include "hal/Irq.h"
#else
#include "hal/TickTimer.h"
#endif

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/** Storage for task structure pointer. */
static Task * gTasks[SCHEDULER_MAX_TASKS];

/* EXTERNAL FUNCTIONS *****************************************************************************/

Scheduler_Ret Scheduler_init(void)
{
    /* Task data. */
    for (UInt8 index = 0; index < SCHEDULER_MAX_TASKS; ++index)
    {
        gTasks[index] = NULL;
    }

    return TASK_RET_SUCCESS;
}

void Scheduler_execute(void)
{
    #ifndef TARGET_NATIVE
    IRQ_ENABLE_INTERRUPTS();

    while(1)
    #else
    while(TickTimer_simulationStep())
    #endif
    {
        for (UInt8 index = 0; index < SCHEDULER_MAX_TASKS; ++index)
        {
            Task * pTask = gTasks[index];

            if ((NULL != pTask) && TASK_STATE_RUNNING == TASK_GET_STATE(pTask))
            {
                TASK_EXECUTE(pTask);
            }
        }

    }
}

Scheduler_Ret Scheduler_addTask(Task * pNewTask)
{
    UInt8 index = 0;
    UInt8 slot = SCHEDULER_MAX_TASKS;
    Scheduler_Ret result = SCHEDULER_RET_NOMORETASKS;

    /* Check if task is already added and search free slot. */
    for (index = 0; index < SCHEDULER_MAX_TASKS; ++index)
    {
        Task * pTask = gTasks[index];

        if (pNewTask == pTask)
        {
            result = SCHEDULER_RET_TASKADDEDTWICE;
            break;
        }

        if (NULL == pTask)
        {
            slot = index;
        }
    }

    if (SCHEDULER_MAX_TASKS == index)
    {
        if (SCHEDULER_MAX_TASKS != slot)
        {
            /* Task not yet present and free slot found. */
            gTasks[slot] = pNewTask;
            result = SCHEDULER_RET_SUCCESS;
        }
    }

    return result;
}

Scheduler_Ret Scheduler_removeTask(Task * pTaskToRemove)
{
    Scheduler_Ret result = SCHEDULER_RET_UNKNOWNTASK;

    for (UInt8 index = 0; index < SCHEDULER_MAX_TASKS; ++index)
    {
        Task * pTask = gTasks[index];

        if (pTaskToRemove == pTask)
        {
            pTask = NULL;
            result = SCHEDULER_RET_SUCCESS;
            break;
        }
    }
    return result;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
