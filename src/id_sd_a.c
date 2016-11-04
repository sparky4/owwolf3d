#include "id_sd_a.h"

boolean		alNoIRQ;

int count_time=0;
int count_fx=0;
int extreme=0;
volatile boolean pcindicate;
volatile boolean MyDS;

static  boolean                 DigiMissed,DigiLastSegment;
static  memptr                  DigiNextAddr;
static  word                    DigiNextLen;

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
void interrupt SDL_t0FastAsmService()
{
	SDL_DoFast();
}

// Timer 0 ISR for 140Hz interrupts
void interrupt SDL_t0SlowAsmService()
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
