/*
 *  bios.h      BIOS functions
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
#ifndef _BIOS_H_INCLUDED
#define _BIOS_H_INCLUDED
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

struct  _ibm_diskinfo_t {       /* disk parameters */
        unsigned drive;         /* drive number    */
        unsigned head;          /* head number     */
        unsigned track;         /* track number    */
        unsigned sector;        /* sector number   */
        unsigned nsectors;      /* number of sectors to read/write/compare  */
        void _WCFAR *buffer;    /* buffer to read to,write from, or compare */
};
#define _diskinfo_t     _ibm_diskinfo

#if !defined(NO_EXT_KEYS) /* extensions enabled */
#define diskinfo_t      _ibm_diskinfo_t
#endif

/* constants for BIOS disk access functions */
#define _DISK_RESET     0
#define _DISK_STATUS    1
#define _DISK_READ      2
#define _DISK_WRITE     3
#define _DISK_VERIFY    4
#define _DISK_FORMAT    5

/* constants for BIOS serial communications (RS-232) support */

/* serial port services */

#define _COM_INIT       0       /* init serial port */
#define _COM_SEND       1       /* send character */
#define _COM_RECEIVE    2       /* receive character */
#define _COM_STATUS     3       /* get serial port status */

/* serial port initializers.  One and only one constant from each of the
 * following four groups - character size, stop bit, parity, and baud rate -
 * must be specified in the initialization byte.
 */

/* character size initializers */

#define _COM_CHR7       2       /* 7 bits characters */
#define _COM_CHR8       3       /* 8 bits characters */

/* stop bit values - on or off */

#define _COM_STOP1      0       /* 1 stop bit */
#define _COM_STOP2      4       /* 2 stop bits */

/*  parity initializers */

#define _COM_NOPARITY   0       /* no parity */
#define _COM_ODDPARITY  8       /* odd parity */
#define _COM_SPACEPARITY 16     /* space parity */
#define _COM_EVENPARITY 24      /* even parity */

/*  baud rate initializers */

#define _COM_110        0       /* 110 baud */
#define _COM_150        32      /* 150 baud */
#define _COM_300        64      /* 300 baud */
#define _COM_600        96      /* 600 baud */
#define _COM_1200       128     /* 1200 baud */
#define _COM_2400       160     /* 2400 baud */
#define _COM_4800       192     /* 4800 baud */
#define _COM_9600       224     /* 9600 baud */

/* keyboard services */

#define _KEYBRD_READ        0   /* read next character from keyboard */
#define _KEYBRD_READY       1   /* check for keystroke */
#define _KEYBRD_SHIFTSTATUS 2   /* get current shift key status */

/* services for enhanced keyboards */

#define _NKEYBRD_READ           0x10    /* read next character from keyboard */
#define _NKEYBRD_READY          0x11    /* check for keystroke */
#define _NKEYBRD_SHIFTSTATUS    0x12    /* get current shift key status */

/* printer services */

#define _PRINTER_WRITE  0       /* write character to printer */
#define _PRINTER_INIT   1       /* initialize printer */
#define _PRINTER_STATUS 2       /* get printer status */

/* time-of-day services */

#define _TIME_GETCLOCK  0       /* get current clock count */
#define _TIME_SETCLOCK  1       /* set current clock count */


_WCRTLINK extern unsigned short _bios_disk(unsigned __cmd,
                                           struct diskinfo_t *__diskinfo);
_WCRTLINK extern unsigned short _bios_equiplist(void);
_WCRTLINK extern unsigned short _bios_keybrd(unsigned __cmd);
_WCRTLINK extern unsigned short _bios_memsize(void);
_WCRTLINK extern unsigned short _bios_printer(unsigned __cmd,unsigned __port,
                                              unsigned __data);
_WCRTLINK extern unsigned short _bios_serialcom(unsigned __cmd,unsigned __port,
                                                unsigned __data);
_WCRTLINK extern unsigned short _bios_timeofday(unsigned __cmd,long *__timeval);


#pragma pack( __pop )
#ifdef __cplusplus
} /* End of extern "C" */
#endif
#endif
