//
//  USSDK_RosterModel.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_RosterModel_h
#define USSDK_RosterModel_h

#include <stdio.h>
#include <string>

using namespace std;

namespace NS_USSDK_RosterService
{
	typedef enum
	{
		RosterType_SingleContact,
		RosterType_Group,
		RosterType_Pubacc,
	}RosterType;

	typedef enum
	{
		//from strong relationship
		RosterSource_Default, 
		RosterSource_MessageArrived,
		RosterSource_Employee,
		RosterSource_GroupInfo,
	}RosterSource;

	typedef enum
	{
		DataGetStatus_Never,
		DataGetStatus_Getting,
		DataGetStatus_Success,
		DataGetStatus_Failed,
	}DataGetStatus;

	typedef enum
	{
		RosterRelationShip_Weak,
		RosterRelationShip_Strong,
		RosterRelationShip_MySelf,
	}RosterRelationShip;

	class RosterBriefInfo
	{
	public:
		string URI;
		string name;
		RosterBriefInfo();
		RosterBriefInfo(string uri, string name);
		~RosterBriefInfo();
	};

	class USSDK_Roster
	{
	public:
		string URI;
		string identifier;
		string domain;

		string name;
		string enName;
		string pinyinName;

		string displayName;
		string displayNamePinyin;

		RosterRelationShip rosterRelationShip;

	public:
		DataGetStatus detailGetStatus;
		string dataEtag;
		string tmpDataEtag;
	
	public:
		USSDK_Roster();
		~USSDK_Roster();

	public:
		void generateDisplayName();
	};


}
#endif /* USSDK_RosterModel_h */
