#ifndef __TYPE_H__
#define __TYPE_H__

#define VERSION __DATE__ " " __TIME__
#define _FCORELEFT 0x90000UL+16UL
static union REGS CPURegs;

#define _AX CPURegs.x.ax
#define _BX CPURegs.x.bx
#define _CX CPURegs.x.cx
#define _DX CPURegs.x.dx

#define _SI CPURegs.x.si
#define _DI CPURegs.x.di

#define _AH CPURegs.h.ah
#define _AL CPURegs.h.al
#define _BH CPURegs.h.bh
#define _BL CPURegs.h.bl
#define _CH CPURegs.h.ch
#define _CL CPURegs.h.cl
#define _DH CPURegs.h.dh
#define _DL CPURegs.h.dl

#define geninterrupt(n) int86(n,&CPURegs,&CPURegs);
#define peekb(segm,ofs) (*(byte far*)MK_FP((segm),(ofs)))
#define peekw(segm,ofs) (*(word far*)MK_FP((segm),(ofs)))
#define pokeb(segm,ofs,value) (peekb((segm),(ofs)) = (byte)(value))
#define pokew(segm,ofs,value) (peekw((segm),(ofs)) = (word)(value))

typedef union REGPACK	regs_t;

typedef	enum	{false,true}	boolean;
typedef	unsigned	char		byte;
typedef	unsigned	int			word;
typedef	unsigned	long		longword;
typedef	byte *					Ptr;

typedef	struct
		{
			int	x,y;
		} Point;
typedef	struct
		{
			Point	ul,lr;
		} Rect;

typedef void __based( void ) * memptr;	////old //----typedef void __based(__self) * memptr;
#define _seg
typedef long fixed;

#define _argv __argv
#define _argc __argc
#endif
