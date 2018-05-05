//
//  USSDK_LogService.cpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/19.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_LogService.hpp"
#include "../../LogAgentProc.hpp"

#include "../../../Foundation/USSDKFile/USSDK_FileManager.hpp"
#include "../../../Foundation/USSDKTime/USSDK_Time.hpp"

#include <iostream>

using namespace std;
using namespace NS_USSDK_LogService;
using namespace NS_USSDK_Foundation;

static string logFileDirectoryName = "IMSClientLog";
static string logFileName = "USSDKLog.txt";
static string OldLogFileName = "USSDKLog.old";

USSDK_LogService *USSDK_LogService::shareInstance()
{
    static USSDK_LogService instance;
    return &instance;
}

void USSDK_LogService::threadInit()
{
    if (!isThreadInit)
    {
        logAgentThreadInit();
        isThreadInit = true;
    }
}

void USSDK_LogService::threadUnInit()
{
    if (isThreadInit)
    {
        logAgentThreadUnInit();
        isThreadInit = false;
    }
}

string USSDK_LogService::logFileDirPath()
{
    return logRootPath + pathSeparator + logFileDirectoryName;
}

string USSDK_LogService::logFilePath()
{
    return logFileDirPath() + pathSeparator + logFileName;
}

string USSDK_LogService::oldLogFilePath()
{
    return logFileDirPath() + pathSeparator + OldLogFileName;
}

bool USSDK_LogService::log(USSDK_LogDataModel *req)
{
    if (!fp)
    {
        USSDK_FileManager::fixDirPath(logFileDirPath());
    
        fp = fopen(logFilePath().c_str(), "a+");
        
        if (!fp)
        {
            std::cout << "open logFile " << logFilePath() << " Failed" << endl;
            return false;
        }
    }
    
    string logLevel;
    switch(req->iLevel)
    {
        case 1 : logLevel = "debug"; break;
        case 2 : logLevel = "info"; break;
        case 3 : logLevel = "warn"; break;
        case 4 : logLevel = "error"; break;
        default:
            logLevel = "info"; break;
    }
    
    char logBuffer[LOG_BUFFER_SIZE + 1] = {0};
    
#ifdef _WIN32
    _snprintf(logBuffer, LOG_BUFFER_SIZE, LOG_FORMAT, USSDK_Time::currentTimeStringOfLogFormat().c_str(), req->module, logLevel.c_str(), req->sFile, req->sFunc, req->iLine, req->sContent);
#else
    snprintf(logBuffer, LOG_BUFFER_SIZE, LOG_FORMAT, USSDK_Time::currentTimeStringOfLogFormat().c_str(), req->module, logLevel.c_str(), req->sFile, req->sFunc, req->iLine, req->sContent);
#endif
    
    std::cout << logBuffer;
    
    fprintf(fp, "%s", logBuffer);

    fflush(fp);
    long filepos = ftell(fp);
    if (filepos >= MAX_LOG_FILE_SIZE)
    {
        fclose(fp);
        fp = NULL;
        rename(logFilePath().c_str(), oldLogFilePath().c_str());
    }
    
    return true;
}
