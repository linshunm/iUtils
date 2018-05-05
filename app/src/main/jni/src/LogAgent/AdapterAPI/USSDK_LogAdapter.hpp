//
//  USSDK_LogAdapter.hpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/19.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_LogAdapter_hpp
#define USSDK_LogAdapter_hpp

#include <stdio.h>
#include <string>

namespace NS_USSDK_LogService
{
    /*!
     *    @brief  logService out
     */
    class USSDK_LogServiceAdapter
    {
    public:
		static bool log(std::string aModule, int iLevel, const char* sFile, const char* sFunc, int iLine, const char* sFormat, ...);
    };

	enum LOG_LEVEL_ENUM
	{
		LOG_LEVEL_DEBUGGING = 1,
		LOG_LEVEL_INFORMATION,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR,
	};

	#define LOG(Module,LEVEL, FORMAT, ...) USSDK_LogServiceAdapter::log(Module, LEVEL, __FILE__, __FUNCTION__, __LINE__, FORMAT, ## __VA_ARGS__);

	#define LOG_DEBUGGING(Module,FORMAT, ...) LOG(Module,LOG_LEVEL_DEBUGGING, FORMAT, ## __VA_ARGS__)
	#define LOG_INFORMATION(Module,FORMAT, ...) LOG(Module,LOG_LEVEL_INFORMATION, FORMAT, ## __VA_ARGS__)
	#define LOG_WARNING(Module,FORMAT, ...) LOG(Module,LOG_LEVEL_WARNING, FORMAT, ## __VA_ARGS__)
	#define LOG_ERROR(Module,FORMAT, ...) LOG(Module,LOG_LEVEL_ERROR, FORMAT, ## __VA_ARGS__)
}



#endif /* USSDK_LogAdapter_hpp */
