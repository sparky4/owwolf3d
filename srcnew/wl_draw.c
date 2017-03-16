// WL_DRAW.C

#include "WL_DEF.H"
#include <DOS.H>
#pragma hdrstop

//#define DEBUGWALLS
//#define DEBUGTICS

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

// the door is the last picture before the sprites
#define DOORWALL	(PMSpriteStart-8)

#define ACTORSIZE	0x4000

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/


#ifdef DEBUGWALLS
unsigned screenloc[3]= {0,0,0};
#else
unsigned screenloc[3]= {PAGE1START,PAGE2START,PAGE3START};
#endif
unsigned freelatch = FREESTART;

long 	lasttimecount;
long 	frameon;

unsigned	wallheight[MAXVIEWWIDTH];

fixed	tileglobal	= TILEGLOBAL;
fixed	mindist		= MINDIST;


//
// math tables
//
int			pixelangle[MAXVIEWWIDTH];
long		far finetangent[FINEANGLES/4];
fixed 		far sintable[ANGLES+ANGLES/4],far *costable = sintable+(ANGLES/4);

//
// refresh variables
//
fixed	viewx,viewy;			// the focal point
int		viewangle;
fixed	viewsin,viewcos;



fixed	FixedByFrac (fixed a, fixed b);
void	TransformActor (objtype *ob);
void	BuildTables (void);
void	ClearScreen (void);
int		CalcRotate (objtype *ob);
void	DrawScaleds (void);
void	CalcTics (void);
void	FixOfs (void);
void	ThreeDRefresh (void);



//
// wall optimization variables
//
int		lastside;		// true for vertical
long	lastintercept;
int		lasttilehit;


//
// ray tracing variables
//
int			focaltx,focalty,viewtx,viewty;

int			midangle,angle;
unsigned	xpartial,ypartial;
unsigned	xpartialup,xpartialdown,ypartialup,ypartialdown;
unsigned	xinttile,yinttile;

unsigned	tilehit;
unsigned	pixx;

int		xtile,ytile;
int		xtilestep,ytilestep;
long	xintercept,yintercept;
long	xstep,ystep;

int		horizwall[MAXWALLTILES],vertwall[MAXWALLTILES];


/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/


void AsmRefresh (void);			// in WL_DR_A.ASM
boolean chsw=0;

/*
============================================================================

			   3 - D  DEFINITIONS

============================================================================
*/


//==========================================================================


/*
========================
=
= FixedByFrac
=
= multiply a 16/16 bit, 2's complement fixed point number by a 16 bit
= fraction, passed as a signed magnitude 32 bit number
=
========================
*/

#pragma warn -rvl			// I stick the return value in with ASMs

fixed FixedByFrac(fixed a, fixed b)
{
//#ifdef __WATCOMC__
	fixed pee;//,c,d;
//#endif
//
// setup
//
	__asm {
		mov	si,[WORD PTR b+2]	// sign of result = sign of fraction

		mov	ax,[WORD PTR a]
		mov	cx,[WORD PTR a+2]

		or	cx,cx
		jns	aok				// negative?
		neg	cx
		neg	ax
		sbb	cx,0
		xor	si,0x8000			// toggle sign of result
#ifdef __BORLANDC__
	}
#endif
aok:
#ifdef __BORLANDC__
	__asm {
#endif

//
// multiply  cx:ax by bx
//
		mov	bx,[WORD PTR b]
		mul	bx					// fraction*fraction
		mov	di,dx				// di is low word of result
		mov	ax,cx				//
		mul	bx					// units*fraction
		add	ax,di
		adc	dx,0

//
// put result dx:ax in 2's complement
//
		test	si,0x8000		// is the result negative?
		jz	ansok
		neg	dx
		neg	ax
		sbb	dx,0
#ifdef __BORLANDC__
	}
#endif
ansok:;

//#ifdef __WATCOMC__
#ifdef __BORLANDC__
	__asm {
#endif
		mov	[WORD PTR pee],ax
		mov	[WORD PTR pee+2],dx
//		mov	[word ptr c],ax
//		mov	[word ptr d],dx
	}
//	printf("FixedByFrac_()\n");
//	printf("	ax=%ld\n", c);
//	printf("	dx=%ld\n", d);
//	printf("	pee=%ld\n", pee);
	return pee;
//#endif
}

#pragma warn +rvl

//==========================================================================

/*
========================
=
= TransformActor
=
= Takes paramaters:
=   gx,gy		: globalx/globaly of point
=
= globals:
=   viewx,viewy		: point of view
=   viewcos,viewsin	: sin/cos of viewangle
=   scale		: conversion from global value to screen value
=
= sets:
=   screenx,transx,transy,screenheight: projected edge location and size
=
========================
*/


//
// transform actor
//
void TransformActor (objtype *ob)
{
	int ratio;
	fixed gx,gy,gxt,gyt,nx,ny;
	long	temp;

//
// translate point to view centered coordinates
//
	gx = ob->x-viewx;
	gy = ob->y-viewy;

//
// calculate newx
//
	gxt = FixedByFrac(gx,viewcos);
	gyt = FixedByFrac(gy,viewsin);
	nx = gxt-gyt-ACTORSIZE;		// fudge the shape forward a bit, because
								// the midpoint could put parts of the shape
								// into an adjacent wall

//
// calculate newy
//
	gxt = FixedByFrac(gx,viewsin);
	gyt = FixedByFrac(gy,viewcos);
	ny = gyt+gxt;

//
// calculate perspective ratio
//
	ob->transx = nx;
	ob->transy = ny;

	if (nx<mindist)			// too close, don't overflow the divide
	{
	  ob->viewheight = 0;
	  return;
	}

	ob->viewx = centerx + ny*scale/nx;	// DEBUG: use assembly divide

//
// calculate height (heightnumerator/(nx>>8))
//
	asm	mov	ax,[WORD PTR heightnumerator]
	asm	mov	dx,[WORD PTR heightnumerator+2]
	asm	idiv	[WORD PTR nx+1]			// nx>>8
	asm	mov	[WORD PTR temp],ax
	asm	mov	[WORD PTR temp+2],dx

	ob->viewheight = temp;
}

//==========================================================================

/*
========================
=
= TransformTile
=
= Takes paramaters:
=   tx,ty		: tile the object is centered in
=
= globals:
=   viewx,viewy		: point of view
=   viewcos,viewsin	: sin/cos of viewangle
=   scale		: conversion from global value to screen value
=
= sets:
=   screenx,transx,transy,screenheight: projected edge location and size
=
= Returns true if the tile is withing getting distance
=
========================
*/

