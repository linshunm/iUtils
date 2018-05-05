//
//  USSDK_RosterServiceAdapter.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include "USSDK_RosterServiceAdapter.h"

//temp TODO:
#include "../RosterService/Service/USSDK_RosterService.h"
#include "../RosterAgentProcTransModel.hpp"
#include "../../Foundation/ThreadManager/USSDK_ThreadManager.h"

using namespace NS_USSDK_RosterService;

namespace NS_USSDK_RosterService
{
	USSDK_RosterService *CorrespondService()
	{
		return USSDK_RosterService::shareInstance();
	}
}

void USSDK_RosterServiceAdapter::addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID)
{
	CorrespondService()->addObserver(observer, anEventName, callBackThreadID);
}

void USSDK_RosterServiceAdapter::addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID)
{
	CorrespondService()->addObserver(observer, callBackThreadID);
}

void USSDK_RosterServiceAdapter::removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName)
{
	CorrespondService()->removeObserver(observer, anEventName);
}

void USSDK_RosterServiceAdapter::removeObserver(USSDK_UNotifyMethodClass * observer)
{
	CorrespondService()->removeObserver(observer);
}

string USSDK_RosterServiceAdapter::logoPathOfRoster(string rosterURI, RosterType aRosterType)
{
	return CorrespondService()->logoPathOfRoster(rosterURI, aRosterType);
}

USSDK_SingleContactMap USSDK_RosterServiceAdapter::getFriendList()
{
	return CorrespondService()->getFriendList();
}

bool USSDK_RosterServiceAdapter::getOneSingleContact(string aRosterURI, USSDK_SingleContact &out)
{
	return CorrespondService()->getOneSingleContact(aRosterURI, out);
}

int USSDK_RosterServiceAdapter::friendCount()
{
	return CorrespondService()->friendCount();
}


USSDK_GroupMap USSDK_RosterServiceAdapter::getPublicGroupList()
{
	return CorrespondService()->getPublicGroupList();
}

bool USSDK_RosterServiceAdapter::getOnePublicGroup(string aRosterURI, USSDK_Group &out)
{
	return CorrespondService()->getOnePublicGroup(aRosterURI, out);
}

