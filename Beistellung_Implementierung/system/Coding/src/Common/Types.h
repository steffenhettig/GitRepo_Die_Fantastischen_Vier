/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/Common/Types.h $
***************************************************************************************************/
/**
@addtogroup Global_group Global
@{
@file       Types.h

Basic types definition for ATmega128

@version    %$Id: Types.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/
/* If C99 used, use the boolean types of C99. */
#if __STDC_VERSION__ == 199901L
/* C99 */
#include <stdbool.h>
#endif

/* CONSTANTS **************************************************************************************/
/** NULL */
#ifndef NULL
#define NULL ((void *) 0)
#endif

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/* The following base types shall be used for software development. */
/* TODO: In dependence to the used compiler / processor define the remaining base types! */

/** Boolean data type */
#ifdef __bool_true_false_are_defined
/* If C99 used, use the boolean types of C99. */
#define Bool    bool
#define FALSE   false
#define TRUE    true
#else
/* If C99 not used (stdbool.h is not included), define the boolean types as follows. */
typedef enum
{
    FALSE = 0,
    TRUE = 1
} Bool;
#endif

typedef unsigned char               Byte;       /**< 8Bit data type */

typedef unsigned char               UChar;      /**< 8Bit character ASCII256 */
typedef signed char                 Char;       /**< 8Bit character ASCII128 */

typedef signed char                 Int8;       /**< 8Bit signed integer */
typedef unsigned char               UInt8;      /**< 8Bit unsigned integer */
typedef volatile unsigned char      VUInt8;     /**< 8Bit volatile unsigned integer */
typedef volatile signed char        VInt8;      /**< 8Bit volatile integer */

typedef signed int                  Int16;      /**< 16Bit signed integer */
typedef unsigned int                UInt16;     /**< 16Bit unsigned integer */
typedef volatile unsigned int       VUInt16;    /**< 16Bit volatile unsigned integer */
typedef volatile signed int         VInt16;     /**< 16Bit volatile unsigned integer */

typedef signed long                 Int32;      /**< 32Bit signed integer */
typedef unsigned long               UInt32;     /**< 32Bit unsigned integer */
typedef volatile unsigned long      VUInt32;    /**< 32Bit volatile unsigned integer */
typedef volatile signed long        VInt32;     /**< 32Bit volatile unsigned integer */

typedef signed long long            Int64;      /**< 64Bit signed integer */
typedef unsigned long long          UInt64;     /**< 64Bit unsigned integer */
typedef volatile unsigned long long VUInt64;    /**< 64Bit volatile unsigned integer */
typedef volatile signed long long   VInt64;     /**< 64Bit volatile unsigned integer */

typedef float                       Float32;    /**< 32Bit floating point */
typedef volatile float              VFloat32;   /**< 32Bit floating point */

typedef long double                 Float64;    /**< 32Bit floating point */
typedef volatile long double        VFloat64;   /**< 32Bit floating point */

typedef VUInt8                      Reg8;       /**< 8Bit register */
typedef VUInt16                     Reg16;      /**< 16Bit register */
typedef VUInt32                     Reg32;      /**< 32Bit register */
typedef VUInt64                     Reg64;      /**< 64Bit register */

#ifdef __cplusplus
}
#endif

#endif  /* TYPES_H */