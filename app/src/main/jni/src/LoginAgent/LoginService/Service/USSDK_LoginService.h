//
//  USSDK_LoginService.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_LoginService_h
#define USSDK_LoginService_h

#include <stdio.h>
#include <string>
#include <map>
#include "../Model/USSDK_LoginModel.h"
#include "../../../Foundation/USSDKBase/USSDK_BaseClass.hpp"

using namespace NS_USSDK_Foundation;
using namespace std;
using namespace NS_USSDK_LoginService;


namespace NS_USSDK_LoginService
{
    class USSDK_LoginService : public USSDK_BaseClass, public USSDK_UNotifyMethodClass
    {
	private:
		typedef map<string, SingleServerInfo> ServerInfoMap;
		ServerInfoMap serverInfoMap;
		CurrentLoginInfo loginInfo;

		LoginStatus loginStatus;
		LoginStep loginStep;
		LogoutStep logoutStep;

		bool isInternalDataClear;

    public:
        static USSDK_LoginService *shareInstance();
        
    public:
        void threadInit();
        void threadUnInit();

	private:
		SingleServerInfo *singleServerInfoOfID(string aServerID);
		bool doLogin();
		bool doLogout(bool isDataClear = false);

    public:
		void setServerInfo(SingleServerInfo* aSingleServerInfo);

		bool login(CurrentLoginInfo *aLoginInfo);
		bool cancelLogin();
		bool reLogin();
		
		bool logout();

	public:
		void onLoginResult(bool success, int resultCode, int extendResultCode);
		void onLogoutResult(long lReason);
		void onServerLinkLost();

	public:
		bool isLoginSuccess();

	public:
		void onNotify(string anEventName, void* aPara, string sourceName);
    };
}

#endif /* USSDK_LoginService_h */