boolean TransformTile (int tx, int ty, int *dispx, int *dispheight)
{
	int ratio;
	fixed gx,gy,gxt,gyt,nx,ny;
	long	temp;

//
// translate point to view centered coordinates
//
	gx = ((long)tx<<TILESHIFT)+0x8000-viewx;
	gy = ((long)ty<<TILESHIFT)+0x8000-viewy;

//
// calculate newx
//
	gxt = FixedByFrac(gx,viewcos);
	gyt = FixedByFrac(gy,viewsin);
	nx = gxt-gyt-0x2000;		// 0x2000 is size of object

//
// calculate newy
//
	gxt = FixedByFrac(gx,viewsin);
	gyt = FixedByFrac(gy,viewcos);
	ny = gyt+gxt;


//
// calculate perspective ratio
//
	if (nx<mindist)			// too close, don't overflow the divide
	{
		*dispheight = 0;
		return false;
	}

	*dispx = centerx + ny*scale/nx;	// DEBUG: use assembly divide

//
// calculate height (heightnumerator/(nx>>8))
//
	asm	mov	ax,[WORD PTR heightnumerator]
	asm	mov	dx,[WORD PTR heightnumerator+2]
	asm	idiv	[WORD PTR nx+1]			// nx>>8
	asm	mov	[WORD PTR temp],ax
	asm	mov	[WORD PTR temp+2],dx

	*dispheight = temp;

//
// see if it should be grabbed
//
	if (nx<TILEGLOBAL && ny>-TILEGLOBAL/2 && ny<TILEGLOBAL/2)
		return true;
	else
		return false;
}

//==========================================================================

/*
====================
=
= CalcHeight
=
= Calculates the height of xintercept,yintercept from viewx,viewy
=
====================
*/

#pragma warn -rvl			// I stick the return value in with ASMs

unsigned CalcHeight (void)
{
	unsigned pee;
	//unsigned	transheight;
	//unsigned ratio;
	fixed gxt,gyt,nx;//,ny;
	long	gx,gy;
	unsigned a,b,c,d;

	gx = xintercept-viewx;
	gxt = FixedByFrac(gx,viewcos);

	gy = yintercept-viewy;
	gyt = FixedByFrac(gy,viewsin);

	nx = gxt-gyt;

  //
  // calculate perspective ratio (heightnumerator/(nx>>8))
  //
	if (nx<mindist)
		nx=mindist;			// don't let divide overflow

	asm	mov	ax,[WORD PTR heightnumerator]
	asm	mov	dx,[WORD PTR heightnumerator+2]
	asm	div	[WORD PTR nx+1]			// nx>>8
	if(chsw)
	{
	__asm {
		mov	ax,[WORD PTR heightnumerator]
		mov	dx,[WORD PTR heightnumerator+2]
		div	[WORD PTR d+1]			// nx>>8
		mov	a,ax
		mov	b,dx
		//mov	[byte ptr b],al
	}
	pee=(heightnumerator/(nx>>8));
	printf("CalcHeight()\n");
	printf("heightnumerator\n%ld	", heightnumerator);
	printf("%u	", heightnumerator);
	printf("%u	", heightnumerator>>8);
	printf("%u\n", heightnumerator/heightnumerator>>8);
	printf("a	%u\n", a);
	printf("b	%u\n", b);
	printf("d	%u\n", d);
	printf("d	%u\n", d>>10);
	printf("ax	%u\n", _AX);
	printf("al	%u\n", _AL);
	printf("	nx	%u\n", nx>>16);//*((&nx)+1));
	printf("pee	%u\n", pee);
	printf("gxt	gyt	nx	gx	gy\n");
	printf("%ld	%ld	%ld	%ld	%ld\n", gxt, gyt, nx, gx, gy);
	printf("%u	%u	%u	%u	%u\n", gxt, gyt, nx, gx, gy);
	//printf("	nx=%lu\n", nx);
	}/*
	__asm {
	mov	ax,[word ptr xintercept]
	sub	ax,[word ptr viewx]
	imul	[word ptr viewcos]
	shr	ax,1
	shr	ax,1
	shr	dx,1
	shr	dx,1
	mov	di,ax
	mov	ax,[word ptr yintercept]
	sub	ax,[word ptr viewy]
	imul	[word ptr viewsin]
	shr	ax,1
	shr	ax,1
	shr	dx,1
	shr	dx,1
	sub	di,ax
	cmp	di,1024
	jge	dontclip
	mov	di,1024
#ifdef __BORLANDC__
	}
#endif
dontclip:
#ifdef __BORLANDC__
	__asm {
#endif
	xor	dx,dx
	sar	di,1
	sar	di,1
	sar	di,1
	sar	di,1
	sar	di,1
	sar	di,1
	mov	ax,[word ptr heightnumerator]
	sar	ax,1
	div	di
	mov	pee,ax
	}*/
	//return pee;
}


//==========================================================================

/*
===================
=
= ScalePost
=
===================
*/

long		postsource;
unsigned	postx;
unsigned	postwidth;

void	near ScalePost (void)		// VGA version
{
	__asm {
			mov	ax,SCREENSEG
			mov	es,ax

			mov	bx,[postx]
			shl	bx,1
			mov	bp,WORD PTR [wallheight+bx]		// fractional height (low 3 bits frac)
			and	bp,0xfff8				// bp = heightscaler*4
			shr	bp,1
			cmp	bp,[maxscaleshl2]
			jle	heightok
			mov	bp,[maxscaleshl2]
#ifdef __BORLANDC__
	}
#endif
heightok:
#ifdef __BORLANDC__
	__asm {
#endif
			add	bp,OFFSET fullscalefarcall
	//
	// scale a byte wide strip of wall
	//
			mov	bx,[postx]
			mov	di,bx
			shr	di,1						// X in bytes
			shr	di,1
			add	di,[bufferofs]

			and	bx,3
	/* begin 8086 hack
			shl	bx,3
	*/
			push cx
			mov cl,3
			shl bx,cl
			pop cx
	/* end 8086 hack */
			add	bx,[postwidth]

			mov	al,BYTE PTR [mapmasks1-1+bx]	// -1 because no widths of 0
			mov	dx,SC_INDEX+1
			out	dx,al						// set bit mask register
			lds	si,DWORD PTR [postsource]
			call DWORD PTR [bp]				// scale the line of pixels

			mov	al,BYTE PTR [ss:mapmasks2-1+bx]   // -1 because no widths of 0
			or	al,al
			jz	nomore

	//
	// draw a second byte for vertical strips that cross two bytes
	//
			inc	di
			out	dx,al						// set bit mask register
			call DWORD PTR [bp]				// scale the line of pixels

			mov	al,BYTE PTR [ss:mapmasks3-1+bx]	// -1 because no widths of 0
			or	al,al
			jz	nomore
	//
	// draw a third byte for vertical strips that cross three bytes
	//
			inc	di
			out	dx,al						// set bit mask register
			call DWORD PTR [bp]				// scale the line of pixels
#ifdef __BORLANDC__
	}
#endif
nomore:
#ifdef __BORLANDC__
	__asm {
#endif
			mov	ax,ss
			mov	ds,ax
	}
}

