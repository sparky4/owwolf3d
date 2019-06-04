/***************************************************************************
 * FILE: inttypes.h (Format conversion of integer types)
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
 * Description: This header is part of the C standard library. It declares
 *              various macros to help format stdint integers.
 ***************************************************************************/
#ifndef _INTTYPES_H_INCLUDED
#define _INTTYPES_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#ifndef _STDINT_H_INCLUDED
  #include <stdint.h>
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

typedef struct {
    intmax_t    quot;
    intmax_t    rem;
} imaxdiv_t;

/* Format macros. According to C99 these are only visible in C++ programs
   if __STDC_FORMAT_MACROS is defined. */
#if !defined(__cplusplus) || defined(__STDC_FORMAT_MACROS)

#define PRId8   "hd"   /* Should be "hhd" but "hh" not yet implemented. */
#define PRId16  "hd"
#define PRId32  "ld"
#define PRId64  "lld"
#define PRIi8   "hi"   /* Should be "hhi". */
#define PRIi16  "hi"
#define PRIi32  "li"
#define PRIi64  "lli"
#define PRIo8   "ho"   /* Should be "hho". */
#define PRIo16  "ho"
#define PRIo32  "lo"
#define PRIo64  "llo"
#define PRIu8   "hu"   /* Should be "hhu". */
#define PRIu16  "hu"
#define PRIu32  "lu"
#define PRIu64  "llu"
#define PRIx8   "hx"   /* Should be "hhx". */
#define PRIx16  "hx"
#define PRIx32  "lx"
#define PRIx64  "llx"
#define PRIX8   "hX"   /* Should be "hhX". */
#define PRIX16  "hX"
#define PRIX32  "lX"
#define PRIX64  "llX"
/* #define SCNd8   "hhd" */
#define SCNd16  "hd"
#define SCNd32  "ld"
#define SCNd64  "lld"
/* #define SCNi8   "hhi" */
#define SCNi16  "hi"
#define SCNi32  "li"
#define SCNi64  "lli"
/* #define SCNo8   "hho" */
#define SCNo16  "ho"
#define SCNo32  "lo"
#define SCNo64  "llo"
/* #define SCNu8   "hhu" */
#define SCNu16  "hu"
#define SCNu32  "lu"
#define SCNu64  "llu"
/* #define SCNx8   "hhx" */
#define SCNx16  "hx"
#define SCNx32  "lx"
#define SCNx64  "llx"

#define PRIdLEAST8   "hd"
#define PRIdLEAST16  "hd"
#define PRIdLEAST32  "ld"
#define PRIdLEAST64  "lld"
#define PRIiLEAST8   "hi"
#define PRIiLEAST16  "hi"
#define PRIiLEAST32  "li"
#define PRIiLEAST64  "lli"
#define PRIoLEAST8   "ho"
#define PRIoLEAST16  "ho"
#define PRIoLEAST32  "lo"
#define PRIoLEAST64  "llo"
#define PRIuLEAST8   "hu"
#define PRIuLEAST16  "hu"
#define PRIuLEAST32  "lu"
#define PRIuLEAST64  "llu"
#define PRIxLEAST8   "hx"
#define PRIxLEAST16  "hx"
#define PRIxLEAST32  "lx"
#define PRIxLEAST64  "llx"
#define PRIXLEAST8   "hX"
#define PRIXLEAST16  "hX"
#define PRIXLEAST32  "lX"
#define PRIXLEAST64  "llX"
/* #define SCNdLEAST8   "hhd" */
#define SCNdLEAST16  "hd"
#define SCNdLEAST32  "ld"
#define SCNdLEAST64  "lld"
/* #define SCNiLEAST8   "hhi" */
#define SCNiLEAST16  "hi"
#define SCNiLEAST32  "li"
#define SCNiLEAST64  "lli"
/* #define SCNoLEAST8   "hho" */
#define SCNoLEAST16  "ho"
#define SCNoLEAST32  "lo"
#define SCNoLEAST64  "llo"
/* #define SCNuLEAST8   "hhu" */
#define SCNuLEAST16  "hu"
#define SCNuLEAST32  "lu"
#define SCNuLEAST64  "llu"
/* #define SCNxLEAST8   "hhx" */
#define SCNxLEAST16  "hx"
#define SCNxLEAST32  "lx"
#define SCNxLEAST64  "llx"

