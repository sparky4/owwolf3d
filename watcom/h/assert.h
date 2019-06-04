/*
 *  assert.h
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
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#undef assert

#ifdef NDEBUG
 #define assert(ignore) ((void)0)
#else
 _WCRTLINK extern void _assert( char *, char *, int );
 _WCRTLINK extern void _wassert( char *, char *, int );
 _WCRTLINK extern void __assert( int, char *, char *, int );
 _WCRTLINK extern void __wassert( int, char *, char *, int );
 _WCRTLINK extern void _assert99( char *, char *, char *, int );
 _WCRTLINK extern void _wassert99( char *, char *, char *, int );
 _WCRTLINK extern void __assert99( int, char *, char *, char *, int );
 _WCRTLINK extern void __wassert99( int, char *, char *, char *, int );
#if !defined(NO_EXT_KEYS) /* extensions enabled */
 #ifdef __C89
  #ifdef _UNICODE
   #define assert(expr)   ((expr)?(void)0:_wassert(#expr,__FILE__,__LINE__))
  #else
   #define assert(expr)   ((expr)?(void)0:_assert(#expr,__FILE__,__LINE__))
  #endif
 #else
  #ifdef _UNICODE
   #define assert(expr)   ((expr)?(void)0:_wassert99(#expr,__func__,__FILE__,__LINE__))
  #else
   #define assert(expr)   ((expr)?(void)0:_assert99(#expr,__func__,__FILE__,__LINE__))
  #endif
 #endif
#else
 #ifdef __C89
  #ifdef _UNICODE
   #define assert(expr)  __wassert(expr,#expr,__FILE__,__LINE__)
  #else
   #define assert(expr)  __assert(expr,#expr,__FILE__,__LINE__)
  #endif
 #else
  #ifdef _UNICODE
   #define assert(expr)  __wassert99(expr,#expr,__func__,__FILE__,__LINE__)
  #else
   #define assert(expr)  __assert99(expr,#expr,__func__,__FILE__,__LINE__)
  #endif
 #endif
#endif
#endif
#ifdef __cplusplus
} /* End of extern "C" */
#endif

