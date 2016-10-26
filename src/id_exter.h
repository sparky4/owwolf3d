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
extern	boolean		IN_UserInput(dword delay);


extern void SpawnBJVictory (void);
boolean		SetViewSize (unsigned width, unsigned height);
void		NewGame (int difficulty,int episode);
void 		NewViewSize (int width);
boolean 	LoadTheGame(int file,int x,int y);
boolean		SaveTheGame(int file,int x,int y);
void 		ShowViewSize (int width);
void PushWall (int checkx, int checky, int dir);
void OperateDoor (int door);
void InitAreas (void);

extern statetype s_grdstand, s_grdpath1, s_grdpath1s, s_grdpath2, s_grdpath3, s_grdpath3s, s_grdpath4, s_grdpain, s_grdpain1,s_grdgiveup, s_grdshoot1, s_grdshoot2, s_grdshoot3, s_grdshoot4, s_grdchase1, s_grdchase1s, s_grdchase2, s_grdchase3, s_grdchase3s, s_grdchase4, s_grddie1, s_grddie1d, s_grddie2, s_grddie3, s_grddie4;


#endif
