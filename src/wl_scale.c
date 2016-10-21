// WL_SCALE.C

#include "src/wl_def.h"
#pragma hdrstop

#define OP_RETF	0xcb

/*
=============================================================================

						  GLOBALS

=============================================================================
*/

t_compscale /*_seg*/ *scaledirectory[MAXSCALEHEIGHT+1];
long			fullscalefarcall[MAXSCALEHEIGHT+1];

int			maxscale,maxscaleshl2;

boolean	insetupscaling;

/*
=============================================================================

						  LOCALS

=============================================================================
*/

t_compscale 	/*_seg*/ *work;
unsigned BuildCompScale (int height, memptr *finalspot);

int			stepbytwo;

//===========================================================================

/*
==============
=
= BadScale
=
==============
*/

void far BadScale (void)
{
	Quit ("BadScale called!");
}


/*
==========================
=
= SetupScaling
=
==========================
*/

void SetupScaling (int maxscaleheight)
{
	int		i,x,y;
	byte	far *dest;

	insetupscaling = true;

	maxscaleheight/=2;			// one scaler every two pixels

	maxscale = maxscaleheight-1;
	maxscaleshl2 = maxscale<<2;

//
// free up old scalers
//
	for (i=1;i<MAXSCALEHEIGHT;i++)
	{
		if (scaledirectory[i])
			MM_FreePtr ((memptr)scaledirectory[i]);
		if (i>=stepbytwo)
			i += 2;
	}
	memset (scaledirectory,0,sizeof(scaledirectory));

	MM_SortMem ();

//
// build the compiled scalers
//
	stepbytwo = viewheight/2;	// save space by double stepping
	MM_GetPtr ((memptr)work,20000);

	for (i=1;i<=maxscaleheight;i++)
	{
		BuildCompScale (i*2,(memptr)scaledirectory[i]);
		if (i>=stepbytwo)
			i+= 2;
	}
	MM_FreePtr ((memptr)work);

//
// compact memory and lock down scalers
//
	MM_SortMem ();
	for (i=1;i<=maxscaleheight;i++)
	{
		MM_SetLock ((memptr)scaledirectory[i],true);
		fullscalefarcall[i] = (unsigned)scaledirectory[i];
		fullscalefarcall[i] <<=16;
		fullscalefarcall[i] += scaledirectory[i]->codeofs[0];
		if (i>=stepbytwo)
		{
			scaledirectory[i+1] = scaledirectory[i];
			fullscalefarcall[i+1] = fullscalefarcall[i];
			scaledirectory[i+2] = scaledirectory[i];
			fullscalefarcall[i+2] = fullscalefarcall[i];
			i+=2;
		}
	}
	scaledirectory[0] = scaledirectory[1];
	fullscalefarcall[0] = fullscalefarcall[1];

//
// check for oversize wall drawing
//
	for (i=maxscaleheight;i<MAXSCALEHEIGHT;i++)
		fullscalefarcall[i] = (long)BadScale;

	insetupscaling = false;
}

//===========================================================================

/*
========================
=
= BuildCompScale
=
= Builds a compiled scaler object that will scale a 64 tall object to
= the given height (centered vertically on the screen)
=
= height should be even
=
= Call with
= ---------
= DS:SI		Source for scale
= ES:DI		Dest for scale
=
= Calling the compiled scaler only destroys AL
=
========================
*/

