/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/Irq.h $
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       Pwm.h

This module provides interrupt support.


@version    %$Id: Irq.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef IRQ_H
#define IRQ_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include <util/atomic.h>
#include <avr/interrupt.h>

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/** Enable hardware interrupts. */
#define IRQ_ENABLE_INTERRUPTS()     sei()

/** Disable hardware interrupts. */
#define IRQ_DISABLE_INTERRUPTS()    cli()

/** Enter atomic section that executes with interrupts locked. */
#define IRQ_ENTER_ATOMIC_SECTION()  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

/** Leave atomic section and restore the interrupt state active when the section got entered. */
#define IRQ_LEAVE_ATOMIC_SECTION()  }

/* TYPES ******************************************************************************************/

/** IRQ sources. */
typedef enum tag_IrqID
{
    IRQ_ENCODER_LEFT = 0,         /**< Left encoder XOR signal.           */
    IRQ_ENCODER_RIGHT,            /**< Right encoder XOR signal.          */
    IRQ_MAX_ID                    /**< Upper limit range for enumeration. */
} IrqID;

/** Callback function type for installing an interrupt callback function. */
typedef void (*IrqCallback)(void);

/* PROTOTYPES *************************************************************************************/

/** Initializes IRQ module. */
extern void Irq_init (void);

/** Enable given interrupt.
*
* @param[in] id The id of the IRQ.
*/
extern void Irq_enable (IrqID id);

/** Disable given interrupt.
*
* @param[in] id The id of the IRQ.
*/
extern void Irq_disable (IrqID id);

/** Set callback function for given IRQ.
*
* @param[in] id The id of the IRQ.
* @param[in] callback The callback function to call when IRQ fires.
*/
extern void Irq_setCallback (IrqID id, IrqCallback callback);

#ifdef __cplusplus
}
#endif

#endif  /* IRQ_H */