/*
 *  jctype.h    Japanese character test macros
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
#ifndef _JCTYPE_H_INCLUDED
#define _JCTYPE_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif


/*
 *       iskana(c)       �J�i�E�R�[�h
 *       iskpun(c)       �J�i��ؕ���
 *       iskmoji(c)      �J�i����
 *       isalkana(c)     �p�����܂��̓J�i����
 *       ispnkana(c)     �p��ؕ����܂��̓J�i��ؕ���
 *       isalnmkana(c)   �p�����܂��̓J�i����
 *       isprkana(c)     �\���\�����i�󔒂��܂ށj
 *       isgrkana(c)     �\���\�����i�󔒂������j
 *
 *       iskanji(c)      ������P�o�C�g
 *       iskanji2(c)     ������Q�o�C�g
 *
 */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif
#ifndef _CTYPE_H_INCLUDED
 #include <ctype.h>
#endif

#define _K      0x01    /* Kana moji      */
#define _KP     0x02    /* Kana punct.    */
#define _J1     0x04    /* Kanji 1st byte */
#define _J2     0x08    /* Kanji 2nd byte */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
_WCRTLINK extern int    iskana(int);
_WCRTLINK extern int    iskpun(int);
_WCRTLINK extern int    iskmoji(int);
_WCRTLINK extern int    isalkana(int);
_WCRTLINK extern int    ispnkana(int);
_WCRTLINK extern int    isalnmkana(int);
_WCRTLINK extern int    isprkana(int);
_WCRTLINK extern int    isgrkana(int);
_WCRTLINK extern int    iskanji(int);
_WCRTLINK extern int    iskanji2(int);

/* kana, kanji type table */
#if defined(__SW_BR) || defined(_RTDLL)
 #define _IsKTable _IsKTable_br
#endif
_WCRTLINK extern const char _WCDATA _IsKTable[257];
#ifdef __cplusplus
} /* End of extern "C" */
#endif

#if !defined(__FUNCTION_DATA_ACCESS)
#define iskana(__c)     (_IsKTable[(unsigned char)(__c)+1] & (_K|_KP))
#define iskpun(__c)     (_IsKTable[(unsigned char)(__c)+1] & _KP)
#define iskmoji(__c)    (_IsKTable[(unsigned char)(__c)+1] & _K)
#define isalkana(__c)   (isalpha(__c) || iskmoji(__c))
#define ispnkana(__c)   (ispunct(__c) || iskpun(__c))
#define isalnmkana(__c) (isalnum(__c) || iskmoji(__c))
#define isprkana(__c)   (isprint(__c) || iskana(__c))
#define isgrkana(__c)   (isgraph(__c) || iskana(__c))

#define iskanji(__c)    (_IsKTable[(unsigned char)(__c)+1] & _J1)
#define iskanji2(__c)   (_IsKTable[(unsigned char)(__c)+1] & _J2)
#endif

#endif
