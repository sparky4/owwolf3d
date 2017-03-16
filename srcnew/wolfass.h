#ifndef __WOLFASS_H__
#define __WOLFASS_H__
#include "type.h"
//#include "id_sd.h"

#define	SDL_SoundFinished()	{SoundNumber = SoundPriority = 0;}
//void SetScreen (unsigned offset);
//#define VL_SetScreen(crtc,pan) SetScreen(crtc)

extern volatile byte	pcLastSample,far *pcSound;
extern longword		pcLengthLeft;
extern soundnames		SoundNumber,DigiNumber;
extern word			SoundPriority,DigiPriority;
extern byte			far *alSound;
extern word			alBlock;
extern longword		alLengthLeft;
extern boolean			sqActive;
extern word			far *sqHack,far *sqHackPtr,sqHackLen,sqHackSeqLen;
extern long			sqHackTime;
extern longword		alTimeCount;
extern volatile longword	ssLengthLeft;
extern word				ssControl,ssStatus,ssData;
extern byte				ssOn,ssOff;
extern longword		TimerDivisor,TimerCount;
extern volatile byte		far *ssSample;

extern void interrupt	(*t0OldService)(void);
extern void SDL_PlayDigiSegment(memptr addr,word len);

void alOutInIRQ(byte n,byte b);
void SDL_DigitizedDoneInIRQ(void);
void SDL_turnOnPCSpeaker(word timerval);
void SDL_DigitizedDoneInIRQ(void);

void interrupt	SDL_t0ExtremeAsmService(void);
void interrupt	SDL_t0FastAsmService(void);
void interrupt	SDL_t0SlowAsmService(void);
int	VL_VideoID (void),
	US_RndT(void);
void	SDL_SetDS(void),
	SDL_IndicatePC(boolean on),
	VL_WaitVBL (int vbls),
	VH_UpdateScreen(),
	US_InitRndT(unsigned randomize),
	VL_ScreenToScreen (unsigned source, unsigned dest,int width, int height);

/*#pragma aux VL_SetScreen = \
		"cli" \
		"mov	dx,0x3d4" \
		"mov	al,0x0c" \
		"out	dx,al" \
		"inc	dx" \
		"mov	al,ah" \
		"out	dx,al" \
		"sti" \
		parm [ax] \
		modify exact [dx ax]*/
#endif
