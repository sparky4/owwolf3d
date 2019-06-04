/*
 *  crtdbg.h    Run-time debugging support.
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
#ifndef _CRTDBG_H_INCLUDED
#define _CRTDBG_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif



#ifdef _DEBUG

        /********************************************************
        *                                                       *
        *                       Debug ON                        *
        *                                                       *
        ********************************************************/


/*
 * Some data types and #defines.
 */

#ifdef __NT__
    typedef void *      _HFILE;
#else
    typedef int         _HFILE;
#endif
typedef int             (*_CRT_REPORT_HOOK)( int, char *, int * );

#define _CRT_WARN               0
#define _CRT_ERROR              1
#define _CRT_ASSERT             2
#define _CRT_ERRCNT             3

#define _CRTDBG_MODE_FILE       0x1
#define _CRTDBG_MODE_DEBUG      0x2
#define _CRTDBG_MODE_WNDW       0x4
#define _CRTDBG_REPORT_MODE     (-1)

#define _CRTDBG_INVALID_HFILE   ((_HFILE)-1)
#define _CRTDBG_HFILE_ERROR     ((_HFILE)-2)
#define _CRTDBG_FILE_STDOUT     ((_HFILE)-4)
#define _CRTDBG_FILE_STDERR     ((_HFILE)-5)
#define _CRTDBG_REPORT_FILE     ((_HFILE)-6)


/*
 * Function prototypes.
 */

_WCRTLINK void              _CrtDbgBreak( void );

_WCRTLINK int               _CrtSetReportMode( int reporttype,
                                               int reportmode );

_WCRTLINK _HFILE            _CrtSetReportFile( int reporttype,
                                               _HFILE reportfile );

_WCRTLINK _CRT_REPORT_HOOK  _CrtSetReportHook( _CRT_REPORT_HOOK hook );

_WCRTLINK int               _CrtDbgReport( int reporttype,
                                           const char *filename,
                                           int linenumber,
                                           const char *modulename,
                                           const char *format,
                                           ... );


/*
 * Asserts and reports.
 */

#define _ASSERT(__expr)                                                     \
            if( !(__expr) ) {                                               \
                _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__,             \
                               NULL, NULL );                                \
            }
#define _ASSERTE(__expr)                                                    \
            if( !(__expr) ) {                                               \
                _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__,             \
                               NULL, #__expr );                             \
            }

#define _RPT0(__n,__msg)                                                    \
            _CrtDbgReport( __n, NULL, 0, NULL, "%s", __msg )
#define _RPT1(__n,__msg,__a)                                                \
            _CrtDbgReport( __n, NULL, 0, NULL, __msg, __a )
#define _RPT2(__n,__msg,__a,__b)                                            \
            _CrtDbgReport( __n, NULL, 0, NULL, __msg, __a, __b )
#define _RPT3(__n,__msg,__a,__b,__c)                                        \
            _CrtDbgReport( __n, NULL, 0, NULL, __msg, __a, __b, __c )
#define _RPT4(__n,__msg,__a,__b,__c,__d)                                    \
            _CrtDbgReport( __n, NULL, 0, NULL, __msg, __a, __b, __c, __d )

#define _RPTF0(__n,__msg)                                                   \
            _CrtDbgReport( __n, __FILE__, __LINE__, NULL, "%s", __msg )
#define _RPTF1(__n,__msg,__a)                                               \
            _CrtDbgReport( __n, __FILE__, __LINE__, NULL, __msg, __a )
#define _RPTF2(__n,__msg,__a,__b)                                           \
            _CrtDbgReport( __n, __FILE__, __LINE__, NULL, __msg, __a, __b )
#define _RPTF3(__n,__msg,__a,__b,__c)                                       \
            _CrtDbgReport( __n, __FILE__, __LINE__, NULL, __msg, __a, __b, __c )
#define _RPTF4(__n,__msg,__a,__b,__c,__d)                                   \
            _CrtDbgReport( __n, __FILE__, __LINE__, NULL, __msg, __a, __b, __c, __d )



#else   /* _DEBUG */



        /********************************************************
        *                                                       *
        *                       Debug OFF                       *
        *                                                       *
        ********************************************************/


/*
 * Some #defines to ignore debugging routines when _DEBUG isn't #defined.
 */

#define _CrtDbgBreak(__a)
#define _CrtSetReportMode(__a,__b)
#define _CrtSetReportFile(__a,__b)
#define _CrtSetReportHook(__a)

#define _ASSERT(__expr)
#define _ASSERTE(__expr)

#define _RPT0(__n,__msg)
#define _RPT1(__n,__msg,__a)
#define _RPT2(__n,__msg,__a,__b)
#define _RPT3(__n,__msg,__a,__b,__c)
#define _RPT4(__n,__msg,__a,__b,__c,__d)

#define _RPTF0(__n,__msg)
#define _RPTF1(__n,__msg,__a)
#define _RPTF2(__n,__msg,__a,__b)
#define _RPTF3(__n,__msg,__a,__b,__c)
#define _RPTF4(__n,__msg,__a,__b,__c,__d)


#endif  /* _DEBUG */

#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
