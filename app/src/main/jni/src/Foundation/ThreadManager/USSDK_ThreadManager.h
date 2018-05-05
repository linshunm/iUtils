//
//  USSDK_ThreadManager.hpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/12.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_ThreadManager_hpp
#define USSDK_ThreadManager_hpp

#include <stdio.h>
#include <string>
#include <map>

#include "../USSDKMutex/USSDK_Mutex.hpp"
#include "../../clt_include/CtlZXMethod.h"

using namespace std;
using namespace NS_USSDK_Foundation;

namespace NS_USSDK_Foundation
{
	 /*!
     *    @brief    线程管理模块，提供：（1）线程号分配/维护 和 线程通信的维护 （2）线程启动和关闭, 暂未启动 （3）关闭所有线程，暂未启动  (4)临时线程的生成，代码段的执行，暂未提供接口。
     *   （5）需要先启动底层ocx，线程启动才能okay
     */
    class USSDK_ThreadManager
    {
    private:
        map<string,int> threadNoMap;
        int curMaxPno;
        USSDK_Mutex m_PnoMutex;
        
    private:
        int getPno(string anAgentName);
        int allocationPno(string anAgentName);
        
    public:
        static USSDK_ThreadManager *shareInstance();

		USSDK_ThreadManager();
		~USSDK_ThreadManager();
        
        void startOneAgent(string anAgentName, LPENTRY pEntry);
        void stopOneAgent(string anAgentName);
        void stopAllAgent();
        
        bool sendToAgent(unsigned short event, void *In, unsigned short InLen, string toAgent, string sourceAgent);
        
    public:
        bool createThread(LPHANDLE_FUNC startaddr, void* parg);
    };

	#define sendEventToAgent(EVENT, IN, InLen, toAgent, sourceAgent) USSDK_ThreadManager::shareInstance()->sendToAgent(EVENT, IN, InLen, toAgent, sourceAgent);
}

#endif /* USSDK_ThreadManager_hpp */
