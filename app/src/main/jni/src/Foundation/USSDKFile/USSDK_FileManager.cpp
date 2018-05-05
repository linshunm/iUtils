//
//  USSDK_FileManager.cpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/20.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#include "USSDK_FileManager.hpp"

#ifdef _WIN32
#include "../../../stdafx.h"
#include <tchar.h>
#include <errno.h>
#else
#include <unistd.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#endif

using namespace std;
using namespace NS_USSDK_Foundation;

bool USSDK_FileManager::isFileExist(string aFilePath)
{
#ifdef _WIN32
	USES_CONVERSION;
	CString filePath =  A2CT(aFilePath.c_str());

	int nRet = _taccess(filePath, 0);
	return 0 == nRet || EACCES == nRet;
#else
    if(0 != access(aFilePath.c_str(), F_OK))
    {
        return false;
    }
    else
    {
        return true;
    }
#endif
}

bool USSDK_FileManager::fixDirPath(string aDirPath)
{
#ifdef _WIN32
	if (!isFileExist(aDirPath))
	{
		USES_CONVERSION;
		CString dirPath =  A2CT(aDirPath.c_str());

		CreateDirectory(dirPath, NULL);
	}

	return true;
#else
    if (!isFileExist(aDirPath))
    {
        mkdir(aDirPath.c_str(), 0777);
    }
    
    return true;
#endif
}

bool USSDK_FileManager::copyFile(string aSourcePath, string aDestPath)
{
#ifdef _WIN32
	USES_CONVERSION;
	return CopyFile(A2CT(aSourcePath.c_str()), A2CT(aSourcePath.c_str()), FALSE);
#else
    if (!aSourcePath.compare(aDestPath))
    {
        return false;
    }
    
    ifstream in(aSourcePath.c_str(), ios::binary);
    if (!in.is_open())
    {
        return false;
    }
    
    ofstream out(aDestPath.c_str(), ios::binary);
    if (!out.is_open())
    {
        in.close();
        return false;
    }
    
    char buf[2048];
    long long totalBytes = 0;
    while (in)
    {
        in.read(buf, 2048);
        
        out.write(buf, in.gcount());
    }
    
    in.close();
    out.close();
    
    return true;
#endif
}

bool USSDK_FileManager::removeFile(string aPath)
{
    return remove(aPath.c_str());
}
