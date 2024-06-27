/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/Hal.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Button.c

Button state provider.

For a detailed description see the detailed description in @ref Button.h

@version    %$Id: Hal.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "Hal.h"

#include "hal/Gpio.h"
#include "hal/Irq.h"
#include "hal/Pwm.h"
#include "hal/TickTimer.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

void Hal_init(void)
{
    Gpio_init();
    Pwm_init();
    Irq_init();
    TickTimer_init();
}

/**
 * Halt execution
 */
void Hal_halt(void)
{
  for (;;)
  {
    IRQ_DISABLE_INTERRUPTS();
  }
}

/* INTERNAL FUNCTIONS *****************************************************************************/
