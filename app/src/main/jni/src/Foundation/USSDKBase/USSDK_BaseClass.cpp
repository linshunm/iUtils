//
//  USSDK_BaseClass.cpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/14.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_BaseClass.hpp"

using namespace NS_USSDK_Foundation;

void USSDK_BaseClass::addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID)
{
    aNotificationCenter.addObserver(observer, anEventName, callBackThreadID);
}

void USSDK_BaseClass::addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID)
{
    aNotificationCenter.addObserver(observer, callBackThreadID);
}

void USSDK_BaseClass::removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName)
{
    aNotificationCenter.removeObserver(observer, anEventName);
}

void USSDK_BaseClass::removeObserver(USSDK_UNotifyMethodClass * observer)
{
    aNotificationCenter.removeObserver(observer);
}

void USSDK_BaseClass::postNotification(string anEventName, void* aPara, string sourceName)
{
    aNotificationCenter.postNotification(anEventName, aPara, sourceName);
}
