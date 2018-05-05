//集中定义提供给包容器的接口定义等等
#ifndef __ZXCTLMETHOD_
#define __ZXCTLMETHOD_

#include "OcxZXInterface.h"

#ifdef _WIN32
	#ifdef SOFTDACTRL_EXPORTS
		#define SOFTDACTRL_API __declspec(dllexport)
	#else
		#define SOFTDACTRL_API __declspec(dllimport)
	#endif
#else
	#define SOFTDACTRL_API   
#endif

#ifdef __cplusplus
extern "C"
{
#endif

//ZXOS公共函数
SOFTDACTRL_API BOOL bCtl_ZXStartAgent(UINT16 pno, LPCTSTR name, LPENTRY lpEntry);
SOFTDACTRL_API BOOL bCtl_ZXStopAgent(UINT16 pno);
SOFTDACTRL_API BOOL bCtl_ZXSendToAgent(UINT16 Event, UINT8 * In, UINT16 InLen, UINT16 DstPno, UINT16 CurPno);
SOFTDACTRL_API UINT16 bCtl_ZXGetCurPno();
SOFTDACTRL_API UINT16 bCtl_ZXGetEvent();
SOFTDACTRL_API UINT16 bCtl_ZXGetLength();
SOFTDACTRL_API UINT16 bCtl_ZXGetSenderPno();

//ZXOS线程相关函数
SOFTDACTRL_API BOOL bCtl_ZXCREATE_THREAD(LPHANDLE_FUNC startaddr, void* parg, UINT32 stack, DWORD * dwThread, UINT32 * hThread);
SOFTDACTRL_API UINT8 bCtl_ZXTERMINATE_THREAD(UINT32 hThread, UINT32 exitcode);
SOFTDACTRL_API void bCtl_ZXEXIT_THREAD(UINT32 exitcode);
SOFTDACTRL_API UINT8 bCtl_ZXCLOSE_HANDLE(UINT32 * hThread);

SOFTDACTRL_API void bCtl_ZXMUTEX_INIT(MUTEX_T * mptr);
SOFTDACTRL_API void bCtl_ZXMUTEX_FREE(MUTEX_T * mptr);
SOFTDACTRL_API void bCtl_ZXMUTEX_LOCK(MUTEX_T * mptr);
SOFTDACTRL_API void bCtl_ZXMUTEX_UNLOCK(MUTEX_T * mptr);

// 定时器相关函数
SOFTDACTRL_API UINT16 bCtl_ZXSET_TIMER(UINT32 Count100ms, UINT8 TimerNo);
SOFTDACTRL_API UINT8 bCtl_ZXKILL_TIMER(UINT8 TimerNo);
SOFTDACTRL_API UINT16 bCtl_ZXEXTRA_SET_TIMER(UINT32 Count100ms, UINT8 TimerNo, UINT32 Extra);
SOFTDACTRL_API UINT8 bCtl_ZXEXTRA_KILL_TIMER(UINT16 tid);
SOFTDACTRL_API UINT16 bCtl_ZXSET_LOOP_TIMER(UINT32 Count100ms, UINT8 TimerNo);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif
