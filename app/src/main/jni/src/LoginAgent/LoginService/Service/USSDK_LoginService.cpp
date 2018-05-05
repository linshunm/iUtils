//
//  USSDK_LoginService.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "../../LoginAgentProc.hpp"
#include "../../LoginAgentProcTransModel.hpp"
#include "USSDK_LoginService.h"
#include "../NotifyAPI/USSDK_LoginServiceCallBack.h"

#include "../../../OcxAdapter/USSDK_OcxAdapterHeader.h"
#include "../../../LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"

using namespace NS_USSDK_LoginService;
using namespace NS_USSDK_OcxAdapter;
using namespace NS_USSDK_LogService;


USSDK_LoginService *USSDK_LoginService::shareInstance()
{
    static USSDK_LoginService instance;
    return &instance;
}

void USSDK_LoginService::threadInit()
{
    if (!isThreadInit)
    {
        loginAgentThreadInit();
        isThreadInit = true;
    }
}

void USSDK_LoginService::threadUnInit()
{
    if (isThreadInit)
    {
		loginAgentThreadUnInit();
        isThreadInit = false;
    }
}

SingleServerInfo *USSDK_LoginService::singleServerInfoOfID(string aServerID)
{
	ServerInfoMap::iterator it = serverInfoMap.find(aServerID);

	bool bFindKey = false;
	SingleServerInfo *value = NULL;

	//find key
	if (it != serverInfoMap.end())
	{
		bFindKey = true;
		value = &(it->second);
	}
	
	return value;
}

bool USSDK_LoginService::doLogin()
{
	LOG_DEBUGGING(loginAgentName, "dologin: loginStep[%d]", loginStep);
	switch(loginStep)
	{
	case LoginStep_Init:
		{
			OcxLogonPara aPara;
			SingleServerInfo *curServerInfo = singleServerInfoOfID(loginInfo.serverID);

			aPara.chSoftPhone = loginInfo.account;
			aPara.chPassword = loginInfo.passWord;

			aPara.chSSIP = "";
			aPara.lSSPort = curServerInfo->sipPort;
			aPara.chSipServerIP = curServerInfo->sipDomain;
			aPara.lSipServerPort = curServerInfo->sipPort;
			aPara.chVersion = loginInfo.appVersion;
			aPara.nLogonType = loginInfo.loginType;		

			//call ocxService
			if (USSDK_OcxAdapter::login(&aPara))
			{
				LOG_DEBUGGING(loginAgentName, "sync login success, account[%s]", loginInfo.account.c_str());
				loginStep = LoginStep_Ocx_Register;
				loginStatus = LoginStatus_Login_ing;
				return true;
			}
			
			LOG_DEBUGGING(loginAgentName, "sync login failed, account[%s]", loginInfo.account.c_str());
			return false;
		}
	case LoginStep_Ocx_Register:
		{
			return false;
		}
	}
	return false;
}

bool USSDK_LoginService::doLogout(bool isDataClear)
{
	LOG_DEBUGGING(loginAgentName, "doLogout: logoutStep[%d]", logoutStep);
	switch(logoutStep)
	{
	case LogoutStep_Init:
		{
			//need call logout
			if (USSDK_OcxAdapter::logout(isDataClear))
			{
				LOG_DEBUGGING(loginAgentName, "sync logout success, account[%s] isDataClear[%d]", loginInfo.account.c_str(), isDataClear);
				logoutStep = LogoutStep_Ocx_Unregister;
				loginStatus = LoginStatus_Logout_ing;
				isInternalDataClear = isDataClear;
				return true;
			}
			
			LOG_DEBUGGING(loginAgentName, "sync logout failed, account[%s] isDataClear[%d]", loginInfo.account.c_str(), isDataClear);
			return false;
		}
	case LogoutStep_Ocx_Unregister:
		{
			return false;
		}
	}
	
	return false;
}

void USSDK_LoginService::setServerInfo(SingleServerInfo* aSingleServerInfo)
{
	LOG_DEBUGGING(loginAgentName, "setServerInfo, serverID[%s]", aSingleServerInfo->serverID.c_str());
	
	string key = aSingleServerInfo->serverID;
	serverInfoMap[key] = *aSingleServerInfo;
}