unsigned BuildCompScale (int height, memptr *finalspot)
{
	byte		far *code;

	int			i;
	long		fix,step;
	unsigned	src,totalscaled,totalsize;
	int			startpix,endpix,toppix;


	step = ((long)height<<16) / 64;
	code = &work->code[0];
	toppix = (viewheight-height)/2;
	fix = 0;

	for (src=0;src<=64;src++)
	{
		startpix = fix>>16;
		fix += step;
		endpix = fix>>16;

		if (endpix>startpix)
			work->width[src] = endpix-startpix;
		else
			work->width[src] = 0;

//
// mark the start of the code
//
		work->codeofs[src] = FP_OFF(code);

//
// compile some code if the source pixel generates any screen pixels
//
		startpix+=toppix;
		endpix+=toppix;

		if (startpix == endpix || endpix < 0 || startpix >= viewheight || src == 64)
			continue;

	//
	// mov al,[si+src]
	//
		*code++ = 0x8a;
		*code++ = 0x44;
		*code++ = src;

		for (;startpix<endpix;startpix++)
		{
			if (startpix >= viewheight)
				break;						// off the bottom of the view area
			if (startpix < 0)
				continue;					// not into the view area

		//
		// mov [es:di+heightofs],al
		//
			*code++ = 0x26;
			*code++ = 0x88;
			*code++ = 0x85;
			//*((unsigned far *)
			*(code)++ = startpix*SCREENBWIDE;
		}

	}

//
// retf
//
	*code++ = 0xcb;

	totalsize = FP_OFF(code);
	MM_GetPtr (finalspot,totalsize);
	_fmemcpy ((byte /*_seg*/ *)(*finalspot),(byte /*_seg*/ *)work,totalsize);

	return totalsize;
}


/*
=======================
=
= ScaleLine
=
= linescale should have the high word set to the segment of the scaler
=
=======================
*/

extern	int			slinex,slinewidth;
extern	unsigned	far *linecmds;
extern	long		linescale;
extern	unsigned	maskword;

byte	mask1,mask2,mask3;


