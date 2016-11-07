#include "id_heads.h"
#include "type.h"
#include "id_vl.h"
#include "wolfass.h"
typedef unsigned char byte;
extern void VGAMAPMASK(byte x);
extern void VGAWRITEMODE(byte x);
extern byte	update[UPDATEHIGH][UPDATEWIDE];
//SDL_t0ExtremeAsmService
//SDL_t0FastAsmService
//SDL_t0SlowAsmService
//SDL_IndicatePC
//SDL_SetDS

//
// SC_INDEX is expected to stay at SC_MAPMASK for proper operation
//

byte *vbuf=(byte *)0xa0000;
byte *vdisp=(byte *)0xa0000;

boolean		alNoIRQ;

extern volatile byte	pcLastSample,far *pcSound;
extern longword		pcLengthLeft;
extern word			pcSoundLookup[255];
extern byte			far *alSound;
extern soundnames		SoundNumber,DigiNumber;
extern word			SoundPriority,DigiPriority;

int count_time=0;
int count_fx=0;
int extreme=0;
volatile boolean pcindicate;
volatile boolean MyDS;

static  boolean                 DigiMissed,DigiLastSegment;
static  memptr                  DigiNextAddr;
static  word                    DigiNextLen;


int rndindex = 0;

static byte rndtable[] = {
      0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
	 74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
	 95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
	 52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
	149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
	145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0,
	175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
	 25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
	 94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
	136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
	135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
	 80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
	 24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
	145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
	 28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
	 71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
	 17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
	197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
	120, 163, 236, 249 };

int VL_VideoID()
{
	return 0;
}
void VL_WaitVBL(int num)
{
	__asm {
		mov	cx,num
		mov	dx,03dah
		VBLActive:
		in al,dx
		test al,8
		jnz VBLActive
		noVBL:
		in al,dx
		test al,8
		jz noVBL
		loop VBLActive
		//parm [cx]
		//modify exact [dx al cx]
	}
}

//void SetScreen (unsigned int offset)
void	VL_SetScreen (unsigned int crtc, int pelpan)
{
	__asm {
		cli
//		mov	ax,[word ptr offset]
		mov	dx,0x3d4
		mov	al,0x0c
		out	dx,al
		inc	dx
		mov	al,ah
		out	dx,al
		sti
	}
}

/*
=============================================================================

				Double buffer management routines

=============================================================================
*/

void VH_UpdateScreen()
{
	int i,x,y,offs;
	byte *updateptr=(byte *) update;
	VGAMAPMASK(15);
	VGAWRITEMODE(1);

//
// copy a tile
//
	__asm {
		mov	ax,SCREENSEG
		mov	ds,ax
	}
	for(y=0;y<UPDATEHIGH;y++)
	{
		for(x=0;x<UPDATEWIDE;x++,updateptr++)
		{
			if(*updateptr)
			{
				*updateptr=0;
				offs=y*16*SCREENWIDTH+x*TILEWIDTH;
				for(i=0;i<16;i++,offs+=linewidth)
				{
/*						__asm {
	mov	al,[si]
	mov	[di],al
	mov	al,[si+1]
	mov	[di+1],al
	mov	al,[si+2]
	mov	[di+2],al
	mov	al,[si+3]
	mov	[di+3],al
	add	si,dx
	add	di,dx
						}*/
					*(vdisp+offs)=*(vbuf+offs);
					*(vdisp+offs+1)=*(vbuf+offs+1);
					*(vdisp+offs+2)=*(vbuf+offs+2);
					*(vdisp+offs+3)=*(vbuf+offs+3);
				}
			}
		}
	}
	VGAWRITEMODE(0);
}

void SDL_SetDS()
{
	__asm {
		mov	ax,ds
		mov	[cs:MyDS],ds
		ret
	}
}

void SDL_turnOnPCSpeaker(word timerval)
{
	__asm {
		mov	bx,timerval
		mov	al,0b6h
		out	43h,al
		mov	al,bl
		out	42h,al
		mov	al,bh
		out	42h,al
		in	al,61h
		or	al,3
		out	61h,al
	}
}

void SDL_turnOffPCSpeaker()
{
	__asm {
		in	al,61h
		and	al,0fch
		out	61h,al
	}
}

void SDL_setPCSpeaker(byte val)
{
	__asm {
		mov	al,val
		in	al,61h
		and	al,0fch
		or	al,ah
		out	61h,al
	}
}

void SDL_DoFX()
{
	if(pcSound)
	{
		if(*pcSound!=pcLastSample)
		{
			pcLastSample=*pcSound;

			if(pcLastSample)
				SDL_turnOnPCSpeaker(pcLastSample*60);
			else
				SDL_turnOffPCSpeaker();
		}
		pcSound++;
		pcLengthLeft--;
		if(!pcLengthLeft)
		{
			pcSound=0;
			SoundNumber=(soundnames)0;
			SoundPriority=0;
			SDL_turnOffPCSpeaker();
		}
	}

	if(alSound && !alNoIRQ)
	{
		if(*alSound)
		{
			alOutInIRQ(alFreqL,*alSound);
			alOutInIRQ(alFreqH,alBlock);
		}
		else alOutInIRQ(alFreqH,0);
		alSound++;
		alLengthLeft--;
		if(!alLengthLeft)
		{
			alSound=0;
			SoundNumber=(soundnames)0;
			SoundPriority=0;
			alOutInIRQ(alFreqH,0);
		}
	}

}

