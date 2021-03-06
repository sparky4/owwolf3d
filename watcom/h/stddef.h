/***************************************************************************
 * FILE: stddef.h/cstddef (Standard definitions)
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
 * Description: This header is part of the C/C++ standard library. It
 *              introduces certain commonly needed type names and
 *              supplies the offsetof macro.
 ***************************************************************************/
#ifndef _STDDEF_H_INCLUDED
#define _STDDEF_H_INCLUDED

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
#include <cstddef>

// C99 types in stddef.h
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
#define _SIZE_T_DEFINED_
using std::size_t;
#endif
#ifndef _PTRDIFF_T_DEFINED
#define _PTRDIFF_T_DEFINED
#define _PTRDIFF_T_DEFIEND_
using std::ptrdiff_t;
#endif

#else /* __cplusplus not defined */


#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

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

#ifdef __cplusplus
  #ifndef _STDWCHAR_T_DEFINED
    #define _STDWCHAR_T_DEFINED
    namespace std {
      typedef long char wchar_t;
    }
  #endif
  #ifndef _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED_
    using std::wchar_t;
  #endif
#else
  #ifndef _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED_
    typedef unsigned short wchar_t;
  #endif
#endif /* __cplusplus */

#ifndef NULL
#if defined( __SMALL__ ) || defined( __MEDIUM__ ) || defined( __386__ ) || defined( __AXP__ ) || defined( __PPC__ )
#define                         NULL                    0
#else
#define                         NULL                    0L
#endif
#endif

#ifdef __cplusplus
  #ifndef _STDPTRDIFF_T_DEFINED
    #define _STDPTRDIFF_T_DEFINED
    namespace std {
      #if defined(__HUGE__)
        typedef long ptrdiff_t;
      #else
        typedef int ptrdiff_t;
      #endif
    }
  #endif
  #ifndef _PTRDIFF_T_DEFINED
    #define _PTRDIFF_T_DEFINED
    #define _PTRDIFF_T_DEFINED_
    using std::ptrdiff_t;
  #endif
#else
  #ifndef _PTRDIFF_T_DEFINED
    #define _PTRDIFF_T_DEFINED
    #define _PTRDIFF_T_DEFINED_
    #if defined(__HUGE__)
      typedef long ptrdiff_t;
    #else
      typedef int ptrdiff_t;
    #endif
  #endif
#endif /* __cplusplus */

#ifdef __cplusplus
#define offsetof(__typ,__id) __offsetof(__typ,__id)
#else
#define offsetof(__typ,__id) ((size_t)&(((__typ*)0)->__id))
#endif
#if !defined(NO_EXT_KEYS) /* extensions enabled */
#if defined(__386__) || defined(__AXP__) || defined(__PPC__)
 _WCRTLINK extern int *__threadid(void); /* pointer to thread id */
 #define _threadid (__threadid())
 #if defined(__NT__)
  _WCRTLINK extern unsigned long __threadhandle( void );
 #endif
#else
 _WCRTLINK extern int _WCFAR *_threadid;        /* pointer to thread id */
#endif
#endif


#endif

#endif
