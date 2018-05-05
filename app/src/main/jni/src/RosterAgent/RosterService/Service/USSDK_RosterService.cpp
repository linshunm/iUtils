//
//  USSDK_RosterService.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "USSDK_RosterService.h"

#include "../../RosterAgentProc.hpp"
#include "../../RosterAgentProcTransModel.hpp"
#include "../NotifyAPI/USSDK_RosterServiceCallBack.h"

#include "../../../LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"
#include "../../../OcxAdapter/USSDK_OcxAdapterHeader.h"
#include ".././../../clt_include/ussdk_Typedefs.h"

using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_LogService;
using namespace NS_USSDK_OcxAdapter;

USSDK_RosterService *USSDK_RosterService::shareInstance()
{
    static USSDK_RosterService instance;
    return &instance;
}

void USSDK_RosterService::threadInit()
{
    if (!isThreadInit)
    {
        rosterAgentThreadInit();
        isThreadInit = true;
    }
}

void USSDK_RosterService::threadUnInit()
{
    if (isThreadInit)
    {
		rosterAgentThreadUnInit();
        isThreadInit = false;
    }
}

string USSDK_RosterService::logoPathOfRoster(string rosterURI, RosterType aRosterType)
{
	if (aRosterType == RosterType_SingleContact)
	{
		USSDK_SingleContact aSingleContact;
		if (!getOneSingleContact(rosterURI, aSingleContact))
		{
			return "";
		}

		if (aSingleContact.isDefaultLogo())
		{
			return "";
		}

		if (rosterLogoManager.isExistLogoOfRoster(rosterURI, RosterType_SingleContact, aSingleContact.logo))
		{
			return rosterLogoManager.logoPathOfRoster(rosterURI, RosterType_SingleContact, aSingleContact.logo);
		}

		return "";
	}
	
	if (aRosterType == RosterType_Group)
	{
		return "";
	}

	if (aRosterType == RosterType_Pubacc)
	{
		return "";
	}
	
	return "";
}

USSDK_SingleContactMap USSDK_RosterService::getFriendList()
{
	return friendList.getAll();
}

bool USSDK_RosterService::getOneSingleContact(string aRosterURI, USSDK_SingleContact& outBuffer)
{
	if (isMyself(aRosterURI))
	{
		outBuffer = myInfo;
		return true;
	}

	if (friendList.getElement(aRosterURI, outBuffer))
	{
		return true;
	}

	return nonFriendList.getElement(aRosterURI, outBuffer);
}

int USSDK_RosterService::friendCount()
{
	return friendList.countOfKey();
}

USSDK_GroupMap USSDK_RosterService::getPublicGroupList()
{
	return publicGroupList.getAll();
}

bool USSDK_RosterService::getOnePublicGroup(string aRosterURI, USSDK_Group &out)
{
	if (publicGroupList.getElement(aRosterURI, out))
	{
		return true;
	}

	return nonValidPublicGroupList.getElement(aRosterURI, out);
}

void USSDK_RosterService::loadCacheData(string anUserURI)
{
	LOG_DEBUGGING(rosterAgentName, "loadCacheData[%s]", anUserURI.c_str());
	if (userURI.compare(anUserURI))
	{
		RemoveCacheData();
	}

	userURI = anUserURI;

	rosterLogoManager = USSDK_RosterLogoManager(userDataPath, userURI);

	myInfo.URI = userURI;
	myInfo.generateDisplayName();
	myInfo.setIsMyself();
	
	//load user Info, when db okay
	rosterDBManager = USSDK_RosterDBManager(userDataPath, userURI);
	rosterDBManager.databaseInit();
	rosterDBManager.getAllRosterInfo(friendList);
}

//if exist, do nothing, if no, add to memory
void USSDK_RosterService::checkRoster(string aRosterURI, RosterType aRosterType, RosterSource aRosterSource)
{
	LOG_DEBUGGING(rosterAgentName, "checkRoster [%s][%d][%d]", aRosterURI.c_str(), aRosterType, aRosterSource);
	switch(aRosterType)
	{
	case RosterType_SingleContact:
		{
			USSDK_SingleContact aSingleContact;
			if (!getOneSingleContact(aRosterURI, aSingleContact))
			{
				aSingleContact.URI = aRosterURI;
				nonFriendList.add(aRosterURI, aSingleContact);
			}

			if (aSingleContact.detailGetStatus != DataGetStatus_Success)
			{
				USSDK_OcxAdapter::getOneRosterDetailInfo(aRosterURI);
			}
			else if (aSingleContact.logoGetStatus != DataGetStatus_Success && rosterLogoManager.isExistLogoOfRoster(aRosterURI, RosterType_SingleContact, aSingleContact.logo))
			{
				USSDK_OcxAdapter::getOneRosterPhotoInfo(aRosterURI, aSingleContact.logo);
			}

			break;
		}
	case RosterType_Group:
		{
			USSDK_Group aGroup;
			if (!getOnePublicGroup(aRosterURI, aGroup))
			{
				aGroup.URI = aRosterURI;
				nonValidPublicGroupList.add(aRosterURI, aGroup);
			}
			
			if (aGroup.isValid() && aGroup.detailGetStatus != DataGetStatus_Success)
			{
				USSDK_OcxAdapter::getOnePublicDetailInfo(aRosterURI);
			}

			break;
		}
	case RosterType_Pubacc:
		{
			break;
		}
	}
}

