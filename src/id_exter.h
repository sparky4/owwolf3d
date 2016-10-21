#ifndef __ID_EXTER_H__
#define __ID_EXTER_H__

#include "src/wl_def.h"
#include "src/type.h"
#define	nil	((void *)0)
#define	TickBase	70		// 70Hz per tick - used as a base for timer 0

//typedef short fixed;
#ifndef	__ID_US__
extern	boolean		NoWait;

int US_CheckParm(char *parm,char **strings);
#endif
#ifndef __ID_VL_H__
void VL_Plot (int x, int y, int color);
void VL_Hlin (unsigned x, unsigned y, unsigned width, unsigned color);
void VL_Vlin (int x, int y, int height, int color);
void VL_FadeIn (int start, int end, byte far *palette, int steps);
#endif
#ifndef __ID_VH_H__
int VW_MarkUpdateBlock (int x1, int y1, int x2, int y2);
#endif

extern	void		IN_Startup(void),IN_Shutdown(void);
extern	boolean		IN_UserInput(longword delay);

#endif
