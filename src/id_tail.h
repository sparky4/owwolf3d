#ifndef __ID_TAIL_H__
#define __ID_TAIL_H__

extern unsigned bakapee;
#define PTOK
#define PTDT

//#define VIDEO_NORUN
//#define __DEBUG_MM__

#define MEMPTRCONV			(memptr *)&

//bcc
#ifdef __BORLANDC__
#define MEMPTRANDPERCONV	&
#define BYTEFARPTRCONV		(unsigned char far *)
//#define SMDPTRANDPERCONV	&
#define SDFPTRANDPERCONV	&
//#define OBTPTRANDPERCONV	&
#define SDTPTRANDPERCONV	&
#endif
//

//wcc
#ifdef __WATCOMC__
#define MEMPTRANDPERCONV
#define BYTEFARPTRCONV
//#define SMDPTRANDPERCONV	(void *)&
#define SDFPTRANDPERCONV	(struct diskfree_t *)&
//#define OBTPTRANDPERCONV	(void const *)&
#define SDTPTRANDPERCONV	(struct dostime_t *)&

//#endif

//
// enabled means do NOT run
//
#define VIDEO_NORUN
#define NOSAFERUN
#define NOSAFERUNCHKFOREP
#define NOSAFERUNDEMOLOOP
#define SD_NORUN
//#define CA_NORUN
//#define CA_NORUNMISC

#ifdef PTOK
#undef PTOK
#endif
#ifdef PTDT
#undef PTDT
#endif

#define PTOK printf("%u ok\n", bakapee++);
//#define PTOK printf("ok\n");
#define PTDT printf(".");
#endif
//

#endif