bool USSDK_RosterService::addOneSingleContact(string aRosterURI)
{
	LOG_DEBUGGING(rosterAgentName, "addOneSingleContact[%s]", aRosterURI.c_str());

	return USSDK_OcxAdapter::addOneAddressList(aRosterURI, NULL, NULL);
}

bool USSDK_RosterService::removeOneSingleContact(string aRosterURI)
{
	LOG_DEBUGGING(rosterAgentName, "removeOneSingleContact[%s]", aRosterURI.c_str());

	return USSDK_OcxAdapter::deleteOneAddressList(aRosterURI);
}

bool USSDK_RosterService::createPublicGroup(string aGroupName, vector<RosterBriefInfo> &aGroupMembersInfo, string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "createPublicGroup: memberCount[%d] anOperateID[%s]",  aGroupMembersInfo.size(),  anOperateID.c_str());

	publicGroupCreateCache[anOperateID] = aGroupMembersInfo;
	
	return USSDK_OcxAdapter::createOnePublicInfo(myInfo.displayName);
}

bool USSDK_RosterService::deletePublicGroupCreateCache(string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "deletePublicGroupCreateCache: anOperateID[%s]", anOperateID.c_str());
	publicGroupCreateCache.erase(anOperateID);
	return true;
}

//add, remove
bool USSDK_RosterService::inviteSomeoneToPublicGroup(string aGroupURI, RosterBriefInfo &aMemberInfo)
{
	LOG_DEBUGGING(rosterAgentName, "inviteSomeoneToPublicGroup: aGroupURI[%s] member[%s][%s]", aGroupURI.c_str(), aMemberInfo.URI.c_str(), aMemberInfo.name.c_str());
	return USSDK_OcxAdapter::inviteSomeoneToPublicGroup(aGroupURI, aMemberInfo.URI, aMemberInfo.name);
}

bool USSDK_RosterService::inviteMembersToPublicGroup(string aGroupURI, vector<RosterBriefInfo> &aMemberInfo)
{
	LOG_DEBUGGING(rosterAgentName, "inviteMembersToPublicGroup: aGroupURI[%s] memberCount[%d]", aGroupURI.c_str(), aMemberInfo.size());
	
	vector<RosterBriefInfo>::iterator it = aMemberInfo.begin();   
	for(; it != aMemberInfo.end(); ++it)  
	{  
		RosterBriefInfo *info = (RosterBriefInfo *)&it;
		
		LOG_DEBUGGING(rosterAgentName, "inviteMembersToPublicGroup: aGroupURI[%s] member[%s][%s]", aGroupURI.c_str(), info->URI.c_str(), info->name.c_str());
		USSDK_OcxAdapter::inviteSomeoneToPublicGroup(aGroupURI, info->URI, info->name);
	}  
	
	return true;
}

bool USSDK_RosterService::deleteSomeOneFromPublicGroup(string aGroupURI, string aMemberURI)
{
	LOG_DEBUGGING(rosterAgentName, "deleteSomeOneFromPublicGroup: aGroupURI[%s] member[%s]", aGroupURI.c_str(), aMemberURI.c_str());
	return USSDK_OcxAdapter::deleteSomeOneFromPublicGroup(aGroupURI, aMemberURI);
}

bool USSDK_RosterService::deleteMembersFromPublicGroup(string aGroupURI, vector<string> &aMembersURI)
{
	LOG_DEBUGGING(rosterAgentName, "deleteMembersFromPublicGroup: aGroupURI[%s] memberCount[%d]", aGroupURI.c_str(), aMembersURI.size());

	vector<string>::iterator it = aMembersURI.begin();   
	for(; it != aMembersURI.end(); ++it)  
	{  
		string aMemberURI = *it;

		LOG_DEBUGGING(rosterAgentName, "deleteMembersFromPublicGroup: aGroupURI[%s] member[%s]", aGroupURI.c_str(), aMemberURI.c_str());
		USSDK_OcxAdapter::deleteSomeOneFromPublicGroup(aGroupURI, aMemberURI);
	}  

	return true;
}

//modify
bool USSDK_RosterService::modifyPublicGroupAttribute(string aGroupURI, GroupModifyType aModifyType, int aNewIntValue, string aNewStrValue,  string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "modifyPublicGroupAttribute: aGroupURI[%s] GroupModifyType[%d] aNewIntValue[%d] aNewStrValue[%s] anOperateID[%s]", aGroupURI.c_str(), aModifyType, aNewIntValue, aNewStrValue.c_str(), anOperateID.c_str());

	if (!publicGroupList.hasElement(aGroupURI))
	{
		LOG_ERROR(rosterAgentName, "modifyPublicGroupAttribute: aGroupURI[%s] not exist", aGroupURI.c_str());
		return false;
	}

	string aModifyTypeDown;
	string aNewValue;

	if (aModifyType == GroupModifyType_GroupName)
	{
		aModifyTypeDown = "name";
		aNewValue = aNewStrValue;
	}
	else if (aModifyType == GroupModifyType_GroupTopic)
	{
		aModifyTypeDown = "bulletin";
		aNewValue = aNewStrValue;
	}
	else if (aModifyType == GroupModifyType_forbidShareQRCode)
	{
		aModifyTypeDown = "forbid_2D_join";
		if (aNewIntValue == 1)
		{
			aNewValue = "true";
		}
		else
		{
			aNewValue = "false";
		}
	}
	else if (aModifyType == GroupModifyType_AllowmemberInvite)
	{
		aModifyTypeDown = "member_invite_enable";
		if (aNewIntValue == 1)
		{
			aNewValue = "true";
		}
		else
		{
			aNewValue = "false";
		}
	}
	else if (aModifyType == GroupModifyType_GroupOwnerTansfer)
	{
		aModifyTypeDown = "owner";
		aNewValue = aNewStrValue;
	}

	return USSDK_OcxAdapter::modifyPublicGroupAttribute(aGroupURI, aModifyTypeDown, aNewValue);
}

