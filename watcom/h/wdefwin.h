/*
 *  wdefwin.h   default windowing calls
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
#ifndef _WDEFWIN_H_INCLUDED
#define _WDEFWIN_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __SW_BW
  #error wdefwin.h is for use the default windowing system, use the /bw switch
#endif

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#if ( defined(__OS2__) && (defined(__386__)||defined(__PPC__)) ) || defined(__NT__) || \
    defined(__WINDOWS_386__) || defined(__WINDOWS__)
_WCRTLINK extern int _dwDeleteOnClose( int handle );
_WCRTLINK extern int _dwSetAboutDlg( const char *title, const char *text );
_WCRTLINK extern int _dwSetAppTitle( const char *title );
_WCRTLINK extern int _dwSetConTitle( int handle, const char *title );
_WCRTLINK extern int _dwYield( void );
_WCRTLINK extern int _dwShutDown( void );
#else
  #error You cannot use the default windowing system in a non GUI environment
  #error Make sure you use the /bt= switch to select the target system
#endif

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
