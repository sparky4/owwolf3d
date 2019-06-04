/***************************************************************************
 * FILE: fenv.h (Floating-point environment)
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
 * Description: This header is part of the C99 standard library. It
 *              describes the floating-point environment
 ***************************************************************************/
#ifndef _FENV_H_INCLUDED
#define _FENV_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif


#if !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 199901L

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

/* type for holding FPU environment */
#ifdef _M_IX86
#pragma pack( __push, 1 )
#else
#pragma pack( __push, 8 )
#endif
#if defined(__386__)
typedef struct
{
    unsigned short control_word;
    unsigned short pad0;
    unsigned short status_word;
    unsigned short pad1;
    unsigned short tag_word;
    unsigned short pad2;
    unsigned int   ins_ptr_offset;
    unsigned short ins_ptr_seg;
    unsigned short opcode;
    unsigned int   op_ptr_offset;
    unsigned short op_ptr_seg;
    unsigned short pad3;
} fenv_t;
#else
typedef struct
{
    unsigned short control_word;
    unsigned short status_word;
    unsigned short tag_word;
    unsigned short ins_ptr_offset;
    unsigned short ins_ptr_seg;
    unsigned short op_ptr_offset;
    unsigned short op_ptr_seg;
} fenv_t;
#endif
#pragma pack( __pop )

/* type for holding FPU exception flags */
typedef unsigned short fexcept_t;

/* exception flags */
#define FE_INVALID   1
#define FE_DENORMAL  2
#define FE_DIVBYZERO 4
#define FE_OVERFLOW  8
#define FE_UNDERFLOW 16
#define FE_INEXACT   32

#define FE_ALL_EXCEPT (FE_DIVBYZERO|FE_INEXACT|FE_INVALID|FE_OVERFLOW|FE_UNDERFLOW|FE_DENORMAL)

/* rounding modes */
#define FE_TONEAREST  0x0000
#define FE_DOWNWARD   0x0400
#define FE_TOWARDZERO 0x0C00
#define FE_UPWARD     0x0800

/* default FPU setup */
_WCRTLINK extern const fenv_t __fenv_h_default_environment;
#define FE_DFL_ENV &__fenv_h_default_environment

/* exception handling functions */
_WCRTLINK extern void feclearexcept(int excepts);
_WCRTLINK extern void fegetexceptflag(fexcept_t *flagp, int excepts);
_WCRTLINK extern void feraiseexcept(int excepts);
_WCRTLINK extern void fesetexceptflag(const fexcept_t *flagp, int excepts);
_WCRTLINK extern int  fetestexcept(int excepts);

#ifndef NO_EXT_KEYS
/* OW extensions */
_WCRTLINK extern void __feenableexcept(int excepts);
_WCRTLINK extern void __fedisableexcept(int excepts);
#endif

/* rounding mode functions */
_WCRTLINK extern int fegetround(void);
_WCRTLINK extern int fesetround(int round);

/* FPU environment functions */
_WCRTLINK extern void fegetenv(fenv_t *envp);
_WCRTLINK extern int feholdexcept(fenv_t *envp);
_WCRTLINK extern void fesetenv(const fenv_t *envp);
_WCRTLINK extern void feupdateenv(const fenv_t *envp);

#endif
#endif
