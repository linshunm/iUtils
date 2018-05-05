//
//  USSDK_FileManager.hpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/20.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_FileManager_hpp
#define USSDK_FileManager_hpp

#include <stdio.h>
#include <string>

using namespace std;
namespace NS_USSDK_Foundation
{
#ifdef _WIN32
	static std::string pathSeparator = "\\";
#else
	static std::string pathSeparator = "/";
#endif

    class USSDK_FileManager
    {
    public:
        
        static bool isFileExist(string aFilePath);
        static bool fixDirPath(string aDirPath);
		static bool copyFile(string aSourcePath, string aDestPath);
		static bool removeFile(string aPath);
    };
}

#endif /* USSDK_FileManager_hpp */
