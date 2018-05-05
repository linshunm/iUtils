//
//  USSDK_Mutex.hpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/16.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_Mutex_hpp
#define USSDK_Mutex_hpp

#include <stdio.h>

#ifdef _WIN32
	#include <afxwin.h>
#else
	#include <mutex>
	using namespace std;
#endif

namespace NS_USSDK_Foundation
{
    /*!
     *    @brief    互斥锁， 由于 mutex 是 c++11的标准，pc vs2008不支持
     */
    class USSDK_Mutex
    {
    private:
#ifdef _WIN32
        CRITICAL_SECTION m_mutex;
#else
        std::mutex m_mutex;
#endif
        
    public:
        USSDK_Mutex();
        ~USSDK_Mutex();
        
    public:
        void lock();
        void unlock();
    };  
}

#endif /* USSDK_Mutex_hpp */
