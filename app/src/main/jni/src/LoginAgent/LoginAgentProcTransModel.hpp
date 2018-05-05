//
//  LoginAgentProcTransModel.hpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef LoginAgentProcTransModel_hpp
#define LoginAgentProcTransModel_hpp

#include <stdio.h>
#include <string>
#include "../Foundation/USSDKMessageQueue/USSDK_MessageQueue.hpp"
#include "LoginService/Model/USSDK_LoginModel.h"

using namespace std;
using namespace NS_USSDK_Foundation;
using namespace NS_USSDK_LoginService;

namespace NS_USSDK_LoginService
{
	static string loginAgentName = "loginAgent";
	static string loginAdapterName = "loginAdapter";

	#define Event_LoginAdapterReq 3000

	typedef enum
	{
		SubEvent_setServerInfo = 1,
		SubEvent_login,
		SubEvent_cancelLogin,
		SubEvent_reloginLogin,
		SubEvent_Logout,
		SubEvent_onLoginResult,
		SubEvent_onLogoutResult,
		SubEvent_onServerLinkLost,
	}SubEventType;

	typedef struct
	{
		bool success;
		int resultCode;
		int extendResultCode;
	}LoginResult;

	typedef struct
	{
		int lReason;
	}LogoutResult;

	typedef struct
	{
		SubEventType aSubEventType;
		union
		{
			LoginResult aLoginResult;
			LogoutResult aLogoutResult;
		}argument;
	}LoginAgentEventReq;

	extern USSDK_MessageQueue<SingleServerInfo> g_setServerInfo;
	extern USSDK_MessageQueue<CurrentLoginInfo> g_loginInfo;
}

#endif /* LoginAgentProc_hpp */
