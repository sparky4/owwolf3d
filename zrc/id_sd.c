//
//	ID Engine
//	ID_SD.c - Sound Manager for Wolfenstein 3D
//	v1.2
//	By Jason Blochowiak
//

//
//	This module handles dealing with generating sound on the appropriate
//		hardware
//
//	Depends on: User Mgr (for parm checking)
//
//	Globals:
//		For User Mgr:
//			SoundSourcePresent - Sound Source thingie present?
//			SoundBlasterPresent - SoundBlaster card present?
//			AdLibPresent - AdLib card present?
//			SoundMode - What device is used for sound effects
//				(Use SM_SetSoundMode() to set)
//			MusicMode - What device is used for music
//				(Use SM_SetMusicMode() to set)
//			DigiMode - What device is used for digitized sound effects
//				(Use SM_SetDigiDevice() to set)
//
//		For Cache Mgr:
//			NeedsDigitized - load digitized sounds?
//			NeedsMusic - load music?
//

#pragma hdrstop		// Wierdo thing with MUSE

#include <dos.h>

#ifdef	_MUSE_      // Will be defined in ID_Types.h
#include "ID_SD.h"
#else
#include "id_heads.h"
#endif
#pragma	hdrstop
#pragma	warn	-pia

//#ifdef	nil
//#undef	nil
//#endif
//#define	nil	0

#define	SDL_SoundFinished()	{SoundNumber = SoundPriority = 0;}

// Macros for SoundBlaster stuff
#define	sbOut(n,b)	outp((n) + sbLocation,b)
#define	sbIn(n)		inp((n) + sbLocation)
#define	sbWriteDelay()	while (sbIn(sbWriteStat) & 0x80);
#define	sbReadDelay()	while (sbIn(sbDataAvail) & 0x80);

// Macros for AdLib stuff
#define	selreg(n)	outp(alFMAddr,n)
#define	writereg(n)	outp(alFMData,n)
#define	readstat()	inp(alFMStatus)

//	Imports from ID_SD_A.ASM
// // extern	void			SDL_SetDS(void),
// // 						SDL_IndicatePC(boolean on);
// // extern	void __interrupt	SDL_t0ExtremeAsmService(void),
// // 						SDL_t0FastAsmService(void),
// // 						SDL_t0SlowAsmService(void);

//	Global variables
	boolean		SoundSourcePresent,
				AdLibPresent,
				SoundBlasterPresent,SBProPresent,
				NeedsDigitized,NeedsMusic,
				SoundPositioned;
	SDMode		SoundMode;
	SMMode		MusicMode;
	SDSMode		DigiMode;
	longword	TimeCount;
	word		HackCount;
	word		*SoundTable;	// Really * _seg *SoundTable, but that don't work
	boolean		ssIsTandy;
	word		ssPort = 2;
	int			DigiMap[LASTSOUND];

//	Internal variables
static	boolean			SD_Started;
		boolean			nextsoundpos;
		longword		TimerDivisor,TimerCount;
static	char			*ParmStringssd[] =
						{
							"noal",
							"nosb",
							"nopro",
							"noss",
							"sst",
							"ss1",
							"ss2",
							"ss3",
							nil
						};
static	void			(*SoundUserHook)(void);
		soundnames		SoundNumber,DigiNumber;
		word			SoundPriority,DigiPriority;
		int				LeftPosition,RightPosition;
		void interrupt	(*t0OldService)(void);
		long			LocalTime;
		word			TimerRate;

		word			NumDigi,DigiLeft,DigiPage;
		word			*DigiList;
		word			DigiLastStart,DigiLastEnd;
		boolean			DigiPlaying;
static	boolean			DigiMissed,DigiLastSegment;
static	memptr			DigiNextAddr;
static	word			DigiNextLen;

//	SoundBlaster variables
static	boolean					sbNoCheck,sbNoProCheck;
static	volatile boolean		sbSamplePlaying;
static	byte					sbOldIntMask = -1;
static	volatile byte			huge *sbNextSegPtr;
static	byte					sbDMA = 1,
								sbDMAa1 = 0x83,sbDMAa2 = 2,sbDMAa3 = 3,
								sba1Vals[] = {0x87,0x83,0,0x82},
								sba2Vals[] = {0,2,0,6},
								sba3Vals[] = {1,3,0,7};
static	int						sbLocation = -1,sbInterrupt = 7,sbIntVec = 0xf,
								sbIntVectors[] = {-1,-1,0xa,0xb,-1,0xd,-1,0xf,-1,-1,-1};
static	volatile longword		sbNextSegLen;
static	volatile SampledSound	huge *sbSamples;
static	void interrupt			(*sbOldIntHand)(void);
static	byte					sbpOldFMMix,sbpOldVOCMix;

//	SoundSource variables
		boolean				ssNoCheck;
		boolean				ssActive;
		word				ssControl,ssStatus,ssData;
		byte				ssOn,ssOff;
		volatile byte		far *ssSample;
		volatile longword	ssLengthLeft;

//	PC Sound variables
		volatile byte	pcLastSample,far *pcSound;
		word		pcLengthLeft;
		word			pcSoundLookup[255];

//	AdLib variables
		boolean			alNoCheck;
		byte			far *alSound;
		word			alBlock;
		longword		alLengthLeft;
		longword		alTimeCount;
		Instrument		alZeroInst;
		boolean	 alNoIRQ;

// This table maps channel numbers to carrier and modulator op cells
static	byte			carriers[9] =  { 3, 4, 5,11,12,13,19,20,21},
						modifiers[9] = { 0, 1, 2, 8, 9,10,16,17,18},
// This table maps percussive voice numbers to op cells
						pcarriers[5] = {19,0xff,0xff,0xff,0xff},
						pmodifiers[5] = {16,17,18,20,21};

//	Sequencer variables
		boolean			sqActive;
static	word			alFXReg;
static	ActiveTrack		*tracks[sqMaxTracks],
						mytracks[sqMaxTracks];
static	word			sqMode,sqFadeStep;
		word			far *sqHack,far *sqHackPtr,sqHackLen,sqHackSeqLen;
		long			sqHackTime;

//	Internal routines
		void			SDL_DigitizedDone(void);

int count_time=0;
int count_fx=0;
int extreme=0;
volatile boolean pcindicate;

void SDL_turnOnPCSpeaker(word timerval);
#pragma aux SDL_turnOnPCSpeaker = \
	"mov    al,0b6h" \
	"out    43h,al" \
	"mov    al,bl" \
	"out    42h,al" \
	"mov    al,bh" \
	"out    42h,al" \
	"in     al,61h" \
	"or     al,3"   \
	"out    61h,al" \
	parm [bx] \
	modify exact [al]

void SDL_turnOffPCSpeaker();
#pragma aux SDL_turnOffPCSpeaker = \
	"in     al,61h" \
	"and    al,0fch" \
	"out    61h,al" \
	modify exact [al]

