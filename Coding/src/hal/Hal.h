/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/Hal.h $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Service.h

General HAL layer functionality.


@version    %$Id: Hal.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef HAL_H
#define HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/**
 * Initialize HAL layer.
 */
extern void Hal_init(void);

/**
 * Halt execution
 */
extern void Hal_halt(void);
#endif  /* HAL_H */