//leave
bool USSDK_RosterService::leavePublicGroup(string aGroupURI, string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "leavePublicGroup: aGroupURI[%s] anOperateID[%s]", aGroupURI.c_str(), anOperateID.c_str());
	return USSDK_OcxAdapter::leavePublicGroup(aGroupURI);
}

bool USSDK_RosterService::deletePublicGroup(string aGroupURI)
{
	LOG_DEBUGGING(rosterAgentName, "leavePublicGroup: aGroupURI[%s]", aGroupURI.c_str());
	return USSDK_OcxAdapter::deletePublicGroup(aGroupURI);
}
//QRCode to join group
bool USSDK_RosterService::requestToJoinPublicGroup(string aGroupURI, string groupIdentifier, string myName, string shareUserURI, string shareUserName)
{
	LOG_DEBUGGING(rosterAgentName, "requestToJoinPublicGroup: aGroupURI[%s] groupIdentifier[%s] myName[%s] shareUser[%s-%s]", aGroupURI.c_str(), groupIdentifier.c_str(), myName.c_str(), shareUserURI.c_str(), shareUserName.c_str());
	return USSDK_OcxAdapter::requestToJoinPublicGroup(aGroupURI, groupIdentifier, myName, shareUserURI, shareUserName);
}

//USER_INFO--void*
void USSDK_RosterService::onGotOneSingleContactInfo(int result, string aRosterURI, void *aRosterInfo, bool isMyself)
{
	LOG_DEBUGGING(rosterAgentName, "onGotOneSingleContactInfo [%d][%s] isMyself[%d]", result, aRosterURI.c_str(), isMyself);
	//result = 0 means success, but now no etag @windows version, so we donot use it @20180420
	if (!result)
	{
		USSDK_SingleContact aSingleContact;
		if (!getOneSingleContact(aRosterURI, aSingleContact))
		{
			return;
		}

		generateSingleContactInfo( (USER_INFO *)aRosterInfo, aSingleContact);
		aSingleContact.detailGetStatus = DataGetStatus_Success;
		aSingleContact.generateDisplayName();
		saveOneSingleContactToMemory(aSingleContact);
		//save to db, TODO:

		//need to get logo
		checkRoster(aRosterURI, RosterType_SingleContact, RosterSource_Default);

		//notify
		SingleContactNotifyPara aPara;
		aPara.aNotifyType = SingleContactNotifyType_OneSingleContactInfoArrived;
		aPara.result = result;
		aPara.aRosterURI = aRosterURI;
		aPara.aSingleContact = &aSingleContact;

		postNotification(EVT_SingleContactNotify, &aPara, EVT_SourceName_RosterService);
	}
	else
	{
		//notify
		SingleContactNotifyPara aPara;
		aPara.aNotifyType = SingleContactNotifyType_OneSingleContactInfoArrived;
		aPara.result = result;
		aPara.aRosterURI = aRosterURI;

		postNotification(EVT_SingleContactNotify, &aPara, EVT_SourceName_RosterService);
	}
}

//USER_INFO--void*
void USSDK_RosterService::onGotOneSingleContactSubInfo(int result, int type, string aRosterURI, void *info, bool isMyself)
{
	
}


