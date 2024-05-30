/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/ErrorHandler.c $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       ErrorHandler.c

ErrorHandler for all error processing.

@version    %$Id: ErrorHandler.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include "os/ErrorHandler.h"

#include "hal/Hal.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/** Error Handler
 * @param[in] errorCode The error code.
 * @param[in] halt Flag indicating if we should switch to a safe state.
 * */
static void handleError (ErrorHandlerErrorCode errorCode, bool halt);

/* VARIABLES **************************************************************************************/

/** Print callback for showing 2 line error messages. */
static PrintCallback gPrintCallback = NULL;

/** Error callback handler */
static ErrorCallback gErrorCallback = NULL;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void ErrorHandler_setPrintCallback(PrintCallback callback)
{
    gPrintCallback = callback;
}

void ErrorHandler_setErrorCallback(ErrorCallback callback)
{
    gErrorCallback = callback;
}

void ErrorHandler_show(ErrorHandlerErrorCode errorCode)
{
    handleError(errorCode, false);
}

void ErrorHandler_halt(ErrorHandlerErrorCode errorCode)
{
    handleError(errorCode, true);
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void handleError(ErrorHandlerErrorCode errorCode, bool halt)
{
    if (NULL != gPrintCallback)
    {
        char secondline[9];
        char * p;

        /* Write error code to line 2 (maxium is 9999)*/
        p = secondline;

        if (errorCode >= 1000)
        {
            *p++ = '0' + (errorCode / 1000);
            errorCode %= 1000;
        }
        else
        {
            *p++ = '0';
        }

        if (errorCode >= 100)
        {
            *p++ = '0' + (errorCode / 100);
            errorCode %= 100;
        }
        else
        {
            *p++ = '0';
        }

        if (errorCode >= 10)
        {
            *p++ = '0' + (errorCode / 10);
            errorCode %= 10;
        }
        else
        {
            *p++ = '0';
        }

        *p++ = '0' + errorCode;

        /* Fill end with blanks and null byte. */
        while (p < &secondline[sizeof(secondline)-2])
        {
            *p++ = ' ';
        }
        *p = '\0';

        gPrintCallback("!ERROR! ", secondline);
    }

    if (NULL != gErrorCallback)
    {
        gErrorCallback(errorCode);
    }

    if (halt)
    {
        for(;;)
        {
            Hal_halt();
        }
    } 
}