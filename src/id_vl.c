// ID_VL.C

#include <dos.h>
#include <malloc.h>
#include <mem.h>
#include <string.h>
#include "src/id_head.h"
#include "src/id_vl.h"
#pragma hdrstop

//
// SC_INDEX is expected to stay at SC_MAPMASK for proper operation
//

unsigned	bufferofs;
unsigned	displayofs,pelpan;

unsigned	screenseg=SCREENSEG;		// set to 0xa000 for asm convenience

unsigned	linewidth;
unsigned	ylookup[MAXSCANLINES];

boolean		screenfaded;
unsigned	bordercolor;

boolean		fastpalette;				// if true, use outsb to set

byte		far	palette1[256][3],far palette2[256][3];

//===========================================================================

// asm

int	 VL_VideoID (void);
void VL_SetCRTC (int crtc);
void VL_SetScreen (unsigned int crtc, int pelpan);
void VL_WaitVBL (int vbls);

//===========================================================================


/*
=======================
=
= VL_Startup
=
=======================
*/

#if 0
void	VL_Startup (void)
{
	if ( !MS_CheckParm ("HIDDENCARD") && VL_VideoID () != 5)
		MS_Quit ("You need a VGA graphics card to run this!");

	asm	cld;				// all string instructions assume forward
}

#endif

/*
=======================
=
= VL_Startup	// WOLFENSTEIN HACK
=
=======================
*/

static	char *ParmStringsvl[] = {"HIDDENCARD",""};

void	VL_Startup (void)
{
	int i,videocard;

	__asm	cld;

	videocard = VL_VideoID ();
	for (i = 1;i < _argc;i++)
		if (US_CheckParm(_argv[i],ParmStringsvl) == 0)
		{
			videocard = 5;
			break;
		}

	if (videocard != 5)
Quit ("Improper video card!  If you really have a VGA card that I am not \n"
	  "detecting, use the -HIDDENCARD command line parameter!");

}



/*
=======================
=
= VL_Shutdown
=
=======================
*/

void	VL_Shutdown (void)
{
	VL_SetTextMode ();
}


/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void	VL_SetVGAPlaneMode (void)
{
	__asm {
		mov	ax,0x13
		int	0x10
	}
	VL_DePlaneVGA ();
	vgamapmask(15);
	VL_SetLineWidth (40);
}


/*
=======================
=
= VL_SetTextMode
=
=======================
*/

void	VL_SetTextMode (void)
{
	__asm {
		mov	ax,3
		int	0x10
	}
}

//===========================================================================

/*
=================
=
= VL_ClearVideo
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearVideo (byte color)
{
	__asm {
		mov	dx,GC_INDEX
		mov	al,GC_MODE
		out	dx,al
		inc	dx
		in	al,dx
		and	al,0xfc				// write mode 0 to store directly to video
		out	dx,al

		mov	dx,SC_INDEX
		mov	ax,SC_MAPMASK+15*256
		out	dx,ax				// write through all four planes

		mov	ax,SCREENSEG
		mov	es,ax
		mov	al,[color]
		mov	ah,al
		mov	cx,0x8000			// 0x8000 words, clearing 8 video bytes/word
		xor	di,di
		rep	stosw
	}
}


/*
=============================================================================

			VGA REGISTER MANAGEMENT ROUTINES

=============================================================================
*/


/*
=================
=
= VL_DePlaneVGA
=
=================
*/

void VL_DePlaneVGA (void)
{

//
// change CPU addressing to non linear mode
//

//
// turn off chain 4 and odd/even
//
	outp (SC_INDEX,SC_MEMMODE);
	outp (SC_INDEX+1,(inp(SC_INDEX+1)&~8)|4);

	outp (SC_INDEX,SC_MAPMASK);		// leave this set throughought

//
// turn off odd/even and set write mode 0
//
	outp (GC_INDEX,GC_MODE);
	outp (GC_INDEX+1,inp(GC_INDEX+1)&~0x13);

//
// turn off chain
//
	outp (GC_INDEX,GC_MISCELLANEOUS);
	outp (GC_INDEX+1,inp(GC_INDEX+1)&~2);

//
// clear the entire buffer space, because int 10h only did 16 k / plane
//
	VL_ClearVideo (0);

//
// change CRTC scanning from doubleword to byte mode, allowing >64k scans
//
	outp (CRTC_INDEX,CRTC_UNDERLINE);
	outp (CRTC_INDEX+1,inp(CRTC_INDEX+1)&~0x40);

	outp (CRTC_INDEX,CRTC_MODE);
	outp (CRTC_INDEX+1,inp(CRTC_INDEX+1)|0x40);
}