//LIST_INFO--void
void USSDK_RosterService::onGotSingleContactListInfo(string aRosterURI, string aRosterName, int result, int count, void *info)
{
	LOG_DEBUGGING(rosterAgentName, "onGotSingleContactListInfo, count[%d]", count);

	static vector<string> needInsertList;
	static vector<string> needUpdateList;

	if (count)
	{
		//refresh friendList, 
		USSDK_SingleContact aSingleContact;

		aSingleContact.setIsFriend(true);
		aSingleContact.URI = aRosterURI;
		aSingleContact.name = aRosterName;
		LIST_INFO *listInfo = (LIST_INFO *) info;

		if (aSingleContact.name.length() == 0)
		{
			USES_CONVERSION;
			aSingleContact.name = T2CA(listInfo->cRealName);
		}

		aSingleContact.generateDisplayName();
		LOG_DEBUGGING(rosterAgentName, "onGotSingleContactListInfo, oneSingleContact[%s][%s]", aSingleContact.URI.c_str(), aSingleContact.name.c_str());

		tmpFriendList.add(aSingleContact.URI, aSingleContact.URI);
	
		USSDK_SingleContact aSingleContactLocal;
		if (getOneSingleContact(aSingleContact.URI, aSingleContactLocal))
		{
			//if not equal, move
			if (!aSingleContactLocal.merger(aSingleContact, true))
			{
				needUpdateList.push_back(aSingleContactLocal.URI);
				friendList.moveFrom(nonFriendList, aSingleContactLocal, aSingleContactLocal.URI);
			}
		}
		else
		{
			needInsertList.push_back(aSingleContact.URI);
			friendList.cover(aSingleContact.URI, aSingleContact);
		}
	}
	else
	{
		LOG_DEBUGGING(rosterAgentName, "onGotSingleContactListInfo, report end");

		//report list over,  compare with tmpFriendList and FriendList, then saveToDB multi, then report
		vector<USSDK_SingleContact> notAtList;
		friendList.getNotAtList(tmpFriendList, notAtList);

		vector<USSDK_SingleContact>::iterator it = notAtList.begin();   
		for(; it != notAtList.end(); ++it)  
		{  
			USSDK_SingleContact *info = (USSDK_SingleContact *)&it;
			info->setIsFriend(false);
			friendList.moveTo(nonFriendList, *info, info->URI);
		}  

		//saveToDB
		vector<string>::iterator itt = needInsertList.begin();
		for(; itt != needInsertList.end(); ++itt)
		{
			USSDK_SingleContact insertSingleContact;
			if(friendList.getElement(*itt, insertSingleContact))
			{
				rosterDBManager.insertRosterInfo(insertSingleContact);
			}
		}

		itt = needUpdateList.begin();
		for(; itt != needUpdateList.end(); ++itt)
		{
			USSDK_SingleContact updateSingleContact;
			if(friendList.getElement(*itt, updateSingleContact))
			{
				rosterDBManager.updateRosterInfo(updateSingleContact);
			}
		}

		//clear static
		needInsertList.clear();
		needUpdateList.clear();

		SingleContactNotifyPara aPara;
		aPara.aNotifyType = SingleContactNotifyType_SingleContactListInfoArrived;

		postNotification(EVT_SingleContactNotify, &aPara, EVT_SourceName_RosterService);
	}
}

