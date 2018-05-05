//
//  USSDK_BaseClass.hpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/14.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_BaseClass_hpp
#define USSDK_BaseClass_hpp

#include <stdio.h>
#include "../NotificationCenter/USSDK_NotificationCenter.h"

using namespace NS_USSDK_Foundation;

namespace NS_USSDK_Foundation
{
	class USSDK_BaseClass : public USSDK_NotificationRegistrationAndCancelRegistration, public USSDK_PostNotification
    {
    private:
        USSDK_NotificationCenter aNotificationCenter;
        
    public:
        bool isThreadInit;
        
    public:
        void addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID);
        void addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID);
        
        void removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName);
        void removeObserver(USSDK_UNotifyMethodClass * observer);
        
	public:
        void postNotification(string anEventName, void* aPara, string sourceName);
    };
}

#endif /* USSDK_BaseClass_hpp */
