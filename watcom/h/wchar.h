/***************************************************************************
 * FILE: wchar.h/cwchar (Wide character functions)
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
 *              declares functions and types pertaining to the use of
 *              wide characters.
 ***************************************************************************/
#ifndef _WCHAR_H_INCLUDED
#define _WCHAR_H_INCLUDED

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
#include <cwchar>

// C99 types in wchar.h
using std::mbstate_t;
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
#define _SIZE_T_DEFINED_
using std::size_t;
#endif
using std::tm;              // <= Fix me?
#ifndef _WCTYPE_T_DEFINED
#define _WCTYPE_T_DEFINED
#define _WCTYPE_T_DEFINED_
using std::wint_t;
#endif

// C99 functions in wchar.h
using std::fgetwc;
using std::fgetws;
using std::fputwc;
using std::fputws;
using std::fwprintf;
using std::fwscanf;
using std::getwc;
using std::getwchar;
using std::mbrlen;
using std::mbrtowc;
using std::mbsrtowcs;
using std::putwc;
using std::putwchar;
using std::swprintf;
using std::swscanf;
using std::ungetwc;
using std::vfwprintf;
using std::vswprintf;
using std::vwprintf;
using std::vwscanf;
using std::wcrtomb;
using std::wcscat;
using std::wcschr;
using std::wcscmp;
using std::wcscoll;
using std::wcscpy;
using std::wcscspn;
using std::wcsftime;
using std::wcslen;
using std::wcsncat;
using std::wcsncmp;
using std::wcsncpy;
using std::wcspbrk;
using std::wcsrchr;
using std::wcsrtombs;
using std::wcsspn;
using std::wcsstr;
using std::wcstod;
using std::wcstok;
using std::wcstol;
using std::wcstoll;
using std::wcstoul;
using std::wcstoull;
using std::wcsxfrm;
using std::wctob;
using std::wprintf;
using std::wscanf;

#if !defined(NO_EXT_KEYS) /* extensions enabled */

// C99 functions not specified for wchar.h
using std::iswalnum;
using std::iswalpha;
using std::iswascii;
using std::iswcntrl;
using std::iswdigit;
using std::iswgraph;
using std::iswlower;
using std::iswprint;
using std::iswpunct;
using std::iswspace;
using std::iswupper;
using std::iswxdigit;
using std::iswctype;
using std::wctype;
using std::towlower;
using std::towupper;
#endif

#else /* __cplusplus not defined */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif
#ifndef _IO_H_INCLUDED
    #include <io.h>
#endif
#ifndef _STAT_H_INCLUDED
    #include <sys/stat.h>
#endif
#ifndef _STDARG_H_INCLUDED
    #include <stdarg.h>
#endif
#ifndef _STDIO_H_INCLUDED
    #include <stdio.h>
#endif
#ifndef _STDLIB_H_INCLUDED
    #include <stdlib.h>
#endif
#ifndef _TIME_H_INCLUDED
    #include <time.h>
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

#ifdef __cplusplus
  #ifndef _STDWCTYPE_T_DEFINED
    #define _STDWCTYPE_T_DEFINED
    namespace std {
      typedef wchar_t wint_t;
      typedef wchar_t wctype_t;
    }
  #endif
  #ifndef _WCTYPE_T_DEFINED
    #define _WCTYPE_T_DEFINED
    #define _WCTYPE_T_DEFINED_
    using std::wint_t;
    using std::wctype_t;
  #endif
#else
  #ifndef _WCTYPE_T_DEFINED
    #define _WCTYPE_T_DEFINED
    #define _WCTYPE_T_DEFINED_
    typedef wchar_t wint_t;
    typedef wchar_t wctype_t;
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

#ifndef NULL
#if defined( __SMALL__ ) || defined( __MEDIUM__ ) || defined( __386__ ) || defined( __AXP__ ) || defined( __PPC__ )
#define                         NULL                    0
#else
#define                         NULL                    0L
#endif
#endif

#ifndef MB_CUR_MAX
    #define MB_CUR_MAX  2
#endif
#ifndef MB_LEN_MAX
    #define MB_LEN_MAX  2
#endif

