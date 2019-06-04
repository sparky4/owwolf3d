/*
 *  varargs.h   Variable argument macros (UNIX System V definition)
 *              definitions for use with variable argument lists
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
#ifndef _VARARGS_H_INCLUDED
#define _VARARGS_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif
#ifndef _STDARG_H_INCLUDED
 #include <stdarg.h>
#else
 #error stdarg.h has already been included
#endif

#undef va_start

#if defined(__PPC__)
 extern void *__builtin_va_start(va_list*, ...);
 #pragma intrinsic(__builtin_va_start);
 #define va_alist       void *__alist, ...
 #define va_dcl
 #define va_start(ap)   (__builtin_va_start(&ap,__alist,0))
#elif defined(__AXP__)
 extern void *__builtin_va_start(va_list*, ...);
 #pragma intrinsic(__builtin_va_start);
 #define va_alist       void *__alist, ...
 #define va_dcl
 #define va_start(ap)   (__builtin_va_start(&ap,__alist,0))
#else
 #define va_alist       void *__alist, ...
 #define va_dcl
 #if defined(__HUGE__) || defined(__SW_ZU)
  #define va_start(ap)  ((ap)[0]=(char _WCFAR*)&__alist,(void)0)
 #else
  #define va_start(ap)  ((ap)[0]=(char *)&__alist,(void)0)
 #endif
#endif

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
