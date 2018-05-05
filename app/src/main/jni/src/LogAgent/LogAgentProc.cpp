//
//  LogAgentProc.cpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/19.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "LogAgentProc.hpp"
#include <iostream>

#include "../Foundation/ThreadManager/USSDK_ThreadManager.h"

#include "LogService/Model/USSDK_LogDataModel.hpp"
#include "LogService/Service/USSDK_LogService.hpp"

using namespace std;
using namespace NS_USSDK_Foundation;
using namespace NS_USSDK_LogService;

void far logAgentProc(UINT8 far * lpMsg, UINT8 far * lpNULL0, UINT8 far * lpNULL1)
{
    unsigned short event, msgLen;
    
    event  = bCtl_ZXGetEvent();
    msgLen = bCtl_ZXGetLength();
    
    switch(event)
    {
        case InitAllProcessEvent:
            cout<< "logAgentProc start success" <<endl;
            break;
        case QuitAllProcessEvent:
        {
            cout<< "logAgentProc stop success" <<endl;
            break;
        }
        case logWriteEvent:
        {
            USSDK_LogDataModel *req = (USSDK_LogDataModel *)lpMsg;
            USSDK_LogService::shareInstance()->log(req);
            break;
        }
        default:
        {
            cout<< "logAgentProc unknow event" << event << "from" << bCtl_ZXGetSenderPno <<endl;
            break;
        }
    }
    
}

void logAgentThreadInit()
{
    USSDK_ThreadManager *aManager = USSDK_ThreadManager::shareInstance();
    aManager->startOneAgent(logAgentName, logAgentProc);
}

void logAgentThreadUnInit()
{
    USSDK_ThreadManager *aManager = USSDK_ThreadManager::shareInstance();
    aManager->stopOneAgent(logAgentName);
}
