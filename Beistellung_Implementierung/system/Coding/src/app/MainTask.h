/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/app/MainTask.h $
***************************************************************************************************/
/**
@addtogroup App
@{
@file       MainTask.h

The main application task.

@version    %$Id: MainTask.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef MAINTASK_H
#define MAINTASK_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"
#include "os/Task.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Statuses of the Main task. */
typedef enum
{
    MAINTASK_RET_SUCCESS = 0,             /**< Ok.                                */
    MAINTASK_RET_INIT_TASK_FAIL,          /**< Initalization failed.              */
    MAINTASK_RET_ADD_TASK_FAIL,           /**< Adding failed.                     */
    MAINTASK_RET_INTERNAL_ERROR           /**< Internal error.                    */
    /* TODO: Add own errorcodes here. */

} MainTask_Ret;

/* PROTOTYPES *************************************************************************************/

/** Initialize the main task. */
extern MainTask_Ret MainTask_init (void);

#ifdef __cplusplus
}
#endif

#endif  /* MAINTASK_H */