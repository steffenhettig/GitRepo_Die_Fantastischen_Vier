/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/Irq.c $
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       Irq.c

This module provides interrupt support.

@version    %$Id: Irq.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <util/atomic.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "Irq.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/**
 * Simple nop function.
 */
static void nop (void);

/* VARIABLES **************************************************************************************/

/** IRQ handler for each interrupt. */
static IrqCallback gCallBack[IRQ_MAX_ID];

/* EXTERNAL FUNCTIONS *****************************************************************************/

void Irq_init(void)
{
    for (UInt8 i = 0; i < IRQ_MAX_ID; ++i)
    {
        gCallBack[i] = nop;
    }

    /* Disable pin change interrupts PCINT0..7. */
    PCICR = 0u;
    PCMSK0 = 0u;

    /* Disable external pin change interrupts INT3..0 and INT6. */
    EIMSK = 0u;
}

void Irq_enable(IrqID id)
{
    switch (id)
    {
        case IRQ_ENCODER_LEFT:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                PCICR  |= (1<<PCIE0);    /* Enable pin change interrupt for PB4. */
                PCMSK0 |= (1<<PCINT4);
                PCIFR = (1 << PCIF0);    /* clear interrupt flag. */
            }
            break;

        case IRQ_ENCODER_RIGHT:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                EICRB  |= (0<<ISC61)|(1<<ISC60);    /* INT6 any level change triggers. */
                EIMSK  |= (1<<INT6);
                EIFR   |= (1<<INTF6);                /* clear interrupt flag. */
            }
            break;

        default:
            break;
    }
}

void Irq_disable(IrqID id)
{
    switch (id)
    {
        case IRQ_ENCODER_LEFT:
            /* disable pin change interrupt for PB4. */
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                PCICR  &= ~(1<<PCIE0);
                PCMSK0 &= ~(1<<PCINT4);
            }
            break;

        case IRQ_ENCODER_RIGHT:
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                /* Disable INT6. */
                EIMSK &= ~(1<<INT6);
            }
            break;

        default:
            break;
    }
}

void Irq_setCallback(IrqID id, IrqCallback callback)
{
    if (id < IRQ_MAX_ID)
    {
        if (NULL == callback)
        {
            callback = nop;
        }

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            gCallBack[id] = callback;
        }
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/

void nop(void)
{
    /* Nothing to do. */
}

/**
 * Interrupt service routine which is called for every IRQ_ENCODER_LEFT.
 */
ISR(PCINT0_vect)
{
    gCallBack[IRQ_ENCODER_LEFT]();
}

/**
 * Interrupt service routine which is called for every IRQ_ENCODER_RIGHT.
 */
ISR(INT6_vect)
{
    gCallBack[IRQ_ENCODER_RIGHT]();
}