void  FarScalePost (void)				// just so other files can call
{
	ScalePost ();
}


/*
====================
=
= HitVertWall
=
= tilehit bit 7 is 0, because it's not a door tile
= if bit 6 is 1 and the adjacent tile is a door tile, use door side pic
=
====================
*/

void HitVertWall (void)
{
	int			wallpic;
	unsigned	texture;

	texture = (yintercept>>4)&0xfc0;
	if (xtilestep == -1)
	{
		texture = 0xfc0-texture;
		xintercept += TILEGLOBAL;
	}
	wallheight[pixx] = CalcHeight();

	if (lastside==1 && lastintercept == xtile && lasttilehit == tilehit)
	{
		// in the same wall type as last time, so check for optimized draw
		if (texture == (unsigned)postsource)
		{
		// wide scale
			postwidth++;
			wallheight[pixx] = wallheight[pixx-1];
			return;
		}
		else
		{
			ScalePost ();
			(unsigned)postsource = texture;
			postwidth = 1;
			postx = pixx;
		}
	}
	else
	{
	// new wall
		if (lastside != -1)				// if not the first scaled post
			ScalePost ();

		lastside = true;
		lastintercept = xtile;

		lasttilehit = tilehit;
		postx = pixx;
		postwidth = 1;

		if (tilehit & 0x40)
		{								// check for adjacent doors
			ytile = yintercept>>TILESHIFT;
			if ( tilemap[xtile-xtilestep][ytile]&0x80 )
				wallpic = DOORWALL+3;
			else
				wallpic = vertwall[tilehit & ~0x40];
		}
		else
			wallpic = vertwall[tilehit];

		*( ((unsigned *)&postsource)+1) = (unsigned)PM_GetPage(wallpic);
		(unsigned)postsource = texture;

	}
}


/*
====================
=
= HitHorizWall
=
= tilehit bit 7 is 0, because it's not a door tile
= if bit 6 is 1 and the adjacent tile is a door tile, use door side pic
=
====================
*/

void HitHorizWall (void)
{
	int			wallpic;
	unsigned	texture;

	texture = (xintercept>>4)&0xfc0;
	if (ytilestep == -1)
		yintercept += TILEGLOBAL;
	else
		texture = 0xfc0-texture;
	wallheight[pixx] = CalcHeight();

	if (lastside==0 && lastintercept == ytile && lasttilehit == tilehit)
	{
		// in the same wall type as last time, so check for optimized draw
		if (texture == (unsigned)postsource)
		{
		// wide scale
			postwidth++;
			wallheight[pixx] = wallheight[pixx-1];
			return;
		}
		else
		{
			ScalePost ();
			(unsigned)postsource = texture;
			postwidth = 1;
			postx = pixx;
		}
	}
	else
	{
	// new wall
		if (lastside != -1)				// if not the first scaled post
			ScalePost ();

		lastside = 0;
		lastintercept = ytile;

		lasttilehit = tilehit;
		postx = pixx;
		postwidth = 1;

		if (tilehit & 0x40)
		{								// check for adjacent doors
			xtile = xintercept>>TILESHIFT;
			if ( tilemap[xtile][ytile-ytilestep]&0x80 )
				wallpic = DOORWALL+2;
			else
				wallpic = horizwall[tilehit & ~0x40];
		}
		else
			wallpic = horizwall[tilehit];

		*( ((unsigned *)&postsource)+1) = (unsigned)PM_GetPage(wallpic);
		(unsigned)postsource = texture;
	}

}

//==========================================================================

/*
====================
=
= HitHorizDoor
=
====================
*/

void HitHorizDoor (void)
{
	unsigned	texture,doorpage,doornum;

	doornum = tilehit&0x7f;
	texture = ( (xintercept-doorposition[doornum]) >> 4) &0xfc0;

	wallheight[pixx] = CalcHeight();

	if (lasttilehit == tilehit)
	{
	// in the same door as last time, so check for optimized draw
		if (texture == (unsigned)postsource)
		{
		// wide scale
			postwidth++;
			wallheight[pixx] = wallheight[pixx-1];
			return;
		}
		else
		{
			ScalePost ();
			(unsigned)postsource = texture;
			postwidth = 1;
			postx = pixx;
		}
	}
	else
	{
		if (lastside != -1)				// if not the first scaled post
			ScalePost ();			// draw last post
	// first pixel in this door
		lastside = 2;
		lasttilehit = tilehit;
		postx = pixx;
		postwidth = 1;

		switch (doorobjlist[doornum].lock)
		{
		case dr_normal:
			doorpage = DOORWALL;
			break;
		case dr_lock1:
		case dr_lock2:
		case dr_lock3:
		case dr_lock4:
			doorpage = DOORWALL+6;
			break;
		case dr_elevator:
			doorpage = DOORWALL+4;
			break;
		}

		*( ((unsigned *)&postsource)+1) = (unsigned)PM_GetPage(doorpage);
		(unsigned)postsource = texture;
	}
}

//==========================================================================

/*
====================
=
= HitVertDoor
=
====================
*/

void HitVertDoor (void)
{
	unsigned	texture,doorpage,doornum;

	doornum = tilehit&0x7f;
	texture = ( (yintercept-doorposition[doornum]) >> 4) &0xfc0;

	wallheight[pixx] = CalcHeight();

	if (lasttilehit == tilehit)
	{
	// in the same door as last time, so check for optimized draw
		if (texture == (unsigned)postsource)
		{
		// wide scale
			postwidth++;
			wallheight[pixx] = wallheight[pixx-1];
			return;
		}
		else
		{
			ScalePost ();
			(unsigned)postsource = texture;
			postwidth = 1;
			postx = pixx;
		}
	}
	else
	{
		if (lastside != -1)				// if not the first scaled post
			ScalePost ();			// draw last post
	// first pixel in this door
		lastside = 2;
		lasttilehit = tilehit;
		postx = pixx;
		postwidth = 1;

		switch (doorobjlist[doornum].lock)
		{
		case dr_normal:
			doorpage = DOORWALL;
			break;
		case dr_lock1:
		case dr_lock2:
		case dr_lock3:
		case dr_lock4:
			doorpage = DOORWALL+6;
			break;
		case dr_elevator:
			doorpage = DOORWALL+4;
			break;
		}

		*( ((unsigned *)&postsource)+1) = (unsigned)PM_GetPage(doorpage+1);
		(unsigned)postsource = texture;
	}
}

