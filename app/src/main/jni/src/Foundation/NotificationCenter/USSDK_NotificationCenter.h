//
//  USSDK_NotificationCenter.hpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/9.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_NotificationCenter_hpp
#define USSDK_NotificationCenter_hpp

#include <stdio.h>
#include <map>
#include <string>
#include <vector>

#include "../USSDKMutex/USSDK_Mutex.hpp"
#include "../../USSDKMethod.h"

using namespace std;
using namespace NS_USSDK_Foundation;

/*!
 *    @brief    通知中心相关类， （1）class USSDK_UNotifyMethodClass， class USSDK_CallBackInfo， class USSDK_NotificationCenter  （2）DEFINE_EVENT 事件定义方法
 */
namespace NS_USSDK_Foundation
{
    /*!
     *    @brief   前置声明
     */
    class USSDK_UNotifyMethodClass;
    class USSDK_CallBackInfo;
    class USSDK_NotificationCenter;
    
    #define DEFINE_EVENT(EVENT) static string EVENT = #EVENT;
    DEFINE_EVENT(EVT_DEFAULT)
    
    /*!
     *    @brief    统一的回调方法
     */
    class USSDK_API USSDK_UNotifyMethodClass
    {
    public:
        /*!
         *    @brief    统一的回调方法
         *
         *    @param     anEventName     事件名字
         *    @param     aPara     事件承载结构
         *    @param     sourceName     event发起者，源头
         */
        virtual void onNotify(string anEventName, void* aPara, string sourceName)=0;
    };

	class USSDK_NotificationRegistrationAndCancelRegistration
	{
	public:
		virtual void addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID) = 0;
		virtual void addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID) = 0;

		virtual void removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName) = 0;
		virtual void removeObserver(USSDK_UNotifyMethodClass * observer) = 0;
	};

	class USSDK_PostNotification
	{
	public:
		virtual void postNotification(string anEventName, void* aPara, string sourceName) = 0;
	};
    
    
    /*!
     *    @brief    USSDK_CallBackInfo， 存储观察者信息
     */
    class USSDK_CallBackInfo
    {
    public:
        USSDK_UNotifyMethodClass *aCallBackHandler;
        int callBackThreadID;
    };
    
    /*!
     *    @brief    USSDK_NotificationCenter， 通知中心, （1）提供单例方法 （2）提供单个事件注册方法 （3）提供一个模块的整体事件注册 （4）提供通知方法 （5）通知采用 USSDK_UNotifyMethodClass 方法进行通知，需要观察者继承此类，并实现必要的方法。
     */
    typedef vector<USSDK_CallBackInfo> CallBackInfoVector;
    typedef map<string, CallBackInfoVector> SingleEventMap;
    
	class USSDK_NotificationCenter : public USSDK_NotificationRegistrationAndCancelRegistration, public USSDK_PostNotification
    {
    private:
        SingleEventMap aSingleEventMap;
        CallBackInfoVector allEventVector;
        USSDK_Mutex m_SingleEventMutex;
        USSDK_Mutex m_AllEventMutex;
    public:
        static USSDK_NotificationCenter *shareInstance();
        
    public:
        void addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID);
        void addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID);
        
        void removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName);
        void removeObserver(USSDK_UNotifyMethodClass * observer);
        
	public:
        void postNotification(string anEventName, void* aPara, string sourceName);
    };
    
}

#endif /* USSDK_NotificationCenter_hpp */
