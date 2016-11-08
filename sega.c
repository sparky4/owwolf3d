#include <stdio.h>
#ifdef __WATCOMC__
//#include "type.h"
#define _seg __based( void )
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

void main()
{
	int _seg *x;
	int value;

	//printf("%Fp:	%d\n", x, value);
	x = (int _seg *)0x40;
	value = *(x + 20);
	printf("%Fp:	*x\n", *x);
	printf("%Fp:	%d\n", x, value);
	printf("0x40:0x28\n");
}