void SDL_setPCSpeaker(byte val);
#pragma aux SDL_setPCSpeaker = \
	"in     al,61h" \
	"and    al,0fch" \
	"or     al,ah" \
	"out    61h,al" \
	parm [ah] \
	modify exact [al]

void inline SDL_DoFX()
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

void inline SDL_DoFast()
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
void __interrupt SDL_t0ExtremeAsmService(void)
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
void __interrupt __SDL_t0ExtremeAsmService(void)
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
void __interrupt SDL_t0FastAsmService(void)
{
	SDL_DoFast();
}

// Timer 0 ISR for 140Hz interrupts
void __interrupt SDL_t0SlowAsmService(void)
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

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SetTimer0() - Sets system timer 0 to the specified speed
//
///////////////////////////////////////////////////////////////////////////
#pragma	argsused
static void
SDL_SetTimer0(word speed)
{
#ifndef TPROF	// If using Borland's profiling, don't screw with the timer
__asm{
	pushf
	cli
}

	outp(0x43,0x36);				// Change timer 0
	outp(0x40,speed);
	outp(0x40,speed >> 8);
	// Kludge to handle special case for digitized PC sounds
	if (TimerDivisor == (1192030 / (TickBase * 100)))
		TimerDivisor = (1192030 / (TickBase * 10));
	else
		TimerDivisor = speed;

	__asm popf
#else
	TimerDivisor = 0x10000;
#endif
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SetIntsPerSec() - Uses SDL_SetTimer0() to set the number of
//		interrupts generated by system timer 0 per second
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_SetIntsPerSec(word ints)
{
	TimerRate = ints;
	SDL_SetTimer0(1192030 / ints);
}

static void
SDL_SetTimerSpeed(void)
{
	word	rate;
	void interrupt	(*isr)(void);

	if ((DigiMode == sds_PC) && DigiPlaying)
	{
		rate = TickBase * 100;
		isr = SDL_t0ExtremeAsmService;
	}
	else if
	(
		(MusicMode == smm_AdLib)
	||	((DigiMode == sds_SoundSource) && DigiPlaying)
	)
	{
		rate = TickBase * 10;
		isr = SDL_t0FastAsmService;
	}
	else
	{
		rate = TickBase * 2;
		isr = SDL_t0SlowAsmService;
	}

	if (rate != TimerRate)
	{
		_dos_setvect(8,isr);
		SDL_SetIntsPerSec(rate);
		TimerRate = rate;
	}
}

//
//	SoundBlaster code
//

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SBStopSample() - Stops any active sampled sound and causes DMA
//		requests from the SoundBlaster to cease
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_SBStopSample(void)
{
	byte	is;

	__asm {
		pushf
		cli

	}


	if (sbSamplePlaying)
	{
		sbSamplePlaying = false;

		sbWriteDelay();
		sbOut(sbWriteCmd,0xd0);	// Turn off DSP DMA

		is = inp(0x21);	// Restore interrupt mask bit
		if (sbOldIntMask & (1 << sbInterrupt))
			is |= (1 << sbInterrupt);
		else
			is &= ~(1 << sbInterrupt);
		outp(0x21,is);
	}

__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SBPlaySeg() - Plays a chunk of sampled sound on the SoundBlaster
//	Insures that the chunk doesn't cross a bank boundary, programs the DMA
//	 controller, and tells the SB to start doing DMA requests for DAC
//
///////////////////////////////////////////////////////////////////////////
static longword
SDL_SBPlaySeg(volatile byte huge *data,longword length)
{
	unsigned		datapage;
	longword		dataofs,uselen;

	uselen = length;
	datapage = FP_SEG(data) >> 12;
	dataofs = ((FP_SEG(data) & 0xfff) << 4) + FP_OFF(data);
	if (dataofs >= 0x10000)
	{
		datapage++;
		dataofs -= 0x10000;
	}

	if (dataofs + uselen > 0x10000)
		uselen = 0x10000 - dataofs;

	uselen--;

	// Program the DMA controller
	__asm {
		pushf
		cli
	}
	outp(0x0a,sbDMA | 4);					// Mask off DMA on channel sbDMA
	outp(0x0c,0);							// Clear byte ptr flip-flop to lower byte
	outp(0x0b,0x49);						// Set transfer mode for D/A conv
	outp(sbDMAa2,(byte)dataofs);			// Give LSB of address
	outp(sbDMAa2,(byte)(dataofs >> 8));		// Give MSB of address
	outp(sbDMAa1,(byte)datapage);			// Give page of address
	outp(sbDMAa3,(byte)uselen);				// Give LSB of length
	outp(sbDMAa3,(byte)(uselen >> 8));		// Give MSB of length
	outp(0x0a,sbDMA);						// Re-enable DMA on channel sbDMA

	// Start playing the thing
	sbWriteDelay();
	sbOut(sbWriteCmd,0x14);
	sbWriteDelay();
	sbOut(sbWriteData,(byte)uselen);
	sbWriteDelay();
	sbOut(sbWriteData,(byte)(uselen >> 8));
__asm popf

	return(uselen + 1);
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SBService() - Services the SoundBlaster DMA interrupt
//
///////////////////////////////////////////////////////////////////////////
void interrupt
SDL_SBService(void)
{
	longword	used;

	sbIn(sbDataAvail);	// Ack interrupt to SB

	if (sbNextSegPtr)
	{
		used = SDL_SBPlaySeg(sbNextSegPtr,sbNextSegLen);
		if (sbNextSegLen <= used)
			sbNextSegPtr = nil;
		else
		{
			sbNextSegPtr += used;
			sbNextSegLen -= used;
		}
	}
	else
	{
		SDL_SBStopSample();
		SDL_DigitizedDone();
	}

	outp(0x20,0x20);	// Ack interrupt
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SBPlaySample() - Plays a sampled sound on the SoundBlaster. Sets up
//		DMA to play the sound
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_SBPlaySample(byte huge *data,longword len)
{
	longword	used;

	SDL_SBStopSample();

	__asm {
		pushf
		cli
	}

	used = SDL_SBPlaySeg(data,len);
	if (len <= used)
		sbNextSegPtr = nil;
	else
	{
		sbNextSegPtr = data + used;
		sbNextSegLen = len - used;
	}

	// Save old interrupt status and unmask ours
	sbOldIntMask = inp(0x21);
	outp(0x21,sbOldIntMask & ~(1 << sbInterrupt));

	sbWriteDelay();
	sbOut(sbWriteCmd,0xd4);						// Make sure DSP DMA is enabled

	sbSamplePlaying = true;

__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PositionSBP() - Sets the attenuation levels for the left and right
//		channels by using the mixer chip on the SB Pro. This hits a hole in
//		the address map for normal SBs.
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_PositionSBP(int leftpos,int rightpos)
{
	byte	v;

	if (!SBProPresent)
		return;

	leftpos = 15 - leftpos;
	rightpos = 15 - rightpos;
	v = ((leftpos & 0x0f) << 4) | (rightpos & 0x0f);

	__asm {
		pushf
		cli
	}

	sbOut(sbpMixerAddr,sbpmVoiceVol);
	sbOut(sbpMixerData,v);

__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_CheckSB() - Checks to see if a SoundBlaster resides at a
//		particular I/O location
//
///////////////////////////////////////////////////////////////////////////
static boolean
SDL_CheckSB(int port)
{
	int	i;

	sbLocation = port << 4;		// Initialize stuff for later use

	sbOut(sbReset,true);		// Reset the SoundBlaster DSP
	__asm {
		mov	dx,0x388				// Wait >4usec
		in	al, dx
	in	al, dx
	in	al, dx
	in	al, dx
	in	al, dx
	in	al, dx
	in	al, dx
	in	al, dx
	in	al, dx
	}
	sbOut(sbReset,false);		// Turn off sb DSP reset
	__asm {
	mov	dx,0x388				// Wait >100usec
	mov	cx,100
usecloop:
	in	al,dx
	loop usecloop
	}
	for (i = 0;i < 100;i++)
	{
		if (sbIn(sbDataAvail) & 0x80)		// If data is available...
		{
			if (sbIn(sbReadData) == 0xaa)	// If it matches correct value
				return(true);
			else
			{
				sbLocation = -1;			// Otherwise not a SoundBlaster
				return(false);
			}
		}
	}
	sbLocation = -1;						// Retry count exceeded - fail
	return(false);
}

///////////////////////////////////////////////////////////////////////////
//
//	Checks to see if a SoundBlaster is in the system. If the port passed is
//		-1, then it scans through all possible I/O locations. If the port
//		passed is 0, then it uses the default (2). If the port is >0, then
//		it just passes it directly to SDL_CheckSB()
//
///////////////////////////////////////////////////////////////////////////
static boolean
SDL_DetectSoundBlaster(int port)
{
	int	i;

	if (port == 0)					// If user specifies default, use 2
		port = 2;
	if (port == -1)
	{
		if (SDL_CheckSB(2))			// Check default before scanning
			return(true);

		if (SDL_CheckSB(4))			// Check other SB Pro location before scan
			return(true);

		for (i = 1;i <= 6;i++)		// Scan through possible SB locations
		{
			if ((i == 2) || (i == 4))
				continue;

			if (SDL_CheckSB(i))		// If found at this address,
				return(true);		//	return success
		}
		return(false);				// All addresses failed, return failure
	}
	else
		return(SDL_CheckSB(port));	// User specified address or default
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SBSetDMA() - Sets the DMA channel to be used by the SoundBlaster
//		code. Sets up sbDMA, and sbDMAa1-sbDMAa3 (used by SDL_SBPlaySeg()).
//
///////////////////////////////////////////////////////////////////////////
void
SDL_SBSetDMA(byte channel)
{
	if (channel > 3)
		Quit("SDL_SBSetDMA() - invalid SoundBlaster DMA channel");

	sbDMA = channel;
	sbDMAa1 = sba1Vals[channel];
	sbDMAa2 = sba2Vals[channel];
	sbDMAa3 = sba3Vals[channel];
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_StartSB() - Turns on the SoundBlaster
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_StartSB(void)
{
	byte	timevalue,test;

	sbIntVec = sbIntVectors[sbInterrupt];
	if (sbIntVec < 0)
		Quit("SDL_StartSB: Illegal or unsupported interrupt number for SoundBlaster");

	sbOldIntHand = _dos_getvect(sbIntVec);	// Get old interrupt handler
	_dos_setvect(sbIntVec,SDL_SBService);	// Set mine

	sbWriteDelay();
	sbOut(sbWriteCmd,0xd1);				// Turn on DSP speaker

	// Set the SoundBlaster DAC time constant for 7KHz
	timevalue = 256 - (1000000 / 7000);
	sbWriteDelay();
	sbOut(sbWriteCmd,0x40);
	sbWriteDelay();
	sbOut(sbWriteData,timevalue);

	SBProPresent = false;
	if (sbNoProCheck)
		return;

	// Check to see if this is a SB Pro
	sbOut(sbpMixerAddr,sbpmFMVol);
	sbpOldFMMix = sbIn(sbpMixerData);
	sbOut(sbpMixerData,0xbb);
	test = sbIn(sbpMixerData);
	if (test == 0xbb)
	{
		// Boost FM output levels to be equivilent with digitized output
		sbOut(sbpMixerData,0xff);
		test = sbIn(sbpMixerData);
		if (test == 0xff)
		{
			SBProPresent = true;

			// Save old Voice output levels (SB Pro)
			sbOut(sbpMixerAddr,sbpmVoiceVol);
			sbpOldVOCMix = sbIn(sbpMixerData);

			// Turn SB Pro stereo DAC off
			sbOut(sbpMixerAddr,sbpmControl);
			sbOut(sbpMixerData,0);				// 0=off,2=on
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutSB() - Turns off the SoundBlaster
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_ShutSB(void)
{
	SDL_SBStopSample();

	if (SBProPresent)
	{
		// Restore FM output levels (SB Pro)
		sbOut(sbpMixerAddr,sbpmFMVol);
		sbOut(sbpMixerData,sbpOldFMMix);

		// Restore Voice output levels (SB Pro)
		sbOut(sbpMixerAddr,sbpmVoiceVol);
		sbOut(sbpMixerData,sbpOldVOCMix);
	}

	_dos_setvect(sbIntVec,sbOldIntHand);		// Set vector back
}

//	Sound Source Code

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SSStopSample() - Stops a sample playing on the Sound Source
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_SSStopSample(void)
{
	__asm {
		pushf
		cli
	}

	/*(long)*/ssSample = 0;

__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SSService() - Handles playing the next sample on the Sound Source
//
///////////////////////////////////////////////////////////////////////////
void
SDL_SSService(void)
{
	boolean	gotit;
	boolean doneflag=false;
	byte	v;

	while (ssSample)
	{
	__asm {
		mov		dx,[ssStatus]	// Check to see if FIFO is currently empty
		in		al,dx
		test	al,0x40
		jnz		done			// Nope - don't push any more data out
		jmp end
		done:
		mov	doneflag,1
		end:
	}
		if(!doneflag)
		{
			v = *ssSample++;
			if (!(--ssLengthLeft))
			{
				/*(long)*/ssSample = 0;
				SDL_DigitizedDone();
			}

			__asm {
				mov		dx,[ssData]		// Pump the value out
				mov		al,[v]
				out		dx,al

				mov		dx,[ssControl]	// Pulse printer select
				mov		al,[ssOff]
				out		dx,al
				push	ax
				pop		ax
				mov		al,[ssOn]
				out		dx,al

				push	ax				// Delay a short while
				pop		ax
				push	ax
				pop		ax
done:;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SSPlaySample() - Plays the specified sample on the Sound Source
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_SSPlaySample(byte huge *data,longword len)
{
	__asm {
		pushf
		cli
	}

	ssLengthLeft = len;
	ssSample = (volatile byte far *)data;

__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_StartSS() - Sets up for and turns on the Sound Source
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_StartSS(void)
{
	if (ssPort == 3)
		ssControl = 0x27a;	// If using LPT3
	else if (ssPort == 2)
		ssControl = 0x37a;	// If using LPT2
	else
		ssControl = 0x3be;	// If using LPT1
	ssStatus = ssControl - 1;
	ssData = ssStatus - 1;

	ssOn = 0x04;
	if (ssIsTandy)
		ssOff = 0x0e;				// Tandy wierdness
	else
		ssOff = 0x0c;				// For normal machines

	outp(ssControl,ssOn);		// Enable SS
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutSS() - Turns off the Sound Source
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_ShutSS(void)
{
	outp(ssControl,ssOff);
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_CheckSS() - Checks to see if a Sound Source is present at the
//		location specified by the sound source variables
//
///////////////////////////////////////////////////////////////////////////
static boolean
SDL_CheckSS(void)
{
	boolean		present = false, chkdone=0;
	longword	lasttime;

	// Turn the Sound Source on and wait awhile (4 ticks)
	SDL_StartSS();

	lasttime = TimeCount;
	while (TimeCount < lasttime + 4)
	{}

	__asm {
		mov		dx,[ssStatus]	// Check to see if FIFO is currently empty
		in		al,dx
		test	al,0x40
		jnz		checkdone		// Nope - Sound Source not here

		mov		cx,32			// Force FIFO overflow (FIFO is 16 bytes)
outloop:
		mov		dx,[ssData]		// Pump a neutral value out
		mov		al,0x80
		out		dx,al

		mov		dx,[ssControl]	// Pulse printer select
		mov		al,[ssOff]
		out		dx,al
		push	ax
		pop		ax
		mov		al,[ssOn]
		out		dx,al

		push	ax				// Delay a short while before we do this again
		pop		ax
		push	ax
		pop		ax

		loop	outloop

		mov		dx,[ssStatus]	// Is FIFO overflowed now?
		in		al,dx
		test	al,0x40
		jz		checkdone		// Nope, still not - Sound Source not here
		jmp end
		checkdone:
		mov	chkdone,1
		end:
	}
	if(!chkdone) present = true;			// Yes - it's here!

//checkdone:
	SDL_ShutSS();
	return(present);
}

static boolean
SDL_DetectSoundSource(void)
{
	for (ssPort = 1;ssPort <= 3;ssPort++)
		if (SDL_CheckSS())
			return(true);
	return(false);
}

//
//	PC Sound code
//

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCPlaySample() - Plays the specified sample on the PC speaker
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_PCPlaySample(byte huge *data,longword len)
{
	__asm {
		pushf
		cli
	}

	SDL_IndicatePC(true);

	pcLengthLeft = len;
	pcSound = (volatile byte far *)data;

	__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCStopSample() - Stops a sample playing on the PC speaker
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_PCStopSample(void)
{
	__asm {
		pushf
		cli
	}

	/*(long)*/pcSound = 0;

	SDL_IndicatePC(false);
	__asm {
		in	al,0x61		  	// Turn the speaker off
		and	al,0xfd			// ~2
		out	0x61,al

	__asm popf
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCPlaySound() - Plays the specified sound on the PC speaker
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_PCPlaySound(PCSound far *sound)
{
	__asm {
		pushf
		cli
	}
	pcLastSample = -1;
	pcLengthLeft = sound->common.length;
	pcSound = sound->data;

	__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCStopSound() - Stops the current sound playing on the PC Speaker
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_PCStopSound(void)
{
	__asm {
		pushf
		cli
	}

	/*(long)*/pcSound = 0;

	__asm {
		in	al,0x61		  	// Turn the speaker off
		and	al,0xfd			// ~2
		out	0x61,al

	__asm popf
	}
}

#if 0
///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCService() - Handles playing the next sample in a PC sound
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_PCService(void)
{
	byte	s;
	word	t;

	if (pcSound)
	{
		s = *pcSound++;
		if (s != pcLastSample)
		{
			pushf
			cli

			pcLastSample = s;
			if (s)					// We have a frequency!
			{
				t = pcSoundLookup[s];
				mov	bx,[t]

				mov	al,0xb6			// Write to channel 2 (speaker) timer
				out	43h,al
				mov	al,bl
				out	42h,al			// Low byte
				mov	al,bh
				out	42h,al			// High byte

				in	al,0x61			// Turn the speaker & gate on
				or	al,3
				out	0x61,al
			}
			else					// Time for some silence
			{
				in	al,0x61		  	// Turn the speaker & gate off
				and	al,0xfc			// ~3
				out	0x61,al
			}

		__asm popf
		}

		if (!(--pcLengthLeft))
		{
			SDL_PCStopSound();
			SDL_SoundFinished();
		}
	}
}
#endif

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutPC() - Turns off the pc speaker
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_ShutPC(void)
{
	__asm {
		pushf
		cli
	}

	pcSound = 0;

	__asm {
		in	al,0x61		  	// Turn the speaker & gate off
		and	al,0xfc			// ~3
		out	0x61,al

	__asm popf
	}
}

//
//	Stuff for digitized sounds
//
memptr
SDL_LoadDigiSegment(word page)
{
	memptr	addr;

#if 0	// for debugging
	mov	dx,STATUS_REGISTER_1
	in	al,dx
	mov	dx,ATR_INDEX
	mov	al,ATR_OVERSCAN
	out	dx,al
	mov	al,10	// bright green
	out	dx,al
#endif

	addr = PM_GetSoundPage(page);
	PM_SetPageLock(PMSoundStart + page,pml_Locked);

#if 0	// for debugging
	mov	dx,STATUS_REGISTER_1
	in	al,dx
	mov	dx,ATR_INDEX
	mov	al,ATR_OVERSCAN
	out	dx,al
	mov	al,3	// blue
	out	dx,al
	mov	al,0x20	// normal
	out	dx,al
#endif

	return(addr);
}

void
SDL_PlayDigiSegment(memptr addr,word len)
{
	switch (DigiMode)
	{
	case sds_PC:
    	SDL_PCPlaySample(addr,len);
		break;
	case sds_SoundSource:
		SDL_SSPlaySample(addr,len);
		break;
	case sds_SoundBlaster:
		SDL_SBPlaySample(addr,len);
		break;
	}
}

void
SD_StopDigitized(void)
{
	int	i;

	__asm {
		pushf
		cli
	}

	DigiLeft = 0;
	DigiNextAddr = nil;
	DigiNextLen = 0;
	DigiMissed = false;
	DigiPlaying = false;
	DigiNumber = DigiPriority = 0;
	SoundPositioned = false;
	if ((DigiMode == sds_PC) && (SoundMode == sdm_PC))
		SDL_SoundFinished();

	switch (DigiMode)
	{
	case sds_PC:
		SDL_PCStopSample();
		break;
	case sds_SoundSource:
		SDL_SSStopSample();
		break;
	case sds_SoundBlaster:
		SDL_SBStopSample();
		break;
	}

	__asm popf

	for (i = DigiLastStart;i < DigiLastEnd;i++)
		PM_SetPageLock(i + PMSoundStart,pml_Unlocked);
	DigiLastStart = 1;
	DigiLastEnd = 0;
}

void
SD_Poll(void)
{
	if (DigiLeft && !DigiNextAddr)
	{
		DigiNextLen = (DigiLeft >= PMPageSize)? PMPageSize : (DigiLeft % PMPageSize);
		DigiLeft -= DigiNextLen;
		if (!DigiLeft)
			DigiLastSegment = true;
		DigiNextAddr = SDL_LoadDigiSegment(DigiPage++);
	}
	if (DigiMissed && DigiNextAddr)
	{
		SDL_PlayDigiSegment(DigiNextAddr,DigiNextLen);
		DigiNextAddr = nil;
		DigiMissed = false;
		if (DigiLastSegment)
		{
			DigiPlaying = false;
			DigiLastSegment = false;
		}
	}
	SDL_SetTimerSpeed();
}

void
SD_SetPosition(int leftpos,int rightpos)
{
	if
	(
		(leftpos < 0)
	||	(leftpos > 15)
	||	(rightpos < 0)
	||	(rightpos > 15)
	||	((leftpos == 15) && (rightpos == 15))
	)
		Quit("SD_SetPosition: Illegal position");

	switch (DigiMode)
	{
	case sds_SoundBlaster:
		SDL_PositionSBP(leftpos,rightpos);
		break;
	}
}

void
SD_PlayDigitized(word which,int leftpos,int rightpos)
{
	word	len;
	memptr	addr;

	if (!DigiMode)
		return;

	SD_StopDigitized();
	if (which >= NumDigi)
		Quit("SD_PlayDigitized: bad sound number");

	SD_SetPosition(leftpos,rightpos);

	DigiPage = DigiList[(which * 2) + 0];
	DigiLeft = DigiList[(which * 2) + 1];

	DigiLastStart = DigiPage;
	DigiLastEnd = DigiPage + ((DigiLeft + (PMPageSize - 1)) / PMPageSize);

	len = (DigiLeft >= PMPageSize)? PMPageSize : (DigiLeft % PMPageSize);
	addr = SDL_LoadDigiSegment(DigiPage++);

	DigiPlaying = true;
	DigiLastSegment = false;

	SDL_PlayDigiSegment(addr,len);
	DigiLeft -= len;
	if (!DigiLeft)
		DigiLastSegment = true;

	SD_Poll();
}

void
SDL_DigitizedDone(void)
{
	if (DigiNextAddr)
	{
		SDL_PlayDigiSegment(DigiNextAddr,DigiNextLen);
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
				DigiNumber = DigiPriority = 0;
			SoundPositioned = false;
		}
		else
			DigiMissed = true;
	}
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

void
SD_SetDigiDevice(SDSMode mode)
{
	boolean	devicenotpresent;

	if (mode == DigiMode)
		return;

	SD_StopDigitized();

	devicenotpresent = false;
	switch (mode)
	{
	case sds_SoundBlaster:
		if (!SoundBlasterPresent)
		{
			if (SoundSourcePresent)
				mode = sds_SoundSource;
			else
				devicenotpresent = true;
		}
		break;
	case sds_SoundSource:
		if (!SoundSourcePresent)
			devicenotpresent = true;
		break;
	}

	if (!devicenotpresent)
	{
		if (DigiMode == sds_SoundSource)
			SDL_ShutSS();

		DigiMode = mode;

		if (mode == sds_SoundSource)
			SDL_StartSS();

		SDL_SetTimerSpeed();
	}
}

void
SDL_SetupDigi(void)
{
	memptr	list;
	word	far *p,
			pg;
	int		i;

	PM_UnlockMainMem();
	MM_GetPtr(&list,PMPageSize);
	PM_CheckMainMem();
	p = (word far *)MK_FP(PM_GetPage(ChunksInFile - 1),0);
	_fmemcpy((void far *)list,(void far *)p,PMPageSize);
	pg = PMSoundStart;
	for (i = 0;i < PMPageSize / (sizeof(word) * 2);i++,p += 2)
	{
		if (pg >= ChunksInFile - 1)
			break;
		pg += (p[1] + (PMPageSize - 1)) / PMPageSize;
	}
	PM_UnlockMainMem();
	MM_GetPtr((memptr *)&DigiList,i * sizeof(word) * 2);
	_fmemcpy((void far *)DigiList,(void far *)list,i * sizeof(word) * 2);
	MM_FreePtr(&list);
	NumDigi = i;

	for (i = 0;i < LASTSOUND;i++)
		DigiMap[i] = -1;
}

//      AdLib Code

///////////////////////////////////////////////////////////////////////////
//
//      alOut(n,b) - Puts b in AdLib card register n
//
///////////////////////////////////////////////////////////////////////////
void
alOut(byte n,byte b)
{
	__asm {
	      pushf
		cli

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

	      popf
		sti

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

#if 0
///////////////////////////////////////////////////////////////////////////
//
//	SDL_SetInstrument() - Puts an instrument into a generator
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_SetInstrument(int track,int which,Instrument far *inst,boolean percussive)
{
	byte		c,m;

	if (percussive)
	{
		c = pcarriers[which];
		m = pmodifiers[which];
	}
	else
	{
		c = carriers[which];
		m = modifiers[which];
	}

	tracks[track - 1]->inst = *inst;
	tracks[track - 1]->percussive = percussive;

	alOut(m + alChar,inst->mChar);
	alOut(m + alScale,inst->mScale);
	alOut(m + alAttack,inst->mAttack);
	alOut(m + alSus,inst->mSus);
	alOut(m + alWave,inst->mWave);

	// Most percussive instruments only use one cell
	if (c != 0xff)
	{
		alOut(c + alChar,inst->cChar);
		alOut(c + alScale,inst->cScale);
		alOut(c + alAttack,inst->cAttack);
		alOut(c + alSus,inst->cSus);
		alOut(c + alWave,inst->cWave);
	}

	alOut(which + alFeedCon,inst->nConn);	// DEBUG - I think this is right
}
#endif

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ALStopSound() - Turns off any sound effects playing through the
//		AdLib card
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_ALStopSound(void)
{
	__asm {
		pushf
		cli
	}

	/*(long)*/alSound = 0;
	alOut(alFreqH + 0,0);

	__asm popf
}

static void
SDL_AlSetFXInst(Instrument far *inst)
{
	byte		c,m;

	m = modifiers[0];
	c = carriers[0];
	alOut(m + alChar,inst->mChar);
	alOut(m + alScale,inst->mScale);
	alOut(m + alAttack,inst->mAttack);
	alOut(m + alSus,inst->mSus);
	alOut(m + alWave,inst->mWave);
	alOut(c + alChar,inst->cChar);
	alOut(c + alScale,inst->cScale);
	alOut(c + alAttack,inst->cAttack);
	alOut(c + alSus,inst->cSus);
	alOut(c + alWave,inst->cWave);

	// Note: Switch commenting on these lines for old MUSE compatibility
//	alOut(alFeedCon,inst->nConn);
	alOut(alFeedCon,0);
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ALPlaySound() - Plays the specified sound on the AdLib card
//
///////////////////////////////////////////////////////////////////////////
#ifdef	_MUSE_
void
#else
static void
#endif
SDL_ALPlaySound(AdLibSound far *sound)
{
	Instrument	far *inst;
	byte		huge *data;

	SDL_ALStopSound();

	__asm {
		pushf
		cli
	}

	alLengthLeft = sound->common.length;
	data = sound->data;
	data++;
	data--;
	alSound = (byte far *)data;
	alBlock = ((sound->block & 7) << 2) | 0x20;
	inst = &sound->inst;

	if (!(inst->mSus | inst->cSus))
	{
		__asm popf
		Quit("SDL_ALPlaySound() - Bad instrument");
	}

	SDL_AlSetFXInst(&alZeroInst);	// DEBUG
	SDL_AlSetFXInst(inst);

	__asm popf
}

#if 0
///////////////////////////////////////////////////////////////////////////
//
// 	SDL_ALSoundService() - Plays the next sample out through the AdLib card
//
///////////////////////////////////////////////////////////////////////////
//static void
void
SDL_ALSoundService(void)
{
	byte	s;

	if (alSound)
	{
		s = *alSound++;
		if (!s)
			alOut(alFreqH + 0,0);
		else
		{
			alOut(alFreqL + 0,s);
			alOut(alFreqH + 0,alBlock);
		}

		if (!(--alLengthLeft))
		{
			/*(long)*/alSound = 0;
			alOut(alFreqH + 0,0);
			SDL_SoundFinished();
		}
	}
}
#endif

#if 0
void
SDL_ALService(void)
{
	byte	a,v;
	word	w;

	if (!sqActive)
		return;

	while (sqHackLen && (sqHackTime <= alTimeCount))
	{
		w = *sqHackPtr++;
		sqHackTime = alTimeCount + *sqHackPtr++;
		mov	dx,[w]
		mov	[a],dl
		mov	[v],dh
		alOut(a,v);
		sqHackLen -= 4;
	}
	alTimeCount++;
	if (!sqHackLen)
	{
		sqHackPtr = (word far *)sqHack;
		sqHackLen = sqHackSeqLen;
		alTimeCount = sqHackTime = 0;
	}
}
#endif

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutAL() - Shuts down the AdLib card for sound effects
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_ShutAL(void)
{
	__asm {
		pushf
		cli
	}

	alOut(alEffects,0);
	alOut(alFreqH + 0,0);
	SDL_AlSetFXInst(&alZeroInst);
	alSound = 0;

	__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_CleanAL() - Totally shuts down the AdLib card
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_CleanAL(void)
{
	int	i;

	__asm {
		pushf
		cli
	}

	alOut(alEffects,0);
	for (i = 1;i < 0xf5;i++)
		alOut(i,0);

	__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_StartAL() - Starts up the AdLib card for sound effects
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_StartAL(void)
{
	alFXReg = 0;
	alOut(alEffects,alFXReg);
	SDL_AlSetFXInst(&alZeroInst);
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_DetectAdLib() - Determines if there's an AdLib (or SoundBlaster
//		emulating an AdLib) present
//
///////////////////////////////////////////////////////////////////////////
static boolean
SDL_DetectAdLib(void)
{
	byte	status1,status2;
	int		i;

	alOut(4,0x60);	// Reset T1 & T2
	alOut(4,0x80);	// Reset IRQ
	status1 = readstat();
	alOut(2,0xff);	// Set timer 1
	alOut(4,0x21);	// Start timer 1
#if 0
	SDL_Delay(TimerDelay100);
#else
	__asm {
	mov	dx,0x388
	mov	cx,100
usecloop:
	in	al,dx
	loop usecloop
	}
#endif

	status2 = readstat();
	alOut(4,0x60);
	alOut(4,0x80);

	if (((status1 & 0xe0) == 0x00) && ((status2 & 0xe0) == 0xc0))
	{
		for (i = 1;i <= 0xf5;i++)	// Zero all the registers
			alOut(i,0);

		alOut(1,0x20);	// Set WSE=1
		alOut(8,0);		// Set CSM=0 & SEL=0

		return(true);
	}
	else
		return(false);
}

#if 0
///////////////////////////////////////////////////////////////////////////
//
//	SDL_t0Service() - My timer 0 ISR which handles the different timings and
//		dispatches to whatever other routines are appropriate
//
///////////////////////////////////////////////////////////////////////////
static void interrupt
SDL_t0Service(void)
{
static	word	count = 1;

#if 1	// for debugging
	mov	dx,STATUS_REGISTER_1
	in	al,dx
	mov	dx,ATR_INDEX
	mov	al,ATR_OVERSCAN
	out	dx,al
	mov	al,4	// red
	out	dx,al
#endif

	HackCount++;

	if ((MusicMode == smm_AdLib) || (DigiMode == sds_SoundSource))
	{
		SDL_ALService();
		SDL_SSService();
//		if (!(++count & 7))
		if (!(++count % 10))
		{
			LocalTime++;
			TimeCount++;
			if (SoundUserHook)
				SoundUserHook();
		}
//		if (!(count & 3))
		if (!(count % 5))
		{
			switch (SoundMode)
			{
			case sdm_PC:
				SDL_PCService();
				break;
			case sdm_AdLib:
				SDL_ALSoundService();
				break;
			}
		}
	}
	else
	{
		if (!(++count & 1))
		{
			LocalTime++;
			TimeCount++;
			if (SoundUserHook)
				SoundUserHook();
		}
		switch (SoundMode)
		{
		case sdm_PC:
			SDL_PCService();
			break;
		case sdm_AdLib:
			SDL_ALSoundService();
			break;
		}
	}

	mov	ax,[WORD PTR TimerCount]
	add	ax,[WORD PTR TimerDivisor]
	mov	[WORD PTR TimerCount],ax
	jnc	myack
	t0OldService();			// If we overflow a word, time to call old int handler
	jmp	olddone
myack:;
	outp(0x20,0x20);	// Ack the interrupt
olddone:;

#if 1	// for debugging
	mov	dx,STATUS_REGISTER_1
	in	al,dx
	mov	dx,ATR_INDEX
	mov	al,ATR_OVERSCAN
	out	dx,al
	mov	al,3	// blue
	out	dx,al
	mov	al,0x20	// normal
	out	dx,al
#endif
}
#endif

////////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutDevice() - turns off whatever device was being used for sound fx
//
////////////////////////////////////////////////////////////////////////////
static void
SDL_ShutDevice(void)
{
	switch (SoundMode)
	{
	case sdm_PC:
		SDL_ShutPC();
		break;
	case sdm_AdLib:
		SDL_ShutAL();
		break;
	}
	SoundMode = sdm_Off;
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_CleanDevice() - totally shuts down all sound devices
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_CleanDevice(void)
{
	if ((SoundMode == sdm_AdLib) || (MusicMode == smm_AdLib))
		SDL_CleanAL();
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_StartDevice() - turns on whatever device is to be used for sound fx
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_StartDevice(void)
{
	switch (SoundMode)
	{
	case sdm_AdLib:
		SDL_StartAL();
		break;
	}
	SoundNumber = SoundPriority = 0;
}

//	Public routines

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetSoundMode() - Sets which sound hardware to use for sound effects
//
///////////////////////////////////////////////////////////////////////////
boolean
SD_SetSoundMode(SDMode mode)
{
	boolean	result = false;
	word	tableoffset;

	SD_StopSound();

#ifndef	_MUSE_
	if ((mode == sdm_AdLib) && !AdLibPresent)
		mode = sdm_PC;

	switch (mode)
	{
	case sdm_Off:
		NeedsDigitized = false;
		result = true;
		break;
	case sdm_PC:
		tableoffset = STARTPCSOUNDS;
		NeedsDigitized = false;
		result = true;
		break;
	case sdm_AdLib:
		if (AdLibPresent)
		{
			tableoffset = STARTADLIBSOUNDS;
			NeedsDigitized = false;
			result = true;
		}
		break;
	}
#else
	result = true;
#endif

	if (result && (mode != SoundMode))
	{
		SDL_ShutDevice();
		SoundMode = mode;
#ifndef	_MUSE_
		SoundTable = (word *)(&audiosegs[tableoffset]);
#endif
		SDL_StartDevice();
	}

	SDL_SetTimerSpeed();

	return(result);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetMusicMode() - sets the device to use for background music
//
///////////////////////////////////////////////////////////////////////////
boolean
SD_SetMusicMode(SMMode mode)
{
	boolean	result = false;

	SD_FadeOutMusic();
	while (SD_MusicPlaying())
		;

	switch (mode)
	{
	case smm_Off:
		NeedsMusic = false;
		result = true;
		break;
	case smm_AdLib:
		if (AdLibPresent)
		{
			NeedsMusic = true;
			result = true;
		}
		break;
	}

	if (result)
		MusicMode = mode;

	SDL_SetTimerSpeed();

	return(result);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_Startup() - starts up the Sound Mgr
//		Detects all additional sound hardware and installs my ISR
//
///////////////////////////////////////////////////////////////////////////
void
SD_Startup(void)
{
	int	i;

	if (SD_Started)
		return;

	////SDL_SetDS();

	ssIsTandy = false;
	ssNoCheck = false;
	alNoCheck = false;
	sbNoCheck = false;
	sbNoProCheck = false;
#ifndef	_MUSE_
	for (i = 1;i < _argc;i++)
	{
		switch (US_CheckParm(_argv[i],ParmStringssd))
		{
		case 0:						// No AdLib detection
			alNoCheck = true;
			break;
		case 1:						// No SoundBlaster detection
			sbNoCheck = true;
			break;
		case 2:						// No SoundBlaster Pro detection
			sbNoProCheck = true;
			break;
		case 3:
			ssNoCheck = true;		// No Sound Source detection
			break;
		case 4:						// Tandy Sound Source handling
			ssIsTandy = true;
			break;
		case 5:						// Sound Source present at LPT1
			ssPort = 1;
			ssNoCheck = SoundSourcePresent = true;
			break;
		case 6:		     // Sound Source present at LPT2
			ssPort = 2;
			ssNoCheck = SoundSourcePresent = true;
			break;
		case 7:		     // Sound Source present at LPT3
			ssPort = 3;
			ssNoCheck = SoundSourcePresent = true;
			break;
		}
	}
#endif

	SoundUserHook = 0;

	t0OldService = _dos_getvect(8);	// Get old timer 0 ISR

	LocalTime = TimeCount = alTimeCount = 0;

	SD_SetSoundMode(sdm_Off);
	SD_SetMusicMode(smm_Off);

	if (!ssNoCheck)
		SoundSourcePresent = SDL_DetectSoundSource();

	if (!alNoCheck)
	{
		AdLibPresent = SDL_DetectAdLib();
		if (AdLibPresent && !sbNoCheck)
		{
			int port = -1;
			char *env = getenv("BLASTER");
			if (env)
			{
				long temp;
				while (*env)
				{
					while (isspace(*env))
						env++;

					switch (toupper(*env))
					{
					case 'A':
						temp = strtol(env + 1,&env,16);
						if
						(
							(temp >= 0x210)
						&&	(temp <= 0x260)
						&&	(!(temp & 0x00f))
						)
							port = (temp - 0x200) >> 4;
						else
							Quit("SD_Startup: Unsupported address value in BLASTER");
						break;
					case 'I':
						temp = strtol(env + 1,&env,10);
						if
						(
							(temp >= 0)
						&&	(temp <= 10)
						&&	(sbIntVectors[temp] != -1)
						)
						{
							sbInterrupt = temp;
							sbIntVec = sbIntVectors[sbInterrupt];
						}
						else
							Quit("SD_Startup: Unsupported interrupt value in BLASTER");
						break;
					case 'D':
						temp = strtol(env + 1,&env,10);
						if ((temp == 0) || (temp == 1) || (temp == 3))
							SDL_SBSetDMA(temp);
						else
							Quit("SD_Startup: Unsupported DMA value in BLASTER");
						break;
					default:
						while (isspace(*env))
							env++;
						while (*env && !isspace(*env))
							env++;
						break;
					}
				}
			}
			SoundBlasterPresent = SDL_DetectSoundBlaster(port);
		}
	}

	for (i = 0;i < 255;i++)
		pcSoundLookup[i] = i * 60;

	if (SoundBlasterPresent)
		SDL_StartSB();

	SDL_SetupDigi();

	SD_Started = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_Default() - Sets up the default behaviour for the Sound Mgr whether
//		the config file was present or not.
//
///////////////////////////////////////////////////////////////////////////
void
SD_Default(boolean gotit,SDMode sd,SMMode sm)
{
	boolean	gotsd,gotsm;

	gotsd = gotsm = gotit;

	if (gotsd)	// Make sure requested sound hardware is available
	{
		switch (sd)
		{
		case sdm_AdLib:
			gotsd = AdLibPresent;
			break;
		}
	}
	if (!gotsd)
	{
		if (AdLibPresent)
			sd = sdm_AdLib;
		else
			sd = sdm_PC;
	}
	if (sd != SoundMode)
		SD_SetSoundMode(sd);


	if (gotsm)	// Make sure requested music hardware is available
	{
		switch (sm)
		{
		case sdm_AdLib:
			gotsm = AdLibPresent;
			break;
		}
	}
	if (!gotsm)
	{
		if (AdLibPresent)
			sm = smm_AdLib;
	}
	if (sm != MusicMode)
		SD_SetMusicMode(sm);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_Shutdown() - shuts down the Sound Mgr
//		Removes sound ISR and turns off whatever sound hardware was active
//
///////////////////////////////////////////////////////////////////////////
void
SD_Shutdown(void)
{
	if (!SD_Started)
		return;

	SD_MusicOff();
	SD_StopSound();
	SDL_ShutDevice();
	SDL_CleanDevice();

	if (SoundBlasterPresent)
		SDL_ShutSB();

	if (SoundSourcePresent)
		SDL_ShutSS();

	__asm {
		pushf
		cli
	}

	SDL_SetTimer0(0);

	_dos_setvect(8,t0OldService);

		__asm popf

	SD_Started = false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetUserHook() - sets the routine that the Sound Mgr calls every 1/70th
//		of a second from its timer 0 ISR
//
///////////////////////////////////////////////////////////////////////////
void
SD_SetUserHook(void (* hook)(void))
{
	SoundUserHook = hook;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_PositionSound() - Sets up a stereo imaging location for the next
//		sound to be played. Each channel ranges from 0 to 15.
//
///////////////////////////////////////////////////////////////////////////
void
SD_PositionSound(int leftvol,int rightvol)
{
	LeftPosition = leftvol;
	RightPosition = rightvol;
	nextsoundpos = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_PlaySound() - plays the specified sound on the appropriate hardware
//
///////////////////////////////////////////////////////////////////////////
boolean
SD_PlaySound(soundnames sound)
{
	boolean		ispos;
	SoundCommon	far *s;
	int	lp,rp;

	lp = LeftPosition;
	rp = RightPosition;
	LeftPosition = 0;
	RightPosition = 0;

	ispos = nextsoundpos;
	nextsoundpos = false;

	if (sound == -1)
		return(false);

	s = MK_FP(SoundTable[sound],0);
	if ((SoundMode != sdm_Off) && !s)
		Quit("SD_PlaySound() - Uncached sound");

	if ((DigiMode != sds_Off) && (DigiMap[sound] != -1))
	{
		if ((DigiMode == sds_PC) && (SoundMode == sdm_PC))
		{
			if (s->priority < SoundPriority)
				return(false);

			SDL_PCStopSound();

			SD_PlayDigitized(DigiMap[sound],lp,rp);
			SoundPositioned = ispos;
			SoundNumber = sound;
			SoundPriority = s->priority;
		}
		else
		{
		__asm {
			pushf
			cli
		}
			if (DigiPriority && !DigiNumber)
			{
			__asm popf
				Quit("SD_PlaySound: Priority without a sound");
			}
		__asm popf

			if (s->priority < DigiPriority)
				return(false);

			SD_PlayDigitized(DigiMap[sound],lp,rp);
			SoundPositioned = ispos;
			DigiNumber = sound;
			DigiPriority = s->priority;
		}

		return(true);
	}

	if (SoundMode == sdm_Off)
		return(false);
	if (!s->length)
		Quit("SD_PlaySound() - Zero length sound");
	if (s->priority < SoundPriority)
		return(false);

	switch (SoundMode)
	{
	case sdm_PC:
		SDL_PCPlaySound((void far *)s);
		break;
	case sdm_AdLib:
		SDL_ALPlaySound((void far *)s);
		break;
	}

	SoundNumber = sound;
	SoundPriority = s->priority;

	return(false);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SoundPlaying() - returns the sound number that's playing, or 0 if
//		no sound is playing
//
///////////////////////////////////////////////////////////////////////////
word
SD_SoundPlaying(void)
{
	boolean	result = false;

	switch (SoundMode)
	{
	case sdm_PC:
		result = pcSound? true : false;
		break;
	case sdm_AdLib:
		result = alSound? true : false;
		break;
	}

	if (result)
		return(SoundNumber);
	else
		return(false);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StopSound() - if a sound is playing, stops it
//
///////////////////////////////////////////////////////////////////////////
void
SD_StopSound(void)
{
	if (DigiPlaying)
		SD_StopDigitized();

	switch (SoundMode)
	{
	case sdm_PC:
		SDL_PCStopSound();
		break;
	case sdm_AdLib:
		SDL_ALStopSound();
		break;
	}

	SoundPositioned = false;

	SDL_SoundFinished();
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_WaitSoundDone() - waits until the current sound is done playing
//
///////////////////////////////////////////////////////////////////////////
void
SD_WaitSoundDone(void)
{
	while (SD_SoundPlaying())
		;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOn() - turns on the sequencer
//
///////////////////////////////////////////////////////////////////////////
void
SD_MusicOn(void)
{
	sqActive = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOff() - turns off the sequencer and any playing notes
//
///////////////////////////////////////////////////////////////////////////
int
SD_MusicOff(void)
{
	word	i;


	switch (MusicMode)
	{
	case smm_AdLib:
		alFXReg = 0;
		alOut(alEffects,0);
		for (i = 0;i < sqMaxTracks;i++)
			alOut(alFreqH + i + 1,0);
		break;
	}
	sqActive = false;
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StartMusic() - starts playing the music pointed to
//
///////////////////////////////////////////////////////////////////////////
void
SD_StartMusic(MusicGroup far *music)
{
	SD_MusicOff();
	__asm {
		pushf
		cli
	}

	if (MusicMode == smm_AdLib)
	{
		sqHackPtr = sqHack = music->values;
		sqHackSeqLen = sqHackLen = music->length;
		sqHackTime = 0;
		alTimeCount = 0;
		SD_MusicOn();
	}

	__asm popf
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_FadeOutMusic() - starts fading out the music. Call SD_MusicPlaying()
//		to see if the fadeout is complete
//
///////////////////////////////////////////////////////////////////////////
void
SD_FadeOutMusic(void)
{
	switch (MusicMode)
	{
	case smm_AdLib:
		// DEBUG - quick hack to turn the music off
		SD_MusicOff();
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicPlaying() - returns true if music is currently playing, false if
//		not
//
///////////////////////////////////////////////////////////////////////////
boolean
SD_MusicPlaying(void)
{
	boolean	result;

	switch (MusicMode)
	{
	case smm_AdLib:
		result = false;
		// DEBUG - not written
		break;
	default:
		result = false;
	}

	return(result);
}
