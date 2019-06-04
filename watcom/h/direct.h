/*
 *  direct.h    Defines the types and structures used by the directory routines
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
#ifndef _DIRECT_H_INCLUDED
#define _DIRECT_H_INCLUDED
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

#ifndef __TYPES_H_INCLUDED
 #include <sys/types.h>
#endif

#if defined(__OS2__) || defined(__NT__)
#define NAME_MAX        255             /* maximum filename for HPFS or NTFS */
#else
#define NAME_MAX        12              /* 8 chars + '.' +  3 chars */
#endif

struct _wdirent {
    char               d_dta[21];       /* disk transfer area */
    char               d_attr;          /* file's attribute */
    unsigned short int d_time;          /* file's time */
    unsigned short int d_date;          /* file's date */
    long               d_size;          /* file's size */
    wchar_t            d_name[NAME_MAX+1];/* file's name */
    unsigned short     d_ino;           /* serial number (not used) */
    char               d_first;         /* flag for 1st time */
    wchar_t *          d_openpath;      /* path specified to _wopendir */
};

struct dirent {
    char               d_dta[21];       /* disk transfer area */
    char               d_attr;          /* file's attribute */
    unsigned short int d_time;          /* file's time */
    unsigned short int d_date;          /* file's date */
    long               d_size;          /* file's size */
    char               d_name[NAME_MAX+1];/* file's name */
    unsigned short     d_ino;           /* serial number (not used) */
    char               d_first;         /* flag for 1st time */
    char *             d_openpath;      /* path specified to opendir */
};
typedef struct dirent   DIR;

/* File attribute constants for d_attr field */

#define _A_NORMAL       0x00    /* Normal file - read/write permitted */
#define _A_RDONLY       0x01    /* Read-only file */
#define _A_HIDDEN       0x02    /* Hidden file */
#define _A_SYSTEM       0x04    /* System file */
#define _A_VOLID        0x08    /* Volume-ID entry */
#define _A_SUBDIR       0x10    /* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */

#ifndef _DISKFREE_T_DEFINED
#define _DISKFREE_T_DEFINED
#define _DISKFREE_T_DEFINED_
struct _diskfree_t {
        unsigned total_clusters;
        unsigned avail_clusters;
        unsigned sectors_per_cluster;
        unsigned bytes_per_sector;
};
#define diskfree_t _diskfree_t
#endif

_WCRTLINK extern int              chdir( const char *__path );
_WCRTLINK extern int              closedir( struct dirent * );
_WCRTLINK extern char *           getcwd( char *__buf, size_t __size );
_WCRTLINK extern char *           _getcwd( char *__buf, size_t __size );
_WCRTLINK extern char *           _getdcwd( int __drive, char *__buffer,
                                            size_t __maxlen );
_WCRTLINK extern unsigned         _getdrive( void );
_WCRTLINK extern unsigned         _getdiskfree( unsigned __drive,
                                                struct _diskfree_t
                                                *__diskspace);
_WCRTLINK extern int              mkdir( const char *__path );
_WCRTLINK extern int              _mkdir( const char *__path );
_WCRTLINK extern struct dirent *  opendir( const char * );
_WCRTLINK extern struct dirent *  readdir( struct dirent * );
_WCRTLINK extern void             rewinddir( struct dirent * );
_WCRTLINK extern int              rmdir( const char *__path );

_WCRTLINK extern int              _wchdir( const wchar_t *__path );
_WCRTLINK extern int              _wclosedir( struct _wdirent * );
_WCRTLINK extern wchar_t *        _wgetcwd( wchar_t *__buf, size_t __size );
_WCRTLINK extern wchar_t *        _wgetdcwd( int __drive, wchar_t *__buffer,
                                             size_t __maxlen );
_WCRTLINK extern int              _wmkdir( const wchar_t *__path );
_WCRTLINK extern struct _wdirent *_wopendir( const wchar_t * );
_WCRTLINK extern struct _wdirent *_wreaddir( struct _wdirent * );
_WCRTLINK extern void             _wrewinddir( struct _wdirent * );
_WCRTLINK extern int              _wrmdir( const wchar_t *__path );

#pragma pack( __pop )
#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