//===========================================================================

/*
====================
=
= VL_SetLineWidth
=
= Line witdh is in WORDS, 40 words is normal width for vgaplanegr
=
====================
*/

void VL_SetLineWidth (unsigned width)
{
	int i,offset;

//
// set wide virtual screen
//
	outpw (CRTC_INDEX,CRTC_OFFSET+width*256);

//
// set up lookup tables
//
	linewidth = width*2;

	offset = 0;

	for (i=0;i<MAXSCANLINES;i++)
	{
		ylookup[i]=offset;
		offset += linewidth;
	}
}

/*
====================
=
= VL_SetSplitScreen
=
====================
*/

void VL_SetSplitScreen (int linenum)
{
	VL_WaitVBL (1);
	linenum=linenum*2-1;
	outp (CRTC_INDEX,CRTC_LINECOMPARE);
	outp (CRTC_INDEX+1,linenum % 256);
	outp (CRTC_INDEX,CRTC_OVERFLOW);
	outp (CRTC_INDEX+1, 1+16*(linenum/256));
	outp (CRTC_INDEX,CRTC_MAXSCANLINE);
	outp (CRTC_INDEX+1,inp(CRTC_INDEX+1) & (255-64));
}


/*
=============================================================================

						PALETTE OPS

		To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/


/*
=================
=
= VL_FillPalette
=
=================
*/

void VL_FillPalette (int red, int green, int blue)
{
	int	i;

	outp (PEL_WRITE_ADR,0);
	for (i=0;i<256;i++)
	{
		outp (PEL_DATA,red);
		outp (PEL_DATA,green);
		outp (PEL_DATA,blue);
	}
}

//===========================================================================

/*
=================
=
= VL_SetColor
=
=================
*/

void VL_SetColor	(int color, int red, int green, int blue)
{
	outp (PEL_WRITE_ADR,color);
	outp (PEL_DATA,red);
	outp (PEL_DATA,green);
	outp (PEL_DATA,blue);
}

//===========================================================================

/*
=================
=
= VL_GetColor
=
=================
*/

void VL_GetColor	(int color, int *red, int *green, int *blue)
{
	outp (PEL_READ_ADR,color);
	*red = inp (PEL_DATA);
	*green = inp (PEL_DATA);
	*blue = inp (PEL_DATA);
}

//===========================================================================

/*
=================
=
= VL_SetPalette
=
= If fast palette setting has been tested for, it is used
= (some cards don't like outsb palette setting)
=
=================
*/

void VL_SetPalette (byte far *palette)
{
	int	i;

//	outp (PEL_WRITE_ADR,0);
//	for (i=0;i<768;i++)
//		outp(PEL_DATA,*palette++);

	__asm {
		mov	dx,PEL_WRITE_ADR
		mov	al,0
		out	dx,al
		mov	dx,PEL_DATA
		lds	si,[palette]

		test	[ss:fastpalette],1
		jz	slowset
//
// set palette fast for cards that can take it
//
		mov	cx,768
		//????rep outsb
		jmp	done

//
// set palette slowly for some video cards
//
slowset:
		mov	cx,256
setloop:
		lodsb
		out	dx,al
		lodsb
		out	dx,al
		lodsb
		out	dx,al
		loop	setloop

done:
		mov	ax,ss
		mov	ds,ax
	}
}


//===========================================================================

/*
=================
=
= VL_GetPalette
=
= This does not use the port string instructions,
= due to some incompatabilities
=
=================
*/

