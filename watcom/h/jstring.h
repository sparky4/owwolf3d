/*
 *  jstring.h   Japanese DBCS functions
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
#ifndef _JSTRING_H_INCLUDED
#define _JSTRING_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#ifdef __cplusplus
  #ifndef _STDSIZE_T_DEFINED
    #define _STDSIZE_T_DEFINED
    namespace std {
      typedef unsigned size_t;
    }
    typedef std::size_t _w_size_t;
  #endif
  #ifndef _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED_
    using std::size_t;
  #endif
#else
  #ifndef _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED_
    typedef unsigned size_t;
    typedef size_t   _w_size_t;
  #endif
#endif /* __cplusplus */

#ifndef NULL
#if defined( __SMALL__ ) || defined( __MEDIUM__ ) || defined( __386__ ) || defined( __AXP__ ) || defined( __PPC__ )
#define                         NULL                    0
#else
#define                         NULL                    0L
#endif
#endif

/*
 * definitions for chkctype(), nthctype()
 */

#define CT_ANK  0       /* ANK */
#define CT_KJ1  1       /* ������P�o�C�g */
#define CT_KJ2  2       /* ������Q�o�C�g */
#define CT_ILGL -1      /* �C���[�K�� */

typedef unsigned char JCHAR;    /* �����J�i�p��������(char for Japan) */
typedef JCHAR        *JSTRING;  /* ����������(string for Japan) */
typedef JCHAR _WCFAR *FJSTRING; /* ���� far ������(far string for Japan) */
typedef unsigned short JMOJI;   /* �����R�[�h(2byte code) */

/*
 *
 *       ���������֐��p�v���g�^�C�v�錾
 *
 */

_WCRTLINK extern size_t btom( const JCHAR *__s, size_t __nb );
_WCRTLINK extern size_t mtob( const JCHAR *__s, size_t __nm );

_WCRTLINK extern JMOJI hantozen( JMOJI __c );
_WCRTLINK extern JMOJI zentohan( JMOJI __c );

_WCRTLINK extern int chkctype( JCHAR __c, int __n );
_WCRTLINK extern int nthctype( const JCHAR *__s, size_t __n );

_WCRTLINK extern JMOJI jtolower( JMOJI __c );
_WCRTLINK extern JMOJI jtoupper( JMOJI __c );
_WCRTLINK extern JMOJI jtohira( JMOJI __c );
_WCRTLINK extern JMOJI jtokata( JMOJI __c );

_WCRTLINK extern int jisalpha( JMOJI __c );
_WCRTLINK extern int jisdigit( JMOJI __c );
_WCRTLINK extern int jishira( JMOJI __c );
_WCRTLINK extern int jiskana( JMOJI __c );
_WCRTLINK extern int jiskigou( JMOJI __c );
_WCRTLINK extern int jisl0( JMOJI __c );
_WCRTLINK extern int jisl1( JMOJI __c );
_WCRTLINK extern int jisl2( JMOJI __c );
_WCRTLINK extern int jislower( JMOJI __c );
_WCRTLINK extern int jisprint( JMOJI __c );
_WCRTLINK extern int jisspace( JMOJI __c );
_WCRTLINK extern int jisupper( JMOJI __c );
_WCRTLINK extern int jiszen( JMOJI __c );

