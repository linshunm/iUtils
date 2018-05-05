//
//  USSDK_UpNotify.cpp
//  SYDUSSDK
//
//  Created by moa_mini2 on 2018/5/2.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_UpNotify.hpp"

#include "./LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"

#include "./LoginAgent/LoginAgentHeader.h"
#include "./RosterAgent/RosterAgentHeader.h"

using namespace NS_USSDK_DEFAULT;
using namespace NS_USSDK_LogService;
using namespace NS_USSDK_LoginService;
using namespace NS_USSDK_RosterService;

USSDK_UpNotify * USSDK_UpNotify::shareInstance()
{
    static USSDK_UpNotify instance;
    return &instance;
}

void USSDK_UpNotify::startObserver()
{    
    USSDK_LoginServiceAdapter::addObserver(this, 0);
    USSDK_RosterServiceAdapter::addObserver(this, 0);
}

void USSDK_UpNotify::onNotify(string anEventName, void* aPara, string sourceName)
{
    LOG_DEBUGGING("USSDK_UpNotify", "onNotify [%s][%s]", anEventName.c_str(), sourceName.c_str());
    
    if (!sourceName.compare(EVT_SourceName_LoginService))
    {
        eventWrapper->onLoginMatterNotify((LoginMattersNotifyPara*)aPara);
        return;
    }
    
    if (!sourceName.compare(EVT_SourceName_RosterService))
    {
        if (!anEventName.compare(EVT_SingleContactNotify))
        {
            eventWrapper->onSingleContactNotify((SingleContactNotifyPara*)aPara);
            return;
        }
        
        if (!anEventName.compare(EVT_PublicGroupNotify))
        {
            eventWrapper->onPublicGroupNotify((PublicGroupNotifyPara*)aPara);
            return;
        }
        
        if (!anEventName.compare(EVT_PublicGroupMemberChangedNotify))
        {
            eventWrapper->onPublicGroupMemberChangedNotify((PublicGroupMemberChangedNotifyPara*)aPara);
            return;
        }

    }
    
    
}
