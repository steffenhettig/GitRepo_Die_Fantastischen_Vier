/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/branches/%23OLED/system/Coding/src/service/Display.c $
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       Display.c

Enter short description here.

For a detailed description see the detailed description in @ref display.h.

@version    %$Id: Display.c 316 2024-03-04 14:38:23Z dembinsky $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <avr/pgmspace.h>
#include "service/Display.h"
#include "hal/Gpio.h"
#include "hal/TickTimer.h"
#include "os/ErrorHandler.h"
#include "Common/font.h"

/* CONSTANTS **************************************************************************************/

#define DISPLAY_HOME_POSITION (2u)

#define TIMEOUT_RESET (10u)

#define LOWEST_SUPPORTED_ASCII (0x20u)  /**< Space */
#define HIGHEST_SUPPORTED_ASCII (0x7Fu) /**< DEL */

#define COLUMNS_PER_CHARACTER (5u)

#define DISPLAY_MAX_COLUMNS (132u)
 
#define DISPLAY_MAX_LENGTH (21u) /**< 0..20 (DISPLAY_MAX_COLUMNS / DISPLAY_CHARACTER_WITDH) */

#define DISPLAY_MAX_LINES (8u) /**< 0..7 */
#define DISPLAY_CHARACTER_WITDH (6u) /**< Sum of COLUMS_PER_CHARACTER + 1 space between characters. */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** LCD display controller command masks. */
enum tag_Cmds
{
    CMD_FLIP_HORIZONTALLY       = 0xA1, /**< flip display output horizontally. */
    CMD_FLIP_VERTICALLY         = 0xC8, /**< flip display output vertically. */
    CMD_SET_CONTRAST            = 0x81, /**< Set contrast setting of display. Follow up with contrast value 00...FF */
    CMD_SET_DISPLAY_ON          = 0xAF, /**< Turn display on. */
    CMD_SET_COLUMN_ADDR_LOW     = 0x00, /**< Sets 4 lower bits of column address of display. */
    CMD_SET_COLUMN_ADDR_HIGH    = 0x10, /**< Sets 4 higher bits of column address of display. */
    CMD_SET_PAGE_ADDR           = 0xB0, /**< Specifies page address. */
};

/* PROTOTYPES *************************************************************************************/

/** Save multiplexed pins tates and reprogram to display usage. */
static void switchPins (void);

/** Restore multiplexed pins after LCD usage. */
static void restorePins (void);

/** Callback function for displaying error messages on screen.
 *
 * @param[in] line1 First line to display.
 * @param[in] line2 Second line to display.
 */
static void displayError (const char * line1, const char * line2);

/**
 * Write a single character to the OLED.
 * Character is converted to multiple bytes fitting the font.
 *
 * @param[in] glyph The ASCII symbol to write..
 */
static void writeSingleCharacter (UInt8 glyph);

/**
 * Write 8 bit data to the OLED.
 * Set Data/Command pin before using this method.
 *
 * @param[in] byte The byte to send.
 */
static void writeByte (UInt8 byte);

/**
 * Write one line of error code up to maximum display length.
 *
 * @param[in] line1 First line to display.
 */
static void writeError (const char * line);

/**
 * Draw a bar with the size of one character
 */
static void drawBar (void);

/* VARIABLES **************************************************************************************/

/** Storage for saving multiplexed pin states. */
static UInt8 savedPins[2];

/** Flag to check if initialisation was executed. */
static bool isInitialized = false;

/** Current xPosition */
static UInt8 xPosition = 0;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void Display_init(void)
{
    switchPins();

    /* Reset OLED. */
    Gpio_write(GPIO_OLED_RS, GPIO_STATE_OFF);
    TICKTIMER_DELAY_US(TIMEOUT_RESET);
    Gpio_write(GPIO_OLED_RS, GPIO_STATE_ON);
    TICKTIMER_DELAY_US(TIMEOUT_RESET);

    /* Enter command mode. */
    Gpio_write(GPIO_OLED_DC, GPIO_STATE_OFF);

    writeByte(CMD_FLIP_HORIZONTALLY);
    writeByte(CMD_FLIP_VERTICALLY);

    /* maximum brightness */
    writeByte(CMD_SET_CONTRAST);
    writeByte(0xFF);

    writeByte(CMD_SET_DISPLAY_ON);

    restorePins();

    isInitialized = true;
    
    Display_clear();

    /* Route error messages to display. */
    ErrorHandler_setPrintCallback(displayError);
}

void Display_clear(void)
{
    if (isInitialized)
    {
        switchPins();

        /* Enter command mode. */
        Gpio_write(GPIO_OLED_DC, GPIO_STATE_OFF);

        writeByte(CMD_SET_COLUMN_ADDR_LOW | (DISPLAY_HOME_POSITION & 0x0Fu));

        for (UInt8 page = 0; page < DISPLAY_MAX_LINES; page++)
        {
            /* Enter command mode. */
            Gpio_write(GPIO_OLED_DC, GPIO_STATE_OFF);

            writeByte(CMD_SET_PAGE_ADDR | page);
            writeByte(CMD_SET_COLUMN_ADDR_HIGH | (DISPLAY_HOME_POSITION >> 4u));

            Display_clearLine();
        }

        restorePins();

        Display_gotoxy(0, 0);
    }
}

