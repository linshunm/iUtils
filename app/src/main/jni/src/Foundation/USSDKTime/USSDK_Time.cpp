//
//  USSDK_Time.cpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/20.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_Time.hpp"

#ifdef _WIN32
#include <WTypes.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

using namespace NS_USSDK_Foundation;

string USSDK_Time::currentTimeStringOfLogFormat()
{
#ifdef _WIN32
	
	SYSTEMTIME  st;
	GetLocalTime(&st);
	
	TIME_ZONE_INFORMATION tziNow;
	DWORD dwRet;
	ZeroMemory(&tziNow, sizeof(tziNow));
	dwRet = GetTimeZoneInformation(&tziNow);

	char out[100] = {0};
	sprintf(out, "%d-%02d-%02d %02d:%02d:%02d:%02d[GMT+%d]",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, -tziNow.Bias/60);

	return out;

#else
    long seconds;
    struct tm nowTime;
    struct timeval ctv;
    /* get real clock from CMOS, C func */
    gettimeofday(&ctv,NULL);
    
    seconds = ctv.tv_sec;
    localtime_r(&seconds, &nowTime);
    
    char out[100] = {0};

    sprintf(out, "%d-%02d-%02d %02d:%02d:%02d:%02d[%s]",
            nowTime.tm_year+1900, nowTime.tm_mon+1, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec, (int)ctv.tv_usec/10000, nowTime.tm_zone);
            
    return out;
#endif
}
