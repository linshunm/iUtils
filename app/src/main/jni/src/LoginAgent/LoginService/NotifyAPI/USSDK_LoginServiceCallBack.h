//
//  USSDK_LoginServiceCallBack.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_LoginServiceCallBack_h
#define USSDK_LoginServiceCallBack_h
#include <stdio.h>
#include <string>

using namespace std;

namespace NS_USSDK_LoginService
{
	#define DEFINE_EVENT(EVENT) static string EVENT = #EVENT;

	DEFINE_EVENT(EVT_SourceName_LoginService)

	DEFINE_EVENT(EVT_LoginMattersNotify)

	typedef enum
	{
		LoginMattersNotifyType_LoginResult,
		LoginMattersNotifyType_LogoutResult,
		LoginMattersNotifyType_OnServerLinkLost,
	}LoginMattersNotifyType;

	struct LoginMattersNotifyPara
	{
		LoginMattersNotifyType aNotifyType;
		bool success;
		int resultCode;
		int extendResultCode;
		string account;
	};
}

#endif /* USSDK_LoginServiceCallBack_h */