bool USSDK_LoginService::login(CurrentLoginInfo *aLoginInfo)
{
	LOG_DEBUGGING(loginAgentName, "login: loginStatus[%d] account[%s] serverID[%s]", loginStatus, aLoginInfo->account.c_str(), aLoginInfo->serverID.c_str());
	if (loginStatus != LoginStatus_Init)
	{
		//log
		LOG_DEBUGGING(loginAgentName, "error intra logininStatus");
		return false;
	}
	
	loginInfo = *aLoginInfo;

	//get server from serverid
	SingleServerInfo *currentServerInfo = singleServerInfoOfID(loginInfo.serverID);

	if (!currentServerInfo)
	{
		LOG_DEBUGGING(loginAgentName, "error loginServer info, cannot find serverinfo[%s]", loginInfo.serverID.c_str());
		return false;
	}

	return doLogin();
}

bool USSDK_LoginService::cancelLogin()
{
	LOG_DEBUGGING(loginAgentName, "cancelLogin: loginStatus[%d]", loginStatus);
	if (loginStatus == LoginStatus_Init || loginStatus == LoginStatus_Logout_ing)
	{
		isInternalDataClear = false;
		return false;
	}
	
	return doLogout();
}

bool USSDK_LoginService::reLogin()
{
	LOG_DEBUGGING(loginAgentName, "reLogin: loginStatus[%d]", loginStatus);
	if (loginStatus == LoginStatus_Init)
	{
		//login
		return doLogin();
	}
	
	if (loginStatus !=  LoginStatus_Logout_ing)
	{
		//donothing
		return doLogout(true);
	}

	return true;
}

bool USSDK_LoginService::logout()
{
	LOG_DEBUGGING(loginAgentName, "logout: loginStatus[%d]", loginStatus);
	if (loginStatus == LoginStatus_Init || loginStatus == LoginStatus_Logout_ing)
	{
		isInternalDataClear = false;
		return true;
	}

	return doLogout();
}

void USSDK_LoginService::onLoginResult(bool success, int resultCode, int extendResultCode)
{
	LOG_DEBUGGING(loginAgentName, "onLoginResult: success[%d] resultCode[%d] extendResultCode[%d]", success, resultCode, extendResultCode);
	loginStep = LoginStep_Init;

	if (success)
	{
		loginStatus = LoginStatus_Login_success;
	}
	else
	{
		loginStatus = LoginStatus_Init;
	}

	//just report now @20180404
	LoginMattersNotifyPara aPara;
	aPara.aNotifyType = LoginMattersNotifyType_LoginResult;
	aPara.success = success;
	aPara.resultCode = resultCode;
	aPara.extendResultCode = extendResultCode;
	aPara.account = loginInfo.account;

	postNotification(EVT_LoginMattersNotify, &aPara, EVT_SourceName_LoginService);
}

void USSDK_LoginService::onLogoutResult(long lReason)
{
	LOG_DEBUGGING(loginAgentName, "onLogoutResult: lReason[%d]", lReason);
	loginStep = LoginStep_Init;
	logoutStep = LogoutStep_Init;
	loginStatus = LoginStatus_Init;

	//if need relogin, do not report UP
	if (isInternalDataClear)
	{
		LOG_DEBUGGING(loginAgentName, "onLogoutResult: isInternalDataClear, need relogin");
		reLogin();
		return;
	}
	
	LoginMattersNotifyPara aPara;
	aPara.aNotifyType = LoginMattersNotifyType_LogoutResult;
	aPara.success = true;
	aPara.resultCode = lReason;
	aPara.extendResultCode = lReason;
	aPara.account = loginInfo.account;

	postNotification(EVT_LoginMattersNotify, &aPara, EVT_SourceName_LoginService);
}

void USSDK_LoginService::onServerLinkLost()
{
	LOG_DEBUGGING(loginAgentName, "onServerLinkLost");
	LoginMattersNotifyPara aPara;
	aPara.aNotifyType = LoginMattersNotifyType_OnServerLinkLost;
	aPara.success = true;
	aPara.account = loginInfo.account;

	postNotification(EVT_LoginMattersNotify, &aPara, EVT_SourceName_LoginService);
}

bool USSDK_LoginService::isLoginSuccess()
{
	return loginStatus == LoginStatus_Login_success;
}

void USSDK_LoginService::onNotify(string anEventName, void* aPara, string sourceName)
{
	
}