void USSDK_RosterService::onAddSingleContactResult(int result, string aRosterURI, string aRosterName)
{
	LOG_DEBUGGING(rosterAgentName, "onAddSingleContactResult::[%d][%s][%s]", result, aRosterURI.c_str(), aRosterName.c_str());

	if (result >= 200 && result <= 299)
	{
		USSDK_SingleContact singleContact;
		getOneSingleContact(aRosterURI, singleContact);

		singleContact.URI = aRosterURI;
		singleContact.name = aRosterName;
		singleContact.setIsFriend(true);
		singleContact.detailGetStatus = DataGetStatus_Never;	
		singleContact.generateDisplayName();

		nonFriendList.remove(aRosterURI);
		friendList.cover(aRosterURI, singleContact);

		//check detail
		checkRoster(aRosterURI, RosterType_SingleContact, RosterSource_Default);
	}

	SingleContactNotifyPara aPara;
	aPara.aNotifyType = SingleContactNotifyType_AddSingleContactResult;
	aPara.result = result;
	aPara.aRosterURI = aRosterURI;

	postNotification(EVT_SingleContactNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onRemoveSingleContactResult(int result, string aRosterURI, string aRosterName)
{
	LOG_DEBUGGING(rosterAgentName, "onRemoveSingleContactResult::[%d][%s][%s]", result, aRosterURI.c_str(), aRosterName.c_str());

	if (result == 200 || result == 404)
	{
		USSDK_SingleContact singleContact;
		getOneSingleContact(aRosterURI, singleContact);

		singleContact.URI = aRosterURI;
		singleContact.name = aRosterName;
		singleContact.setIsFriend(false);
		singleContact.detailGetStatus = DataGetStatus_Never;	

		friendList.remove(aRosterURI);
		nonFriendList.cover(aRosterURI, singleContact);
	}

	SingleContactNotifyPara aPara;
	aPara.aNotifyType = SingleContactNotifyType_RemoveSingleContactResult;
	aPara.result = result;
	aPara.aRosterURI = aRosterURI;

	postNotification(EVT_SingleContactNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onGotGroupListInfo(int num, void *info)
{
	LOG_DEBUGGING(rosterAgentName, "onGotGroupListInfo, count[%d]", num);

	FIRE_ALL_GROUP *aGroupInfo = (FIRE_ALL_GROUP *)info;
	vector<string> needInsertGroupList;
	vector<string> needUpdateGroupList;

	for (int i = 0; i < num; i++)
	{
		USSDK_Group aGroup;
		if (generatePublicGroupInfo(aGroupInfo, aGroup))
		{
			aGroup.setIsOwner(myInfo.URI);

			LOG_DEBUGGING(rosterAgentName, "onGotGroupListInfo, num[%d] [%s][%s]", i, aGroup.URI.c_str(), aGroup.name.c_str());

			USSDK_Group aGroupLocal;
			if (getOnePublicGroup(aGroup.URI, aGroupLocal))
			{
				//if not equal, move
				if (!aGroupLocal.merger(aGroup, true))
				{
					needUpdateGroupList.push_back(aGroupLocal.URI);
					publicGroupList.moveFrom(nonValidPublicGroupList, aGroupLocal, aGroupLocal.URI);
				}
			}
			else
			{
				needInsertGroupList.push_back(aGroup.URI);
				publicGroupList.cover(aGroup.URI, aGroup);
			}

			tmpPublicGroupList.add(aGroup.URI, aGroup.URI);
		}

		aGroupInfo++;
	}

	LOG_DEBUGGING(rosterAgentName, "onGotGroupListInfo, deal end");

	vector<USSDK_Group> notAtList;
	publicGroupList.getNotAtList(tmpPublicGroupList, notAtList);

	vector<USSDK_Group>::iterator it = notAtList.begin();   
	for(; it != notAtList.end(); ++it)  
	{  
		USSDK_Group *info = (USSDK_Group *)&it;

		info->setValid(false);
		publicGroupList.moveTo(nonValidPublicGroupList, *info, info->URI);
	}  

	//saveToDB: notAtList, 	vector<string> needInsertGroupList;
	//vector<string> needUpdateGroupList;

	PublicGroupNotifyPara aPara;
	aPara.aNotifyType = PublicGroupNotifyType_PublicGroupListInfoArrived;

	postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onGotOneGroupInfo(string aGroupName, string aGroupURI, string aOperUser, void *pAttribute, long iListNum, void *pList)
{
	LOG_DEBUGGING(rosterAgentName, "onGotOneGroupInfo, groupURI[%s]", aGroupURI.c_str());
	USSDK_Group aLocalGroup;
	if (!getOnePublicGroup(aGroupURI, aLocalGroup))
	{
		LOG_ERROR(rosterAgentName, "onGotOneGroupInfo,. unknow group[%s]", aGroupURI.c_str());
		return;
	}

	CompareResult aCompareResult;
	vector<RosterBriefInfo> aNewMember;
	aLocalGroup.merger(aGroupURI, aGroupName, pAttribute, pList, iListNum, aCompareResult, aNewMember);
	
	if (aCompareResult != CompareResult_Equal)
	{
		aLocalGroup.setIsOwner(myInfo.URI);
		//TODO: saveToDB
	}

	//add member to singleContact: TODO 
// 	if (aNewMember.size())
// 	{
// 		
// 	}

	PublicGroupNotifyPara aPara;
	aPara.aNotifyType = PublicGroupNotifyType_OnePublicGroupInfoArrived;
	aPara.aRosterURI = aGroupURI;

	postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onPublicGroupMemberNotify(long iType, string aGroupURI, string anOwnerURI, string aOperUser, string aMemberURI, string aMemberDisplay, string aMemberInfo, string aMemberPhone, string aMemberType, string aMemberCaps, long iStatus, string aMsgID)
{
	//iType: 1--add member 2-- member name changed 3--remove member
	LOG_DEBUGGING(rosterAgentName, "onPublicGroupMemberNotify, groupURI[%s] member[%d-%s-%s] aOperUser[%s]", aGroupURI.c_str(), iType, aMemberURI.c_str(), aMemberDisplay.c_str(), aOperUser.c_str());
	USSDK_Group aLocalGroup;
	if (!getOnePublicGroup(aGroupURI, aLocalGroup))
	{
		LOG_ERROR(rosterAgentName, "onPublicGroupMemberNotify, unknow group[%s]", aGroupURI.c_str());
		return;
	}

	bool isChanged = false;
	PublicGroupMemberOperateType aOperateType;
	if (iType == 1 && iStatus == 1)
	{
		aOperateType = PublicGroupMemberOperateType_invite;
		if (!aLocalGroup.existMember(aMemberURI))
		{
			isChanged = true;
			aLocalGroup.addMember(aMemberURI, aMemberDisplay);
			//TODO: add to singelContact
		}
	}
	else if (iType == 3)
	{
		aOperateType = PublicGroupMemberOperateType_remove;
		if (aLocalGroup.existMember(aMemberURI))
		{
			isChanged = true;
			aLocalGroup.removeMember(aMemberURI);
		}
	}
	
	//save to Group DB: TODO
	PublicGroupMemberChangedNotifyPara aPara;
	aPara.aRosterURI = aGroupURI;
	aPara.aOperateType = aOperateType;
	aPara.operaterInfo = RosterBriefInfo(aOperUser, "");
	aPara.memberInfo = RosterBriefInfo(aMemberURI, aMemberDisplay);

	postNotification(EVT_PublicGroupMemberChangedNotify, &aPara, EVT_SourceName_RosterService);
}

//req-->res
void USSDK_RosterService::onCreatePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "onCreatePublicGroupResult, result[%d] aGroupURI[%s] aOperUser[%s]", result, aGroupURI.c_str(), anOperateID.c_str());
	if (result == 200 || result == 201)
	{
		USSDK_Group aLocalGroup;
		aLocalGroup.URI = aGroupURI;
		aLocalGroup.ownerURI = myInfo.URI;
		aLocalGroup.setValid(true);
		aLocalGroup.setIsOwner(myInfo.URI);
		aLocalGroup.addMember(myInfo.URI, myInfo.displayName);

		vector<RosterBriefInfo> anExpectMembers = publicGroupCreateCache[anOperateID];

		vector<RosterBriefInfo>::iterator it = anExpectMembers.begin();   
		for(; it != anExpectMembers.end(); ++it)  
		{
			RosterBriefInfo *info = (RosterBriefInfo *)&it;
			aLocalGroup.addExpectMember(info->URI, info->name);
			
			inviteSomeoneToPublicGroup(aGroupURI, *info);
		}

		publicGroupList.moveFrom(nonValidPublicGroupList, aLocalGroup, aGroupURI);
	}

	PublicGroupNotifyPara aPara;
	aPara.aNotifyType = PublicGroupNotifyType_onCreatePublicGroupResult;
	aPara.aRosterURI = aGroupURI;
	aPara.result = result;

	postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onInviteSomeoneToPublicGroupResult(int result, string aGroupURI, string anMemberURI)
{
	LOG_DEBUGGING(rosterAgentName, "onInviteSomeoneToPublicGroupResult, result[%d] aGroupURI[%s] anMemberURI[%s]", result, aGroupURI.c_str(), anMemberURI.c_str());

	USSDK_Group aLocalGroup;
	if (!getOnePublicGroup(aGroupURI, aLocalGroup))
	{
		LOG_ERROR(rosterAgentName, "onInviteSomeoneToPublicGroupResult, unknow group[%s]", aGroupURI.c_str());
		return;
	}

	if (result == 1)
	{
		if (!aLocalGroup.existMember(anMemberURI))
		{
			USSDK_SingleContact aSingleContact;
			aSingleContact.URI = anMemberURI;
			getOneSingleContact(anMemberURI, aSingleContact);
			
			aLocalGroup.removeExpectMember(anMemberURI);
			aLocalGroup.addMember(anMemberURI,aSingleContact.displayName);

			publicGroupList.moveFrom(nonValidPublicGroupList, aLocalGroup, aGroupURI);
		}
	}

	PublicGroupNotifyPara aPara;
	aPara.aNotifyType = PublicGroupNotifyType_onInviteSomeoneToPublicGroupResult;
	aPara.aRosterURI = aGroupURI;
	aPara.result = result;
	aPara.aMemberURI = anMemberURI;

	postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::ondeleteMemberFromPublicGroupResult(int result, string aGroupURI, string anMemberURI, string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "ondeleteMemberFromPublicGroupResult, result[%d] aGroupURI[%s] anMemberURI[%s]", result, aGroupURI.c_str(), anMemberURI.c_str());

	USSDK_Group aLocalGroup;
	if (!getOnePublicGroup(aGroupURI, aLocalGroup))
	{
		LOG_ERROR(rosterAgentName, "onInviteSomeoneToPublicGroupResult, unknow group[%s]", aGroupURI.c_str());
		return;
	}

	if (result == 404 || result == 200)
	{
		if (aLocalGroup.existMember(anMemberURI))
		{
			USSDK_SingleContact aSingleContact;
			aSingleContact.URI = anMemberURI;
			getOneSingleContact(anMemberURI, aSingleContact);

			aLocalGroup.removeMember(anMemberURI);

			publicGroupList.moveFrom(nonValidPublicGroupList, aLocalGroup, aGroupURI);
		}
	}

	PublicGroupNotifyPara aPara;
	aPara.aNotifyType = PublicGroupNotifyType_ondeleteMemberFromPublicGroupResult;
	aPara.aRosterURI = aGroupURI;
	aPara.result = result;
	aPara.aMemberURI = anMemberURI;

	postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onModifyPublicGroupAttributeResult(int result, string aGroupURI, string aType, string aNewValue, string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "onModifyPublicGroupAttributeResult, result[%d] aGroupURI[%s] aType[%s][%s][%s]", result, aGroupURI.c_str(), aType.c_str(), aNewValue.c_str(), anOperateID.c_str());

	USSDK_Group aLocalGroup;
	if (!getOnePublicGroup(aGroupURI, aLocalGroup))
	{
		LOG_ERROR(rosterAgentName, "onInviteSomeoneToPublicGroupResult, unknow group[%s]", aGroupURI.c_str());
		return;
	}

	GroupModifyType aModifyType;

	if (!aType.compare("name"))
	{
		aModifyType = GroupModifyType_GroupName;
	}
	else if (!aType.compare("bulletin"))
	{
		aModifyType = GroupModifyType_GroupTopic;
	}
	else if (!aType.compare("forbid_2D_join"))
	{
		aModifyType = GroupModifyType_forbidShareQRCode;
	}
	else if (!aType.compare("member_invite_enable"))
	{
		aModifyType = GroupModifyType_AllowmemberInvite;
	}
	else if (!aType.compare("owner"))
	{
		aModifyType = GroupModifyType_GroupOwnerTansfer;
	}

	if (result == 200 || result == 202)
	{
		if(aModifyType == GroupModifyType_GroupName)
		{
			aLocalGroup.name = aNewValue;
		}
		else if(aModifyType == GroupModifyType_GroupTopic)
		{
			aLocalGroup.topic = aNewValue;
		}
		else if(aModifyType == GroupModifyType_forbidShareQRCode)
		{
			if (aNewValue.compare("true"))
			{
				aLocalGroup.addOneAttribute(GroupAttribute_ForbidShareQRCode);
			}
			else
			{
				aLocalGroup.removeOneAttribute(GroupAttribute_ForbidShareQRCode);
			}
		}
		else if(aModifyType == GroupModifyType_AllowmemberInvite)
		{
			if (aNewValue.compare("true"))
			{
				aLocalGroup.addOneAttribute(GroupAttribute_AllowMemberInvite);
			}
			else
			{
				aLocalGroup.removeOneAttribute(GroupAttribute_AllowMemberInvite);
			}
		}
		else if(aModifyType == GroupModifyType_GroupOwnerTansfer)
		{
			aLocalGroup.ownerURI = aNewValue;
			aLocalGroup.setIsOwner(myInfo.URI);
		}

		publicGroupList.moveFrom(nonValidPublicGroupList, aLocalGroup, aGroupURI);
	}

	PublicGroupNotifyPara aPara;
	aPara.aNotifyType = PublicGroupNotifyType_onModifyPublicGroupAttributeResult;
	aPara.aRosterURI = aGroupURI;
	aPara.result = result;
	aPara.aGroupModifyType = aModifyType;

	postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onModifyPublicGroupResult(int result, string aGroupURI, int modifyType, string aNewStrValue, int aNewIntValue, string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "===not support===onModifyPublicGroupAttributeResult, result[%d] aGroupURI[%s] aType[%d][%s][%d][%s]", result, aGroupURI.c_str(), modifyType, aNewStrValue.c_str(), aNewIntValue, anOperateID.c_str());
}

void USSDK_RosterService::onLeavePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID)
{
	LOG_DEBUGGING(rosterAgentName, "onLeavePublicGroupResult, result[%d] aGroupURI[%s] anOperateID[%s]", result, aGroupURI.c_str(),  anOperateID.c_str());

	USSDK_Group aLocalGroup;
	if (!getOnePublicGroup(aGroupURI, aLocalGroup))
	{
		LOG_ERROR(rosterAgentName, "onLeavePublicGroupResult, unknow group[%s]", aGroupURI.c_str());
		return;
	}

	if (result == 404 || result == 200)
	{
		if (aLocalGroup.existMember(myInfo.URI))
		{
			aLocalGroup.removeMember(myInfo.URI);
			aLocalGroup.setValid(false);
			
			publicGroupList.moveTo(nonValidPublicGroupList, aLocalGroup, aGroupURI);
		}
	}

	PublicGroupNotifyPara aPara;
	aPara.aNotifyType = PublicGroupNotifyType_onLeavePublicGroupResult;
	aPara.aRosterURI = aGroupURI;
	aPara.result = result;

	postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onDeletePublicGroupResult(int result, string aGroupURI)
{
    LOG_DEBUGGING(rosterAgentName, "onDeletePublicGroupResult, result[%d] aGroupURI[%s]", result, aGroupURI.c_str());
    
    USSDK_Group aLocalGroup;
    if (!getOnePublicGroup(aGroupURI, aLocalGroup))
    {
        LOG_ERROR(rosterAgentName, "onDeletePublicGroupResult, unknow group[%s]", aGroupURI.c_str());
        return;
    }
    
    if (result == 404 || result == 200)
    {
        if (aLocalGroup.existMember(myInfo.URI))
        {
            aLocalGroup.removeMember(myInfo.URI);
            aLocalGroup.setValid(false);
            
            publicGroupList.moveTo(nonValidPublicGroupList, aLocalGroup, aGroupURI);
        }
    }
    
    PublicGroupNotifyPara aPara;
    aPara.aNotifyType = PublicGroupNotifyType_onDeletePublicGroupResult;
    aPara.aRosterURI = aGroupURI;
    aPara.result = result;
    
    postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::onRequestToJoinPublicGroupResult(string aGroupURI, string groupIdentifier, int result)
{
    LOG_DEBUGGING(rosterAgentName, "onRequestToJoinPublicGroupResult, result[%d] aGroupURI[%s]", result, aGroupURI.c_str());
    
    if (result == 200)
    {
        USSDK_Group aLocalGroup;
        getOnePublicGroup(aGroupURI, aLocalGroup);
        
        aLocalGroup.addMember(myInfo.URI, myInfo.displayName);
        aLocalGroup.setValid(true);
        
        publicGroupList.moveFrom(nonValidPublicGroupList, aLocalGroup, aGroupURI);
        checkRoster(aGroupURI, RosterType_Group);
    }
    
    PublicGroupNotifyPara aPara;
    aPara.aNotifyType = PublicGroupNotifyType_onRequestToJoinPublicGroupResult;
    aPara.aRosterURI = aGroupURI;
    aPara.result = result;
    
    postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

//notify
void USSDK_RosterService::onDeletedFromPublicGroupNotify(string aGroupURI, int type)
{
    LOG_DEBUGGING(rosterAgentName, "onDeletedFromPublicGroupNotify, type[%d] aGroupURI[%s]", type, aGroupURI.c_str());
    
    USSDK_Group aLocalGroup;
    if(getOnePublicGroup(aGroupURI, aLocalGroup))
    {
        LOG_ERROR(rosterAgentName, "onDeletedFromPublicGroupNotify, unknow group[%s]", aGroupURI.c_str());
        return;
    }
    
    aLocalGroup.removeMember(myInfo.URI);
    aLocalGroup.setValid(false);
    
    publicGroupList.moveTo(nonValidPublicGroupList, aLocalGroup, aGroupURI);
    
    PublicGroupNotifyPara aPara;
    
    aPara.aNotifyType = type == 0 ? PublicGroupNotifyType_onPublicGroupDisolvedNotify :PublicGroupNotifyType_onDeletedFromPublicGroupNotify;
    aPara.aRosterURI = aGroupURI;
    
    postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

//GROUP_INVITE
void USSDK_RosterService::onInvitedToPublicGroupNotify(void * invitition)
{
    GROUP_INVITE *anInvition = (GROUP_INVITE *)invitition;
    LOG_DEBUGGING(rosterAgentName, "onInvitedToPublicGroupNotify, aGroupURI[%s][%s][%s][%s]", anInvition->cGURI, anInvition->cGName, anInvition->cGOwnerURI, anInvition->cGOwnerName);
    
    USSDK_Group aLocalGroup;
    getOnePublicGroup(anInvition->cGURI, aLocalGroup);
    CompareResult aCompareResult;
    aLocalGroup.merger(invitition, aCompareResult);
    if (aCompareResult != CompareResult_Equal)
    {
        aLocalGroup.setIsOwner(myInfo.URI);
        //TODO: saveToDB
    }
    
    publicGroupList.moveFrom(nonValidPublicGroupList, aLocalGroup, anInvition->cGURI);
    
    PublicGroupNotifyPara aPara;
    aPara.aNotifyType = PublicGroupNotifyType_onInvitedToPublicGroupNotify;
    aPara.aRosterURI = anInvition->cGURI;
    
    postNotification(EVT_PublicGroupNotify, &aPara, EVT_SourceName_RosterService);
}

void USSDK_RosterService::RemoveCacheData()
{
	LOG_DEBUGGING(rosterAgentName, "RemoveCacheData[%s]", userURI.c_str());

	myInfo.reset();
	friendList.clear();
	nonFriendList.clear();
}

bool USSDK_RosterService::isMyself(string aRosterURI)
{
	return !aRosterURI.compare(userURI);
}

bool USSDK_RosterService::hasOneSingleContact(string aRosterURI)
{
	if (isMyself(aRosterURI))
	{
		return true;
	}

	if (friendList.hasElement(aRosterURI))
	{
		return true;
	}

	return nonFriendList.hasElement(aRosterURI);
}

void USSDK_RosterService::saveOneSingleContactToMemory(USSDK_SingleContact& aSingleContact)
{
	if (aSingleContact.isMyself())
	{
		myInfo = aSingleContact;
	}
	else	if (aSingleContact.isFriend())
	{
		friendList.cover(aSingleContact.URI, aSingleContact);
	}
	else 
	{
		nonFriendList.cover(aSingleContact.URI, aSingleContact);
	}
}

bool USSDK_RosterService::generateSingleContactInfo(USER_INFO *info, USSDK_SingleContact &aSingleContactInfo, bool isDetailInfo)
{
	USES_CONVERSION;

	//aSingleContactInfo.identifier = ;
	aSingleContactInfo.domain = T2CA(info->cDomain);
	aSingleContactInfo.name = T2CA(info->cName);
	//aSingleContactInfo.enName = ;
	//aSingleContactInfo.pinyinName = ;
	//aSingleContactInfo.displayName = ;
	//aSingleContactInfo.displayNamePinyin = ;

	aSingleContactInfo.logo = T2CA(info->cPhotoIndex);
	aSingleContactInfo.sign = T2CA(info->cOtherAddr);

	if (aSingleContactInfo.isMyself())
	{
		aSingleContactInfo.userStatus = UserStatus_Online;
	}

	if (isDetailInfo)
	{
		aSingleContactInfo.detailGetStatus = DataGetStatus_Success;

		aSingleContactInfo.userCapability = 0;
		if (info->IsVoipEnabled)
		{
			aSingleContactInfo.addOneCapability(UserCapability_Voip);
		}

		if (info->IsRecieptEnabled)
		{
			aSingleContactInfo.addOneCapability(UserCapability_Receipt);
		}

		if (info->IsTransfileEnabled)
		{
			aSingleContactInfo.addOneCapability(UserCapability_TransFile);
		}

		if (info->IsWatermarkEnabled)
		{
			aSingleContactInfo.addOneCapability(UserCapability_WaterMark);
		}		
	}

	return true;
}

bool USSDK_RosterService::generatePublicGroupInfo(FIRE_ALL_GROUP *info, USSDK_Group &aOutPublicGroupInfo)
{
	USES_CONVERSION;
	string URI = T2CA(info->cGroupURI);

	switch(info->iGroupType)
	{
	case FIRE_GROUP_TYPE_PUBLIC:
	case FIRE_GROUP_TYPE_PUBLIC_ASSOC:
	case FIRE_GROUP_TYPE_PUBLIC_BASIC:
	case FIRE_GROUP_TYPE_PUBLIC_TEMP:
		{
			break;
		}
	case FIRE_GROUP_TYPE_PRIVATE:
	case FIRE_GROUP_TYPE_NOGROUP:
		{
			LOG_ERROR(rosterAgentName, "not support this type group[%s]", URI.c_str());
			return false;
		}
	default:
		{
			LOG_ERROR(rosterAgentName, "invalid type group[%s]", URI.c_str());
			return false;
		}
	}

	aOutPublicGroupInfo.URI = URI;
	aOutPublicGroupInfo.name = T2CA(info->cGroupName);
	aOutPublicGroupInfo.ownerURI = T2CA(info->cOwnerURI);
	aOutPublicGroupInfo.setValid(true);
	return true;
}

void USSDK_RosterService::onNotify(string anEventName, void* aPara, string sourceName)
{
	
}

bool USSDK_RosterService::databaseFunctionTest()
{
	return rosterDBManager.databaseFunctionTest(dbDataPath);
}