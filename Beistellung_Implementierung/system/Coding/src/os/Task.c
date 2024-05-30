/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/Task.c $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       Task.c

Simple task scheduler.

For a detailed description see the detailed description in @ref Scheduler.h.

@version    %$Id: Task.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <string.h>

#include "os/Task.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

Task_Ret Task_init(Task * pTask, TaskWorkCallback callback, TaskState state, void * data)
{
    pTask->state = state;
    pTask->instanceData = data;
    pTask->workCallback = callback;

    return (NULL != callback) ? TASK_RET_SUCCESS : TASK_RET_FAIL;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
