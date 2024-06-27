/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/os/ErrorHandler.h $
***************************************************************************************************/
/**
@addtogroup Os
@{
@file       ErrorHandler.h

Enter short description here.

Enter detailed description here.

@version    %$Id: ErrorHandler.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/

#include "Types.h"

/* CONSTANTS **************************************************************************************/


/* MACROS *****************************************************************************************/


/* TYPES ******************************************************************************************/

/** All error codes. */
typedef enum tag_ErrorCode
{
    ERRORHANDLER_MAIN_SCHEDULER_EXIT           =   1,   /**< Unexpected scheduler return.                           */
    ERRORHANDLER_MAINTASK_INIT_FAIL            = 100,   /**< Initialization of main task failed.                    */
    ERRORHANDLER_STARTUP_INIT_FAIL             = 110,   /**< Startup state init error                               */
    ERRORHANDLER_STARTUP_UNINIT_FAIL           = 111,   /**< Startup state unint error                              */
    ERRORHANDLER_CALIBRATE_TIMER_INIT_FAIL     = 200,   /**< Timer initialization in state CALIBRATION failed.      */
    ERRORHANDLER_CALIBRATE_ALOGRITMIC_FAIL     = 201,   /**< Logical failure in state CALIBRATION.                  */
    ERRORHANDLER_CALIBRATE_TIMEOUT             = 202,   /**< Timeout in state CALIBRATIION.                         */
    ERRORHANDLER_CALIBRATE_TIMER_UNINIT_FAIL   = 203,   /**< Timer uninitialization failed in state CALIBRATION.    */
    ERRORHANDLER_RELEASETRACK_TIMER_INIT_FAIL  = 250,   /**< Timer initialization in state RELEASE_TRACK failed.    */
    ERRORHANDLER_RELEASETRACK_TIMER_UNINIT_FAIL= 251,   /**< Timer uninitialization failed in state RELEASE_TRACK.  */
    ERRORHANDLER_RELEASETRACK_TIMER_START_FAIL = 252,   /**< Timer start failed in state RELEASE_TRACK.             */
    ERRORHANDLER_DRIVING_TIMER_INIT_FAIL       = 300,   /**< Timer initialization in state DRIVING failed.          */
    ERRORHANDLER_DRIVING_TIMER_UNINIT_FAIL     = 301,   /**< Timer uninitialization failed in state DRIVING.        */
    ERRORHANDLER_DRIVING_TIMER_START_FAIL      = 302,   /**< Timer start failed in state DRIVING.                   */
    ERRORHANDLER_DRIVING_TIMEOUT               = 303,   /**< Timeout in state DRIVING.                              */
    ERRORHANDLER_LINELOST_TIMEOUT              = 304,   /**< Timeout in state LINE LOST.                            */
    ERRORHANDLER_FINISH_TIMER_INIT_FAIL        = 400,   /**< Finish state timer register error                      */
    ERRORHANDLER_FINISH_TIMER_UNINIT_FAIL      = 401,   /**< Finish state timer un-register error                   */
} ErrorHandlerErrorCode;

/** Callback function type for showing errror codes "somewhere".
 * Assume 2 lines auf output is possible (@see Display).
 */
typedef void (*PrintCallback)(const char * line1, const char * line2);

/**  Callback function type for app specific error handling 
 */
typedef void (*ErrorCallback)(ErrorHandlerErrorCode errorcode);

/* PROTOTYPES *************************************************************************************/

/** Show error code and halt system.
 *
 * @param[in] errorCode Numeric error code to display.
 */
extern void ErrorHandler_halt (ErrorHandlerErrorCode errorCode);

/** Show error code and return.
 *
 * @param[in] errorCode Numeric error code to display.
 */
extern void ErrorHandler_show (ErrorHandlerErrorCode errorCode);

/** Install error message output handler via callback interface.
 *
 * @param[in] callback callback function for displaying error message.
 */
extern void ErrorHandler_setPrintCallback (PrintCallback callback);

/** Install application error handler
 *
 * @param[in] callback callback function error handling.
 */
extern void ErrorHandler_setErrorCallback (ErrorCallback callback);

#ifdef __cplusplus
}
#endif

#endif  /* ERRORHANDLER_H */