void near ScaleLine (void)
{
	__asm {
		mov	cx,WORD PTR [linescale+2]
		mov	es,cx						// segment of scaler

		mov bp,WORD PTR [linecmds]
		mov	dx,SC_INDEX+1				// to set SC_MAPMASK

		mov	bx,[slinex]
		mov	di,bx
		shr	di,1						// X in bytes
		shr	di,1						// X in bytes
		add	di,[bufferofs]
		and	bx,3
		shl	bx,1
		shl	bx,1
		shl	bx,1
		add	bx,[slinewidth]				// bx = (pixel*8+pixwidth)
		mov	al,BYTE ptr [mapmasks3-1+bx]	// -1 because pixwidth of 1 is first
		mov	ds,WORD PTR [linecmds+2]
		or	al,al
		jz	notthreebyte				// scale across three bytes
		jmp	threebyte
notthreebyte:
		mov	al,BYTE PTR ss:[mapmasks2-1+bx]	// -1 because pixwidth of 1 is first
		or	al,al
		jnz	twobyte						// scale across two bytes

//
// one byte scaling
//
		mov	al,BYTE PTR ss:[mapmasks1-1+bx]	// -1 because pixwidth of 1 is first
		out	dx,al						// set map mask register

scalesingle:

		mov	bx,[ds:bp]					// table location of rtl to patch
		or	bx,bx
		jz	linedone					// 0 signals end of segment list
		mov	bx,[es:bx]
		mov	dl,[es:bx]					// save old value
		mov	BYTE PTR es:[bx],OP_RETF	// patch a RETF in
		mov	si,[ds:bp+4]				// table location of entry spot
		mov	ax,[es:si]
		mov	WORD PTR ss:[linescale],ax	// call here to start scaling
		mov	si,[ds:bp+2]				// corrected top of shape for this segment
		add	bp,6						// next segment list

		mov	ax,SCREENSEG
		mov	es,ax
		call ss:[linescale]				// scale the segment of pixels

		mov	es,cx						// segment of scaler
		mov	BYTE PTR es:[bx],dl			// unpatch the RETF
		jmp	scalesingle					// do the next segment


//
// done
//
linedone:
		mov	ax,ss
		mov	ds,ax
		jmp end

//
// two byte scaling
//
twobyte:
		mov	ss:[mask2],al
		mov	al,BYTE PTR ss:[mapmasks1-1+bx]	// -1 because pixwidth of 1 is first
		mov	ss:[mask1],al

scaledouble:

		mov	bx,[ds:bp]					// table location of rtl to patch
		or	bx,bx
		jz	linedone					// 0 signals end of segment list
		mov	bx,[es:bx]
		mov	cl,[es:bx]					// save old value
		mov	BYTE PTR es:[bx],OP_RETF	// patch a RETF in
		mov	si,[ds:bp+4]				// table location of entry spot
		mov	ax,[es:si]
		mov	WORD PTR ss:[linescale],ax	// call here to start scaling
		mov	si,[ds:bp+2]				// corrected top of shape for this segment
		add	bp,6						// next segment list

		mov	ax,SCREENSEG
		mov	es,ax
		mov	al,ss:[mask1]
		out	dx,al						// set map mask register
		call ss:[linescale]				// scale the segment of pixels
		inc	di
		mov	al,ss:[mask2]
		out	dx,al						// set map mask register
		call ss:[linescale]				// scale the segment of pixels
		dec	di

		mov	es,WORD PTR ss:[linescale+2] // segment of scaler
		mov	BYTE PTR es:[bx],cl			// unpatch the RETF
		jmp	scaledouble					// do the next segment


//
// three byte scaling
//
threebyte:
		mov	ss:[mask3],al
		mov	al,BYTE PTR ss:[mapmasks2-1+bx]	// -1 because pixwidth of 1 is first
		mov	ss:[mask2],al
		mov	al,BYTE PTR ss:[mapmasks1-1+bx]	// -1 because pixwidth of 1 is first
		mov	ss:[mask1],al

scaletriple:

		mov	bx,[ds:bp]					// table location of rtl to patch
		or	bx,bx
		jz	linedone					// 0 signals end of segment list
		mov	bx,[es:bx]
		mov	cl,[es:bx]					// save old value
		mov	BYTE PTR es:[bx],OP_RETF	// patch a RETF in
		mov	si,[ds:bp+4]				// table location of entry spot
		mov	ax,[es:si]
		mov	WORD PTR ss:[linescale],ax	// call here to start scaling
		mov	si,[ds:bp+2]				// corrected top of shape for this segment
		add	bp,6						// next segment list

		mov	ax,SCREENSEG
		mov	es,ax
		mov	al,ss:[mask1]
		out	dx,al						// set map mask register
		call ss:[linescale]				// scale the segment of pixels
		inc	di
		mov	al,ss:[mask2]
		out	dx,al						// set map mask register
		call ss:[linescale]				// scale the segment of pixels
		inc	di
		mov	al,ss:[mask3]
		out	dx,al						// set map mask register
		call ss:[linescale]				// scale the segment of pixels
		dec	di
		dec	di

		mov	es,WORD PTR ss:[linescale+2] // segment of scaler
		mov	BYTE PTR es:[bx],cl			// unpatch the RETF
		jmp	scaletriple					// do the next segment
		end:
	}

}


/*
=======================
=
= ScaleShape
=
= Draws a compiled shape at [scale] pixels high
=
= each vertical line of the shape has a pointer to segment data:
= 	end of segment pixel*2 (0 terminates line) used to patch rtl in scaler
= 	top of virtual line with segment in proper place
=	start of segment pixel*2, used to jsl into compiled scaler
=	<repeat>
=
= Setup for call
= --------------
= GC_MODE			read mode 1, write mode 2
= GC_COLORDONTCARE  set to 0, so all reads from video memory return 0xff
= GC_INDEX			pointing at GC_BITMASK
=
=======================
*/

static	long		longtemp;

