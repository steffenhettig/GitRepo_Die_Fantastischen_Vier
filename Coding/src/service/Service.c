/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/Service.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Button.c

Button state provider.

For a detailed description see the detailed description in @ref Button.h

@version    %$Id: Service.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "Service.h"

#include "Button.h"
#include "Buzzer.h"
#include "Display.h"
#include "DriveControl.h"
#include "Led.h"
#include "LineSensor.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

void Service_init(void)
{
    Button_init();
    Buzzer_init();
    Display_init();
    DriveControl_init();
    Led_init();
    LineSensor_init();
}

/* INTERNAL FUNCTIONS *****************************************************************************/
