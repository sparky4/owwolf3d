/*
 *  sys/timeb.h     timeb structure used with ftime()
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
 */
#ifndef __TIMEB_H_INCLUDED
#define __TIMEB_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#ifdef __cplusplus
  #ifndef _STDTIME_T_DEFINED
    #define _STDTIME_T_DEFINED
    namespace std {
      typedef unsigned long time_t;
    }
  #endif
  #ifndef _TIME_T_DEFINED
    #define _TIME_T_DEFINED
    #define _TIME_T_DEFINED_
    using std::time_t;
  #endif
#else
  #ifndef _TIME_T_DEFINED
    #define _TIME_T_DEFINED
    #define _TIME_T_DEFINED_
    typedef unsigned long time_t;
  #endif
#endif /* __cplusplus */

#ifdef _M_IX86
#pragma pack( __push, 1 )
#else
#pragma pack( __push, 8 )
#endif
struct timeb {
    time_t              time;           /* seconds since Jan 1, 1970 UTC */
    unsigned short      millitm;        /* milliseconds */
    short               timezone;       /* difference in minutes from UTC */
    short               dstflag;        /* nonzero if daylight savings time */
};
struct _timeb {
    time_t              time;           /* seconds since Jan 1, 1970 UTC */
    unsigned short      millitm;        /* milliseconds */
    short               timezone;       /* difference in minutes from UTC */
    short               dstflag;        /* nonzero if daylight savings time */
};
#pragma pack( __pop )

_WCRTLINK extern int ftime( struct timeb *__timeptr );
_WCRTLINK extern int _ftime( struct _timeb *__timeptr );

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