void VL_GetPalette (byte far *palette)
{
	int	i;

	outp (PEL_READ_ADR,0);
	for (i=0;i<768;i++)
		*palette++ = inp(PEL_DATA);
}


//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut (int start, int end, int red, int green, int blue, int steps)
{
	int		i,j,orig,delta;
	byte	far *origptr, far *newptr;

	VL_WaitVBL(1);
	VL_GetPalette (&palette1[0][0]);
	_fmemcpy (palette2,palette1,768);

//
// fade through intermediate frames
//
	for (i=0;i<steps;i++)
	{
		origptr = &palette1[start][0];
		newptr = &palette2[start][0];
		for (j=start;j<=end;j++)
		{
			orig = *origptr++;
			delta = red-orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = green-orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = blue-orig;
			*newptr++ = orig + delta * i / steps;
		}

		VL_WaitVBL(1);
		VL_SetPalette (&palette2[0][0]);
	}

//
// final color
//
	VL_FillPalette (red,green,blue);

	screenfaded = true;
}


/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn (int start, int end, byte far *palette, int steps)
{
	int		i,j,delta;

	VL_WaitVBL(1);
	VL_GetPalette (&palette1[0][0]);
	_fmemcpy (&palette2[0][0],&palette1[0][0],sizeof(palette1));

	start *= 3;
	end = end*3+2;

//
// fade through intermediate frames
//
	for (i=0;i<steps;i++)
	{
		for (j=start;j<=end;j++)
		{
			delta = palette[j]-palette1[0][j];
			palette2[0][j] = palette1[0][j] + delta * i / steps;
		}

		VL_WaitVBL(1);
		VL_SetPalette (&palette2[0][0]);
	}

//
// final color
//
	VL_SetPalette (palette);
	screenfaded = false;
}



/*
=================
=
= VL_TestPaletteSet
=
= Sets the palette with outsb, then reads it in and compares
= If it compares ok, fastpalette is set to true.
=
=================
*/

void VL_TestPaletteSet (void)
{
	int	i;

	for (i=0;i<768;i++)
		palette1[0][i] = i;

	fastpalette = true;
	VL_SetPalette (&palette1[0][0]);
	VL_GetPalette (&palette2[0][0]);
	if (_fmemcmp (&palette1[0][0],&palette2[0][0],768))
		fastpalette = false;
}


/*
==================
=
= VL_ColorBorder
=
==================
*/

void VL_ColorBorder (int color)
{
	_AH=0x10;
	_AL=1;
	_BH=color;
	geninterrupt (0x10);
	bordercolor = color;
}



/*
=============================================================================

							PIXEL OPS

=============================================================================
*/

byte	pixmasks[4] = {1,2,4,8};
byte	leftmasks[4] = {15,14,12,8};
byte	rightmasks[4] = {1,3,7,15};


/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot (int x, int y, int color)
{
	byte mask;

	mask = pixmasks[x&3];
	vgamapmask(mask);
	*(byte far *)MK_FP(SCREENSEG,bufferofs+(ylookup[y]+(x>>2))) = color;
	vgamapmask(15);
}


/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin (unsigned x, unsigned y, unsigned width, unsigned color)
{
	unsigned		xbyte;
	byte			far *dest;
	byte			leftmask,rightmask;
	int				midbytes;

	xbyte = x>>2;
	leftmask = leftmasks[x&3];
	rightmask = rightmasks[(x+width-1)&3];
	midbytes = ((x+width+3)>>2) - xbyte - 2;

	dest = MK_FP(SCREENSEG,bufferofs+ylookup[y]+xbyte);

	if (midbytes<0)
	{
	// all in one byte
		vgamapmask(leftmask&rightmask);
		*dest = color;
		vgamapmask(15);
		return;
	}

	vgamapmask(leftmask);
	*dest++ = color;

	vgamapmask(15);
	_fmemset (dest,color,midbytes);
	dest+=midbytes;

	vgamapmask(rightmask);
	*dest = color;

	vgamapmask(15);
}


