/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/Buzzer.h $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Buzzer.h

This module provides the buzzer service.


@version    %$Id: Buzzer.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** List of available buzzer sounds. */
typedef enum tag_BuzzerID
{
    BUZZER_ALARM,   /**< Buzzer sound ALARM.  */
    BUZZER_NOTIFY   /**< Buzzer sound NOTIFY. */
} BuzzerID;

/* PROTOTYPES *************************************************************************************/

/** Initializes the buzzer module. */
extern void Buzzer_init (void);

/** Play a buzzer sound.
*
* @param[in] id The id of the buzzer sound
*/
extern void Buzzer_beep (BuzzerID id);

#ifdef __cplusplus
}
#endif

#endif  /* BUZZER_H */