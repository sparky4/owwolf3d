///////////////////////////////////////////////////////////////////////////
// FILE: strstrea/strstrea.h (strstream support)
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
//              defines the necessary classes for strstream support.
///////////////////////////////////////////////////////////////////////////
#ifndef _STRSTREA_H_INCLUDED
#define _STRSTREA_H_INCLUDED

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif


#ifndef _STRSTREA_INCLUDED
  #include <strstrea>
#endif
using std::strstreambuf;
using std::strstreambase;
using std::istrstream;
using std::ostrstream;
using std::strstream;

// All included names should also be in the global namespace.
#ifndef _IOSTREAM_H_INCLUDED
 #include <iostream.h>
#endif

#endif
