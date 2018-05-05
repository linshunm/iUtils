//
//  USSDK_NotificationCenter.cpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/9.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_NotificationCenter.h"

using namespace std;
using namespace NS_USSDK_Foundation;

USSDK_NotificationCenter * USSDK_NotificationCenter::shareInstance()
{
    static USSDK_NotificationCenter instance;
    return &instance;
}

void USSDK_NotificationCenter::addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID)
{
    
    if (anEventName.length() == 0)
    {
        return;
    }
    
    //lock
    m_SingleEventMutex.lock();
    
    //find value use key from map
    SingleEventMap::iterator it = aSingleEventMap.find(anEventName);
    
    bool bFindKey = false;
    bool bFindValue = false;
    CallBackInfoVector valueVector;
    
    //find key
    if (it != aSingleEventMap.end())
    {
        bFindKey = true;
        valueVector = it->second;
        
        for (size_t nIndex = 0; nIndex < valueVector.size(); nIndex++)
        {
            USSDK_CallBackInfo * singleValue = &valueVector[nIndex];
            
            if (singleValue->aCallBackHandler == observer)
            {
                bFindValue = true;
                break;
            }
        }
    }
    
    if (!bFindValue)
    {
        USSDK_CallBackInfo singleValue;
        singleValue.aCallBackHandler = observer;
        singleValue.callBackThreadID = callBackThreadID;

        valueVector.push_back(singleValue);
        
        aSingleEventMap[anEventName] = valueVector;
    }
    
    //unlock
    m_SingleEventMutex.unlock();
}

void USSDK_NotificationCenter::addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID)
{
    //lock
    m_AllEventMutex.lock();
    
    bool bFind = false;
    for (size_t nIndex = 0; nIndex < allEventVector.size(); nIndex++)
    {
        USSDK_CallBackInfo * singleValue = &allEventVector[nIndex];

        if (singleValue->aCallBackHandler == observer)
        {
            bFind = true;
            break;
        }
    }

    if (!bFind)
    {
        USSDK_CallBackInfo singleValue;
        singleValue.aCallBackHandler = observer;
        singleValue.callBackThreadID = callBackThreadID;

        allEventVector.push_back(singleValue);
    }

    //unlock
    m_AllEventMutex.unlock();
}

void USSDK_NotificationCenter::removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName)
{
    if (anEventName.length() == 0)
    {
        return;
    }

    //lock
    m_SingleEventMutex.lock();
    
    //find value use key from map
    SingleEventMap::iterator itOfMap = aSingleEventMap.find(anEventName);

    //find key
    if (itOfMap != aSingleEventMap.end())
    {
        CallBackInfoVector valueVector = itOfMap->second;

        CallBackInfoVector::iterator itOfVector = valueVector.begin();

        while (itOfVector != valueVector.end())
        {
            USSDK_CallBackInfo * singleValue = &*itOfVector;

            if (singleValue->aCallBackHandler == observer)
            {
                itOfVector = valueVector.erase(itOfVector);
                break;
            }
            else
            {
                ++itOfVector;
            }
        }
    }

    //unlock
    m_SingleEventMutex.unlock();
}

void USSDK_NotificationCenter::removeObserver(USSDK_UNotifyMethodClass * observer)
{
    //lock
    m_AllEventMutex.lock();
    
    CallBackInfoVector::iterator it = allEventVector.begin();

    while (it != allEventVector.end())
    {
        USSDK_CallBackInfo * singleValue = &*it;

        if (singleValue->aCallBackHandler == observer)
        {
            it = allEventVector.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }

    //unlock
    m_AllEventMutex.unlock();
}

void USSDK_NotificationCenter::postNotification(string anEventName, void* aPara, string sourceName)
{
    //lock
    m_SingleEventMutex.lock();
    
    //find value use key from map
    SingleEventMap::iterator itOfMap = aSingleEventMap.find(anEventName);
    
    //find key
    if (itOfMap != aSingleEventMap.end())
    {
        CallBackInfoVector valueVector = itOfMap->second;
        
        CallBackInfoVector::iterator itOfVector = valueVector.begin();
        
        for ( ; itOfVector != valueVector.end(); ++itOfVector)
        {
            USSDK_CallBackInfo *singleValue = (USSDK_CallBackInfo *)&*itOfVector;
            
			singleValue->aCallBackHandler->onNotify(anEventName, aPara, sourceName);
        }
    }
    
    //unlock
    m_SingleEventMutex.unlock();
    
    //lock
    m_AllEventMutex.lock();
    
    CallBackInfoVector::iterator itOfVector = allEventVector.begin();

    for ( ; itOfVector != allEventVector.end(); ++itOfVector)
    {
        USSDK_CallBackInfo *singleValue = (USSDK_CallBackInfo *)&*itOfVector;
        
        singleValue->aCallBackHandler->onNotify(anEventName, aPara, sourceName);
    }
    
    //unlock
    m_AllEventMutex.unlock();
}

