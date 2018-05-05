//
//  USSDK_SingleContact.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_SingleContact_h
#define USSDK_SingleContact_h

#include <stdio.h>
#include <string>
#include <map>
#include "USSDK_RosterModel.h"
#include "../../../USSDKMethod.h"

using namespace std;
using namespace NS_USSDK_RosterService;

namespace NS_USSDK_RosterService
{

	class USSDK_Roster;
	class USSDK_SingleContact;

	typedef map<string, USSDK_SingleContact> USSDK_SingleContactMap;

	typedef enum
	{
		NonFriend_Operate_Nothing,
		NonFriend_Operate_Accept,
		NonFriend_Operate_Refuse,
	}NonFriend_Operate;

	typedef enum
	{
		UserStatus_init,
		UserStatus_Online,
		UserStatus_Busy,
		UserStatus_Away,
		UserStatus_Dining,
		UserStatus_Hidden,
		UserStatus_Offline,
		UserStatus_Calling,
		UserStatus_Donthinder,
		UserStatus_Conference,
		UserStatus_Custom,
	}UserStatus;

	typedef enum
	{
		UserCapability_Audio = 1 << 0,
		UserCapability_Video = 1 << 1,
		UserCapability_OperationReport = 1 << 2,
		UserCapability_Pstn = 1 << 3,
		UserCapability_SendOrigPhoto = 1 << 4,
		UserCapability_Voip = 1 << 5,
		UserCapability_Receipt = 1 << 6,
		UserCapability_TransFile = 1 << 7,
		UserCapability_WaterMark = 1 << 8,
	}UserCapability;

	class USSDK_API USSDK_SingleContact : public USSDK_Roster
	{ 
	public:
		int userCapability;
		DataGetStatus logoGetStatus;
		string logo;
		string sign;
		UserStatus userStatus;
		
	public:
		USSDK_SingleContact();
		~USSDK_SingleContact();

	public:
		void reset();

	public:
		bool isFriend();
		void setIsFriend(bool isFriend);
		bool isMyself();
		void setIsMyself();

	public:
		void addOneCapability(UserCapability aUserCapability);
		void removeOneCapability(UserCapability aUserCapability);

		bool isAudioEnable();
		bool isVideoEnable();
		bool isOperationReportEnable();
		bool isPstnEnable();
		bool isSendOrigPhotoEnable();
		bool isVoipEnable();
		bool isReceiptEnable();
		bool isWaterMarkEnable();

	public:
		bool isDefaultLogo();
		//		string getUserStatusString();

	public:
		bool merger(USSDK_SingleContact &aCompare, bool isBriefInfo);
	};

	class IMSingleContact
	{
		string URI;
		string name;
		string mobile;
		string groupID;
		bool isMoa;
	};
}
#endif /* USSDK_SingleContact_h */