typedef int mbstate_t;

_WCRTLINK extern wint_t   fgetwc( FILE *__strm );
_WCRTLINK extern wchar_t *fgetws( wchar_t *__s, int __n, FILE *__strm );
_WCRTLINK extern wint_t   fputwc( wint_t __wc, FILE *__strm );
_WCRTLINK extern int      fputws( const wchar_t *__s, FILE *__strm );
_WCRTLINK extern int      fwprintf( FILE *__strm, const wchar_t *__format, ... );
_WCRTLINK extern int      fwscanf( FILE *__strm, const wchar_t *__format, ... );
_WCRTLINK extern wint_t   getwc( FILE *__strm );
_WCRTLINK extern wint_t   getwchar( void );
_WCRTLINK extern int      mbrlen( const char *__s, size_t __n, mbstate_t *__ps );
_WCRTLINK extern int      mbrtowc( wchar_t *__pwc, const char *__s, size_t __n, mbstate_t *__ps );
_WCRTLINK extern size_t   mbsrtowcs( wchar_t *__dst, const char **__src, size_t __n, mbstate_t *__ps );
_WCRTLINK extern wint_t   putwc( wint_t __wc, FILE *__strm );
_WCRTLINK extern wint_t   putwchar( wint_t __wc );
#ifndef swprintf
_WCRTLINK extern int      swprintf( wchar_t *__s, size_t __n, const wchar_t *__format, ... );
#endif
_WCRTLINK extern int      swscanf( const wchar_t *__s, const wchar_t *__format, ... );
_WCRTLINK extern wint_t   ungetwc( wint_t __wc, FILE *__strm );
_WCRTLINK extern int      vfwprintf( FILE *__strm, const wchar_t *__format, va_list __arg );
_WCRTLINK extern int      vswprintf( wchar_t *__s, size_t __n, const wchar_t *__format, va_list __arg );
_WCRTLINK extern int      vwprintf( const wchar_t *__format, va_list __arg );
_WCRTLINK extern int      vwscanf( const wchar_t *, __va_list );
_WCRTLINK extern int      wcrtomb( char *__s, wchar_t __wc, mbstate_t *__ps );
_WCRTLINK extern wchar_t *wcscat( wchar_t *__s1, const wchar_t *__s2 );
_WCRTLINK extern wchar_t *wcschr( const wchar_t *__s, wint_t __wc );
_WCRTLINK extern int      wcscmp( const wchar_t *__s1, const wchar_t *__s2 );
_WCRTLINK extern int      wcscoll( const wchar_t *__s1, const wchar_t *__s2 );
_WCRTLINK extern wchar_t *wcscpy( wchar_t *__s1, const wchar_t *__s2 );
_WCRTLINK extern size_t   wcscspn( const wchar_t *__s1, const wchar_t *__s2 );
_WCRTLINK extern size_t   wcsftime( wchar_t *__s, size_t __maxsize, const wchar_t *__format, const struct tm *__timeptr );
_WCRTLINK extern size_t   wcslen( const wchar_t *__s );
_WCRTLINK extern wchar_t *wcsncat( wchar_t *__s1, const wchar_t *__s2, size_t __n );
_WCRTLINK extern int      wcsncmp( const wchar_t *__s1, const wchar_t *__s2, size_t __n );
_WCRTLINK extern wchar_t *wcsncpy( wchar_t *__s1, const wchar_t *__s2, size_t __n );
_WCRTLINK extern wchar_t *wcspbrk( const wchar_t *__s1, const wchar_t *__s2 );
_WCRTLINK extern wchar_t *wcsrchr( const wchar_t *__s, wint_t __wc );
_WCRTLINK extern size_t   wcsrtombs( char *__dst, const wchar_t **__src, size_t __n, mbstate_t *__ps );
_WCRTLINK extern size_t   wcsspn( const wchar_t *__s1, const wchar_t *__s2 );
_WCRTLINK extern wchar_t *wcsstr( const wchar_t *__s1, const wchar_t *__s2 );
_WMRTLINK extern double   wcstod( const wchar_t *__nptr, wchar_t **__endptr );
_WCRTLINK extern wchar_t *wcstok( wchar_t *__s1, const wchar_t *__s2, wchar_t **__ptr );
_WCRTLINK extern long int wcstol( const wchar_t *, wchar_t **, int );
_WCRTLINK extern long long int wcstoll( const wchar_t *, wchar_t **, int );
_WCRTLINK extern unsigned long int wcstoul( const wchar_t *, wchar_t **, int );
_WCRTLINK extern unsigned long long int wcstoull( const wchar_t *, wchar_t **, int );
_WCRTLINK extern size_t   wcsxfrm( wchar_t *__s1, const wchar_t *__s2, size_t __n );
_WCRTLINK extern int      wctob( wint_t __wc );
_WCRTLINK extern int      wprintf( const wchar_t *__format, ... );
_WCRTLINK extern int      wscanf( const wchar_t *__format, ... );

_WCRTLINK extern wchar_t *_wcsdup( const wchar_t * );
_WCRTLINK extern int      _wcsicmp( const wchar_t *, const wchar_t * );
_WCRTLINK extern wchar_t *_wcslwr( wchar_t * );
_WCRTLINK extern int      _wcsnicmp( const wchar_t *, const wchar_t *, size_t );
_WCRTLINK extern wchar_t *_wcsnset( wchar_t *, int, size_t );
_WCRTLINK extern wchar_t *_wcsupr( wchar_t * );
_WCRTLINK extern wchar_t *_wcsrev( wchar_t * );
_WCRTLINK extern wchar_t *_wcsset( wchar_t *, wchar_t );
_WCRTLINK extern wchar_t *_wmktemp( wchar_t *__template );
_WCRTLINK extern wchar_t *_wtempnam( wchar_t *__dir, wchar_t *__prefix );

#if !defined(NO_EXT_KEYS) /* extensions enabled */

_WCRTLINK extern int      iswalnum( wint_t __wc );
_WCRTLINK extern int      iswalpha( wint_t __wc );
_WCRTLINK extern int      iswascii( wint_t __wc );
_WCRTLINK extern int      iswcntrl( wint_t __wc );
_WCRTLINK extern int      iswdigit( wint_t __wc );
_WCRTLINK extern int      iswgraph( wint_t __wc );
_WCRTLINK extern int      iswlower( wint_t __wc );
_WCRTLINK extern int      iswprint( wint_t __wc );
_WCRTLINK extern int      iswpunct( wint_t __wc );
_WCRTLINK extern int      iswspace( wint_t __wc );
_WCRTLINK extern int      iswupper( wint_t __wc );
_WCRTLINK extern int      iswxdigit( wint_t __wc );
_WCRTLINK extern int      iswctype( wint_t __wc, wctype_t __desc );
_WCRTLINK extern wctype_t wctype( const char *__property );
_WCRTLINK extern wint_t   towlower( wint_t __wc );
_WCRTLINK extern wint_t   towupper( wint_t __wc );

_WCRTLINK extern int     sisinit( const mbstate_t *__ps );

_WCRTLINK extern int    _fmbrlen( const char _WCFAR *__s, size_t __n, mbstate_t _WCFAR *__ps );
_WCRTLINK extern int    _fmbrtowc( wchar_t _WCFAR *__pwc, const char _WCFAR *__s, size_t __n, mbstate_t _WCFAR *__ps );
_WCRTLINK extern size_t _fmbsrtowcs( wchar_t _WCFAR *__dst, const char _WCFAR * _WCFAR *__src, size_t __n, mbstate_t _WCFAR *__ps );
_WCRTLINK extern int    _fwcrtomb( char _WCFAR *__s, wchar_t __wc, mbstate_t _WCFAR *__ps );
_WCRTLINK extern size_t _fwcsrtombs( char _WCFAR *__dst, const wchar_t _WCFAR * _WCFAR *__src, size_t __n, mbstate_t _WCFAR *__ps );
_WCRTLINK extern int    _swprintf( wchar_t *__s, const wchar_t *__format, ... );
_WCRTLINK extern int    _vswprintf( wchar_t *__s, const wchar_t *__format, va_list __arg );

#define WEOF ((wint_t)(-1))

#endif


#endif

#endif
