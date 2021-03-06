#include <stdio.h>
#ifdef __WATCOMC__
//#include "type.h"
//#define _seg __segment// __based( void )
#define _SEGA __segment
#else
#define _SEGA int _seg
#endif

/*
 Q. What does the _seg modifier do?
 A. Using _seg causes a pointer to become a storage place for a
    segment value, rather than an offset ( or a segment/offset ).
    For instance, if "int _seg *x" contains the value 0x40,
    then when you use "*x", the value pointed to will be at
    segment 0x40, offset 0. If you add a value to the pointer,
    the value is multiplied by the size of the pointer type. That
    new value is used as an offset, and is combined with the segment
    value contained in the pointer. For instance,

       int _seg *x;
       int value;

       x = (int _seg *)0x40;
       value = *(x + 20);

    value is assigned the value of the integer at 0x40:0x28
    (Remember, 20 * sizeof(int) = 40 = 0x28).

    You can find a more detailed description of _seg in the Borland C++
    Programmer's Guide, Chapter 9.
*/
/*
	__segment screen;
	char __based( void ) * scrptr;

	screen = 0xB800;
	scrptr = 0;
	printf( "Top left character is '%c'.\n", *(screen:>scrptr) );
*/
//_seg *x;

void main()
{
	_SEGA *x;
#ifdef __WATCOMC__
	__segment y;
#endif
	int value;

	//printf("%Fp:	%d\n", x, value);
	x = (_SEGA *)0x40;
	value = *(x + 20);
#ifdef __WATCOMC__
	//printf("%Fp:	*y\n", *y);
	printf("%Fp:	y\n", &y);
#endif
	printf("%Fp:	*x\n", *(x));
	printf("%Fp:	x =%d\n", x, value);
	printf("0x40:0x28\n");
}
