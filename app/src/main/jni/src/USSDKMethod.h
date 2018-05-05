//
// USSDKMethod.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDKMethod_h
#define USSDKMethod_h

#ifdef _WIN32
#ifdef USSDK_EXPORTS
#define USSDK_API __declspec(dllexport)
#else
#define USSDK_API __declspec(dllimport)
#endif
#else
#define USSDK_API
#endif

#endif /* USSDKMethod_h */
