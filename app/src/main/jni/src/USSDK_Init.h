//
//  USSDK_Init.h
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/12.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_Init_h
#define USSDK_Init_h

#include <stdio.h>
#include <string>
#include "USSDKMethod.h"
#include "OcxAdapter/USSDK_EventWrapper.h"

using namespace NS_USSDK_DEFAULT;

namespace NS_USSDK_DEFAULT
{
    /*!
     @defgroup Group_USSDK_DEFAULT USSDK启动模块
     */
    
    class USSDK_API USSDK_Main
    {
    public:
        /*!
         * @addtogroup Group_USSDK_DEFAULT
         * @{
         */
		
        /*!
         *    @brief    startUp all agent
         *
         *    @remark   ios上两个目录相同，AD待确认
         *
         *    @param     anUserDataPath     用户数据根目录: 图片/语音等数据
         *    @param     anAppRunDataPath   APP运行目录: log
         *    @param     anPrivatePath      私密数据区目录: db
         *
         *    @return    返回为空
         */
		static void USSDK_Init(std::string anUserDataPath, std::string anAppRunDataPath, std::string anPrivatePath);
        static void setEventWrapper(USSDK_EventWrapper *anEventWrapper);
        
        /*!
         * @} // Group_USSDK_DEFAULT
         */
    };
}

    
#endif /* USSDK_Init_h */