//==========================================================================


/*
====================
=
= HitHorizPWall
=
= A pushable wall in action has been hit
=
====================
*/

void HitHorizPWall (void)
{
	int			wallpic;
	unsigned	texture,offset;

	texture = (xintercept>>4)&0xfc0;
	offset = pwallpos<<10;
	if (ytilestep == -1)
		yintercept += TILEGLOBAL-offset;
	else
	{
		texture = 0xfc0-texture;
		yintercept += offset;
	}

	wallheight[pixx] = CalcHeight();

	if (lasttilehit == tilehit)
	{
		// in the same wall type as last time, so check for optimized draw
		if (texture == (unsigned)postsource)
		{
		// wide scale
			postwidth++;
			wallheight[pixx] = wallheight[pixx-1];
			return;
		}
		else
		{
			ScalePost ();
			(unsigned)postsource = texture;
			postwidth = 1;
			postx = pixx;
		}
	}
	else
	{
	// new wall
		if (lastside != -1)				// if not the first scaled post
			ScalePost ();

		lasttilehit = tilehit;
		postx = pixx;
		postwidth = 1;

		wallpic = horizwall[tilehit&63];

		*( ((unsigned *)&postsource)+1) = (unsigned)PM_GetPage(wallpic);
		(unsigned)postsource = texture;
	}

}


/*
====================
=
= HitVertPWall
=
= A pushable wall in action has been hit
=
====================
*/

void HitVertPWall (void)
{
	int			wallpic;
	unsigned	texture,offset;

	texture = (yintercept>>4)&0xfc0;
	offset = pwallpos<<10;
	if (xtilestep == -1)
	{
		xintercept += TILEGLOBAL-offset;
		texture = 0xfc0-texture;
	}
	else
		xintercept += offset;

	wallheight[pixx] = CalcHeight();

	if (lasttilehit == tilehit)
	{
		// in the same wall type as last time, so check for optimized draw
		if (texture == (unsigned)postsource)
		{
		// wide scale
			postwidth++;
			wallheight[pixx] = wallheight[pixx-1];
			return;
		}
		else
		{
			ScalePost ();
			(unsigned)postsource = texture;
			postwidth = 1;
			postx = pixx;
		}
	}
	else
	{
	// new wall
		if (lastside != -1)				// if not the first scaled post
			ScalePost ();

		lasttilehit = tilehit;
		postx = pixx;
		postwidth = 1;

		wallpic = vertwall[tilehit&63];

		*( ((unsigned *)&postsource)+1) = (unsigned)PM_GetPage(wallpic);
		(unsigned)postsource = texture;
	}

}

//==========================================================================

//==========================================================================

#if 0
/*
=====================
=
= ClearScreen
=
=====================
*/

void ClearScreen (void)
{
 unsigned floor=egaFloor[gamestate.episode*10+mapon],
	  ceiling=egaCeiling[gamestate.episode*10+mapon];

  //
  // clear the screen
  //
asm	mov	dx,GC_INDEX
asm	mov	ax,GC_MODE + 256*2		// read mode 0, write mode 2
asm	out	dx,ax
asm	mov	ax,GC_BITMASK + 255*256
asm	out	dx,ax

asm	mov	dx,40
asm	mov	ax,[viewwidth]
asm	shr	ax,1
asm	shr	ax,1
asm	shr	ax,1
asm	sub	dx,ax					// dx = 40-viewwidth/8

asm	mov	bx,[viewwidth]
asm	shr	bx,1					// bl = viewwidth/16
asm	shr	bx,1
asm	shr	bx,1
asm	shr	bx,1
asm	mov	bh,BYTE PTR [viewheight]
asm	shr	bh,1					// half height

asm	mov	ax,[ceiling]
asm	mov	es,[screenseg]
asm	mov	di,[bufferofs]

toploop:
asm	mov	cl,bl
asm	rep	stosw
asm	add	di,dx
asm	dec	bh
asm	jnz	toploop

asm	mov	bh,BYTE PTR [viewheight]
asm	shr	bh,1					// half height
asm	mov	ax,[floor]

bottomloop:
asm	mov	cl,bl
asm	rep	stosw
asm	add	di,dx
asm	dec	bh
asm	jnz	bottomloop


asm	mov	dx,GC_INDEX
asm	mov	ax,GC_MODE + 256*10		// read mode 1, write mode 2
asm	out	dx,ax
asm	mov	al,GC_BITMASK
asm	out	dx,al

}
#endif
//==========================================================================

#define HitHorizBorder HitHorizWall
#define HitVertBorder HitVertWall

//==========================================================================

unsigned vgaCeiling[]=
{
#ifndef SPEAR
 0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0xbfbf,
 0x4e4e,0x4e4e,0x4e4e,0x1d1d,0x8d8d,0x4e4e,0x1d1d,0x2d2d,0x1d1d,0x8d8d,
 0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x2d2d,0xdddd,0x1d1d,0x1d1d,0x9898,

 0x1d1d,0x9d9d,0x2d2d,0xdddd,0xdddd,0x9d9d,0x2d2d,0x4d4d,0x1d1d,0xdddd,
 0x7d7d,0x1d1d,0x2d2d,0x2d2d,0xdddd,0xd7d7,0x1d1d,0x1d1d,0x1d1d,0x2d2d,
 0x1d1d,0x1d1d,0x1d1d,0x1d1d,0xdddd,0xdddd,0x7d7d,0xdddd,0xdddd,0xdddd
#else
 0x6f6f,0x4f4f,0x1d1d,0xdede,0xdfdf,0x2e2e,0x7f7f,0x9e9e,0xaeae,0x7f7f,
 0x1d1d,0xdede,0xdfdf,0xdede,0xdfdf,0xdede,0xe1e1,0xdcdc,0x2e2e,0x1d1d,0xdcdc
#endif
};

/*
=====================
=
= VGAClearScreen
=
=====================
*/

