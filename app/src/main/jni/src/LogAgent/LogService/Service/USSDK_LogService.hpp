//
//  USSDK_LogService.hpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/19.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_LogService_hpp
#define USSDK_LogService_hpp

#include <stdio.h>
#include "../../../Foundation/USSDKBase/USSDK_BaseClass.hpp"
#include "../Model/USSDK_LogDataModel.hpp"
#include <string>

using namespace std;
using namespace NS_USSDK_Foundation;

namespace NS_USSDK_LogService
{
    /*!
     *    @brief   logService
     */
    class USSDK_LogService : public USSDK_BaseClass
    {
    private:
        FILE *fp;
        
        string logFileDirPath();
        string logFilePath();
        string oldLogFilePath();

    public:
        string logRootPath;
        
    public:
        static USSDK_LogService *shareInstance();
        
    public:
        void threadInit();
        void threadUnInit();
        
    public:
        bool log(USSDK_LogDataModel *req);
    };
}
#endif /* USSDK_LogService_hpp */
