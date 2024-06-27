/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/branches/%23OLED/system/Coding/src/service/Display.h $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       display.h

HD44780 display driver  for text output.

@version    %$Id: Display.h 316 2024-03-04 14:38:23Z dembinsky $
* @}
***************************************************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

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

/** Initialize the display module.
 */
extern void Display_init (void);

/**
 * Clear the display and set cursor to the upper left corner (0,0).
 */
extern void Display_clear (void);

/**
 * Clear the current line.
 */
extern void Display_clearLine(void);

/**
 * Set cursor to home position.
 */
extern void Display_home (void);

/**
 * Set cursor to given position.
 *
 * @param[in] x  X position (0..20).
 * @param[in] y  Y position (0..8).
 */
extern void Display_gotoxy (UInt8 x, UInt8 y);

/** Write on the display.
*
* @param[in] line The text to display.
* @param[in] length The length of the text to display (1..21).
*/
void Display_write(const char * line, UInt8 length);


/** Write a bar char with given height.
 *
 * @param[in] height The height of the bar (1..21).
 */
extern void Display_writeBar (UInt8 height);

#ifdef __cplusplus
}
#endif

#endif  /* DISPLAY_H */