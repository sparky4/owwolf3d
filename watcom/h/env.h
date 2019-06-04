/*
 *  env.h       Environment string operations
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
#ifndef _ENV_H_INCLUDED
#define _ENV_H_INCLUDED
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

/*
 *  POSIX 1003.1 Prototypes.
 */
_WCRTLINK extern int   clearenv( void );
#ifdef __cplusplus
namespace std {
#endif
_WCRTLINK extern char *getenv( const char *__name );
#ifdef __cplusplus
}
using std::getenv;
#endif
_WCRTLINK extern int   setenv( const char *__name, const char *__newvalue,
                              int __overwrite );
_WCRTLINK extern void unsetenv(const char *name);
#if !defined(NO_EXT_KEYS) /* extensions enabled */
_WCRTLINK extern int   putenv( const char *__env_string );
#endif

_WCRTLINK extern int       _setenv( const char *__name, const char *__newvalue,
                                    int __overwrite );
_WCRTLINK extern wchar_t * _wgetenv( const wchar_t *__name );
_WCRTLINK extern int       _wsetenv( const wchar_t *__name,
                                      const wchar_t *__newvalue,
                                      int __overwrite );
_WCRTLINK extern int       _wputenv( const wchar_t *__env_string );

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
