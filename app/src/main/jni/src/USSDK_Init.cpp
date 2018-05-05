//
//  USSDK_Init.cpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/12.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_Init.h"
#include "./LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"

#include "USSDK_UpNotify.hpp"
#include "./OcxAdapter/USSDK_OcxAdapterHeader.h"

#include "./LogAgent/LogService/Service/USSDK_LogService.hpp"
#include "./LoginAgent/LoginService/Service/USSDK_LoginService.h"
#include "./RosterAgent/RosterService/Service/USSDK_RosterService.h"

using namespace std;
using namespace NS_USSDK_DEFAULT;

using namespace NS_USSDK_OcxAdapter;
using namespace NS_USSDK_LogService;
using namespace NS_USSDK_LoginService;
using namespace NS_USSDK_RosterService;

void USSDK_Main::USSDK_Init(std::string anUserDataPath, std::string anAppRunDataPath, std::string anPrivatePath)
{
    //start ocx
    USSDK_OcxAdapter::bInitControl();
    USSDK_OcxAdapter::bSetEventWrapper(USSDK_OcxAdapter::shareInstance());
    
	//log
    USSDK_LogService::shareInstance()->logRootPath = anAppRunDataPath;
    USSDK_LogService::shareInstance()->threadInit();
	LOG_DEBUGGING("Default", "ussdk Start module[%s] end", "USSDK_LogService");

	//login
	USSDK_LoginService::shareInstance()->threadInit();
	LOG_DEBUGGING("Default", "ussdk Start module[%s] end", "USSDK_LoginService");

	//roster
	USSDK_RosterService::shareInstance()->userDataPath = anUserDataPath;
    USSDK_RosterService::shareInstance()->dbDataPath = anPrivatePath;
	USSDK_RosterService::shareInstance()->threadInit();
	LOG_DEBUGGING("Default", "ussdk Start module[%s] end", "USSDK_RosterService");
    
    USSDK_UpNotify::shareInstance()->startObserver();
    LOG_DEBUGGING("Default", "ussdk Start end");
}

void USSDK_Main::setEventWrapper(USSDK_EventWrapper *anEventWrapper)
{
    USSDK_UpNotify::shareInstance()->eventWrapper = anEventWrapper;
    
}


