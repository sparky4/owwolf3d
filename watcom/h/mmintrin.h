/*
 *  mmintrin.h  Intrinsics for MMX instructions
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
#ifndef _MMINTRIN_H_INCLUDED
#define _MMINTRIN_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __386__
   #error mmintrin.h only for use with 32-bit Intel Architectures
#endif

typedef union {
    #ifdef __WATCOM_INT64__
       unsigned __int64 _64[1];
    #endif
    unsigned long       _32[2];
    unsigned short      _16[4];
    unsigned char        _8[8];
} __m64;

/* General support intrinsics */
void  _m_empty(void);
__m64 _m_from_int(int i);
int   _m_to_int(__m64 *__m);
__m64 _m_packsswb(__m64 *__m1, __m64 *__m2);
__m64 _m_packssdw(__m64 *__m1, __m64 *__m2);
__m64 _m_packuswb(__m64 *__m1, __m64 *__m2);
__m64 _m_punpckhbw(__m64 *__m1, __m64 *__m2);
__m64 _m_punpckhwd(__m64 *__m1, __m64 *__m2);
__m64 _m_punpckhdq(__m64 *__m1, __m64 *__m2);
__m64 _m_punpcklbw(__m64 *__m1, __m64 *__m2);
__m64 _m_punpcklwd(__m64 *__m1, __m64 *__m2);
__m64 _m_punpckldq(__m64 *__m1, __m64 *__m2);

/* Packed arithmetic intrinsics */
__m64 _m_paddb(__m64 *__m1, __m64 *__m2);
__m64 _m_paddw(__m64 *__m1, __m64 *__m2);
__m64 _m_paddd(__m64 *__m1, __m64 *__m2);
__m64 _m_paddsb(__m64 *__m1, __m64 *__m2);
__m64 _m_paddsw(__m64 *__m1, __m64 *__m2);
__m64 _m_paddusb(__m64 *__m1, __m64 *__m2);
__m64 _m_paddusw(__m64 *__m1, __m64 *__m2);
__m64 _m_psubb(__m64 *__m1, __m64 *__m2);
__m64 _m_psubw(__m64 *__m1, __m64 *__m2);
__m64 _m_psubd(__m64 *__m1, __m64 *__m2);
__m64 _m_psubsb(__m64 *__m1, __m64 *__m2);
__m64 _m_psubsw(__m64 *__m1, __m64 *__m2);
__m64 _m_psubusb(__m64 *__m1, __m64 *__m2);
__m64 _m_psubusw(__m64 *__m1, __m64 *__m2);
__m64 _m_pmaddwd(__m64 *__m1, __m64 *__m2);
__m64 _m_pmulhw(__m64 *__m1, __m64 *__m2);
__m64 _m_pmullw(__m64 *__m1, __m64 *__m2);

/* Shift intrinsics */
__m64 _m_psllw(__m64 *__m, __m64 *__count);
__m64 _m_psllwi(__m64 *__m, int __count);
__m64 _m_pslld(__m64 *__m, __m64 *__count);
__m64 _m_pslldi(__m64 *__m, int __count);
__m64 _m_psllq(__m64 *__m, __m64 *__count);
__m64 _m_psllqi(__m64 *__m, int __count);
__m64 _m_psraw(__m64 *__m, __m64 *__count);
__m64 _m_psrawi(__m64 *__m, int __count);
__m64 _m_psrad(__m64 *__m, __m64 *__count);
__m64 _m_psradi(__m64 *__m, int __count);
__m64 _m_psrlw(__m64 *__m, __m64 *__count);
__m64 _m_psrlwi(__m64 *__m, int __count);
__m64 _m_psrld(__m64 *__m, __m64 *__count);
__m64 _m_psrldi(__m64 *__m, int __count);
__m64 _m_psrlq(__m64 *__m, __m64 *__count);
__m64 _m_psrlqi(__m64 *__m, int __count);

/* Logical intrinsics */
__m64 _m_pand(__m64 *__m1, __m64 *__m2);
__m64 _m_pandn(__m64 *__m1, __m64 *__m2);
__m64 _m_por(__m64 *__m1, __m64 *__m2);
__m64 _m_pxor(__m64 *__m1, __m64 *__m2);

