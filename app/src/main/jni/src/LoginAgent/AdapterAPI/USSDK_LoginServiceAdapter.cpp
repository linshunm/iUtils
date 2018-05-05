//
//  USSDK_LoginServiceAdapter.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include "USSDK_LoginServiceAdapter.h"

//temp TODO:
#include "../LoginService/Service/USSDK_LoginService.h"
#include "../../Foundation/ThreadManager/USSDK_ThreadManager.h"
#include "../LoginAgentProcTransModel.hpp"

using namespace NS_USSDK_LoginService;

namespace NS_USSDK_LoginService
{
	USSDK_LoginService *CorrespondService()
	{
		return USSDK_LoginService::shareInstance();
	}
}

void USSDK_LoginServiceAdapter::addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID)
{
	CorrespondService()->addObserver(observer, anEventName, callBackThreadID);
}

void USSDK_LoginServiceAdapter::addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID)
{
	CorrespondService()->addObserver(observer, callBackThreadID);
}

void USSDK_LoginServiceAdapter::removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName)
{
	CorrespondService()->removeObserver(observer, anEventName);
}

void USSDK_LoginServiceAdapter::removeObserver(USSDK_UNotifyMethodClass * observer)
{
	CorrespondService()->removeObserver(observer);
}

bool USSDK_LoginServiceAdapter::setServerInfo(SingleServerInfo* aSingleServerInfo)
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_setServerInfo;

	g_setServerInfo.add(aSingleServerInfo);

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::login(CurrentLoginInfo *aLoginInfo)
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_login;

	g_loginInfo.add(aLoginInfo);

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::cancelLogin()
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_cancelLogin;

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::reLogin()
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_reloginLogin;

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::logout()
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_Logout;

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::onLoginResult(bool success, int resultCode, int extendResultCode)
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_onLoginResult;
	req.argument.aLoginResult.success = success;
	req.argument.aLoginResult.resultCode = resultCode;
	req.argument.aLoginResult.extendResultCode = extendResultCode;

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::onLogoutResult(long lReason)
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_onLogoutResult;
	req.argument.aLogoutResult.lReason = lReason;

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::onServerLinkLost()
{
	LoginAgentEventReq req;
	req.aSubEventType = SubEvent_onServerLinkLost;

	return sendEventToAgent(Event_LoginAdapterReq, &req, sizeof(req), loginAgentName, loginAdapterName);
}

bool USSDK_LoginServiceAdapter::isLoginSuccess()
{
	return CorrespondService()->isLoginSuccess();
}