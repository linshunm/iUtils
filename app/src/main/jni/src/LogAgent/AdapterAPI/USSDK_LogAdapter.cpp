//
//  USSDK_LogAdapter.cpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/19.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_LogAdapter.hpp"
#include "../LogAgentProc.hpp"
#include "../LogService/Model/USSDK_LogDataModel.hpp"
#include "../../Foundation/ThreadManager/USSDK_ThreadManager.h"

#include "../../Foundation/USSDKStringTool/USSDK_StringTool.hpp"

using namespace NS_USSDK_Foundation;
using namespace NS_USSDK_LogService;

bool USSDK_LogServiceAdapter::log(std::string aModule, int iLevel, const char* sFile, const char* sFunc, int iLine, const char* sFormat, ...)
{
    va_list args;
    char logBuf[LOG_BUFFER_SIZE] = {0};
    va_start(args, sFormat);
    vsnprintf(logBuf, sizeof(logBuf)-1, sFormat, args);
    va_end(args);
    
    USSDK_LogDataModel req = {0};
	strncpy(req.module, aModule.c_str(), sizeof(req.module) - 1);
    req.iLevel = iLevel;
//    req.threadId = threadId;
    strncpy(req.sFile, USSDK_StringTool::lastPathCompent(sFile).c_str(), sizeof(req.sFile) - 1);
	strncpy(req.sFunc, USSDK_StringTool::lastCompentUseSeparator(sFunc, "::").c_str(), sizeof(req.sFunc) - 1);
    req.iLine = iLine;
    strncpy(req.sContent, logBuf, sizeof(req.sContent));
    
    return sendEventToAgent(logWriteEvent, &req, sizeof(req), logAgentName, "self");
}
