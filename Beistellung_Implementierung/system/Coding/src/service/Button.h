/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/Button.h $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Button.h

Button state provider.

@version    %$Id: Button.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** List of available buttons. */
typedef enum tag_ButtonID
{
    BUTTON_ID_A = 0,    /**< Button A.                          */
    BUTTON_ID_B,        /**< Button B.                          */
    BUTTON_ID_C,        /**< Button C.                          */
    BUTTON_ID_MAX       /**< Upper limit range for enumeration. */
} ButtonID;

/** Logical button states (after debounce). */
typedef enum tag_ButtonStatus
{
    BUTTON_STATE_UNKNOWN = 0,   /**< Unknown button state.  */
    BUTTON_STATE_PRESSED,       /**< Button pressed.        */
    BUTTON_STATE_RELEASED,      /**< Button released.       */
    BUTTON_STATE_TRIGGERED      /**< Button triggered.      */

} ButtonState;

/* PROTOTYPES *************************************************************************************/

/** Initialize the button module. */
extern void Button_init (void);

/** Get debounced state of a button.
 *
 * @param[in] id The button id.
 * @return the debounced button state.
 */
extern ButtonState Button_getState (ButtonID id);

#ifdef __cplusplus
}
#endif

#endif  /* BUTTON_H */