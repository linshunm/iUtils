//
//  USSDK_StringTool.hpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/21.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_StringTool_hpp
#define USSDK_StringTool_hpp

#include <stdio.h>
#include <string>
#include <algorithm>

using namespace std;
namespace NS_USSDK_Foundation
{
    class USSDK_StringTool
    {
    public:
        static string lastPathCompent(string aPath);
		static string lastCompentUseSeparator(string aInString, string aSeparator);
		static string subStrBetweenSeparators(string aInString, string aBeginSeparator, string anEndSeparator);
		static void makerUpper(string &aInString);
		static void makerLower(string &aInString);
    };
}

#endif /* USSDK_StringTool_hpp */
