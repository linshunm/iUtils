//
//  USSDK_ThreadManager.cpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/12.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_ThreadManager.h"
#include <iostream>
#include "../../clt_include/CtlZXMethod.h"
#include "../../clt_include/ussdk_Typedefs.h"

using namespace std;
using namespace NS_USSDK_Foundation;

USSDK_ThreadManager *USSDK_ThreadManager::shareInstance()
{
    static USSDK_ThreadManager instance;
    return &instance;
}

USSDK_ThreadManager::USSDK_ThreadManager()
{
	curMaxPno = 20;
}

USSDK_ThreadManager::~USSDK_ThreadManager()
{
	
}

int USSDK_ThreadManager::getPno(string anAgentName)
{
    //lock
    m_PnoMutex.lock();
    
    //find value use key from map
    map<string,int>::iterator it = threadNoMap.find(anAgentName);
    
    bool bFind = false;
    int pno = 0;
    
    //找到了key
    if (it != threadNoMap.end())
    {
        bFind = true;
        pno = it->second;
    }
    
    //unlock
    m_PnoMutex.unlock();
    
    return pno;
}

int USSDK_ThreadManager::allocationPno(string anAgentName)
{
    //lock
    m_PnoMutex.lock();
    
    //find value use key from map
    map<string,int>::iterator it = threadNoMap.find(anAgentName);
    
    int pno = 0;
    
    //找到了key
    if (it == threadNoMap.end())
    {
        pno = curMaxPno++;
        threadNoMap[anAgentName] = pno;
    }
    
    //unlock
    m_PnoMutex.unlock();
    
    return pno;
}

void USSDK_ThreadManager::startOneAgent(string anAgentName, LPENTRY pEntry)
{
    int pno = allocationPno(anAgentName);
    
    if (0 == pno)
    {
        std::cout << "start agent " << anAgentName << " fail" << std::endl;
        return;
    }

	USES_CONVERSION;
	CString name = (char *)A2CT(anAgentName.c_str());
    
    if (bCtl_ZXStartAgent(pno, name, pEntry))
    {
        std::cout << "start agent " << anAgentName << " success" << std::endl;
    }
    else
    {
        std::cout << "start agent " << anAgentName << " fail" << std::endl;
    }
}

void USSDK_ThreadManager::stopOneAgent(string anAgentName)
{
    int pno = getPno(anAgentName);
	bCtl_ZXStopAgent(pno);
}

void USSDK_ThreadManager::stopAllAgent()
{
    
}

bool USSDK_ThreadManager::sendToAgent(unsigned short event, void *In, unsigned short InLen, string toAgent, string sourceAgent)
{
    int destPno = getPno(toAgent);
    int sourcePno = getPno(sourceAgent);
    
    if (0 == destPno)
    {
        std::cout << "send " << event << " " << sourceAgent << "->" << toAgent << "fail" << std::endl;
        return false;
    }

	return bCtl_ZXSendToAgent(event, (UINT8 far *)In, InLen, destPno, sourcePno);
}

bool USSDK_ThreadManager::createThread(LPHANDLE_FUNC startaddr, void* parg)
{
    DWORD thread_id;
    UINT32  hThread;
    return bCtl_ZXCREATE_THREAD(startaddr, parg, 0, &thread_id, &hThread);
}