void VGAClearScreen (void)
{
 unsigned ceiling=vgaCeiling[gamestate.episode*10+mapon];

  //
  // clear the screen
  //
	__asm {
		mov	dx,SC_INDEX
		mov	ax,SC_MAPMASK+15*256	// write through all planes
		out	dx,ax

		mov	dx,80
		mov	ax,[viewwidth]
		shr	ax,1
		shr	ax,1
		sub	dx,ax					// dx = 40-viewwidth/2

		mov	bx,[viewwidth]
		shr	bx,1					// bl = viewwidth/8
		shr	bx,1
		shr	bx,1
		mov	bh,BYTE PTR [viewheight]
		shr	bh,1					// half height

		mov	es,[screenseg]
		mov	di,[bufferofs]
		mov	ax,[ceiling]
#ifdef __BORLANDC__
	}
#endif
toploop:
#ifdef __BORLANDC__
	__asm {
#endif
		mov	cl,bl
		rep	stosw
		add	di,dx
		dec	bh
		jnz	toploop

		mov	bh,BYTE PTR [viewheight]
		shr	bh,1					// half height
		mov	ax,0x1919
#ifdef __BORLANDC__
	}
#endif
bottomloop:
#ifdef __BORLANDC__
	__asm {
#endif
		mov	cl,bl
		rep	stosw
		add	di,dx
		dec	bh
		jnz	bottomloop
	}
}

//==========================================================================

/*
=====================
=
= CalcRotate
=
=====================
*/

int	CalcRotate (objtype *ob)
{
	int	angle,viewangle;

	// this isn't exactly correct, as it should vary by a trig value,
	// but it is close enough with only eight rotations

	viewangle = player->angle + (centerx - ob->viewx)/8;

	if (ob->obclass == rocketobj || ob->obclass == hrocketobj)
		angle =  (viewangle-180)- ob->angle;
	else
		angle =  (viewangle-180)- dirangle[ob->dir];

	angle+=ANGLES/16;
	while (angle>=ANGLES)
		angle-=ANGLES;
	while (angle<0)
		angle+=ANGLES;

	if (ob->state->rotate == 2)             // 2 rotation pain frame
		return 4*(angle/(ANGLES/2));        // seperated by 3 (art layout...)

	return angle/(ANGLES/8);
}


/*
=====================
=
= DrawScaleds
=
= Draws all objects that are visable
=
=====================
*/

#define MAXVISABLE	50

typedef struct
{
	int	viewx,
		viewheight,
		shapenum;
} visobj_t;

visobj_t	vislist[MAXVISABLE],*visptr,*visstep,*farthest;

void DrawScaleds (void)
{
	int 		i,j,least,numvisable,height;
	memptr		shape;
	byte		*tilespot,*visspot;
	int			shapenum;
	unsigned	spotloc;

	statobj_t	*statptr;
	objtype		*obj;

	visptr = &vislist[0];

//
// place static objects
//
	for (statptr = &statobjlist[0] ; statptr !=laststatobj ; statptr++)
	{
		if ((visptr->shapenum = statptr->shapenum) == -1)
			continue;						// object has been deleted

		if (!*statptr->visspot)
			continue;						// not visable

		if (TransformTile (statptr->tilex,statptr->tiley
			,&visptr->viewx,&visptr->viewheight) && statptr->flags & FL_BONUS)
		{
			GetBonus (statptr);
			continue;
		}

		if (!visptr->viewheight)
			continue;						// to close to the object

		if (visptr < &vislist[MAXVISABLE-1])	// don't let it overflow
			visptr++;
	}

//
// place active objects
//
	for (obj = player->next;obj;obj=obj->next)
	{
		if (!(visptr->shapenum = obj->state->shapenum))
			continue;						// no shape

		spotloc = (obj->tilex<<6)+obj->tiley;	// optimize: keep in struct?
		visspot = &spotvis[0][0]+spotloc;
		tilespot = &tilemap[0][0]+spotloc;

		//
		// could be in any of the nine surrounding tiles
		//
		if (*visspot
		|| ( *(visspot-1) && !*(tilespot-1) )
		|| ( *(visspot+1) && !*(tilespot+1) )
		|| ( *(visspot-65) && !*(tilespot-65) )
		|| ( *(visspot-64) && !*(tilespot-64) )
		|| ( *(visspot-63) && !*(tilespot-63) )
		|| ( *(visspot+65) && !*(tilespot+65) )
		|| ( *(visspot+64) && !*(tilespot+64) )
		|| ( *(visspot+63) && !*(tilespot+63) ) )
		{
			obj->active = true;
			TransformActor (obj);
			if (!obj->viewheight)
				continue;						// too close or far away

			visptr->viewx = obj->viewx;
			visptr->viewheight = obj->viewheight;
			if (visptr->shapenum == -1)
				visptr->shapenum = obj->temp1;	// special shape

			if (obj->state->rotate)
				visptr->shapenum += CalcRotate (obj);

			if (visptr < &vislist[MAXVISABLE-1])	// don't let it overflow
				visptr++;
			obj->flags |= FL_VISABLE;
		}
		else
			obj->flags &= ~FL_VISABLE;
	}

//
// draw from back to front
//
	numvisable = visptr-&vislist[0];

	if (!numvisable)
		return;									// no visable objects

	for (i = 0; i<numvisable; i++)
	{
		least = 32000;
		for (visstep=&vislist[0] ; visstep<visptr ; visstep++)
		{
			height = visstep->viewheight;
			if (height < least)
			{
				least = height;
				farthest = visstep;
			}
		}
		//
		// draw farthest
		//
		ScaleShape(farthest->viewx,farthest->shapenum,farthest->viewheight);

		farthest->viewheight = 32000;
	}

}

//==========================================================================

/*
==============
=
= DrawPlayerWeapon
=
= Draw the player's hands
=
==============
*/

int	weaponscale[NUMWEAPONS] = {SPR_KNIFEREADY,SPR_PISTOLREADY
	,SPR_MACHINEGUNREADY,SPR_CHAINREADY};

void DrawPlayerWeapon (void)
{
	int	shapenum;

#ifndef SPEAR
	if (gamestate.victoryflag)
	{
		if (player->state == &s_deathcam && (TimeCount&32) )
			SimpleScaleShape(viewwidth/2,SPR_DEATHCAM,viewheight+1);
		return;
	}
#endif

	if (gamestate.weapon != -1)
	{
		shapenum = weaponscale[gamestate.weapon]+gamestate.weaponframe;
		SimpleScaleShape(viewwidth/2,shapenum,viewheight+1);
	}

	if (demorecord || demoplayback)
		SimpleScaleShape(viewwidth/2,SPR_DEMO,viewheight+1);
}


//==========================================================================


/*
=====================
=
= CalcTics
=
=====================
*/

void CalcTics (void)
{
	long	newtime,oldtimecount;

//
// calculate tics since last refresh for adaptive timing
//
	if (lasttimecount > TimeCount)
		TimeCount = lasttimecount;		// if the game was paused a LONG time

	do
	{
		newtime = TimeCount;
		tics = newtime-lasttimecount;
	} while (!tics);			// make sure at least one tic passes

	lasttimecount = newtime;

#ifdef FILEPROFILE
		strcpy (scratch,"\tTics:");
		itoa (tics,str,10);
		strcat (scratch,str);
		strcat (scratch,"\n");
		write (profilehandle,scratch,strlen(scratch));
#endif

	if (tics>MAXTICS)
	{
		TimeCount -= (tics-MAXTICS);
		tics = MAXTICS;
	}
}


