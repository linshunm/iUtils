//
//  USSDK_LoginModel.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//
#include "USSDK_LoginModel.h"
#include <string>
using namespace std;
using namespace NS_USSDK_LoginService;

SingleServerInfo::SingleServerInfo()
{

}
SingleServerInfo::~SingleServerInfo()
{

}

SingleServerInfo::SingleServerInfo(string aServerID, string aCNName, string anENName, string aSipDomain, int aSipPort, string aXcapDomain, int aXcapPort)
	: serverID(aServerID), CNName(aCNName), ENName(anENName), sipDomain(aSipDomain), sipPort(aSipPort), xcapDomain(aXcapDomain), xcapPort(aXcapPort)
{

}

// string SingleServerInfo::description()
// {
//     return "ServerInfoBean [serverID = " + serverID + ", cnName =" + mCnName + ", enName = " + mEnName
//     + ", sipDomain = " + mSipDomain + ", sipPort = " + std::to_string(mSipPort) + ", xcapDomain = " + mXcapDomain + ", xcapPort = " + std::to_string(mXcapPort);
// }

CurrentLoginInfo::CurrentLoginInfo()
{

}

CurrentLoginInfo::~CurrentLoginInfo()
{

}

CurrentLoginInfo::CurrentLoginInfo(string anAccount, string aPassword, DeviceType aDeviceType, LoginType aLoginType, string aServerID, string anAppVersion)
: account(anAccount), passWord(aPassword), deviceType(aDeviceType), loginType(aLoginType), serverID(aServerID), appVersion(anAppVersion)
{

}
