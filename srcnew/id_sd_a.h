#ifndef	__ID_SD_A__
#define	__ID_SD_A__
#ifndef __ID_GLOB__
#include "id_heads.h"
#endif

#define	SDL_SoundFinished()	{SoundNumber = SoundPriority = 0;}

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

void SDL_SetDS();
void alOutInIRQ(byte n,byte b);
void SDL_DigitizedDoneInIRQ(void);
void SDL_turnOnPCSpeaker(word timerval);
void //SDL_SetDS(void),
						SDL_IndicatePC(boolean on),
						SDL_DigitizedDoneInIRQ(void);
//void interrupt	SDL_t0ExtremeAsmService(),		SDL_t0FastAsmService(),	SDL_t0SlowAsmService();
#endif
