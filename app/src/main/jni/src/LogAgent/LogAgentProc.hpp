//
//  LogAgentProc.hpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/19.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef LogAgentProc_hpp
#define LogAgentProc_hpp

#include <stdio.h>
#include <string>

static std::string logAgentName = "logAgent";

#define logWriteEvent 3000

void logAgentThreadInit();
void logAgentThreadUnInit();

#endif /* LogAgentProc_hpp */
