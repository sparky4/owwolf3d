/*
 *  conio.h     Console and Port I/O functions
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
#ifndef _CONIO_H_INCLUDED
#define _CONIO_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
#if defined(__PPC__)
 #if defined(__NT__)
    typedef char * __va_list;
 #else
    typedef struct {
      char  __gpr;
      char  __fpr;
      char  __reserved[2];
      char *__input_arg_area;
      char *__reg_save_area;
    } __va_list;
  #endif
#elif defined(__AXP__)
  typedef struct {
    char *__base;
    int   __offset;
  } __va_list;
#elif defined(__HUGE__) || defined(__SW_ZU)
  typedef char _WCFAR *__va_list[1];
#else
  typedef char *__va_list[1];
#endif
#endif

_WCRTLINK extern char *cgets(char *__buf);
_WCRTLINK extern int cputs(const char *__buf);
_WCRTLINK extern int cprintf(const char *__fmt,...);
_WCRTLINK extern int cscanf(const char *__fmt,...);
_WCRTLINK extern int getch(void);
_WCRTLINK extern int _getch(void);
_WCRTLINK extern int getche(void);
_WCRTLINK extern int _getche(void);
_WCRTLINK extern int kbhit(void);
#if defined(_M_IX86)
 _WCIRTLINK extern unsigned inp(unsigned __port);
 _WCIRTLINK extern unsigned inpw(unsigned __port);
 _WCIRTLINK extern unsigned outp(unsigned __port, unsigned __value);
 _WCIRTLINK extern unsigned outpw(unsigned __port,unsigned __value);
 #if defined(__386__)
  _WCIRTLINK extern unsigned inpd(unsigned __port);
  _WCIRTLINK extern unsigned outpd(unsigned __port, unsigned __value);
 #endif
#endif
_WCRTLINK extern int putch(int __c);
_WCRTLINK extern int ungetch(int __c);
_WCRTLINK extern int vcprintf( const char *__format, __va_list __arg );
_WCRTLINK extern int vcscanf( const char *__format, __va_list __arg );

#if defined(__INLINE_FUNCTIONS__) && defined(_M_IX86)
 #pragma intrinsic(inp,inpw,outp,outpw)
 #if defined(__386__)
  #pragma intrinsic(inpd,outpd)
 #endif
#endif

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
