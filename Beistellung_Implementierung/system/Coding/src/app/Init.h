
/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       Init.h
 *
 * Module description comes here.
 */
/**************************************************************************************************/
#ifndef INIT_H
#define INIT_H

/* INCLUDES ***************************************************************************************/
#include "hal/Gpio.h"
#include "hal/Hal.h"
#include "hal/Irq.h"
#include "hal/Pwm.h"
#include "hal/TickTimer.h"
#include "os/ErrorHandler.h"
#include "os/Os.h"
#include "os/Scheduler.h"
#include "os/SoftTimer.h"
#include "os/Task.h"
#include "service/Button.h"
#include "service/Buzzer.h"
#include "service/Display.h"
#include "service/DriveControl.h"
#include "service/Led.h"
#include "service/LineSensor.h"
#include "service/Service.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** calls the init methods of the used modules
* @param void
*/
void Init_process(void);


#endif /* INIT_H */
