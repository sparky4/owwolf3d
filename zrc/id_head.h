// ID_HEAD.H


#define	EXTENSION	"WLF"
#define WOLF

#define	TEXTGR	0
#define	CGAGR	1
#define	EGAGR	2
#define	VGAGR	3

#define GRMODE	VGAGR

#include "src/version.h"
#include "src/id_heads.h"
#ifndef __TYPE_H__
typedef	struct
		{
			int	x,y;
		} Point;

typedef	struct
		{
			Point	ul,lr;
		} Rect;
#endif

void	Quit (char *error);		// defined in user program