void ScaleShape (int xcenter, int shapenum, unsigned height)
{
	t_compshape	/*_seg*/ *shape;
	t_compscale /*_seg*/ *comptable;
	unsigned	scale,srcx,stopx,tempx;
	int			t;
	unsigned	far *cmdptr;
	boolean		leftvis,rightvis;


	shape = PM_GetSpritePage (shapenum);

	scale = height>>3;						// low three bits are fractional
	if (!scale || scale>maxscale)
		return;								// too close or far away
	comptable = scaledirectory[scale];

	*(((unsigned *)&linescale)+1)=(unsigned)comptable;	// seg of far call
	*(((unsigned *)&linecmds)+1)=(unsigned)shape;		// seg of shape

//
// scale to the left (from pixel 31 to shape->leftpix)
//
	srcx = 32;
	slinex = xcenter;
	stopx = shape->leftpix;
	cmdptr = &shape->dataofs[31-stopx];

	while ( --srcx >=stopx && slinex>0)
	{
		linecmds = (unsigned *)(*cmdptr--);
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		if (slinewidth == 1)
		{
			slinex--;
			if (slinex<viewwidth)
			{
				if (wallheight[slinex] >= height)
					continue;		// obscured by closer wall
				ScaleLine ();
			}
			continue;
		}

		//
		// handle multi pixel lines
		//
		if (slinex>viewwidth)
		{
			slinex -= slinewidth;
			slinewidth = viewwidth-slinex;
			if (slinewidth<1)
				continue;		// still off the right side
		}
		else
		{
			if (slinewidth>slinex)
				slinewidth = slinex;
			slinex -= slinewidth;
		}


		leftvis = (wallheight[slinex] < height);
		rightvis = (wallheight[slinex+slinewidth-1] < height);

		if (leftvis)
		{
			if (rightvis)
				ScaleLine ();
			else
			{
				while (wallheight[slinex+slinewidth-1] >= height)
					slinewidth--;
				ScaleLine ();
			}
		}
		else
		{
			if (!rightvis)
				continue;		// totally obscured

			while (wallheight[slinex] >= height)
			{
				slinex++;
				slinewidth--;
			}
			ScaleLine ();
			break;			// the rest of the shape is gone
		}
	}


//
// scale to the right
//
	slinex = xcenter;
	stopx = shape->rightpix;
	if (shape->leftpix<31)
	{
		srcx = 31;
		cmdptr = &shape->dataofs[32-shape->leftpix];
	}
	else
	{
		srcx = shape->leftpix-1;
		cmdptr = &shape->dataofs[0];
	}
	slinewidth = 0;

	while ( ++srcx <= stopx && (slinex+=slinewidth)<viewwidth)
	{
		linecmds = (unsigned *)(*cmdptr++);
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		if (slinewidth == 1)
		{
			if (slinex>=0 && wallheight[slinex] < height)
			{
				ScaleLine ();
			}
			continue;
		}

		//
		// handle multi pixel lines
		//
		if (slinex<0)
		{
			if (slinewidth <= -slinex)
				continue;		// still off the left edge

			slinewidth += slinex;
			slinex = 0;
		}
		else
		{
			if (slinex + slinewidth > viewwidth)
				slinewidth = viewwidth-slinex;
		}


		leftvis = (wallheight[slinex] < height);
		rightvis = (wallheight[slinex+slinewidth-1] < height);

		if (leftvis)
		{
			if (rightvis)
			{
				ScaleLine ();
			}
			else
			{
				while (wallheight[slinex+slinewidth-1] >= height)
					slinewidth--;
				ScaleLine ();
				break;			// the rest of the shape is gone
			}
		}
		else
		{
			if (rightvis)
			{
				while (wallheight[slinex] >= height)
				{
					slinex++;
					slinewidth--;
				}
				ScaleLine ();
			}
			else
				continue;		// totally obscured
		}
	}
}



/*
=======================
=
= SimpleScaleShape
=
= NO CLIPPING, height in pixels
=
= Draws a compiled shape at [scale] pixels high
=
= each vertical line of the shape has a pointer to segment data:
= 	end of segment pixel*2 (0 terminates line) used to patch rtl in scaler
= 	top of virtual line with segment in proper place
=	start of segment pixel*2, used to jsl into compiled scaler
=	<repeat>
=
= Setup for call
= --------------
= GC_MODE			read mode 1, write mode 2
= GC_COLORDONTCARE  set to 0, so all reads from video memory return 0xff
= GC_INDEX			pointing at GC_BITMASK
=
=======================
*/

