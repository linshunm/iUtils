//
//  LoginAgentProc.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include "LoginAgentProc.hpp"
#include "LoginAgentProcTransModel.hpp"

#include "../Foundation/ThreadManager/USSDK_ThreadManager.h"
#include "../LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"

#include "LoginService/Service/USSDK_LoginService.h"

using namespace NS_USSDK_LogService;
using namespace NS_USSDK_LoginService;

namespace NS_USSDK_LoginService
{
	USSDK_LoginService *CorrespondService2()
	{
		return USSDK_LoginService::shareInstance();
	}

	USSDK_MessageQueue<SingleServerInfo> g_setServerInfo;
	USSDK_MessageQueue<CurrentLoginInfo> g_loginInfo;

	void far loginAgentProc(UINT8 far * lpMsg, UINT8 far * lpNULL0, UINT8 far * lpNULL1)
	{
		unsigned short event, msgLen;
		event  = bCtl_ZXGetEvent();
		msgLen = bCtl_ZXGetLength();

		switch(event)
		{
		case InitAllProcessEvent:
			LOG_DEBUGGING(loginAgentName, "loginAgentProc start success");
			break;
		case QuitAllProcessEvent:
			{
				LOG_DEBUGGING(loginAgentName, "loginAgentProc stop success");
				break;
			}
		case Event_LoginAdapterReq:
			{
				LOG_DEBUGGING(loginAgentName, "receive Event_LoginAdapterReq");
				dealLoginAdapterReq(lpMsg);
				break;
			}
		default:
			{
				LOG_ERROR(loginAgentName, "loginAgentProc unknow event %d from %d", event, bCtl_ZXGetSenderPno());
				break;
			}
		}
	}

	//LoginAgentEventReq
	void dealLoginAdapterReq(void *aReq)
	{
		LoginAgentEventReq *req = (LoginAgentEventReq *)aReq;

		LOG_DEBUGGING(loginAgentName, "receive subEvent[%d]", req->aSubEventType);

		switch(req->aSubEventType)
		{
		case SubEvent_setServerInfo:
			{
				SingleServerInfo aPara =  g_setServerInfo.get();
				CorrespondService2()->setServerInfo(&aPara);
				break;
			}
		case SubEvent_login:
			{
				CurrentLoginInfo aPara = g_loginInfo.get();
				CorrespondService2()->login(&aPara);
				break;
			}
		case SubEvent_cancelLogin:
			{
				CorrespondService2()->cancelLogin();
				break;
			}
		case SubEvent_reloginLogin:
			{
				CorrespondService2()->reLogin();
				break;
			}
		case SubEvent_Logout:
			{
				CorrespondService2()->logout();
				break;
			}
		case SubEvent_onLoginResult:
			{
				LoginResult *aPara = &(req->argument.aLoginResult);
				CorrespondService2()->onLoginResult(aPara->success, aPara->resultCode, aPara->extendResultCode);
				break;
			}
		case SubEvent_onLogoutResult:
			{
				LogoutResult *aPara = &(req->argument.aLogoutResult);
				CorrespondService2()->onLogoutResult(aPara->lReason);
				break;
			}
		case SubEvent_onServerLinkLost:
			{
				CorrespondService2()->onServerLinkLost();
				break;
			}
		default:
			{
				LOG_ERROR(loginAgentName,"unknow subEventType[%d]", req->aSubEventType);
				break;
			}
		}
	}

	void loginAgentThreadInit()
	{
		USSDK_ThreadManager *aManager = USSDK_ThreadManager::shareInstance();
		aManager->startOneAgent(loginAgentName, loginAgentProc);
	}

	void loginAgentThreadUnInit()
	{
		USSDK_ThreadManager *aManager = USSDK_ThreadManager::shareInstance();
		aManager->stopOneAgent(loginAgentName);
	}
}


