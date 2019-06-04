/***************************************************************************
 * FILE: wctype.h/cwctype (Wide character classification functions)
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
 *              declares functions and types used to classify wide
 *              characters.
 ***************************************************************************/
#ifndef _WCTYPE_H_INCLUDED
#define _WCTYPE_H_INCLUDED

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
#include <cwctype>

// C99 types in wctype.h
#ifndef _WCTYPE_T_DEFINED
#define _WCTYPE_T_DEFINED
#define _WCTYPE_T_DEFINED_
using std::wint_t;
using std::wctype_t;
#endif

// C99 functions in wctype.h
using std::iswalnum;
using std::iswalpha;
using std::iswblank;
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

#else /* __cplusplus not defined */

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

#define WEOF ((wint_t)(-1))

_WCRTLINK extern int      iswalnum( wint_t __wc );
_WCRTLINK extern int      iswalpha( wint_t __wc );
_WCRTLINK extern int      iswascii( wint_t __wc );
_WCRTLINK extern int      iswblank( wint_t __wc );
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


#endif

#endif