/* Comparison intrinsics */
__m64 _m_pcmpeqb(__m64 *__m1, __m64 *__m2);
__m64 _m_pcmpeqw(__m64 *__m1, __m64 *__m2);
__m64 _m_pcmpeqd(__m64 *__m1, __m64 *__m2);
__m64 _m_pcmpgtb(__m64 *__m1, __m64 *__m2);
__m64 _m_pcmpgtw(__m64 *__m1, __m64 *__m2);
__m64 _m_pcmpgtd(__m64 *__m1, __m64 *__m2);


#define _m_to_int(__m)                  _m_to_int(&(__m))
#define _m_packsswb(__m1, __m2)         _m_packsswb(&(__m1), &(__m2))
#define _m_packssdw(__m1, __m2)         _m_packssdw(&(__m1), &(__m2))
#define _m_packuswb(__m1, __m2)         _m_packuswb(&(__m1), &(__m2))
#define _m_punpckhbw(__m1, __m2)        _m_punpckhbw(&(__m1), &(__m2))
#define _m_punpckhwd(__m1, __m2)        _m_punpckhwd(&(__m1), &(__m2))
#define _m_punpckhdq(__m1, __m2)        _m_punpckhdq(&(__m1), &(__m2))
#define _m_punpcklbw(__m1, __m2)        _m_punpcklbw(&(__m1), &(__m2))
#define _m_punpcklwd(__m1, __m2)        _m_punpcklwd(&(__m1), &(__m2))
#define _m_punpckldq(__m1, __m2)        _m_punpckldq(&(__m1), &(__m2))
#define _m_paddb(__m1, __m2)            _m_paddb(&(__m1), &(__m2))
#define _m_paddw(__m1, __m2)            _m_paddw(&(__m1), &(__m2))
#define _m_paddd(__m1, __m2)            _m_paddd(&(__m1), &(__m2))
#define _m_paddsb(__m1, __m2)           _m_paddsb(&(__m1), &(__m2))
#define _m_paddsw(__m1, __m2)           _m_paddsw(&(__m1), &(__m2))
#define _m_paddusb(__m1, __m2)          _m_paddusb(&(__m1), &(__m2))
#define _m_paddusw(__m1, __m2)          _m_paddusw(&(__m1), &(__m2))
#define _m_psubb(__m1, __m2)            _m_psubb(&(__m1), &(__m2))
#define _m_psubw(__m1, __m2)            _m_psubw(&(__m1), &(__m2))
#define _m_psubd(__m1, __m2)            _m_psubd(&(__m1), &(__m2))
#define _m_psubsb(__m1, __m2)           _m_psubsb(&(__m1), &(__m2))
#define _m_psubsw(__m1, __m2)           _m_psubsw(&(__m1), &(__m2))
#define _m_psubusb(__m1, __m2)          _m_psubusb(&(__m1), &(__m2))
#define _m_psubusw(__m1, __m2)          _m_psubusw(&(__m1), &(__m2))
#define _m_pmaddwd(__m1, __m2)          _m_pmaddwd(&(__m1), &(__m2))
#define _m_pmulhw(__m1, __m2)           _m_pmulhw(&(__m1), &(__m2))
#define _m_pmullw(__m1, __m2)           _m_pmullw(&(__m1), &(__m2))
#define _m_psllw(__m, __count)          _m_psllw(&(__m), &(__count))
#define _m_psllwi(__m, __count)         _m_psllwi(&(__m), __count)
#define _m_pslld(__m, __count)          _m_pslld(&(__m), &(__count))
#define _m_pslldi(__m, __count)         _m_pslldi(&(__m), __count)
#define _m_psllq(__m, __count)          _m_psllq(&(__m), &(__count))
#define _m_psllqi(__m, __count)         _m_psllqi(&(__m), __count)
#define _m_psraw(__m, __count)          _m_psraw(&(__m), &(__count))
#define _m_psrawi(__m, __count)         _m_psrawi(&(__m), __count)
#define _m_psrad(__m, __count)          _m_psrad(&(__m), &(__count))
#define _m_psradi(__m, __count)         _m_psradi(&(__m), __count)
#define _m_psrlw(__m, __count)          _m_psrlw(&(__m), &(__count))
#define _m_psrlwi(__m, __count)         _m_psrlwi(&(__m), __count)
#define _m_psrld(__m, __count)          _m_psrld(&(__m), &(__count))
#define _m_psrldi(__m, __count)         _m_psrldi(&(__m), __count)
#define _m_psrlq(__m, __count)          _m_psrlq(&(__m), &(__count))
#define _m_psrlqi(__m, __count)         _m_psrlqi(&(__m), __count)
#define _m_pand(__m1, __m2)             _m_pand(&(__m1), &(__m2))
#define _m_pandn(__m1, __m2)            _m_pandn(&(__m1), &(__m2))
#define _m_por(__m1, __m2)              _m_por(&(__m1), &(__m2))
#define _m_pxor(__m1, __m2)             _m_pxor(&(__m1), &(__m2))
#define _m_pcmpeqb(__m1, __m2)          _m_pcmpeqb(&(__m1), &(__m2))
#define _m_pcmpeqw(__m1, __m2)          _m_pcmpeqw(&(__m1), &(__m2))
#define _m_pcmpeqd(__m1, __m2)          _m_pcmpeqd(&(__m1), &(__m2))
#define _m_pcmpgtb(__m1, __m2)          _m_pcmpgtb(&(__m1), &(__m2))
#define _m_pcmpgtw(__m1, __m2)          _m_pcmpgtw(&(__m1), &(__m2))
#define _m_pcmpgtd(__m1, __m2)          _m_pcmpgtd(&(__m1), &(__m2))

