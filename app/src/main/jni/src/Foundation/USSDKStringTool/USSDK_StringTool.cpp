//
//  USSDK_StringTool.cpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/21.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_StringTool.hpp"
#include "../USSDKFile/USSDK_FileManager.hpp"

using namespace NS_USSDK_Foundation;

string USSDK_StringTool::lastPathCompent(string aPath)
{
    string::size_type pos;
    pos = aPath.rfind(pathSeparator);
    
    if (pos != aPath.npos)
    {
        return aPath.substr(pos+1, aPath.size());
    }
    else
    {
        return aPath;
    }
}

string USSDK_StringTool::lastCompentUseSeparator(string aInString, string aSeparator)
{
	string::size_type pos;
	pos = aInString.rfind(aSeparator);

	if (pos != aInString.npos)
	{
		return aInString.substr(pos+ aSeparator.size(), aInString.size());
	}
	else
	{
		return aInString;
	}
}

string USSDK_StringTool::subStrBetweenSeparators(string aInString, string aBeginSeparator, string anEndSeparator)
{
	string::size_type aBeginPos, anEndPos;
	aBeginPos = aInString.find(aBeginSeparator);
	anEndPos = aInString.rfind(anEndSeparator);

	if (aBeginPos > anEndPos)
	{
		return aInString;
	}
	
	if (aBeginPos != aInString.npos && anEndPos != aInString.npos)
	{
		return aInString.substr(aBeginPos+ aBeginSeparator.size(), anEndPos - aBeginPos - aBeginSeparator.size());
	}
	
	if (aBeginPos != aInString.npos)
	{
		return aInString.substr(aBeginPos+ aBeginSeparator.size(), aInString.size());
	}

	return aInString;
}

void USSDK_StringTool::makerUpper(string &aInString)
{
	transform(aInString.begin(), aInString.end(), aInString.begin(), ::toupper);  
}

void USSDK_StringTool::makerLower(string &aInString)
{
	transform(aInString.begin(), aInString.end(), aInString.begin(), ::tolower);  
}