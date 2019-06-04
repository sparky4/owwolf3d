/*
 *  sys/locking.h       Flags for locking() function
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
#ifndef __LOCKING_H_INCLUDED
#define __LOCKING_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#define _LK_UNLCK   0   /* unlock region of a file */
#define LK_UNLCK    _LK_UNLCK
#define _LK_LOCK    1   /* lock a region of a file */
#define LK_LOCK     _LK_LOCK
#define _LK_NBLCK   2   /* non-blocking lock */
#define LK_NBLCK    _LK_NBLCK
#define _LK_RLCK    3   /* lock for writing */
#define LK_RLCK     _LK_RLCK
#define _LK_NBRLCK  4   /* non-blocking lock for writing */
#define LK_NBRLCK   _LK_NBRLCK

_WCRTLINK extern int locking(int __handle,int __mode,unsigned long __nbyte);
_WCRTLINK extern int _locking(int __handle,int __mode,unsigned long __nbyte);

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