#pragma aux _m_empty =  \
                ".586"  \
                "emms"

#pragma aux _m_from_int = \
                ".586"                  \
                "movd   mm0,eax"        \
                "movq   [esi],mm0"      \
                parm [eax]

#pragma aux _m_to_int = \
                "mov    eax,[eax]"      \
                parm [eax]

#pragma aux __m_binary = parm [eax] [edx] modify exact []

#pragma aux (__m_binary) _m_packsswb =  \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "packsswb mm0,mm1"      \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_packssdw =  \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "packssdw mm0,mm1"      \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_packuswb =  \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "packuswb mm0,mm1"      \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_punpckhbw = \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "punpckhbw mm0,mm1"     \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_punpckhwd = \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "punpckhwd mm0,mm1"     \
                "movq   [esi],mm0"


#pragma aux (__m_binary) _m_punpckhdq = \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "punpckhdq mm0,mm1"     \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_punpcklbw = \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "punpcklbw mm0,mm1"     \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_punpcklwd = \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "punpcklwd mm0,mm1"     \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_punpckldq = \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "punpckldq mm0,mm1"     \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_paddb =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "paddb  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_paddw =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "paddw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_paddd =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "paddd  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_paddsb =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "paddsb mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_paddsw =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "paddsw mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_paddusb =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "paddusb mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_paddusw =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "paddusw mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psubb =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psubb  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psubw =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psubw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psubd =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psubd  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psubsb =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psubsb mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psubsw =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psubsw mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psubusb =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psubusb mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psubusw =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psubusw mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pmaddwd =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pmaddwd mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pmulhw =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pmulhw mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pmullw =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pmullw mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psllw =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psllw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psllwi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "psllw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pslld =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pslld  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pslldi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "pslld  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psllq =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psllq  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psllqi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "psllq  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psraw =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psraw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrawi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "psraw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrad =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psrad  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psradi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "psrad  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrlw =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psrlw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrlwi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "psrlw  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrld =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psrld  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrldi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "psrld  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrlq =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "psrlq  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_psrlqi =    \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movd   mm1,edx"        \
                "psrlq  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pand =      \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pand   mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pandn =     \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pandn  mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_por =       \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "por    mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pxor =      \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pxor   mm0,mm1"        \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pcmpeqb =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pcmpeqb mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pcmpeqw =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pcmpeqw mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pcmpeqd =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pcmpeqd mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pcmpgtb =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pcmpgtb mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pcmpgtw =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pcmpgtw mm0,mm1"       \
                "movq   [esi],mm0"

#pragma aux (__m_binary) _m_pcmpgtd =   \
                ".586"                  \
                "movq   mm0,[eax]"      \
                "movq   mm1,[edx]"      \
                "pcmpgtd mm0,mm1"       \
                "movq   [esi],mm0"

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
