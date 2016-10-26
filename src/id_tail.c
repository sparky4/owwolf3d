#include "src/id_tail.h"

//===========================================================================
//ca
byte 		_seg	*tinf;
int			mapon;

unsigned	_seg	*mapsegs[MAPPLANES];
maptype		_seg	*mapheaderseg[NUMMAPS];
byte		_seg	*audiosegs[NUMSNDCHUNKS];
void		_seg	*grsegs[NUMCHUNKS];

byte		far	grneeded[NUMCHUNKS];
byte		ca_levelbit,ca_levelnum;

int			profilehandle,debughandle;

char		audioname[13]="AUDIO.";

//===========================================================================
//in
//
// configuration variables
//
boolean			MousePresent;
boolean			JoysPresent[MaxJoys];
boolean			JoyPadPresent;


// 	Global variables
//		boolean		Keyboard[NumCodes];
		boolean		Paused;
		char		LastASCII;
		ScanCode	LastScan;

		KeyboardDef	KbdDefs = {0x1d,0x38,0x47,0x48,0x49,0x4b,0x4d,0x4f,0x50,0x51};
		JoystickDef	JoyDefs[MaxJoys];
		ControlType	Controls[MaxPlayers];

		/*long*/word	MouseDownCount;

		Demo		DemoMode = demo_Off;
		byte		*DemoBuffer;
		word		DemoOffset,DemoSize;

//===========================================================================
//mm
mminfotype	mminfo;
memptr		bufferseg;
boolean		mmerror;

void		(* beforesort) (void);
void		(* aftersort) (void);
//===========================================================================
//us
//	Global variables
		char		*abortprogram;
		boolean		NoWait;
		word		PrintX,PrintY;
		word		WindowX,WindowY,WindowW,WindowH;

//===========================================================================
//vh
byte		update[UPDATESIZE];

pictabletype *pictable;


int	px,py;
byte	fontcolor,backcolor;
int	fontnumber;
//____int bufferwidth,bufferheight;

//===========================================================================
//===========================================================================
//===========================================================================
//===========================================================================
//===========================================================================
//wl_act1
/*_
PushWall_
OperateDoor_
InitAreas_
scale
heightnumerator
doorposition
doorobjlist
pwallpos
dirangle
statobjlist
laststatobj
s_deathcam
pwalldir
pwallx
pwally
pwalltile
focallength
NewGame_
LoadTheGame_
SaveTheGame_
ShowViewSize_
NewViewSize_
_areabyplayer
MoveDoors_
MovePWalls_
OpenDoor_
_areabyplayer
PlaceItemType_
_s_grddie1
_s_ofcdie1
_s_mutdie1
_s_ssdie1
_s_dogdie1
_s_bossdie1
_s_greteldie1
_s_giftdie1
_s_fatdie1
_s_schabbdie1
_s_fakedie1
_s_mechadie1
_s_hitlerdie1
A_DeathScream_
_s_grdpain
_s_grdpain1
_s_ofcpain
_s_ofcpain1
_s_mutpain
_s_mutpain1
_s_sspain
_s_sspain1
_doorposition
_s_grdchase1
_s_ofcchase1
_s_mutchase1
_s_sschase1
_s_dogchase1
_s_bosschase1
_s_gretelchase1
_s_giftchase1
_s_fatchase1
_s_schabbchase1
_s_fakechase1
_s_mechachase1
_s_hitlerchase1
_s_blinkychase1
SpawnStatic_
SpawnStand_
SpawnPatrol_
SpawnDeadGuard_
SpawnBoss_
SpawnGretel_
SpawnGift_
SpawnFat_
SpawnSchabbs_
SpawnFakeHitler_
SpawnHitler_
SpawnGhosts_
InitDoorList_
InitStaticList_
SpawnDoor_
NewGame_*/

//===========================================================================
//wl_act2
//===========================================================================
//wl_agent
//
// player state info
//
boolean		running;
long		thrustspeed;

unsigned	plux,pluy;			// player coordinates scaled to unsigned

int			anglefrac;
int			gotgatgun;	// JR

objtype		*LastAttacker;
//===========================================================================
//wl_draw

#ifdef DEBUGWALLS
unsigned screenloc[3]= {0,0,0};
#else
unsigned screenloc[3]= {PAGE1START,PAGE2START,PAGE3START};
#endif
unsigned freelatch = FREESTART;

fixed	lasttimecount;
fixed	frameon;