/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin (int x, int y, int height, int color)
{
	byte	far *dest,mask;

	mask = pixmasks[x&3];
	vgamapmask(mask);

	dest = MK_FP(SCREENSEG,bufferofs+ylookup[y]+(x>>2));

	while (height--)
	{
		*dest = color;
		dest += linewidth;
	}

	vgamapmask(15);
}


/*
=================
=
= VL_Bar
=
=================
*/

void VL_Bar (int x, int y, int width, int height, int color)
{
	byte	far *dest;
	byte	leftmask,rightmask;
	int		midbytes,linedelta;

	leftmask = leftmasks[x&3];
	rightmask = rightmasks[(x+width-1)&3];
	midbytes = ((x+width+3)>>2) - (x>>2) - 2;
	linedelta = linewidth-(midbytes+1);

	dest = MK_FP(SCREENSEG,bufferofs+ylookup[y]+(x>>2));

	if (midbytes<0)
	{
	// all in one byte
		vgamapmask(leftmask&rightmask);
		while (height--)
		{
			*dest = color;
			dest += linewidth;
		}
		vgamapmask(15);
		return;
	}

	while (height--)
	{
		vgamapmask(leftmask);
		*dest++ = color;

		vgamapmask(15);
		_fmemset (dest,color,midbytes);
		dest+=midbytes;

		vgamapmask(rightmask);
		*dest = color;

		dest+=linedelta;
	}

	vgamapmask(15);
}

/*
============================================================================

							MEMORY OPS

============================================================================
*/

/*
=================
=
= VL_MemToLatch
=
=================
*/

void VL_MemToLatch (byte far *source, int width, int height, unsigned dest)
{
	unsigned	count;
	byte	plane,mask;

	count = ((width+3)/4)*height;
	mask = 1;
	for (plane = 0; plane<4 ; plane++)
	{
		vgamapmask(mask);
		mask <<= 1;

__asm	mov	cx,count
__asm mov ax,SCREENSEG
__asm mov es,ax
__asm	mov	di,[dest]
__asm	lds	si,[source]
__asm	rep movsb
__asm mov	ax,ss
__asm	mov	ds,ax

		source+= count;
	}
}


//===========================================================================


/*
=================
=
= VL_MemToScreen
=
= Draws a block of data to the screen.
=
=================
*/

void VL_MemToScreen (byte far *source, int width, int height, int x, int y)
{
	byte    far *screen,far *dest,mask;
	int		plane;

	width>>=2;
	dest = MK_FP(SCREENSEG,bufferofs+ylookup[y]+(x>>2) );
	mask = 1 << (x&3);

	for (plane = 0; plane<4; plane++)
	{
		vgamapmask(mask);
		mask <<= 1;
		if (mask == 16)
			mask = 1;

		screen = dest;
		for (y=0;y<height;y++,screen+=linewidth,source+=width)
			_fmemcpy (screen,source,width);
	}
}

//==========================================================================


/*
=================
=
= VL_MaskedToScreen
=
= Masks a block of main memory to the screen.
=
=================
*/

void VL_MaskedToScreen (byte far *source, int width, int height, int x, int y)
{
	byte    far *screen,far *dest,mask;
	byte	far *maskptr;
	int		plane;

	width>>=2;
	dest = MK_FP(SCREENSEG,bufferofs+ylookup[y]+(x>>2) );
//	mask = 1 << (x&3);

//	maskptr = source;

	for (plane = 0; plane<4; plane++)
	{
		vgamapmask(mask);
		mask <<= 1;
		if (mask == 16)
			mask = 1;

		screen = dest;
		for (y=0;y<height;y++,screen+=linewidth,source+=width)
			_fmemcpy (screen,source,width);
	}
}

//==========================================================================

/*
=================
=
= VL_LatchToScreen
=
=================
*/

void VL_LatchToScreen (unsigned source, int width, int height, int x, int y)
{
	vgawritemode(1);
	vgamapmask(15);

	__asm {
		mov	di,[y]				// dest = bufferofs+ylookup[y]+(x>>2)
		shl	di,1
		mov	di,[WORD PTR ylookup+di]
		add	di,[bufferofs]
		mov	ax,[x]
		shr	ax,1
		shr	ax,1
		add	di,ax

		mov	si,[source]
		mov	ax,[width]
		mov	bx,[linewidth]
		sub	bx,ax
		mov	dx,[height]
		mov	cx,SCREENSEG
		mov	ds,cx
		mov	es,cx

drawline:
		mov	cx,ax
		rep movsb
		add	di,bx
		dec	dx
		jnz	drawline

		mov	ax,ss
		mov	ds,ax
	}
	vgawritemode(0);
}


