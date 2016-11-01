// ID_GLOB.H


#include <malloc.h>
#include <ctype.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <mem.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h> /* for LONG_MAX */
//#include <values.h>
//#include <dir.h>
#define __ID_GLOB__
#include "src/version.h"
#include "src/type.h"

//--------------------------------------------------------------------------

//extern	unsigned	char		far signon;

#define	introscn	signon

#ifdef JAPAN
#ifdef JAPDEMO
#include "src/foreign/japan/gfxv_wj1.h"
#else
#include "src/foreign/japan/gfxv_wj6.h"
#endif
#include "src/audiowl6.h"
#include "src/mapswl6.h"
#else

#ifndef SPEAR

#include "src/gfxv_wl6.h"
#include "src/audiowl6.h"
#include "src/mapswl6.h"

#else

#ifndef SPEARDEMO
#include "src/gfxv_sod.h"
#include "src/audiosod.h"
#include "src/mapssod.h"
#else
#include "src/gfxv_sdm.h"
#include "src/audiosdm.h"
#include "src/mapssdm.h"
#endif

#endif
#endif
//-----------------


#define GREXT	"VGA"

//
//	ID Engine
//	Types.h - Generic types, #defines, etc.
//	v1.0d1
//

#define	nil	((void *)0)


#include "src/id_mm.h"
#include "src/id_pm.h"
#include "src/id_ca.h"
#include "src/id_vl.h"
#include "src/id_vh.h"
#include "src/id_in.h"
#include "src/id_sd.h"
#include "src/id_us.h"


void	Quit (char *error);		// defined in user program

//
// replacing refresh manager with custom routines
//

#define	PORTTILESWIDE		20      // all drawing takes place inside a
#define	PORTTILESHIGH		13		// non displayed port of this size

#define UPDATEWIDE			PORTTILESWIDE
#define UPDATEHIGH			PORTTILESHIGH

#define	MAXTICS				10
#define DEMOTICS			4

#define	UPDATETERMINATE	0x0301

extern	unsigned	mapwidth,mapheight,tics;
extern	boolean		compatability;

extern	byte		*updateptr;
extern	unsigned	uwidthtable[UPDATEHIGH];
extern	unsigned	blockstarts[UPDATEWIDE*UPDATEHIGH];

extern	byte		fontcolor,backcolor;

#define SETFONTCOLOR(f,b) fontcolor=f;backcolor=b;
extern byte far signon[];
