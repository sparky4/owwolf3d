/*
 *  dosfunc.h   DOS 2.0 function calls
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


#define DOS_GET_CHAR_NO_ECHO 0x07
#define DOS_CUR_DISK    0x19
#define DOS_SET_DTA     0x1A
#define DOS_SET_INT     0x25
#define DOS_GET_DATE    0x2A
#define DOS_GET_TIME    0x2C
#define DOS_GET_VERSION 0x30
#define DOS_CTRL_BREAK  0x33
#define DOS_GET_INT     0x35
#define DOS_SWITCH_CHAR 0x37
#define DOS_MKDIR       0x39
#define DOS_RMDIR       0x3A
#define DOS_CHDIR       0x3B
#define DOS_CREAT       0x3C
#define DOS_OPEN        0x3D
#define DOS_CLOSE       0x3E
#define DOS_READ        0x3F
#define DOS_WRITE       0x40
#define DOS_UNLINK      0x41
#define DOS_LSEEK       0x42
#define DOS_CHMOD       0x43
#define DOS_IOCTL       0x44
#define DOS_DUP         0x45
#define DOS_DUP2        0x46
#define DOS_GETCWD      0x47
#define DOS_ALLOC_SEG   0x48
#define DOS_FREE_SEG    0x49
#define DOS_MODIFY_SEG  0x4A
#define DOS_EXIT        0x4C
#define DOS_FIND_FIRST  0x4E
#define DOS_FIND_NEXT   0x4F
#define DOS_RENAME      0x56
#define DOS_FILE_DATE   0x57
#define DOS_COMMIT_FILE 0x68
