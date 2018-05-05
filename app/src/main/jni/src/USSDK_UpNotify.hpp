//
//  USSDK_UpNotify.hpp
//  SYDUSSDK
//
//  Created by moa_mini2 on 2018/5/2.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_UpNotify_hpp
#define USSDK_UpNotify_hpp

#include <stdio.h>
#include "./OcxAdapter/USSDK_EventWrapper.h"
#include "./Foundation/NotificationCenter/USSDK_NotificationCenter.h"

namespace NS_USSDK_DEFAULT
{
    class USSDK_UpNotify : public USSDK_UNotifyMethodClass
    {
    public:
        USSDK_EventWrapper *eventWrapper;
        
    public:
        static USSDK_UpNotify * shareInstance();
        void startObserver();
        
    public:
        void onNotify(string anEventName, void* aPara, string sourceName);
    };
}
#endif /* USSDK_UpNotify_hpp */