_WCRTLINK extern JSTRING jgetmoji( const JCHAR *__s, JMOJI *__c );
_WCRTLINK extern unsigned short jistojms( unsigned short __c );
_WCRTLINK extern unsigned short jmstojis( unsigned short __msc );
_WCRTLINK extern void jperror( const char *__msg );
_WCRTLINK extern JSTRING jputmoji( JCHAR *__s, JMOJI __c );
_WCRTLINK extern JSTRING jstradv( const JCHAR *__s, size_t __n );
_WCRTLINK extern JSTRING jstrcat( JCHAR *__to, const JCHAR *__from );
_WCRTLINK extern JSTRING jstrchr( const JCHAR *__s, JMOJI __c );
_WCRTLINK extern int jstrcmp( const JCHAR *__s1, const JCHAR *__s2 );
_WCRTLINK extern size_t jstrcspn( const JCHAR *__s1, const JCHAR *__s2 );
_WCRTLINK extern unsigned char *jstrerror( int __errnum );
_WCRTLINK extern int jstricmp( const JCHAR *__s1, const JCHAR *__s2 );
_WCRTLINK extern size_t jstrlen( const JCHAR *__s );
_WCRTLINK extern JSTRING jstrlwr( JCHAR *__s );
_WCRTLINK extern JSTRING jstrmatch( const JCHAR *__s1, const JCHAR *__s2 );
_WCRTLINK extern JSTRING jstrncat( JCHAR *__to, const JCHAR *__from, size_t __n );
_WCRTLINK extern int jstrncmp( const JCHAR *__s1, const JCHAR *__s2, size_t __n );
_WCRTLINK extern JSTRING jstrncpy( JCHAR *__to, const JCHAR *__from, size_t __n );
_WCRTLINK extern int jstrnicmp( const JCHAR *__s1, const JCHAR *__s2, size_t __n );
_WCRTLINK extern JSTRING jstrnset( JCHAR *__s, JMOJI __c, size_t __n );
_WCRTLINK extern JSTRING jstrrchr( const JCHAR *__s, JMOJI __c );
_WCRTLINK extern JSTRING jstrrev( JCHAR *__s );
_WCRTLINK extern JSTRING jstrset( JCHAR *__s, JMOJI __c );
_WCRTLINK extern JSTRING jstrskip( const JCHAR *__s1, const JCHAR *__s2 );
_WCRTLINK extern size_t jstrspn( const JCHAR *__s1, const JCHAR *__s2 );
_WCRTLINK extern JSTRING jstrstr( const JCHAR *__s1, const JCHAR *__s2 );
_WCRTLINK extern JSTRING jstrtok( JCHAR *__targ, const JCHAR *__brk );
_WCRTLINK extern JSTRING jstrupr( JCHAR *__s );
/* ----------- memory model independent equivalents ----------- */
_WCRTLINK extern size_t _WCI86FAR _fbtom( const JCHAR _WCFAR *__s, size_t __nb );
_WCRTLINK extern size_t _WCI86FAR _fmtob( const JCHAR _WCFAR *__s, size_t __nm );
_WCRTLINK extern int _WCI86FAR _fnthctype( const JCHAR _WCFAR *__string, size_t __n );
_WCRTLINK extern FJSTRING _WCI86FAR _fjgetmoji( const JCHAR _WCFAR *__s, JMOJI _WCFAR *__c );
_WCRTLINK extern FJSTRING _WCI86FAR _fjputmoji( JCHAR _WCFAR *__s, JMOJI __c );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstradv( const JCHAR _WCFAR *__s, size_t __n );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrcat( JCHAR _WCFAR *__to, const JCHAR _WCFAR *__from );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrchr( const JCHAR _WCFAR *__s, JMOJI __c );
_WCRTLINK extern int _WCI86FAR _fjstrcmp( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2 );
_WCRTLINK extern size_t _WCI86FAR _fjstrcspn( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2 );
_WCRTLINK extern int _WCI86FAR _fjstricmp( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2 );
_WCRTLINK extern size_t _WCI86FAR _fjstrlen( const JCHAR _WCFAR *__s );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrlwr( JCHAR _WCFAR *__s );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrmatch( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2 );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrncat( JCHAR _WCFAR *__to, const JCHAR _WCFAR *__from, size_t __n );
_WCRTLINK extern int _WCI86FAR _fjstrncmp( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2, size_t __n );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrncpy( JCHAR _WCFAR *__to, const JCHAR _WCFAR *__from, size_t __n );
_WCRTLINK extern int _WCI86FAR _fjstrnicmp( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2, size_t __n );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrnset( JCHAR _WCFAR *__s, JMOJI __c, size_t __n );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrrchr( const JCHAR _WCFAR *__s, JMOJI __c );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrrev( JCHAR _WCFAR *__s );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrset( JCHAR _WCFAR *__s, JMOJI __c );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrskip( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2 );
_WCRTLINK extern size_t _WCI86FAR _fjstrspn( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2 );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrstr( const JCHAR _WCFAR *__s1, const JCHAR _WCFAR *__s2 );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrtok( JCHAR _WCFAR *__targ, const JCHAR _WCFAR *__brk );
_WCRTLINK extern FJSTRING _WCI86FAR _fjstrupr( JCHAR _WCFAR *__s );
/* ----------- end of memory model independent equivalents ----------- */

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
