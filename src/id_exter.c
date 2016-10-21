#include "src/id_exter.h"

boolean		NoWait;
//longword			TimeCount;

boolean	insetupscaling;

byte		far	palette1[256][3],far palette2[256][3];
unsigned	bufferofs;
unsigned	displayofs,pelpan;

pictabletype *pictable;

///////////////////////////////////////////////////////////////////////////
//
//	US_CheckParm() - checks to see if a string matches one of a set of
//		strings. The check is case insensitive. The routine returns the
//		index of the string that matched, or -1 if no matches were found
//
///////////////////////////////////////////////////////////////////////////
int
US_CheckParm(char *parm,char **strings)
{
	char	cp,cs,
			*p,*s;
	int		i;

	while (!isalpha(*parm))	// Skip non-alphas
		parm++;

	for (i = 0;*strings && **strings;i++)
	{
		for (s = *strings++,p = parm,cs = cp = 0;cs == cp;)
		{
			cs = *s++;
			if (!cs)
				return(i);
			cp = *p++;

			if (isupper(cs))
				cs = tolower(cs);
			if (isupper(cp))
				cp = tolower(cp);
		}
	}
	return(-1);
}

void VL_Plot (int x, int y, int color){}
void VL_Hlin (unsigned x, unsigned y, unsigned width, unsigned color){}
void VL_Vlin (int x, int y, int height, int color){}
void VL_FadeIn (int start, int end, byte far *palette, int steps){}
int VW_MarkUpdateBlock (int x1, int y1, int x2, int y2){ return 0; }
