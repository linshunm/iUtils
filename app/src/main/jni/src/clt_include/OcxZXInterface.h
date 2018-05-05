//集中定义回调等
#ifndef __OcxZXInterface_
#define __OcxZXInterface_

#ifdef _WIN32
    //
#elif defined ANDROID_RCS
    #include <pthread.h>
#else
    #include <objc/objc.h>
	#include <pthread.h>
#endif

typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned short      WORD16;
#ifndef WORD32
	typedef unsigned int    WORD32;
#endif

#ifdef _WIN32
	typedef __int64 INT64;
	typedef CRITICAL_SECTION MUTEX_T;
#else
	typedef pthread_mutex_t MUTEX_T;

	#ifndef far
		#define far
	#endif
	
    #ifdef ANDROID_RCS
        typedef unsigned char BOOL;
    #endif
	typedef const char* LPCTSTR;
	typedef unsigned int DWORD;
	typedef signed  int INT32;
#endif

#define R01_SUCCESS         (UINT8)0
#define R01_PNO_ERROR       (UINT8)3
#define R01_OTHER_ERROR     (UINT8)0xFF

#define R01                        1000
#define TIMER1                     (UINT8)0
#define TIMER2                     (UINT8)1
#define TIMER3                     (UINT8)2
#define TIMER4                     (UINT8)3
#define TIMER5                     (UINT8)4
#define TIMER6                     (UINT8)5
#define TIMER7                     (UINT8)6
#define TIMER8                     (UINT8)7
#define TIMER9                     (UINT8)8

/****** R01 event define *****/
#define Timer1Event             (UINT16)(R01 + 0)
#define Timer2Event             (UINT16)(R01 + 1)
#define Timer3Event             (UINT16)(R01 + 2)
#define Timer4Event             (UINT16)(R01 + 3)
#define Timer5Event             (UINT16)(R01 + 4)
#define Timer6Event             (UINT16)(R01 + 5)
#define Timer7Event             (UINT16)(R01 + 6)
#define Timer8Event             (UINT16)(R01 + 7)
#define Timer9Event             (UINT16)(R01 + 8)

#define MpMasterPowerOnEvent    (UINT16)(R01 + 10)
#define InitAllProcessEvent     (UINT16)MpMasterPowerOnEvent
#define MpMasterToSlaveEvent    (UINT16)(R01 + 12)
#define SetConfigEvent          (UINT16)(R01 + 22)
#define LoadDynaConfigEvent     (UINT16)(R01 + 23)
#define GetServiceInfo          (UINT16)(R01 + 30)

#define QuitAllProcessEvent     (UINT16)(60002) 

typedef void far (* LPENTRY)(UINT8 far *, UINT8 far *, UINT8 far *);
typedef void* (LPHANDLE_FUNC)(void*);


#endif
