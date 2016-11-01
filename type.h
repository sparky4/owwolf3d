#ifndef __TYPE_H__
#define __TYPE_H__
#include <conio.h>

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

#define inportb(port)         inp(port)
#define inportw(port)         inpw(port)
#define outportb(port, val)   outp(port, val)
#define outportw(port, val)   outpw(port, val)
#define asm __asm
#define getvect _dos_getvect
#define setvect _dos_setvect

#define ffblk        find_t
#define ff_name      name
#define ff_attrib    attrib
#define ff_fsize     size
#define ff_ftime     wr_time
#define ff_fdate     wr_date

#define findfirst(name, dta, attrib)   _dos_findfirst(name, attrib, dta)
#define findnext(dta)                  _dos_findnext(dta)

#define random()     rand()
#define srandom(n)   srand(n)

#define _dos_ds      _default_ds()

typedef union REGPACK	regs_t;

typedef	enum	{false,true}	boolean;
typedef	unsigned	char		byte;
typedef	unsigned	int		word;
typedef	signed	short		sword;
typedef	unsigned	long		dword; typedef dword longword;
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
#define _seg __based( void )
typedef long fixed;

#define _argv __argv
#define _argc __argc
#endif