void SDL_DoFast()
{
	count_fx++;
	if(count_fx>=5)
	{
		count_fx=0;

		SDL_DoFX();

		count_time++;
		if(count_time>=2)
		{
			TimeCount++;
			count_time=0;
		}
	}

	if(sqActive && !alNoIRQ)
	{
		if(sqHackLen)
		{
			do
			{
				if(sqHackTime>alTimeCount) break;
				sqHackTime=alTimeCount+*(sqHackPtr+1);
				alOutInIRQ(*(byte *)sqHackPtr,*(((byte *)sqHackPtr)+1));
				sqHackPtr+=2;
				sqHackLen-=4;
			}
			while(sqHackLen);
		}
		alTimeCount++;
		if(!sqHackLen)
		{
			sqHackPtr=sqHack;
			sqHackLen=sqHackSeqLen;
			alTimeCount=0;
			sqHackTime=0;
		}
	}

	if(ssSample)
	{
		if(!(inp(ssStatus)&0x40))
		{
			outp(ssData,*ssSample++);
			outp(ssControl,ssOff);
			__asm push ax
			__asm pop ax
			outp(ssControl,ssOn);
			__asm push ax
			__asm pop ax
			ssLengthLeft--;
			if(!ssLengthLeft)
			{
				ssSample=0;
				SDL_DigitizedDoneInIRQ();
			}
		}
	}

	TimerCount+=TimerDivisor;
	if(*((word *)&TimerCount+1))
	{
		*((word *)&TimerCount+1)=0;
		t0OldService();
	}
	else
	{
		outp(0x20,0x20);
	}
}

// Timer 0 ISR for 7000Hz interrupts
void interrupt SDL_t0ExtremeAsmService(void)
{
	if(pcindicate)
	{
		if(pcSound)
		{
			SDL_setPCSpeaker(((*pcSound++)&0x80)>>6);
			pcLengthLeft--;
			if(!pcLengthLeft)
			{
				pcSound=0;
				SDL_turnOffPCSpeaker();
				SDL_DigitizedDoneInIRQ();
			}
		}
	}
	extreme++;
	if(extreme>=10)
	{
		extreme=0;
		SDL_DoFast();
	}
	else
		outp(0x20,0x20);
}

// Timer 0 ISR for 7000Hz interrupts
void interrupt __SDL_t0ExtremeAsmService()
{
	if(pcindicate)
	{
		if(pcSound)
		{
			SDL_setPCSpeaker(((*pcSound++)&0x80)>>6);
			pcLengthLeft--;
			if(!pcLengthLeft)
			{
				pcSound=0;
				SDL_turnOffPCSpeaker();
				SDL_DigitizedDoneInIRQ();
			}
		}
	}
	extreme++;
	if(extreme>=10)
	{
		extreme=0;
		SDL_DoFast();
	}
	else
		outp(0x20,0x20);
}

// Timer 0 ISR for 700Hz interrupts
void interrupt SDL_t0FastAsmService(void)
{
	SDL_DoFast();
}

// Timer 0 ISR for 140Hz interrupts
void interrupt SDL_t0SlowAsmService(void)
{
	count_time++;
	if(count_time>=2)
	{
		TimeCount++;
		count_time=0;
	}

	SDL_DoFX();

	TimerCount+=TimerDivisor;
	if(*((word *)&TimerCount+1))
	{
		*((word *)&TimerCount+1)=0;
		t0OldService();
	}
	else
		outp(0x20,0x20);
}

void SDL_IndicatePC(boolean ind)
{
	pcindicate=ind;
}

void
SDL_DigitizedDoneInIRQ(void)
{
	if (DigiNextAddr)
	{
		SDL_PlayDigiSegment(DigiNextAddr,DigiNextLen/*,true*/);
		DigiNextAddr = nil;
		DigiMissed = false;
	}
	else
	{
		if (DigiLastSegment)
		{
			DigiPlaying = false;
			DigiLastSegment = false;
			if ((DigiMode == sds_PC) && (SoundMode == sdm_PC))
			{
				SDL_SoundFinished();
			}
			else
			{
				DigiNumber = (soundnames) 0;
				DigiPriority = 0;
			}
			SoundPositioned = false;
		}
		else
			DigiMissed = true;
	}
}

// Inside an interrupt handler interrupts should already be disabled
// so don't disable them again and cause V86 exceptions which cost
// aprox. 300 processor tics!

//static
void alOutInIRQ(byte n,byte b)
{
	__asm {
		mov     dx,0x388
		mov     al,[n]
		out     dx,al
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		inc     dx
		mov     al,[b]
		out     dx,al

		dec     dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx

		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx

		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx

		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
		in      al,dx
	}
}

///////////////////////////////////////////////////////////////////////////
//
// US_InitRndT - Initializes the pseudo random number generator.
//      If randomize is true, the seed will be initialized depending on the
//      current time
//
///////////////////////////////////////////////////////////////////////////
void US_InitRndT(boolean randomize)
{
	__asm {
		mov	ax,SEG rndtable
		mov	es,ax

		mov	ax,[randomize]
		or	ax,ax
		jne	@@timeit		//if randomize is true, really random

		mov	dx,0			//set to a definite value
		jmp	@@setit

	@@timeit:
		mov	ah,2ch
		int	21h			//GetSystemTime
		and	dx,0ffh

	@@setit:
		mov	[es:rndindex],dx
		ret
	}
    /*if(randomize)
        rndindex = TimeIt() & 0xff;
    else
        rndindex = 0;*/
}

///////////////////////////////////////////////////////////////////////////
//
// US_RndT - Returns the next 8-bit pseudo random number
//
///////////////////////////////////////////////////////////////////////////
int US_RndT()
{
    rndindex = (rndindex+1)&0xff;
    return rndtable[rndindex];
}

	/*void US_InitRndT(int randomize);
	#pragma aux US_InitRndT parm [AX] modify exact [ax cx edx]

	int US_RndT();
	#pragma aux US_RndT value [EAX] modify exact [eax ebx]*/