bool USSDK_RosterServiceAdapter::loadCacheData(string anUserURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_loadCacheData;

	CheckRosterAPITrans aPara;
	aPara.aRosterURI = anUserURI;

	g_CheckRosterAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::checkRoster(string aRosterURI, RosterType aRosterType, RosterSource aRosterSource)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_checkRoster;
	
	CheckRosterAPITrans aPara;
	aPara.aRosterURI = aRosterURI;
	aPara.aRosterType = aRosterType;
	aPara.aRosterSource = aRosterSource;

	g_CheckRosterAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::addOneSingleContact(string aRosterURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_addOneSingleContact;

	AddOneSingleContactAPITrans aPara;
	aPara.aRosterURI = aRosterURI;

	g_AddOneSingleContactAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::removeOneSingleContact(string aRosterURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_removeOneSingleContact;

	AddOneSingleContactAPITrans aPara;
	aPara.aRosterURI = aRosterURI;

	g_AddOneSingleContactAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::createPublicGroup(string aGroupName, vector<RosterBriefInfo *> &aGroupMembersInfo, string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_createPublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupName = aGroupName;

	vector<RosterBriefInfo *>::iterator it = aGroupMembersInfo.begin();   
	for(; it != aGroupMembersInfo.end(); ++it)  
	{  
		RosterBriefInfo *anElement = (RosterBriefInfo *)&it;
		aPara.expectGroupMembersInfo.push_back(*anElement);
	}  

	aPara.operateID = anOperateID;

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::deletePublicGroupCreateCache(string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_deletePublicGroupCreateCache;

	GroupOperateAPITrans aPara;
	aPara.operateID = anOperateID;

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

//add, remove
bool USSDK_RosterServiceAdapter::inviteSomeoneToPublicGroup(string aGroupURI, RosterBriefInfo &aMemberInfo)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_inviteSomeoneToPublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;
	aPara.expectGroupMembersInfo.push_back(aMemberInfo);

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::inviteMembersToPublicGroup(string aGroupURI, vector<RosterBriefInfo *> &aMemberInfo)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_inviteMembersToPublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;

	vector<RosterBriefInfo *>::iterator it = aMemberInfo.begin();   
	for(; it != aMemberInfo.end(); ++it)  
	{  
		RosterBriefInfo *anElement = (RosterBriefInfo *)&it;
		aPara.expectGroupMembersInfo.push_back(*anElement);
	}  

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::deleteSomeOneFromPublicGroup(string aGroupURI, string aMemberURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_deleteSomeOneFromPublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;
	aPara.expectGroupMembersURI.push_back(aMemberURI);

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::deleteMembersFromPublicGroup(string aGroupURI, vector<string> &aMembersURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_deleteMembersFromPublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;

	vector<string>::iterator it = aMembersURI.begin();   
	for(; it != aMembersURI.end(); ++it)  
	{  
		string * anElement = (string *)&it;
		aPara.expectGroupMembersURI.push_back(*anElement);
	}  

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

//modify
bool USSDK_RosterServiceAdapter::modifyPublicGroupAttribute(string aGroupURI, GroupModifyType aModifyType, int aNewIntValue, string aNewStrValue,  string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_modifyPublicGroupAttribute;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;
	aPara.modifyType = aModifyType;
	aPara.newIntValue = aNewIntValue;
	aPara.newStrValue = aNewStrValue;

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

//leave
bool USSDK_RosterServiceAdapter::leavePublicGroup(string aGroupURI, string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_leavePublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;
	aPara.operateID = anOperateID;

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::deletePublicGroup(string aGroupURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_deletePublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

//QRCode to join group
bool USSDK_RosterServiceAdapter::requestToJoinPublicGroup(string aGroupURI, string groupIdentifier, string myName, string shareUserURI, string shareUserName)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_requestToJoinPublicGroup;

	GroupOperateAPITrans aPara;
	aPara.groupURI = aGroupURI;
	aPara.groupIdentifier = groupIdentifier;
	aPara.myName = myName;
	aPara.shareUserURI = shareUserURI;
	aPara.shareUserName = shareUserName;

	g_GroupOperateAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onGotOneSingleContactInfo(int result, string aRosterURI, void *aRosterInfo, bool isMyself)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onGotOneSingleContactInfo;

	OnGotOneSingleContactInfoAPITrans aPara;
	aPara.result = result;
	aPara.aRosterURI = aRosterURI;
	memcpy(&aPara.aRosterInfo, aRosterInfo, sizeof(aPara.aRosterInfo));
	aPara.isMyself = isMyself;

	g_OnGotOneSingleContactInfoAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onGotOneSingleContactSubInfo(int result, int type, string aRosterURI, void *info, bool isMyself)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onGotOneSingleContactSubInfo;

	OnGotOneSingleContactInfoAPITrans aPara;
	aPara.result = result;
	aPara.aRosterURI = aRosterURI;
	memcpy(&aPara.aRosterInfo, info, sizeof(aPara.aRosterInfo));
	aPara.isMyself = isMyself;
	aPara.type = type;

	g_OnGotOneSingleContactInfoAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onGotSingleContactListInfo(string aRosterURI, string aRosterName, int result, int count, void *info)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onGotSingleContactListInfo;

	OnGotSingleContactListInfoAPITrans aPara;
	aPara.result = result;
	aPara.count = count;
	if (count)
	{	
		memcpy(&aPara.info, info, sizeof(aPara.info));
		aPara.aURI = aRosterURI;
		aPara.aName = aRosterName;
	}
	
	g_OnGotSingleContactListInfoAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onAddSingleContactResult(int result, string aRosterURI, string aRosterName)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onAddSingleContactResult;

	AddOneSingleContactAPITrans aPara;
	aPara.result = result;
	aPara.aRosterURI = aRosterURI;
	aPara.aRosterName = aRosterName;

	g_AddOneSingleContactAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onRemoveSingleContactResult(int result, string aRosterURI, string aRosterName)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onRemoveSingleContactResult;

	AddOneSingleContactAPITrans aPara;
	aPara.result = result;
	aPara.aRosterURI = aRosterURI;
	aPara.aRosterName = aRosterName;

	g_AddOneSingleContactAPITrans.add(aPara);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onGotGroupListInfo(void *info)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onGotGroupListInfo;

	FIRE_GROUP_INFO *anInfo = (FIRE_GROUP_INFO *)info;

#ifdef _WIN32
	OnGotGroupListInfoAPITrans para;
	para.num = anInfo->iGroupNum;

	para.pList = malloc(para.num * sizeof(FIRE_ALL_GROUP));
	if (para.pList == NULL)
	{
		return false;
	}

	memcpy(para.pList, anInfo->strAllGroup, para.num * sizeof(FIRE_ALL_GROUP));
#else
    OnGotGroupListInfoAPITrans para;
    para.num = anInfo->iNum;
    
    para.pList = malloc(para.num * sizeof(FIRE_ALL_GROUP));
    if (para.pList == NULL)
    {
        return false;
    }
    
    memcpy(para.pList, anInfo->pFireGroupInfo, para.num * sizeof(FIRE_ALL_GROUP));
#endif

	g_OnGotGroupListInfoAPITrans.add(para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onGotOnePublicGroupInfo(string aGroupName, string aGroupURI, string aOperUser, short *pAttribute, long iListNum, short *pList)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onGotOneGroupInfo;
	
	OnGotOneGroupInfoAPITrans para;
	para.groupName = aGroupName;
	para.groupURI = aGroupURI;
	para.operUser = aOperUser;
	memcpy(&para.attribute, pAttribute, sizeof(ATTRIBUTE_TYPE));
	para.iListNum = iListNum;
	
	para.pList = malloc(iListNum * sizeof(ENTRY_TYPE));
	if (para.pList == NULL)
	{
		return false;
	}
	
	memcpy(para.pList, pList, iListNum * sizeof(ENTRY_TYPE));

	g_OnGotOneGroupInfoAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onPublicGroupMemberNotify(long iType, string aGroupURI, string anOwnerURI, string aOperUser, string aMemberURI, string aMemberDisplay, string aMemberInfo, string aMemberPhone, string aMemberType, string aMemberCaps, long iStatus, string aMsgID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onPublicGroupMemberNotify;

	OnPublicGroupMemberNotifyAPITrans para;
	para.iType = iType;
	para.aGroupURI = aGroupURI;
	para.anOwnerURI = anOwnerURI;
	para.aOperUser = aOperUser;
	para.aMemberURI = aMemberURI;
	para.aMemberDisplay = aMemberDisplay;
	para.aMemberInfo = aMemberInfo;
	para.aMemberPhone = aMemberPhone;
	para.aMemberType = aMemberType;
	para.aMemberCaps = aMemberCaps;
	para.iStatus = iStatus;
	para.aMsgID = aMsgID;

	g_OnPublicGroupMemberNotifyAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

//req-->res
bool USSDK_RosterServiceAdapter::onCreatePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onCreatePublicGroupResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;
	para.groupName = aGroupName;
	para.operateID = anOperateID;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onInviteSomeoneToPublicGroupResult(int result, string aGroupURI, string anMemberURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onInviteSomeoneToPublicGroupResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;
	para.memberURI = anMemberURI;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::ondeleteMemberFromPublicGroupResult(int result, string aGroupURI, string anMemberURI, string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_ondeleteMemberFromPublicGroupResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;
	para.memberURI = anMemberURI;
	para.operateID = anOperateID;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onModifyPublicGroupAttributeResult(int result, string aGroupURI, string aType, string aNewValue, string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onModifyPublicGroupAttributeResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;
	para.strType = aType;
	para.newStrValue = aNewValue;
	para.operateID = anOperateID;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onModifyPublicGroupResult(int result, string aGroupURI, int modifyType, string aNewStrValue, int aNewIntValue, string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onModifyPublicGroupResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;
	para.type = modifyType;
	para.newStrValue = aNewStrValue;
	para.newIntValue = aNewIntValue;
	para.operateID = anOperateID;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onLeavePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onLeavePublicGroupResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;
	para.groupName = aGroupName;
	para.operateID = anOperateID;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onDeletePublicGroupResult(int result, string aGroupURI)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onDeletePublicGroupResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

bool USSDK_RosterServiceAdapter::onRequestToJoinPublicGroupResult(string aGroupURI, string groupIdentifier, int result)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onRequestToJoinPublicGroupResult;

	GroupOperateUpAPITrans para;
	para.result = result;
	para.groupURI = aGroupURI;
	para.groupIdentifier = groupIdentifier;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

//notify
bool USSDK_RosterServiceAdapter::onDeletedFromPublicGroupNotify(string aGroupURI, int type)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onDeletedFromPublicGroupNotify;

	GroupOperateUpAPITrans para;
	para.type = type;
	para.groupURI = aGroupURI;

	g_GroupOperateUpAPITrans.add(&para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}
//GROUP_INVITE
bool USSDK_RosterServiceAdapter::onInvitedToPublicGroupNotify(void * invitition)
{
	RosterAgentEventReq req;
	req.aSubEventType = SubEvent_onInvitedToPublicGroupNotify;

	GROUP_INVITE para;
	memcpy(&para, invitition, sizeof(para));

	g_GROUP_INVITEAPITrans.add(para);

	return sendEventToAgent(Event_rosterAdapterReq, &req, sizeof(req), rosterAgentName, rosterAdapterName);
}

//test DB function for android
bool USSDK_RosterServiceAdapter::databaseFunctionTest()
{
	return CorrespondService()->databaseFunctionTest();
}