unsigned	wallheight[MAXVIEWWIDTH];

fixed	tileglobal	= TILEGLOBAL;
fixed	mindist		= MINDIST;


//
// math tables
//
int			pixelangle[MAXVIEWWIDTH];
fixed		far finetangent[FINEANGLES/4];
fixed 		far sintable[ANGLES+ANGLES/4],far *costable = sintable+(ANGLES/4);

//
// refresh variables
//
fixed	viewx,viewy;			// the focal point
int		viewangle;
fixed	viewsin,viewcos;

//===========================================================================
//wl_game
boolean		ingame,fizzlein;
unsigned	latchpics[NUMLATCHPICS];
gametype	gamestate;

fixed		spearx,speary;
unsigned	spearangle;
boolean		spearflag;

//===========================================================================
//wl_play
boolean		madenoise;					// true when shooting or screaming

exit_t		playstate;

//int			DebugOk;

//++++objtype 	objlist[MAXACTORS],*new,*obj,*player,*lastobj,*objfreelist,*killerobj;

unsigned	farmapylookup[MAPSIZE];
byte		*nearmapylookup[MAPSIZE];

boolean		singlestep,godmode,noclip;
int			extravbls;

byte		tilemap[MAPSIZE][MAPSIZE];	// wall values only
byte		spotvis[MAPSIZE][MAPSIZE];
//++++objtype		*actorat[MAPSIZE][MAPSIZE];

//
// replacing refresh manager
//
//unsigned	mapwidth,mapheight,tics;
boolean		compatability;
byte		*updateptr;
unsigned	mapwidthtable[64];
unsigned	uwidthtable[UPDATEHIGH];
unsigned	blockstarts[UPDATEWIDE*UPDATEHIGH];
//byte		update[UPDATESIZE];
unsigned tics;

//
// control info
//
boolean		mouseenabled,joystickenabled,joypadenabled,joystickprogressive;
int			joystickport;
int			dirscan[4] = {sc_UpArrow,sc_RightArrow,sc_DownArrow,sc_LeftArrow};
int			buttonscan[NUMBUTTONS] =
			{sc_Control,sc_Alt,sc_RShift,sc_Space,sc_1,sc_2,sc_3,sc_4};
int			buttonmouse[4]={bt_attack,bt_strafe,bt_use,bt_nobutton};
int			buttonjoy[4]={bt_attack,bt_strafe,bt_use,bt_run};

int			viewsize;

boolean		buttonheld[NUMBUTTONS];

boolean		demorecord,demoplayback;
char		far *demoptr, far *lastdemoptr;
memptr		demobuffer;

//
// curent user input
//
int			controlx,controly;		// range from -100 to 100 per tic
boolean		buttonstate[NUMBUTTONS];

//===========================================================================
//wl_scale
t_compscale *scaledirectory[MAXSCALEHEIGHT+1];
//_seg
long			fullscalefarcall[MAXSCALEHEIGHT+1];

int			maxscale,maxscaleshl2;

boolean	insetupscaling;

//===========================================================================
//===========================================================================
/*
==================
=
= DebugMemory
=
==================
*/

void DebugMemory_(boolean q)
{
	/*VW_FixRefreshBuffer ();
	US_CenterWindow (16,7);

	US_CPrint ("Memory Usage");
	US_CPrint ("------------");
	US_Print ("Total     :");
	US_PrintUnsigned (mminfo.mainmem/1024);
	US_Print ("k\nFree      :");
	US_PrintUnsigned (MM_UnusedMemory()/1024);
	US_Print ("k\nWith purge:");
	US_PrintUnsigned (MM_TotalFree()/1024);
	US_Print ("k\n");
	VW_UpdateScreen();*/
	if(q){
	printf("========================================\n");
	printf("		DebugMemory_\n");
	printf("========================================\n");}
	if(q) { printf("Memory Usage\n");
	printf("------------\n"); }else printf("	%c%c", 0xD3, 0xC4);
	printf("Total:	"); if(q) printf("	"); printf("%uk", mminfo.mainmem/1024);
	if(q) printf("\n"); else printf("	");
	printf("Free:	"); if(q) printf("	"); printf("%uk", MM_UnusedMemory()/1024);
	if(q) printf("\n"); else printf("	");
	printf("With purge:"); if(q) printf("	"); printf("%uk\n", MM_TotalFree()/1024);
	if(q) printf("------------\n");
	IN_Ack ();
	if(q) MM_ShowMemory ();
}
