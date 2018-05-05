//
//  USSDK_RosterLogoManager.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_RosterLogoManager_h
#define USSDK_RosterLogoManager_h

#include <stdio.h>
#include "../Model/USSDK_RosterModel.h"

namespace NS_USSDK_RosterService
{
	//C:\Users\Administrator\AppData\Roaming\eGroup\10145777@zte.com.cn\Avatar
	//C:\Users\Administrator\AppData\Roaming\eGroup\10145777@zte.com.cn\Avatar\ContactFace\00009552@zte.com.cn
	//C:\Users\Administrator\AppData\Roaming\eGroup\10145777@zte.com.cn\Avatar\GroupFace
	//C:\Users\Administrator\AppData\Roaming\eGroup\10145777@zte.com.cn\Avatar\PubAccountFace\career

	static std::string subDirOfLogo = "Avatar";
	static std::string singleContactLogoBaseName = "ContactFace";
	static std::string groupLogoBaseName = "GroupFace";
	static std::string pubAccountLogoBaseName = "PubAccountFace";

    //(1) logo dir check, create (2) logo file,  save, remove
	class USSDK_RosterLogoManager
	{
	private:
		string rootDirPath;
		string userURI;

		string singleContactLogoDir;
		string groupLogoDir;
		string pubaccLogoDir;

	public:
		USSDK_RosterLogoManager(string aRootDirPath, string anUserURI);
		USSDK_RosterLogoManager();
		~USSDK_RosterLogoManager();

	public:
		bool isExistLogoOfRoster(string aRosterURI, RosterType aRosterType, string logoIndex);
		string logoPathOfRoster(string aRosterURI, RosterType aRosterType, string logoIndex);
		
		bool coverRosterLogo(string aRosterURI, RosterType aRosterType, string logoIndex, string sourcePath, bool isRemoveSource = true);
		bool deleteLogoOfRoster(string aRosterURI, RosterType aRosterType, string logoIndex);

	private:
		string logoDirOfRoster(string aRosterURI, RosterType aRosterType);
		bool checkCurrentDir();
	};
}

#endif /* USSDK_RosterLogoManager_h */
