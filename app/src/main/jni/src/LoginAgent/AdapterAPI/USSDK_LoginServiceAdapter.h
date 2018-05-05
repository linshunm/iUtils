//
//  USSDK_LoginServiceAdapter.hpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_LoginServiceAdapter_h
#define USSDK_LoginServiceAdapter_h

#include <stdio.h>
#include <string>
#include <vector>

#include "../LoginService/Model/USSDK_LoginModel.h"
#include "../../Foundation/NotificationCenter/USSDK_NotificationCenter.h"
#include "../../USSDKMethod.h"

using namespace std;
using namespace NS_USSDK_LoginService;
using namespace NS_USSDK_Foundation;


namespace NS_USSDK_LoginService
{
	class USSDK_API USSDK_LoginServiceAdapter
    {
	public:
		static void addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID);
		static void addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID);

		static void removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName);
		static void removeObserver(USSDK_UNotifyMethodClass * observer);

    public:
			//down
		//set serverInfo before first login
        static bool setServerInfo(SingleServerInfo* aSingleServerInfo);
		//login to server
        static bool login(CurrentLoginInfo *aLoginInfo);
		//cancel login when at loginView， just for window now @20180408
		static bool cancelLogin();
		//now, just use for linkLost timer touch. and will use for other module when need relogin, for example, 403/481 results for message send 
		static bool reLogin();

		//logout, for view
        static bool logout();
		
	public:
		//up
		//resultCode--from ocx  extendResultCode--from ocx and inter
		static bool onLoginResult(bool success, int resultCode, int extendResultCode);
		static bool onLogoutResult(long lReason);
		static bool onServerLinkLost();

	public:
		//direct Invoke methods, not run at some other thread
		static bool isLoginSuccess();

    };
}
#endif /* USSDK_LoginServiceAdapter_h */
