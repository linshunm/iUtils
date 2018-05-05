//
//  ussdk_LogDataModel.h
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/20.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_LogDataModel_h
#define USSDK_LogDataModel_h

#include <stdio.h>
#include <string>

using namespace std;

namespace NS_USSDK_LogService
{
    
#define LOG_BUFFER_SIZE 1024
#define MAX_LOG_FILE_SIZE 20*1024*1024
    
//format：[time[timeZone]][Module][ThreadID, not support][LogLevel][File][Func][Code Line][Content]
//#define LOG_FORMAT "[%s][%d] [%s][%u] [%s][%s][%d] [%s] \n"
#define LOG_FORMAT "[%s][%s] [%s] [%s][%s][%d] [%s] \n"
    
typedef struct{
    char module[32+1];
    int iLevel;
//    THREAD_ID threadId;
    char sFile[64+1];
    char sFunc[64+1];
    int iLine;
    char sContent[1024+1];
}USSDK_LogDataModel;
    
}

#endif /* ussdk_LogDataModel_h */
