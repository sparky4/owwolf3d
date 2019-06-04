/***************************************************************************
 * FILE: stdint.h (Various integer type definitions)
 *
 *                          Open Watcom Project
 *
 *    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
 *
 *  ========================================================================
 *
 *    This file contains Original Code and/or Modifications of Original
 *    Code as defined in and that are subject to the Sybase Open Watcom
 *    Public License version 1.0 (the 'License'). You may not use this file
 *    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
 *    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
 *    provided with the Original Code and Modifications, and is also
 *    available at www.sybase.com/developer/opensource.
 *
 *    The Original Code and all software distributed under the License are
 *    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 *    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
 *    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
 *    NON-INFRINGEMENT. Please see the License for the specific language
 *    governing rights and limitations under the License.
 *
 *  ========================================================================
 *
 * Description: This header is part of the C standard library. It
 *              describes a number of useful integer types.
 ***************************************************************************/
#ifndef _STDINT_H_INCLUDED
#define _STDINT_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif


 /*          char | short | int | long | long long                        */
 /* 16 bit : 8      16      16    32     64     All 16 bit targets.       */
 /* 32 bit : 8      16      32    32     64     __386__, __AXP__, __PPC__ */
 /* 64 bit : 8      16      32    64     64     No targets implemented.   */

/* Exact-width types. */
typedef signed   char      int8_t;
typedef signed   short     int16_t;
typedef signed   long      int32_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
typedef signed   long long int64_t;
typedef unsigned long long uint64_t;

/* Minimum-width types. */
typedef signed   char      int_least8_t;
typedef signed   short     int_least16_t;
typedef signed   long      int_least32_t;
typedef signed   long long int_least64_t;

typedef unsigned char      uint_least8_t;
typedef unsigned short     uint_least16_t;
typedef unsigned long      uint_least32_t;
typedef unsigned long long uint_least64_t;

/* Fastest minimum-width types. */
typedef signed   int       int_fast8_t;
typedef signed   int       int_fast16_t;
typedef signed   long      int_fast32_t;
typedef signed   long long int_fast64_t;

typedef unsigned int       uint_fast8_t;
typedef unsigned int       uint_fast16_t;
typedef unsigned long      uint_fast32_t;
typedef unsigned long long uint_fast64_t;

/* Integer types able to hold *object* pointers. */
#if defined(__386__) || defined(__AXP__) || defined (__PPC__)
#if defined(__COMPACT__) || defined(__LARGE__)
typedef long long intptr_t;
typedef unsigned long long uintptr_t;
#else
typedef long intptr_t;
typedef unsigned long uintptr_t;
#endif
#else /* 16 bit */
#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
typedef long intptr_t;
typedef unsigned long uintptr_t;
#else
typedef int intptr_t;
typedef unsigned int uintptr_t;
#endif
#endif

/* Greatest-width types. */
typedef long long intmax_t;
typedef unsigned long long uintmax_t;

/* Limit macros. According to C99 these are only visible in C++ programs
   if __STDC_LIMIT_MACROS is defined. */
#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

/* Exact-width types. */
#define INT8_MIN   (-128)
#define INT8_MAX   127
#define INT16_MIN  (-32767-1)
#define INT16_MAX  32767
#define INT32_MIN  (-2147483647L-1)
#define INT32_MAX  2147483647L
#define INT64_MIN  (-9223372036854775807LL-1)
#define INT64_MAX  9223372036854775807LL

#define UINT8_MAX   255
#if defined(__386__) || defined(__AXP__) || defined(__PPC__)
#define UINT16_MAX  65535
#else
#define UINT16_MAX  65535U
#endif
#define UINT32_MAX  4294967295UL
#define UINT64_MAX  18446744073709551615ULL

/* Minimum-width types. */
#define INT_LEAST8_MIN   (-128)
#define INT_LEAST8_MAX   127
#define INT_LEAST16_MIN  (-32767-1)
#define INT_LEAST16_MAX  32767
#define INT_LEAST32_MIN  (-2147483647L-1)
#define INT_LEAST32_MAX  2147483647L
#define INT_LEAST64_MIN  (-9223372036854775807LL-1)
#define INT_LEAST64_MAX  9223372036854775807LL