#define PRIdFAST8   "d"
#define PRIdFAST16  "d"
#define PRIdFAST32  "ld"
#define PRIdFAST64  "lld"
#define PRIiFAST8   "i"
#define PRIiFAST16  "i"
#define PRIiFAST32  "li"
#define PRIiFAST64  "lli"
#define PRIoFAST8   "o"
#define PRIoFAST16  "o"
#define PRIoFAST32  "lo"
#define PRIoFAST64  "llo"
#define PRIuFAST8   "u"
#define PRIuFAST16  "u"
#define PRIuFAST32  "lu"
#define PRIuFAST64  "llu"
#define PRIxFAST8   "x"
#define PRIxFAST16  "x"
#define PRIxFAST32  "lx"
#define PRIxFAST64  "llx"
#define PRIXFAST8   "X"
#define PRIXFAST16  "X"
#define PRIXFAST32  "lX"
#define PRIXFAST64  "llX"
#define SCNdFAST8   "d"
#define SCNdFAST16  "d"
#define SCNdFAST32  "ld"
#define SCNdFAST64  "lld"
#define SCNiFAST8   "i"
#define SCNiFAST16  "i"
#define SCNiFAST32  "li"
#define SCNiFAST64  "lli"
#define SCNoFAST8   "o"
#define SCNoFAST16  "o"
#define SCNoFAST32  "lo"
#define SCNoFAST64  "llo"
#define SCNuFAST8   "u"
#define SCNuFAST16  "u"
#define SCNuFAST32  "lu"
#define SCNuFAST64  "llu"
#define SCNxFAST8   "x"
#define SCNxFAST16  "x"
#define SCNxFAST32  "lx"
#define SCNxFAST64  "llx"

#define PRIdMAX     "lld"
#define PRIiMAX     "lli"
#define PRIoMAX     "llo"
#define PRIuMAX     "llu"
#define PRIxMAX     "llx"
#define PRIXMAX     "llX"
#define SCNdMAX     "lld"
#define SCNiMAX     "lli"
#define SCNoMAX     "llo"
#define SCNuMAX     "llu"
#define SCNxMAX     "llx"

#if defined(__386__) || defined(__AXP__) || defined (__PPC__)
#if defined(__COMPACT__) || defined(__LARGE__)
#define PRIdPTR     "lld"
#define PRIiPTR     "lli"
#define PRIoPTR     "llo"
#define PRIuPTR     "llu"
#define PRIxPTR     "llx"
#define PRIXPTR     "llX"
#define SCNdPTR     "lld"
#define SCNiPTR     "lli"
#define SCNoPTR     "llo"
#define SCNuPTR     "llu"
#define SCNxPTR     "llx"
#else
#define PRIdPTR     "ld"
#define PRIiPTR     "li"
#define PRIoPTR     "lo"
#define PRIuPTR     "lu"
#define PRIxPTR     "lx"
#define PRIXPTR     "lX"
#define SCNdPTR     "ld"
#define SCNiPTR     "li"
#define SCNoPTR     "lo"
#define SCNuPTR     "lu"
#define SCNxPTR     "lx"
#endif
#else /* 16 bit */
#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
#define PRIdPTR     "ld"
#define PRIiPTR     "li"
#define PRIoPTR     "lo"
#define PRIuPTR     "lu"
#define PRIxPTR     "lx"
#define PRIXPTR     "lX"
#define SCNdPTR     "ld"
#define SCNiPTR     "li"
#define SCNoPTR     "lo"
#define SCNuPTR     "lu"
#define SCNxPTR     "lx"
#else
#define PRIdPTR     "d"
#define PRIiPTR     "i"
#define PRIoPTR     "o"
#define PRIuPTR     "u"
#define PRIxPTR     "x"
#define PRIXPTR     "X"
#define SCNdPTR     "d"
#define SCNiPTR     "i"
#define SCNoPTR     "o"
#define SCNuPTR     "u"
#define SCNxPTR     "x"
#endif
#endif

#endif  /* __STDC_FORMAT_MACROS */

_WCRTLINK extern intmax_t   imaxabs( intmax_t j );
_WCRTLINK extern imaxdiv_t  imaxdiv( intmax_t numer, intmax_t denom );
_WCRTLINK extern intmax_t   strtoimax( const char *nptr, char **endptr, int base );
_WCRTLINK extern uintmax_t  strtoumax( const char *nptr, char **endptr, int base );
_WCRTLINK extern intmax_t   wcstoimax( const wchar_t *nptr, wchar_t **endptr, int base );
_WCRTLINK extern uintmax_t  wcstoumax( const wchar_t *nptr, wchar_t **endptr, int base );

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
