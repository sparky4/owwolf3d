#ifndef __ID_TAIL_H__
#define __ID_TAIL_H__

//#include "id_heads.h"
//#include "id_head.h"
//#include "wl_def.h"
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
#include "type.h"
//#include "id_in.h"

#define	nil	((void *)0)
#define	TickBase	70		// 70Hz per tick - used as a base for timer 0

typedef short fixed;

extern	boolean		NoWait;
extern longword			TimeCount;

int US_CheckParm(char *parm,char **strings);

extern	void		IN_Startup(void),IN_Shutdown(void);
extern	boolean		IN_UserInput(longword delay);

#endif
