/*
 *  sys/types.h Defined system types
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
#ifndef __TYPES_H_INCLUDED
#define __TYPES_H_INCLUDED
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

#ifdef __cplusplus
  #ifndef _STDSIZE_T_DEFINED
    #define _STDSIZE_T_DEFINED
    namespace std {
      typedef unsigned size_t;
    }
    typedef std::size_t _w_size_t;
  #endif
  #ifndef _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED_
    using std::size_t;
  #endif
#else
  #ifndef _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED_
    typedef unsigned size_t;
    typedef size_t   _w_size_t;
  #endif
#endif /* __cplusplus */


typedef unsigned int ino_t;     /* i-node # type */
typedef int dev_t;              /* device code (drive #) */
typedef long  off_t;            /* file offset value */

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