void SimpleScaleShape (int xcenter, int shapenum, unsigned height)
{
	t_compshape	/*_seg*/ *shape;
	t_compscale /*_seg*/ *comptable;
	unsigned	scale,srcx,stopx,tempx;
	int			t;
	unsigned	far *cmdptr;
	boolean		leftvis,rightvis;


	shape = PM_GetSpritePage (shapenum);

	scale = height>>1;
	comptable = scaledirectory[scale];

	*(((unsigned *)&linescale)+1)=(unsigned)comptable;	// seg of far call
	*(((unsigned *)&linecmds)+1)=(unsigned)shape;		// seg of shape

//
// scale to the left (from pixel 31 to shape->leftpix)
//
	srcx = 32;
	slinex = xcenter;
	stopx = shape->leftpix;
	cmdptr = &shape->dataofs[31-stopx];

	while ( --srcx >=stopx )
	{
		linecmds = (unsigned *)(*cmdptr--);
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		slinex -= slinewidth;
		ScaleLine ();
	}


//
// scale to the right
//
	slinex = xcenter;
	stopx = shape->rightpix;
	if (shape->leftpix<31)
	{
		srcx = 31;
		cmdptr = &shape->dataofs[32-shape->leftpix];
	}
	else
	{
		srcx = shape->leftpix-1;
		cmdptr = &shape->dataofs[0];
	}
	slinewidth = 0;

	while ( ++srcx <= stopx )
	{
		linecmds = (unsigned *)(*cmdptr++);
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		ScaleLine ();
		slinex+=slinewidth;
	}
}




//
// bit mask tables for drawing scaled strips up to eight pixels wide
//
// down here so the STUPID inline assembler doesn't get confused!
//


byte	mapmasks1[4][8] = {
{1 ,3 ,7 ,15,15,15,15,15},
{2 ,6 ,14,14,14,14,14,14},
{4 ,12,12,12,12,12,12,12},
{8 ,8 ,8 ,8 ,8 ,8 ,8 ,8} };

byte	mapmasks2[4][8] = {
{0 ,0 ,0 ,0 ,1 ,3 ,7 ,15},
{0 ,0 ,0 ,1 ,3 ,7 ,15,15},
{0 ,0 ,1 ,3 ,7 ,15,15,15},
{0 ,1 ,3 ,7 ,15,15,15,15} };

byte	mapmasks3[4][8] = {
{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
{0 ,0 ,0 ,0 ,0 ,0 ,0 ,1},
{0 ,0 ,0 ,0 ,0 ,0 ,1 ,3},
{0 ,0 ,0 ,0 ,0 ,1 ,3 ,7} };


unsigned	wordmasks[8][8] = {
{0x0080,0x00c0,0x00e0,0x00f0,0x00f8,0x00fc,0x00fe,0x00ff},
{0x0040,0x0060,0x0070,0x0078,0x007c,0x007e,0x007f,0x807f},
{0x0020,0x0030,0x0038,0x003c,0x003e,0x003f,0x803f,0xc03f},
{0x0010,0x0018,0x001c,0x001e,0x001f,0x801f,0xc01f,0xe01f},
{0x0008,0x000c,0x000e,0x000f,0x800f,0xc00f,0xe00f,0xf00f},
{0x0004,0x0006,0x0007,0x8007,0xc007,0xe007,0xf007,0xf807},
{0x0002,0x0003,0x8003,0xc003,0xe003,0xf003,0xf803,0xfc03},
{0x0001,0x8001,0xc001,0xe001,0xf001,0xf801,0xfc01,0xfe01} };

int			slinex,slinewidth;
unsigned	far *linecmds;
long		linescale;
unsigned	maskword;

