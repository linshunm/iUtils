//
//  USSDK_SingleContactModel.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//
#include "USSDK_SingleContactModel.h"
#include <string>
#include "../../../Foundation/USSDKBitOperation/USSDK_BitOperation.hpp"

using namespace std;
using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_Foundation;

USSDK_SingleContact::USSDK_SingleContact()
{
	reset();
}

USSDK_SingleContact::~USSDK_SingleContact()
{

}

void USSDK_SingleContact::reset()
{
	userCapability = 0;
	logoGetStatus = DataGetStatus_Never;
	logo = "";
	sign = "";
	userStatus = UserStatus_init;

	URI = "";
	identifier = "";
	domain = "";
	name = "";
	enName = "";
	pinyinName = "";
	displayName = "";
	displayNamePinyin = "";
	rosterRelationShip = RosterRelationShip_Weak;
	detailGetStatus = DataGetStatus_Never;
	dataEtag = "";
	tmpDataEtag = "";
}

bool USSDK_SingleContact::isFriend()
{
	return rosterRelationShip == RosterRelationShip_Strong; 
}

void USSDK_SingleContact::setIsFriend(bool isFriend)
{
	if (isFriend)
	{
		rosterRelationShip = RosterRelationShip_Strong;
	}
	else
	{
		rosterRelationShip = RosterRelationShip_Weak;
	}
}

bool USSDK_SingleContact::isMyself()
{
	return rosterRelationShip == RosterRelationShip_MySelf; 
}

void USSDK_SingleContact::setIsMyself()
{
	rosterRelationShip = RosterRelationShip_MySelf;
}

void USSDK_SingleContact::addOneCapability(UserCapability aUserCapability)
{
	userCapability = BitAdd(userCapability, (int)aUserCapability);
}

void USSDK_SingleContact::removeOneCapability(UserCapability aUserCapability)
{
	userCapability = BitRemoveB(userCapability, (int)aUserCapability);
}

bool USSDK_SingleContact::isAudioEnable()
{
	return BitHasB(userCapability, (int)UserCapability_Audio);
}

bool USSDK_SingleContact::isVideoEnable()
{
	return BitHasB(userCapability, (int)UserCapability_Video);
}

bool USSDK_SingleContact::isOperationReportEnable()
{
	return BitHasB(userCapability, (int)UserCapability_OperationReport);
}

bool USSDK_SingleContact::isPstnEnable()
{
	return BitHasB(userCapability, (int)UserCapability_Pstn);
}

bool USSDK_SingleContact::isSendOrigPhotoEnable()
{
	return BitHasB(userCapability, (int)UserCapability_SendOrigPhoto);
}

bool USSDK_SingleContact::isVoipEnable()
{
	return BitHasB(userCapability, (int)UserCapability_Voip);
}

bool USSDK_SingleContact::isReceiptEnable()
{
	return BitHasB(userCapability, (int)UserCapability_Receipt);
}

bool USSDK_SingleContact::isWaterMarkEnable()
{
	return BitHasB(userCapability, (int)UserCapability_WaterMark);
}

bool USSDK_SingleContact::isDefaultLogo()
{
	if (logo.length() <= 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool NS_USSDK_RosterService::USSDK_SingleContact::merger(USSDK_SingleContact &aCompare, bool isBriefInfo )
{
	bool isEqual = true;

	if (name.compare(aCompare.name))
	{
		isEqual = false;
		name = aCompare.name;
	}

	if (rosterRelationShip != aCompare.rosterRelationShip)
	{
		isEqual = false;
		rosterRelationShip = aCompare.rosterRelationShip;
	}

	if (displayName != aCompare.displayName)
	{
		isEqual = false;
		displayName = aCompare.displayName;
	}

	if (isBriefInfo)
	{
		return isEqual;
	}
    
    return isEqual;
}
