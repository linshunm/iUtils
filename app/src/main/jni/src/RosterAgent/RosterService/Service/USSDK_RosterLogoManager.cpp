//
//  USSDK_RosterLogoManager.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "USSDK_RosterLogoManager.h"
#include "../../../Foundation/USSDKFile/USSDK_FileManager.hpp"
#include "../../../Foundation/USSDKStringTool/USSDK_StringTool.hpp"

using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_Foundation;

USSDK_RosterLogoManager::USSDK_RosterLogoManager(string aRootDirPath, string anUserURI)
{
	rootDirPath = aRootDirPath;

	if (userURI.compare(anUserURI))
	{
		userURI = anUserURI;
		checkCurrentDir();
	}
}

USSDK_RosterLogoManager::USSDK_RosterLogoManager()
{
	
}

USSDK_RosterLogoManager::~USSDK_RosterLogoManager()
{

}

bool USSDK_RosterLogoManager::isExistLogoOfRoster(string aRosterURI, RosterType aRosterType, string logoIndex)
{
	string logoPath = logoPathOfRoster(aRosterURI, aRosterType, logoIndex);
	return USSDK_FileManager::isFileExist(logoPath);
}

string USSDK_RosterLogoManager::logoPathOfRoster(string aRosterURI, RosterType aRosterType, string logoIndex)
{
	return logoDirOfRoster(aRosterURI, aRosterType) + pathSeparator + logoIndex;
}

bool USSDK_RosterLogoManager::coverRosterLogo(string aRosterURI, RosterType aRosterType, string logoIndex, string sourcePath, bool isRemoveSource)
{
	string destPath = logoPathOfRoster(aRosterURI, aRosterType, logoIndex);

	bool result = USSDK_FileManager::copyFile(sourcePath, destPath);
	if (!isRemoveSource)
	{
		return result;
	}
	else
	{
		return USSDK_FileManager::removeFile(sourcePath);
	}
}

bool USSDK_RosterLogoManager::deleteLogoOfRoster(string aRosterURI, RosterType aRosterType, string logoIndex)
{
	string destPath = logoPathOfRoster(aRosterURI, aRosterType, logoIndex);

	return USSDK_FileManager::removeFile(destPath);
}

string USSDK_RosterLogoManager::logoDirOfRoster(string aRosterURI, RosterType aRosterType)
{
	switch (aRosterType)
	{
	case RosterType_SingleContact:
		{
			string anRosterDir = singleContactLogoDir + pathSeparator + USSDK_StringTool::lastCompentUseSeparator(aRosterURI, ":");
			USSDK_FileManager::fixDirPath(anRosterDir);
			
			return anRosterDir;
		}
		break;
	case RosterType_Group:
		{
			return groupLogoDir;
		}
		break;
	case RosterType_Pubacc:
		{
			string anRosterDir = pubaccLogoDir + pathSeparator + USSDK_StringTool::lastCompentUseSeparator(aRosterURI, ":");
			USSDK_FileManager::fixDirPath(anRosterDir);

			return anRosterDir;
		}
		break;
	default:
		{
			return NULL;
		}
	}
}

bool USSDK_RosterLogoManager::checkCurrentDir()
{
	//check dir for singleContact, group, pubacc
	string anUserDir = rootDirPath + pathSeparator + USSDK_StringTool::lastCompentUseSeparator(userURI, ":");
	USSDK_FileManager::fixDirPath(anUserDir);

	string aSubDirOfLogo = anUserDir + pathSeparator + subDirOfLogo;
	USSDK_FileManager::fixDirPath(aSubDirOfLogo);

	singleContactLogoDir = aSubDirOfLogo + pathSeparator + singleContactLogoBaseName;
	USSDK_FileManager::fixDirPath(singleContactLogoDir);

	groupLogoDir = aSubDirOfLogo + pathSeparator + groupLogoBaseName;
	USSDK_FileManager::fixDirPath(groupLogoDir);

	pubaccLogoDir = aSubDirOfLogo + pathSeparator + pubAccountLogoBaseName;
	USSDK_FileManager::fixDirPath(pubaccLogoDir);

	return true;
}
