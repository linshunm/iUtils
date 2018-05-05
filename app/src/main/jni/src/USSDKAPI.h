//
//  USSDKAPI.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDKAPI_h
#define USSDKAPI_h

//export
#include "USSDKMethod.h"

//ussdk startup
#include "USSDK_Init.h"

//foundation
#include "./Foundation/NotificationCenter/USSDK_NotificationCenter.h"
#include "./Foundation/USSDKMutex/USSDK_Mutex.hpp"

//login
#include "./LoginAgent/LoginAgentHeader.h"

#include "./LoginAgent/AdapterAPI/USSDK_LoginServiceAdapter.h"
#include "./LoginAgent/LoginService/Model/USSDK_LoginModel.h"
#include "./LoginAgent/LoginService/NotifyAPI/USSDK_LoginServiceCallBack.h"

//roster
#include "./RosterAgent/RosterAgentHeader.h"

#include "./RosterAgent/AdapterAPI/USSDK_RosterServiceAdapter.h"
#include "./RosterAgent/RosterService/NotifyAPI/USSDK_RosterServiceCallBack.h"

#include "./RosterAgent/RosterService/Model/USSDK_RosterModel.h"
#include "./RosterAgent/RosterService/Model/USSDK_SingleContactModel.h"
#include "./RosterAgent/RosterService/Model/USSDK_GroupModel.h"
#include "./RosterAgent/RosterService/Model/USSDK_PubaccModel.h"
#include "./RosterAgent/RosterService/Model/USSDK_EmployeeModel.h"
#include "./RosterAgent/RosterService/Model/USSDK_RosterListModel.h"
#include "./RosterAgent/RosterService/Model/USSDK_MergerModel.h"

//bctl
#include "./clt_include/OcxInterface.h"
#include "./clt_include/OcxZXInterface.h"

//ocxAdapter, tmp, need remove when ocxAdapter useless
#include "./OcxAdapter/USSDK_OcxAdapter.h"
#include "./OcxAdapter/USSDK_OcxAdapterModel.h"
#include "./clt_include/CtlEventWrapper.h"

//callBack
#include "./OcxAdapter/USSDK_EventWrapper.h"



#endif /* USSDKAPI_h */
