///////////////////////////////////////////////////////////////////////////
// FILE: iostream/iostream.h (iostreams "top level" file)
//
//                          Open Watcom Project
//
//    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
//
//  ========================================================================
//
//    This file contains Original Code and/or Modifications of Original
//    Code as defined in and that are subject to the Sybase Open Watcom
//    Public License version 1.0 (the 'License'). You may not use this file
//    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
//    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
//    provided with the Original Code and Modifications, and is also
//    available at www.sybase.com/developer/opensource.
//
//    The Original Code and all software distributed under the License are
//    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
//    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
//    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
//    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
//    NON-INFRINGEMENT. Please see the License for the specific language
//    governing rights and limitations under the License.
//
//  ========================================================================
//
// Description: This header is part of the C++ standard library. It
//              declares the global stream objects and includes other
//              iostream headers as necessary.
///////////////////////////////////////////////////////////////////////////
#ifndef _IOSTREAM_H_INCLUDED
#define _IOSTREAM_H_INCLUDED

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif


#ifndef _IOSTREAM_INCLUDED
  #include <iostream>
#endif
using std::cin;
using std::cout;
using std::cerr;
using std::clog;

// All included names should also be in the global namespace.
#ifndef _IOS_H_INCLUDED
  #include <ios.h>
#endif

#ifndef _STREAMBUF_H_INCLUDED
 #include <streambu.h>
#endif

#ifndef _ISTREAM_H_INCLUDED
  #include <istream.h>
#endif

#ifndef _OSTREAM_H_INCLUDED
  #include <ostream.h>
#endif

#endif
