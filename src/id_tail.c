#include "src/id_tail.h"

/*
==================
=
= DebugMemory
=
==================
*/

void DebugMemory_(boolean q)
{
	/*VW_FixRefreshBuffer ();
	US_CenterWindow (16,7);

	US_CPrint ("Memory Usage");
	US_CPrint ("------------");
	US_Print ("Total     :");
	US_PrintUnsigned (mminfo.mainmem/1024);
	US_Print ("k\nFree      :");
	US_PrintUnsigned (MM_UnusedMemory()/1024);
	US_Print ("k\nWith purge:");
	US_PrintUnsigned (MM_TotalFree()/1024);
	US_Print ("k\n");
	VW_UpdateScreen();*/
	if(q){
	printf("========================================\n");
	printf("		DebugMemory_\n");
	printf("========================================\n");}
	if(q) { printf("Memory Usage\n");
	printf("------------\n"); }else printf("	%c%c", 0xD3, 0xC4);
	printf("Total:	"); if(q) printf("	"); printf("%uk", mminfo.mainmem/1024);
	if(q) printf("\n"); else printf("	");
	printf("Free:	"); if(q) printf("	"); printf("%uk", MM_UnusedMemory()/1024);
	if(q) printf("\n"); else printf("	");
	printf("With purge:"); if(q) printf("	"); printf("%uk\n", MM_TotalFree()/1024);
	if(q) printf("------------\n");
	IN_Ack ();
	if(q) MM_ShowMemory ();
}