//===========================================================================

#if 0

/*
=================
=
= VL_ScreenToScreen
=
=================
*/

void VL_ScreenToScreen (unsigned source, unsigned dest,int width, int height)
{
	vgawritemode(1);
	vgamapmask(15);

	__asm {
		mov	si,[source]
		mov	di,[dest]
		mov	ax,[width]
		mov	bx,[linewidth]
		sub	bx,ax
		mov	dx,[height]
		mov	cx,SCREENSEG
		mov	ds,cx
		mov	es,cx

drawline:
		mov	cx,ax
		rep movsb
		add	si,bx
		add	di,bx
		dec	dx
		jnz	drawline

		mov	ax,ss
		mov	ds,ax
		}

	vgawritemode(0);
}


#endif

/*
=============================================================================

						STRING OUTPUT ROUTINES

=============================================================================
*/




/*
===================
=
= VL_DrawTile8String
=
===================
*/

void VL_DrawTile8String (char *str, char far *tile8ptr, int printx, int printy)
{
	int		i;
	unsigned	far *dest,far *screen,far *src;

	dest = MK_FP(SCREENSEG,bufferofs+ylookup[printy]+(printx>>2));

	while (*str)
	{
		src = (unsigned far *)(tile8ptr + (*str<<6));
		// each character is 64 bytes

		vgamapmask(1);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;
		vgamapmask(2);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;
		vgamapmask(4);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;
		vgamapmask(8);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;

		str++;
		printx += 8;
		dest+=2;
	}
}



/*
===================
=
= VL_DrawLatch8String
=
===================
*/

void VL_DrawLatch8String (char *str, unsigned tile8ptr, int printx, int printy)
{
	int		i;
	unsigned	src,dest;

	dest = bufferofs+ylookup[printy]+(printx>>2);

	vgawritemode(1);
	vgamapmask(15);

	while (*str)
	{
		src = tile8ptr + (*str<<4);		// each character is 16 latch bytes

		__asm {
			mov	si,[src]
			mov	di,[dest]
			mov	dx,[linewidth]

			mov	ax,SCREENSEG
			mov	ds,ax

			lodsw
			mov	[di],ax
			add	di,dx
			lodsw
			mov	[di],ax
			add	di,dx
			lodsw
			mov	[di],ax
			add	di,dx
			lodsw
			mov	[di],ax
			add	di,dx
			lodsw
			mov	[di],ax
			add	di,dx
			lodsw
			mov	[di],ax
			add	di,dx
			lodsw
			mov	[di],ax
			add	di,dx
			lodsw
			mov	[di],ax
			add	di,dx

			mov	ax,ss
			mov	ds,ax
	}
		str++;
		printx += 8;
		dest+=2;
	}

	vgawritemode(0);
}


/*
===================
=
= VL_SizeTile8String
=
===================
*/

void VL_SizeTile8String (char *str, int *width, int *height)
{
	*height = 8;
	*width = 8*strlen(str);
}


//===========================================================================

//
// VGA hardware routines
//

/*void vgawritemode(byte x)
{
	__asm {
		cli
		mov dx,GC_INDEX
		mov al,GC_MODE
		out dx,al
		inc dx
		in al,dx
		and al,252
		or al,x
		out dx,al
		sti
	}
}


void vgamapmask(byte x)
{
	__asm {
		cli
		mov dx,SC_INDEX
		mov al,SC_MAPMASK
		mov ah,x
		out dx,ax
		sti
	}
}

void vgareadmap(byte x)
{
	__asm {
		cli
		mov dx,GC_INDEX
		mov al,GC_READMAP
		mov ah,x
		out dx,ax
		sti
	}
}*/
