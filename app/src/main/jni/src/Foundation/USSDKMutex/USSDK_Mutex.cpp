//
//  USSDK_Mutex.cpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/16.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_Mutex.hpp"

using namespace NS_USSDK_Foundation;

USSDK_Mutex::USSDK_Mutex()
{
#ifdef _WIN32
    InitializeCriticalSection(&m_mutex);
#endif
}

USSDK_Mutex::~USSDK_Mutex()
{
#ifdef _WIN32
    DeleteCriticalSection(&m_mutex);
#endif
}

void USSDK_Mutex::lock()
{
#ifdef _WIN32
    EnterCriticalSection(&m_mutex);
#else
    m_mutex.lock();
#endif
}

void USSDK_Mutex::unlock()
{
#ifdef _WIN32
    LeaveCriticalSection(&m_mutex);
#else
     m_mutex.unlock();
#endif
}
