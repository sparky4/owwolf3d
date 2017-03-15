#ifndef __ID_TAIL_H__
#define __ID_TAIL_H__

extern word bakapee;
#define PTOK
#define PTDT
#define VIDEO_NORUN

#ifdef __WATCOMC__
//#define NOSAFERUN
#define NOSAFERUNCHKFOREP
#define SD_NORUN
//#define CA_NORUN
#define CA_NORUNMISC

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

#endif