#define UINT_LEAST8_MAX   255
#if defined(__386__) || defined(__AXP__) || defined(__PPC__)
#define UINT_LEAST16_MAX  65535
#else
#define UINT_LEAST16_MAX  65535U
#endif
#define UINT_LEAST32_MAX  4294967295UL
#define UINT_LEAST64_MAX  18446744073709551615ULL

/* Fast minimum-width types. */
#if defined(__386__) || defined(__AXP__) || defined(__PPC__)
#define INT_FAST8_MIN   (-2147483647-1)
#define INT_FAST8_MAX   2147483647
#define INT_FAST16_MIN  (-2147483647-1)
#define INT_FAST16_MAX  2147483647
#else
#define INT_FAST8_MIN   (-32767-1)
#define INT_FAST8_MAX   32767
#define INT_FAST16_MIN  (-32767-1)
#define INT_FAST16_MAX  32767
#endif
#define INT_FAST32_MIN  (-2147483647L-1)
#define INT_FAST32_MAX  2147483647L
#define INT_FAST64_MIN  (-9223372036854775807LL-1)
#define INT_FAST64_MAX  9223372036854775807LL

#if defined(__386__) || defined(__AXP__) || defined(__PPC__)
#define UINT_FAST8_MAX   4294967295U
#define UINT_FAST16_MAX  4294967295U
#else
#define UINT_FAST8_MAX   65535U
#define UINT_FAST16_MAX  65535U
#endif
#define UINT_FAST32_MAX  4294967295UL
#define UINT_FAST64_MAX  18446744073709551615ULL

/* Integer types able to hold object pointers. */
#if defined(__386__) || defined(__AXP__) || defined (__PPC__)
#if defined(__COMPACT__) || defined(__LARGE__)
#define INTPTR_MIN   (-9223372036854775807LL-1)
#define INTPTR_MAX   9223372036854775807LL
#define UINTPTR_MAX  18446744073709551615ULL
#else
#define INTPTR_MIN   (-2147483647L-1)
#define INTPTR_MAX   2147483647L
#define UINTPTR_MAX  4294967295UL
#endif
#else /* 16 bit */
#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
#define INTPTR_MIN   (-2147483647L-1)
#define INTPTR_MAX   2147483647L
#define UINTPTR_MAX  4294967295UL
#else
#define INTPTR_MIN   (-32767-1)
#define INTPTR_MAX   32767
#define UINTPTR_MAX  65535U
#endif
#endif

/* Greatest-width types. */
#define INTMAX_MIN   (-9223372036854775807LL-1)
#define INTMAX_MAX   9223372036854775807LL
#define UINTMAX_MAX  18446744073709551615ULL

/* Limits of other integer types. */
#if defined(__386__) || defined(__AXP__) || defined(__PPC__)
#define PTRDIFF_MIN     (-2147483647-1)
#define PTRDIFF_MAX     2147483647
#define SIG_ATOMIC_MIN  (-2147483647-1)
#define SIG_ATOMIC_MAX  2147483647
#define SIZE_MAX        4294967295U
#define WCHAR_MIN       0
#define WCHAR_MAX       65535
#define WINT_MIN        WCHAR_MIN
#define WINT_MAX        WCHAR_MAX
#else /* 16 bit */
#if defined(__HUGE__)
#define PTRDIFF_MIN     (-2147483647L-1)
#define PTRDIFF_MAX     2147483647L
#else
#define PTRDIFF_MIN     (-32767-1)
#define PTRDIFF_MAX     32767
#endif
#define SIG_ATOMIC_MIN  (-32767-1)
#define SIG_ATOMIC_MAX  32767
#define SIZE_MAX        65535U
#define WCHAR_MIN       0
#define WCHAR_MAX       65535U
#define WINT_MIN        WCHAR_MIN
#define WINT_MAX        WCHAR_MAX
#endif

#endif /* Limit macros. */

/* Constant macros. According to C99 these are only visible in C++ programs
   if __STDC_CONSTANT_MACROS is defined. */
#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

#define INT8_C(x)    (x)
#define INT16_C(x)   (x)
#define INT32_C(x)   (x ## L)
#define INT64_C(x)   (x ## LL)

#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  (x ## UL)
#define UINT64_C(x)  (x ## ULL)

#define INTMAX_C(x)  (x ## LL)
#define UINTMAX_C(x) (x ## ULL)

#endif /* Constant macros. */

#endif
