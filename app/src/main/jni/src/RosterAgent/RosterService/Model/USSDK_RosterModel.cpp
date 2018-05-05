//
//  USSDK_RosterModel.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//
#include "USSDK_RosterModel.h"
#include "../../../Foundation/USSDKStringTool/USSDK_StringTool.hpp"
#include <string>
using namespace std;
using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_Foundation;

RosterBriefInfo::RosterBriefInfo()
{
	URI = "";
	name = "";
}

RosterBriefInfo::RosterBriefInfo(string uri, string name):URI(uri), name(name)
{

}

RosterBriefInfo::~RosterBriefInfo()
{

}

USSDK_Roster::USSDK_Roster()
{

}

USSDK_Roster::~USSDK_Roster()
{

}

void USSDK_Roster::generateDisplayName()
{
	if (name.length())
	{	
		displayName = name;
	}
	else
	{	
		displayName = USSDK_StringTool::subStrBetweenSeparators(URI, ":", "@");	
	}
}