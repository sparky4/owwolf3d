/*
 *  fcntl.h     File control options used by open
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
#ifndef _FCNTL_H_INCLUDED
#define _FCNTL_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#ifdef _M_IX86
#pragma pack( __push, 1 )
#else
#pragma pack( __push, 8 )
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

#define O_RDONLY        0x0000  /* open for read only */
#define _O_RDONLY       0x0000  /* open for read only */
#define O_WRONLY        0x0001  /* open for write only */
#define _O_WRONLY       0x0001  /* open for write only */
#define O_RDWR          0x0002  /* open for read and write */
#define _O_RDWR         0x0002  /* open for read and write */
#define O_APPEND        0x0010  /* writes done at end of file */
#define _O_APPEND       0x0010  /* writes done at end of file */
#define O_CREAT         0x0020  /* create new file */
#define _O_CREAT        0x0020  /* create new file */
#define O_TRUNC         0x0040  /* truncate existing file */
#define _O_TRUNC        0x0040  /* truncate existing file */
#define O_NOINHERIT     0x0080  /* file is not inherited by child process */
#define _O_NOINHERIT    0x0080  /* file is not inherited by child process */
#define O_TEXT          0x0100  /* text file */
#define _O_TEXT         0x0100  /* text file */
#define O_BINARY        0x0200  /* binary file */
#define _O_BINARY       0x0200  /* binary file */
#define O_EXCL          0x0400  /* exclusive open */
#define _O_EXCL         0x0400  /* exclusive open */

/*
 *  POSIX 1003.1 Prototypes.
 */
_WCRTLINK extern int open( const char *__path, int __oflag, ... );
_WCRTLINK extern int sopen( const char *__path, int __oflag, int __share, ... );

_WCRTLINK extern int _wopen( const wchar_t *, int, ... );
_WCRTLINK extern int _wsopen( const wchar_t *, int, int, ... );
_WCRTLINK extern int _wcreat( const wchar_t *, int );
#pragma pack( __pop )
#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