//==========================================================================


/*
========================
=
= FixOfs
=
========================
*/

void	FixOfs (void)
{
	VW_ScreenToScreen (displayofs,bufferofs,viewwidth/8,viewheight);
}


//==========================================================================
#ifdef __WATCOMC__
word xspot,yspot;
int texdelta, pwalltile;
extern int pwalldir;
//#define DEBUGRAYTRACER

#ifdef DEBUGRAYTRACER
#define MARKPIX(y,col) VGAMAPMASK(1<<(pixx&3)); \
	vbuf[(pixx>>2)+(y)*80]=(col);
#else
#define MARKPIX(y,col)
#endif

void AsmRefresh()
{
    long xstep,ystep;
    dword xpartial,ypartial;

#ifdef DEBUGRAYTRACER
	 static int logpressed=0;
	 int dolog=0;
    FILE *log=NULL;
#endif

    for(pixx=0;pixx<viewwidth;pixx++)
    {
        short angl=midangle+pixelangle[pixx];
        if(angl<0) angl+=FINEANGLES;
        if(angl>=3600) angl-=FINEANGLES;
        if(angl<900)
        {
            xtilestep=1;
            ytilestep=-1;
            xstep=finetangent[900-1-angl];
            ystep=-finetangent[angl];
            xpartial=xpartialup;
            ypartial=ypartialdown;
        }
        else if(angl<1800)
        {
            xtilestep=-1;
            ytilestep=-1;
            xstep=-finetangent[angl-900];
            ystep=-finetangent[1800-1-angl];
            xpartial=xpartialdown;
            ypartial=ypartialdown;
        }
        else if(angl<2700)
        {
            xtilestep=-1;
            ytilestep=1;
            xstep=-finetangent[2700-1-angl];
            ystep=finetangent[angl-1800];
            xpartial=xpartialdown;
            ypartial=ypartialup;
        }
        else if(angl<3600)
        {
            xtilestep=1;
            ytilestep=1;
            xstep=finetangent[angl-2700];
            ystep=finetangent[3600-1-angl];
            xpartial=xpartialup;
            ypartial=ypartialup;
        }
        yintercept=FixedByFrac(ystep,xpartial)+viewy;
        xtile=focaltx+xtilestep;
        xspot=(xtile<<mapshift)+*((word *)&yintercept+1);
        xintercept=FixedByFrac(xstep,ypartial)+viewx;
        ytile=focalty+ytilestep;
        yspot=(*((word *)&xintercept+1)<<mapshift)+ytile;
		  texdelta=0;

        if(xintercept<0) xintercept=0;
        if(xintercept>mapwidth*65536-1) xintercept=mapwidth*65536-1;
        if(yintercept<0) yintercept=0;
        if(yintercept>mapheight*65536-1) yintercept=mapheight*65536-1;

#ifdef DEBUGRAYTRACER
		  if(pixx==93)
		  {
			  VGAMAPMASK(1<<(pixx&3));
			  vbuf[(pixx>>2)+80]=14;
			  if(logpressed)
			  {
				  if(!Keyboard[sc_L]) logpressed=0;
			  }
			  else
			  {
				  if(Keyboard[sc_L])
				  {
					  logpressed=1;
					  dolog=1;
					  log=fopen("draw93.txt","wt");
					  if(!log) return;
					  fprintf(log,"player->x=%.8X  player->y=%.8X  player->angle=%i  pixx=%i\nxintercept=%.8X  xtile=%.4X  xtilestep=%i  xstep=%.8X\nyintercept=%.8X  ytile=%.4X  ytilestep=%i  ystep=%.8X\n",player->x,player->y,player->angle,pixx,xintercept,xtile,xtilestep,xstep,yintercept,ytile,ytilestep,ystep);
				  }
			  }
		  }
#endif

        do
        {
            if(ytilestep==-1 && *((short *)&yintercept+1)<=ytile) goto horizentry;
            if(ytilestep==1 && *((short *)&yintercept+1)>=ytile) goto horizentry;
vertentry:
            if((dword)yintercept>mapheight*65536-1 || (word)xtile>=mapwidth)
            {
                if(xtile<0) xintercept=0;
                if(xtile>=mapwidth) xintercept=mapwidth<<TILESHIFT;
                if(yintercept<0) yintercept=0;
                if(yintercept>=(mapheight<<TILESHIFT)) yintercept=mapheight<<TILESHIFT;
                yspot=0xffff;
                HitHorizBorder();
                break;
            }
            if(xspot>mapspotend) break;
            tilehit=*((byte *)tilemap+xspot);
            if(tilehit)
            {
                if(tilehit&0x80)
                {
                    long yintbuf=yintercept+(ystep>>1);
                    if(*((word *)&yintbuf+1)!=*((word *)&yintercept+1))
                        goto passvert;
                    if((word)yintbuf<doorposition[tilehit&0x7f])
                        goto passvert;
                    yintercept=yintbuf;
                    xintercept=(xtile<<TILESHIFT)|0x8000;
                    HitVertDoor();
                }
                else
                {
                    if(tilehit==64)
                    {
							  	if(pwalldir==di_west || pwalldir==di_east)
								{
	                        long yintbuf;
									int pwallposnorm;
									int pwallposinv;
									if(pwalldir==di_west)
									{
										pwallposnorm = 63-pwallpos;
										pwallposinv = pwallpos;
									}
									else
									{
										pwallposnorm = pwallpos;
										pwallposinv = 63-pwallpos;
									}
									if(pwalldir == di_east && xtile==pwallx && *((word *)&yintercept+1)==pwally
										|| pwalldir == di_west && !(xtile==pwallx && *((word *)&yintercept+1)==pwally))
									{
										yintbuf=yintercept+((ystep*pwallposnorm)>>6);
	   	                     if(*((word *)&yintbuf+1)!=*((word *)&yintercept+1))
   	   	                     goto passvert;

									   MARKPIX(4,2);

         		               xintercept=(xtile<<TILESHIFT)+TILEGLOBAL-(pwallposinv<<10);
      	      	            yintercept=yintbuf;
										tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
									   if(dolog) fprintf(log,"Pushwall hit 1: HitVertWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%i\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	         HitVertWall();
									}
									else
									{
										yintbuf=yintercept+((ystep*pwallposinv)>>6);
	   	                     if(*((word *)&yintbuf+1)!=*((word *)&yintercept+1))
   	   	                     goto passvert;

									   MARKPIX(4,1);

         		               xintercept=(xtile<<TILESHIFT)-(pwallposinv<<10);
      	      	            yintercept=yintbuf;
										tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
									   if(dolog) fprintf(log,"Pushwall hit 2: HitVertWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%i\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	         HitVertWall();
									}
								}
								else
								{
									int pwallposi = pwallpos;
									if(pwalldir==di_north) pwallposi = 63-pwallpos;
									if(pwalldir==di_south && (word)yintercept<(pwallposi<<10)
											|| pwalldir==di_north && (word)yintercept>(pwallposi<<10))
									{
										if(*((word *)&yintercept+1)==pwally && xtile==pwallx)
										{
										   if(pwalldir==di_south && (long)((word)yintercept)+ystep<(pwallposi<<10)
													|| pwalldir==di_north && (long)((word)yintercept)+ystep>(pwallposi<<10))
											   goto passvert;

										   MARKPIX(5,15);

#ifdef DEBUGRAYTRACER
									      if(dolog) fprintf(log,"Pushwall hit 3: HitHorizWall old values:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
											if(pwalldir==di_south)
											   yintercept=(yintercept&0xffff0000)+(pwallposi<<10);
											else
											   yintercept=(yintercept&0xffff0000)-TILEGLOBAL+(pwallposi<<10);
     		      	               xintercept=xintercept-((xstep*(63-pwallpos))>>6);
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
									      if(dolog) fprintf(log,"Pushwall hit 3: HitHorizWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	            HitHorizWall();
										}
										else
										{
										   MARKPIX(3,11);

									      texdelta = -(pwallposi<<10);
											xintercept=xtile<<TILESHIFT;
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
									      if(dolog) fprintf(log,"Pushwall hit 4: HitVertWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
											HitVertWall();
										}
									}
									else
									{
										if(*((word *)&yintercept+1)==pwally && xtile==pwallx)
										{
										   MARKPIX(3,12);

									      texdelta = -(pwallposi<<10);
											xintercept=xtile<<TILESHIFT;
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
										   if(dolog) fprintf(log,"Pushwall hit 5: HitVertWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
											HitVertWall();
										}
										else
										{
										   if(pwalldir==di_south && (long)((word)yintercept)+ystep>(pwallposi<<10)
													|| pwalldir==di_north && (long)((word)yintercept)+ystep<(pwallposi<<10))
											   goto passvert;

										   MARKPIX(3,3);

											if(pwalldir==di_south)
											   yintercept=(yintercept&0xffff0000)-((63-pwallpos)<<10);
											else
											   yintercept=(yintercept&0xffff0000)+((63-pwallpos)<<10);
     		      	               xintercept=xintercept-((xstep*pwallpos)>>6);
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
										   if(dolog) fprintf(log,"Pushwall hit 6: HitHorizWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	            HitHorizWall();
										}
									}
								}
                    }
                    else
                    {
                        xintercept=xtile<<TILESHIFT;
#ifdef DEBUGRAYTRACER
							   if(dolog) fprintf(log,"HitVertWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n",xintercept,xtile,yintercept,ytile);
#endif
                        HitVertWall();
                    }
                }
                break;
            }
passvert:
            *((byte *)spotvis+xspot)=1;
            xtile+=xtilestep;
            yintercept+=ystep;
            xspot=(xtile<<mapshift)+*((word *)&yintercept+1);
#ifdef DEBUGRAYTRACER
			   if(dolog) fprintf(log,"passvert:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n",xintercept,xtile,yintercept,ytile);
#endif
        }
        while(1);
#ifdef DEBUGRAYTRACER
		  if(dolog)
		  {
			  fclose(log);
			  dolog=0;
			  log=NULL;
		  }
#endif
        continue;
        do
        {
            if(xtilestep==-1 && *((short *)&xintercept+1)<=xtile) goto vertentry;
            if(xtilestep==1 && *((short *)&xintercept+1)>=xtile) goto vertentry;
horizentry:
            if((dword)xintercept>mapwidth*65536-1 || (word)ytile>=mapheight)
            {
                if(ytile<0) yintercept=0;
                if(ytile>=mapheight) yintercept=mapheight<<TILESHIFT;
                if(xintercept<0) xintercept=0;
                if(xintercept>=(mapwidth<<TILESHIFT)) xintercept=mapwidth<<TILESHIFT;
                xspot=0xffff;
                HitVertBorder();
                break;
            }
            if(yspot>mapspotend) break;
            tilehit=*((byte *)tilemap+yspot);
            if(tilehit)
            {
                if(tilehit&0x80)
                {
                    long xintbuf=xintercept+(xstep>>1);
                    if(*((word *)&xintbuf+1)!=*((word *)&xintercept+1))
                        goto passhoriz;
                    if((word)xintbuf<doorposition[tilehit&0x7f])
                        goto passhoriz;
                    xintercept=xintbuf;
                    yintercept=(ytile<<TILESHIFT)+0x8000;
                    HitHorizDoor();
                }
                else
                {
                    if(tilehit==64)
                    {
							   if(pwalldir==di_north || pwalldir==di_south)
								{
	                        long xintbuf;
									int pwallposnorm;
									int pwallposinv;
									if(pwalldir==di_north)
									{
										pwallposnorm = 63-pwallpos;
										pwallposinv = pwallpos;
									}
									else
									{
										pwallposnorm = pwallpos;
										pwallposinv = 63-pwallpos;
									}
									if(pwalldir == di_south && ytile==pwally && *((word *)&xintercept+1)==pwallx
										|| pwalldir == di_north && !(ytile==pwally && *((word *)&xintercept+1)==pwallx))
									{
										xintbuf=xintercept+((xstep*pwallposnorm)>>6);
	   	                     if(*((word *)&xintbuf+1)!=*((word *)&xintercept+1))
   	   	                     goto passhoriz;

									   MARKPIX(4,2);

         		               yintercept=(ytile<<TILESHIFT)+TILEGLOBAL-(pwallposinv<<10);
      	      	            xintercept=xintbuf;
										tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
									   if(dolog) fprintf(log,"Pushwall hit 7: HitHorizWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%i\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	         HitHorizWall();
									}
									else
									{
										xintbuf=xintercept+((xstep*pwallposinv)>>6);
	   	                     if(*((word *)&xintbuf+1)!=*((word *)&xintercept+1))
   	   	                     goto passhoriz;

									   MARKPIX(4,1);

         		               yintercept=(ytile<<TILESHIFT)-(pwallposinv<<10);
      	      	            xintercept=xintbuf;
										tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
									   if(dolog) fprintf(log,"Pushwall hit 8: HitHorizWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%i\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	         HitHorizWall();
									}
								}
								else
								{
									int pwallposi = pwallpos;
									if(pwalldir==di_west) pwallposi = 63-pwallpos;
									if(pwalldir==di_east && (word)xintercept<(pwallposi<<10)
											|| pwalldir==di_west && (word)xintercept>(pwallposi<<10))
									{
										if(*((word *)&xintercept+1)==pwallx && ytile==pwally)
										{
										   if(pwalldir==di_east && (long)((word)xintercept)+xstep<(pwallposi<<10)
													|| pwalldir==di_west && (long)((word)xintercept)+xstep>(pwallposi<<10))
											   goto passhoriz;

										   MARKPIX(3,15);

#ifdef DEBUGRAYTRACER
									      if(dolog) fprintf(log,"Pushwall hit 9: HitVertWall old values:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X",xintercept,xtile,yintercept,ytile,pwallpos);
#endif

											if(pwalldir==di_east)
											   xintercept=(xintercept&0xffff0000)+(pwallposi<<10);
											else
											   xintercept=(xintercept&0xffff0000)-TILEGLOBAL+(pwallposi<<10);
     		      	               yintercept=yintercept-((ystep*(63-pwallpos))>>6);
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
									      if(dolog) fprintf(log,"Pushwall hit 9: HitVertWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	            HitVertWall();
										}
										else
										{
										   MARKPIX(3,11);

									      texdelta = -(pwallposi<<10);
											yintercept=ytile<<TILESHIFT;
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
										   if(dolog) fprintf(log,"Pushwall hit 10: HitHorizWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
											HitHorizWall();
										}
									}
									else
									{
										if(*((word *)&xintercept+1)==pwallx && ytile==pwally)
										{
										   MARKPIX(3,12);

									      texdelta = -(pwallposi<<10);
											yintercept=ytile<<TILESHIFT;
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
										   if(dolog) fprintf(log,"Pushwall hit 11: HitHorizWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
											HitHorizWall();
										}
										else
										{
										   if(pwalldir==di_east && (long)((word)xintercept)+xstep>(pwallposi<<10)
													|| pwalldir==di_west && (long)((word)xintercept)+xstep<(pwallposi<<10))
											   goto passhoriz;

										   MARKPIX(3,3);

											if(pwalldir==di_east)
											   xintercept=(xintercept&0xffff0000)-((63-pwallpos)<<10);
											else
											   xintercept=(xintercept&0xffff0000)+((63-pwallpos)<<10);
     		      	               yintercept=yintercept-((ystep*pwallpos)>>6);
											tilehit=pwalltile;
#ifdef DEBUGRAYTRACER
										   if(dolog) fprintf(log,"Pushwall hit 12: HitVertWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n  pwallpos=%.4X\n",xintercept,xtile,yintercept,ytile,pwallpos);
#endif
	               	            HitVertWall();
										}
									}
								}
                    }
                    else
                    {
                        yintercept=ytile<<TILESHIFT;
#ifdef DEBUGRAYTRACER
							   if(dolog) fprintf(log,"HitHorizWall:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n",xintercept,xtile,yintercept,ytile);
#endif
                        HitHorizWall();
                    }
                }
                break;
            }
passhoriz:
            *((byte *)spotvis+yspot)=1;
            ytile+=ytilestep;
            xintercept+=xstep;
            yspot=(*((word *)&xintercept+1)<<mapshift)+ytile;
#ifdef DEBUGRAYTRACER
			   if(dolog) fprintf(log,"passhoriz:\n  xintercept=%.8X  xtile=%.4X\n  yintercept=%.8X  ytile=%.4X\n",xintercept,xtile,yintercept,ytile);
#endif
        }
        while(1);
#ifdef DEBUGRAYTRACER
		  if(dolog)
		  {
			  fclose(log);
			  dolog=0;
			  log=NULL;
		  }
#endif
    }
}
#endif
/*
====================
=
= WallRefresh
=
====================
*/

void WallRefresh (void)
{
//
// set up variables for this view
//
	viewangle = player->angle;
	midangle = viewangle*(FINEANGLES/ANGLES);
	viewsin = sintable[viewangle];
	viewcos = costable[viewangle];
	viewx = player->x - FixedByFrac(focallength,viewcos);
	viewy = player->y + FixedByFrac(focallength,viewsin);

	focaltx = viewx>>TILESHIFT;
	focalty = viewy>>TILESHIFT;

	viewtx = player->x >> TILESHIFT;
	viewty = player->y >> TILESHIFT;

	xpartialdown = viewx&(TILEGLOBAL-1);
	xpartialup = TILEGLOBAL-xpartialdown;
	ypartialdown = viewy&(TILEGLOBAL-1);
	ypartialup = TILEGLOBAL-ypartialdown;

	lastside = -1;			// the first pixel is on a new wall
	AsmRefresh ();
	ScalePost ();			// no more optimization on last post
}

//==========================================================================

/*
========================
=
= ThreeDRefresh
=
========================
*/

void	ThreeDRefresh (void)
{
	int tracedir;

// this wouldn't need to be done except for my debugger/video wierdness
	outportb (SC_INDEX,SC_MAPMASK);

//
// clear out the traced array
//
asm	mov	ax,ds
asm	mov	es,ax
asm	mov	di,OFFSET spotvis
asm	xor	ax,ax
asm	mov	cx,2048							// 64*64 / 2
asm	rep stosw

	bufferofs += screenofs;

//
// follow the walls from there to the right, drawwing as we go
//
	VGAClearScreen ();

	WallRefresh ();

//
// draw all the scaled images
//
	DrawScaleds();			// draw scaled stuff
	DrawPlayerWeapon ();	// draw player's hands

//
// show screen and time last cycle
//
	if (fizzlein)
	{
		FizzleFade(bufferofs,displayofs+screenofs,viewwidth,viewheight,20,false);
		fizzlein = false;

		lasttimecount = TimeCount = 0;		// don't make a big tic count

	}

	bufferofs -= screenofs;
	displayofs = bufferofs;

	asm	cli
	asm	mov	cx,[displayofs]
	asm	mov	dx,3d4h		// CRTC address register
	asm	mov	al,0ch		// start address high register
	asm	out	dx,al
	asm	inc	dx
	asm	mov	al,ch
	asm	out	dx,al   	// set the high byte
	asm	sti

	bufferofs += SCREENSIZE;
	if (bufferofs > PAGE3START)
		bufferofs = PAGE1START;

	frameon++;
	PM_NextFrame();
}


//===========================================================================

