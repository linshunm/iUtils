//
//  RosterAgentProcTransModel.hpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef RosterAgentProcTransModel_hpp
#define RosterAgentProcTransModel_hpp

#include <stdio.h>
#include <string>
#include "../Foundation/USSDKMessageQueue/USSDK_MessageQueue.hpp"

#include "RosterService/Model/USSDK_RosterModel.h"
#include "RosterService/Model/USSDK_SingleContactModel.h"
#include "RosterService/Model/USSDK_GroupModel.h"
#include "RosterService/Model/USSDK_PubaccModel.h"
#include "RosterService/Model/USSDK_EmployeeModel.h"

#ifdef _WIN32
#include "../../stdafx.h"
#endif

#include "../clt_include/OcxInterface.h"

using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_Foundation;

namespace NS_USSDK_RosterService
{
	static string rosterAgentName = "rosterAgent";
	static string rosterAdapterName = "rosterAdapter";

	#define Event_rosterAdapterReq 3000

	typedef enum
	{
		SubEvent_loadCacheData = 1,
		SubEvent_checkRoster,
		SubEvent_addOneSingleContact,
		SubEvent_removeOneSingleContact,
		
		SubEvent_createPublicGroup,
		SubEvent_deletePublicGroupCreateCache,
		SubEvent_inviteSomeoneToPublicGroup,
		SubEvent_inviteMembersToPublicGroup,
		SubEvent_deleteSomeOneFromPublicGroup,
		SubEvent_deleteMembersFromPublicGroup,
		SubEvent_modifyPublicGroupAttribute,
		SubEvent_leavePublicGroup,
		SubEvent_deletePublicGroup,
		SubEvent_requestToJoinPublicGroup,

		SubEvent_onGotOneSingleContactInfo,
		SubEvent_onGotOneSingleContactSubInfo,
		SubEvent_onGotSingleContactListInfo,
		SubEvent_onAddSingleContactResult,
		SubEvent_onRemoveSingleContactResult,

		SubEvent_onGotGroupListInfo,
		SubEvent_onGotOneGroupInfo,
		SubEvent_onPublicGroupMemberNotify,		
		SubEvent_onCreatePublicGroupResult,
		SubEvent_onInviteSomeoneToPublicGroupResult,
		SubEvent_ondeleteMemberFromPublicGroupResult,
		SubEvent_onModifyPublicGroupAttributeResult,
		SubEvent_onModifyPublicGroupResult,
		SubEvent_onLeavePublicGroupResult,
		SubEvent_onDeletePublicGroupResult,
		SubEvent_onRequestToJoinPublicGroupResult,
		SubEvent_onDeletedFromPublicGroupNotify,
		SubEvent_onInvitedToPublicGroupNotify,
	}SubEventType;

	struct CheckRosterAPITrans
	{
		string aRosterURI;
		RosterType aRosterType;
		RosterSource aRosterSource;
	};

	struct OnGotOneSingleContactInfoAPITrans
	{
		int result;
		string aRosterURI;
		USER_INFO aRosterInfo;
		bool isMyself;

		int type;
	};

	struct AddOneSingleContactAPITrans
	{
		string aRosterURI;
		int result;
		string aRosterName;
	};

	struct GroupOperateAPITrans
	{
		string groupURI;
		string groupName; 
		vector<RosterBriefInfo> expectGroupMembersInfo;
		vector<string> expectGroupMembersURI;
		string operateID;

		GroupModifyType modifyType;
		int newIntValue;
		string newStrValue;

		string groupIdentifier;
		string myName;
		string shareUserURI;
		string shareUserName;
	};

	struct GroupOperateUpAPITrans
	{
		int result;
		string groupURI;
		string groupName; 
		string operateID;
		string memberURI;
		
		string strType;
		int type;
		string newStrValue;
		int newIntValue;

		string groupIdentifier;
	};

	struct OnGotSingleContactListInfoAPITrans
	{
		int result; 
		int count;
		LIST_INFO info;
		string aURI;
		string aName;
	};

	struct OnGotGroupListInfoAPITrans
	{
		long num;
		void *pList;
	};

	struct OnGotOneGroupInfoAPITrans
	{
		string groupName;
		string groupURI;
		string operUser;
		ATTRIBUTE_TYPE attribute;
		long iListNum;
		void *pList;
	};

	struct OnPublicGroupMemberNotifyAPITrans
	{
		long iType;
		string aGroupURI;
		string anOwnerURI;
		string aOperUser;
		string aMemberURI;
		string aMemberDisplay;
		string aMemberInfo;
		string aMemberPhone;
		string aMemberType;
		string aMemberCaps;
		long iStatus;
		string aMsgID;
	};

	typedef struct
	{
		SubEventType aSubEventType;
		union
		{

		}argument;
	}RosterAgentEventReq;

	extern USSDK_MessageQueue<CheckRosterAPITrans> g_CheckRosterAPITrans;
	extern USSDK_MessageQueue<OnGotOneSingleContactInfoAPITrans> g_OnGotOneSingleContactInfoAPITrans;
	extern USSDK_MessageQueue<AddOneSingleContactAPITrans> g_AddOneSingleContactAPITrans;
	extern USSDK_MessageQueue<OnGotSingleContactListInfoAPITrans> g_OnGotSingleContactListInfoAPITrans;

	extern USSDK_MessageQueue<OnGotGroupListInfoAPITrans> g_OnGotGroupListInfoAPITrans;
	extern USSDK_MessageQueue<OnGotOneGroupInfoAPITrans> g_OnGotOneGroupInfoAPITrans;
	extern USSDK_MessageQueue<OnPublicGroupMemberNotifyAPITrans> g_OnPublicGroupMemberNotifyAPITrans;

	extern USSDK_MessageQueue<GroupOperateAPITrans> g_GroupOperateAPITrans;
	extern USSDK_MessageQueue<GroupOperateUpAPITrans> g_GroupOperateUpAPITrans;	
	extern USSDK_MessageQueue<GROUP_INVITE> g_GROUP_INVITEAPITrans;
}

#endif /* LoginAgentProc_hpp */
