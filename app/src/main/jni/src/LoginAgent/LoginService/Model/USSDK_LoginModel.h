//
//  USSDK_LoginModel.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_LoginModel_h
#define USSDK_LoginModel_h

#include <stdio.h>
#include <string>
#include "../../../USSDKMethod.h"

using namespace std;

namespace NS_USSDK_LoginService
{
    class USSDK_API SingleServerInfo
    {
    public:
		//server id, unique
		string serverID;
		//chinese name.reserved
        string CNName; 
		//english name, reserved
        string ENName; 
		//sip domain, for sipServer
        string sipDomain;
		//sip port, for sipServer
		int sipPort;  
		//xcap domain, for xcap Server
        string xcapDomain;
		//xcap port, for xcap Server
        int xcapPort; 

    public:
		SingleServerInfo();
		~SingleServerInfo();
		SingleServerInfo(string aServerID, string aCNName, string anENName, string aSipDomain, int aSipPort, string aXcapDomain, int aXcapPort);
//        string description();
    };
    
    typedef enum
    {
        DeviceType_IOS,
        DeviceType_AD,
        DeviceType_Windows
    }DeviceType;
    
    typedef enum
    {
        LoginType_Manual,
        LoginType_Auto,
    }LoginType;
    
	class USSDK_API CurrentLoginInfo
	{
	public:
	    string account;
		string passWord;
		DeviceType deviceType;
		LoginType loginType;
		string serverID;
		string appVersion;

	public:
		CurrentLoginInfo();
		~CurrentLoginInfo();
		CurrentLoginInfo(string anAccount, string aPassword, DeviceType aDeviceType, LoginType aLoginType, string aServerID, string anAppVersion);
	};

	typedef enum
	{
		LoginStatus_Init,
		LoginStatus_Login_ing,
		LoginStatus_Login_success,
		LoginStatus_Logout_ing,
	}LoginStatus;

	typedef enum
	{
		LoginStep_Init,
		LoginStep_Ocx_Register,
	}LoginStep;

	typedef enum
	{
		LogoutStep_Init,
		LogoutStep_Ocx_Unregister,
	}LogoutStep;

}
#endif /* USSDK_LoginModel_h */
