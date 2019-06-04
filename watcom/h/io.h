/*
 *  io.h        Low level I/O routines that work with file handles
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
#ifndef _IO_H_INCLUDED
#define _IO_H_INCLUDED
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

#ifndef _MAX_PATH
 #if defined(__OS2__) || defined(__NT__)
  #define _MAX_PATH   260 /* maximum length of full pathname */
 #else
  #define _MAX_PATH   144 /* maximum length of full pathname */
 #endif
#endif

/* Symbolic constants for the access() function */

#define R_OK    4       /*  Test for read permission    */
#define W_OK    2       /*  Test for write permission   */
#define X_OK    1       /*  Test for execute permission */
#define F_OK    0       /*  Test for existence of file  */

#define ACCESS_WR       0x0002
#define ACCESS_RD       0x0004

/* Symbolic constants for the lseek() function */

#define SEEK_SET    0           /* Seek relative to the start of file   */
#define SEEK_CUR    1           /* Seek relative to current position    */
#define SEEK_END    2           /* Seek relative to the end of the file */

/* Symbolic constants for stream I/O */

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2
#ifndef NO_EXT_KEYS     /* extensions enabled */
#if ! defined(__NT__)
#define STDAUX_FILENO   3
#define STDPRN_FILENO   4
#endif
#endif


typedef unsigned long   _fsize_t;

struct _finddata_t {
    unsigned    attrib;
    time_t      time_create;
    time_t      time_access;
    time_t      time_write;
    _fsize_t    size;
    char        name[_MAX_PATH];
};

struct _wfinddata_t {
    unsigned    attrib;
    time_t      time_create;
    time_t      time_access;
    time_t      time_write;
    _fsize_t    size;
    wchar_t     name[_MAX_PATH];
};

#ifdef __WATCOM_INT64__
struct _finddatai64_t {
    unsigned    attrib;
    time_t      time_create;
    time_t      time_access;
    time_t      time_write;
    __int64     size;
    char        name[_MAX_PATH];
};

struct _wfinddatai64_t {
    unsigned    attrib;
    time_t      time_create;
    time_t      time_access;
    time_t      time_write;
    __int64     size;
    wchar_t     name[_MAX_PATH];
};
#endif

/* File attribute constants for attrib field */
#define _A_NORMAL       0x00    /* Normal file - read/write permitted */
#define _A_RDONLY       0x01    /* Read-only file */
#define _A_HIDDEN       0x02    /* Hidden file */
#define _A_SYSTEM       0x04    /* System file */
#define _A_SUBDIR       0x10    /* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */


_WCRTLINK extern int  access(const char *__path,int __mode);
_WCRTLINK extern int  _access(const char *__path,int __mode);
_WCRTLINK extern int  chmod(const char *__path,int __pmode);
_WCRTLINK extern int  chsize(int __handle,long __size);
_WCRTLINK extern int  close(int __handle);
_WCRTLINK extern int  _close(int __handle);
_WCRTLINK extern int  creat(const char *__path,int __pmode);
_WCRTLINK extern int  dup(int __handle);
_WCRTLINK extern int  _dup(int __handle);
_WCRTLINK extern int  dup2(int __handle1,int __handle2);
_WCRTLINK extern int  eof(int __handle);
_WCRTLINK extern long filelength(int __handle);
_WCRTLINK extern int  fsync( int __filedes );
_WCRTLINK extern long _get_osfhandle( int __posixhandle );
_WCRTLINK extern int  _hdopen(int __handle,int __mode);
_WCRTLINK extern int  isatty(int __handle);
_WCRTLINK extern int  lock(int __handle,unsigned long __offset,
                           unsigned long __nbytes);
_WCRTLINK extern long lseek(int __handle,long __offset,int __origin);
_WCRTLINK extern long _lseek(int __handle,long __offset,int __origin);
_WCRTLINK extern int  open(const char *__path,int __oflag,...);
_WCRTLINK extern int  _open(const char *__path,int __oflag,...);
_WCRTLINK extern int  _open_osfhandle( long __osfhandle, int __flags );
_WCRTLINK extern int  _os_handle(int __handle);
_WCRTLINK extern int  _pipe( int *__phandles, unsigned __psize, int __textmode );
_WCRTLINK extern int  read(int __handle,void *__buf,unsigned int __len);
_WCRTLINK extern int  setmode(int __handle,int __mode);
_WCRTLINK extern int  sopen(const char *__path,int __oflag,int __shflag,...);
_WCRTLINK extern long tell(int __handle);
_WCRTLINK extern int  umask(int __permission);
_WCRTLINK extern int  unlink(const char *__path);
_WCRTLINK extern int  unlock(int __handle,unsigned long __offset,
                             unsigned long __nbytes);
_WCRTLINK extern int  write(int __handle,const void *__buf,unsigned int __len);
_WCRTLINK extern int  _waccess(const wchar_t *__path,int __mode);
_WCRTLINK extern int  _wchmod( const wchar_t *__path, int __pmode );
_WCRTLINK extern int  _wunlink(const wchar_t *__path);

_WCRTLINK extern long _findfirst( const char *__filespec,
                                  struct _finddata_t *__fileinfo );
_WCRTLINK extern long _wfindfirst( const wchar_t *__filespec,
                                   struct _wfinddata_t *__fileinfo );
_WCRTLINK extern int  _findnext( long __handle,
                                 struct _finddata_t *__fileinfo );
_WCRTLINK extern int  _wfindnext( long __handle,
                                  struct _wfinddata_t *__fileinfo );
_WCRTLINK extern int  _findclose( long __handle );

_WCRTLINK extern char *     _mktemp( char *__template );
_WCRTLINK extern wchar_t *  _wmktemp( wchar_t *__template );

#ifdef __WATCOM_INT64__
_WCRTLINK extern long       _findfirsti64( const char *__filespec,
                                           struct _finddatai64_t *__fileinfo );
_WCRTLINK extern long       _wfindfirsti64( const wchar_t *__filespec,
                                            struct _wfinddatai64_t *__fileinfo );
_WCRTLINK extern int        _findnexti64( long __handle,
                                          struct _finddatai64_t *__fileinfo );
_WCRTLINK extern int        _wfindnexti64( long __handle,
                                           struct _wfinddatai64_t *__fileinfo );
_WCRTLINK extern __int64    _filelengthi64( int __handle );
_WCRTLINK extern __int64    _lseeki64( int __handle, __int64 __offset,
                                       int __origin );
_WCRTLINK extern __int64    _telli64( int __handle );
#endif

#ifndef _STAT_H_INCLUDED
 #include <sys/stat.h>
#endif

#pragma pack( __pop )
#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