void Display_clearLine(void)
{
    /* Enter data mode. */
    Gpio_write(GPIO_OLED_DC, GPIO_STATE_ON);
    for (UInt8 column = 0; column < DISPLAY_MAX_COLUMNS; column++)
    {
        writeByte(0);
    }
}

void Display_home(void)
{
    if (isInitialized)
    {
        switchPins();

        /* Enter command mode. */
        Gpio_write(GPIO_OLED_DC, GPIO_STATE_OFF);

        writeByte(CMD_SET_PAGE_ADDR | 0);
        writeByte(CMD_SET_COLUMN_ADDR_HIGH | (DISPLAY_HOME_POSITION >> 4u));
        writeByte(CMD_SET_COLUMN_ADDR_LOW | (DISPLAY_HOME_POSITION & 0x0Fu));

        restorePins();
    }
}

void Display_gotoxy(UInt8 x, UInt8 y)
{
    if ((isInitialized) && (x < DISPLAY_MAX_LENGTH) && (y < DISPLAY_MAX_LINES))
    {
        UInt8 displayXPosition = DISPLAY_HOME_POSITION + (x * DISPLAY_CHARACTER_WITDH);

        xPosition = x;
        switchPins();

        /* Enter command mode. */
        Gpio_write(GPIO_OLED_DC, GPIO_STATE_OFF);

        writeByte(CMD_SET_PAGE_ADDR | y);

        writeByte(CMD_SET_COLUMN_ADDR_HIGH | (displayXPosition >> 4u));
        writeByte(CMD_SET_COLUMN_ADDR_LOW | (displayXPosition & 0xFu));

        restorePins();
    }
}

void Display_write(const char * line, UInt8 length)
{
    if (!isInitialized)
    {
        return;
    }

    UInt8 maxLength = DISPLAY_MAX_LENGTH - xPosition;

    if (length > maxLength)
    {
        length = maxLength;
    }

    switchPins();

    /* Enter data mode. */
    Gpio_write(GPIO_OLED_DC, GPIO_STATE_ON);

    for (UInt8 charIndex = 0; charIndex < length; charIndex++)
    {
        writeSingleCharacter(line[charIndex]);
    }
    
    restorePins();
}

void Display_writeBar(UInt8 height)
{
    UInt8 maxLength = DISPLAY_MAX_LENGTH - xPosition;

    if (height > maxLength)
    {
        height = maxLength;
    }

    for (UInt8 bar = 0; bar < height; bar++)
    {
        drawBar();
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/
static void drawBar (void)
{
    for (UInt8 pixelX = 0; pixelX < 5; pixelX++)
    {
        writeByte(0x7F);
    }

    /* Empty column / space after each symbol. */
    writeByte(0);

    /* Increment x position for next write. */
    xPosition++;
}

static void switchPins()
{
    Gpio_alloc(GPIO_OLED_DC, &savedPins[0]);
    Gpio_alloc(GPIO_OLED_MOSI, &savedPins[1]);
}

static void restorePins()
{
    Gpio_free(GPIO_OLED_DC, savedPins[0]);
    Gpio_free(GPIO_OLED_MOSI, savedPins[1]);
}

static void displayError(const char * line1, const char * line2)
{
    if (NULL != line1)
    {
        Display_gotoxy(0, 0);
        writeError(line1);
    }

    if (NULL != line2)
    {
        Display_gotoxy(0, 1);
        writeError(line2);
    }
}

static void writeError (const char * line)
{
    UInt8 stringEnd = 0;
    for (stringEnd = 0; stringEnd < DISPLAY_MAX_LENGTH; ++stringEnd)
    {
        if ('\0' == line[stringEnd])
        {
            break;
        }
    }

    Display_write(line, stringEnd);
}

static void writeSingleCharacter(UInt8 glyph)
{
    if ((glyph < LOWEST_SUPPORTED_ASCII) || glyph > HIGHEST_SUPPORTED_ASCII)
    {
        return;
    }

    UInt8 asciiOffset = glyph - LOWEST_SUPPORTED_ASCII;

    for (UInt8 pixelX = 0; pixelX < COLUMNS_PER_CHARACTER; pixelX++)
    {
        UInt8 column = pgm_read_byte(&pololuOledFont[asciiOffset][pixelX]);
        writeByte(column);
    }

    /* Empty column / space after each symbol. */
    writeByte(0);

    /* Increment x position for next write. */
    xPosition++;
}

static void writeByte (UInt8 data)
{
    for(int8_t bit=7; bit >= 0; bit--)
    {
        Gpio_State usedState = GPIO_STATE_OFF;

        if ((data >> bit & 1) == 1)
        {
            usedState = GPIO_STATE_ON;
        }

        Gpio_write(GPIO_OLED_SCK, GPIO_STATE_OFF);
        Gpio_write(GPIO_OLED_MOSI, usedState);
        Gpio_write(GPIO_OLED_SCK, GPIO_STATE_ON);
    }
}
