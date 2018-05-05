//
//  USSDK_RosterServiceCallBack.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_RosterServiceCallBack_h
#define USSDK_RosterServiceCallBack_h
#include <stdio.h>
#include <string>

#include "../Model/USSDK_RosterModel.h"
#include "../Model/USSDK_SingleContactModel.h"
#include "../Model/USSDK_GroupModel.h"
#include "../Model/USSDK_PubaccModel.h"
#include "../Model/USSDK_EmployeeModel.h"

using namespace NS_USSDK_RosterService;
using namespace std;

namespace NS_USSDK_RosterService
{
	#define DEFINE_EVENT(EVENT) static string EVENT = #EVENT;

	DEFINE_EVENT(EVT_SourceName_RosterService)
	
	DEFINE_EVENT(EVT_SingleContactNotify)
	DEFINE_EVENT(EVT_PublicGroupNotify)
	DEFINE_EVENT(EVT_PublicGroupMemberChangedNotify)

	typedef enum
	{
		SingleContactNotifyType_OneSingleContactInfoArrived,
		SingleContactNotifyType_SingleContactListInfoArrived,
		SingleContactNotifyType_AddSingleContactResult,
		SingleContactNotifyType_RemoveSingleContactResult,
	}SingleContactNotifyType;

	struct SingleContactNotifyPara
	{
		SingleContactNotifyType aNotifyType;
		int result;
		string aRosterURI;
		USSDK_SingleContact* aSingleContact;
	};

	typedef enum
	{
		PublicGroupNotifyType_PublicGroupListInfoArrived,
		PublicGroupNotifyType_OnePublicGroupInfoArrived,
		PublicGroupNotifyType_onCreatePublicGroupResult,
		PublicGroupNotifyType_onInviteSomeoneToPublicGroupResult,
		PublicGroupNotifyType_ondeleteMemberFromPublicGroupResult,
		PublicGroupNotifyType_onModifyPublicGroupAttributeResult,
		PublicGroupNotifyType_onLeavePublicGroupResult,
        PublicGroupNotifyType_onDeletePublicGroupResult,
        PublicGroupNotifyType_onRequestToJoinPublicGroupResult,
        PublicGroupNotifyType_onDeletedFromPublicGroupNotify,
        PublicGroupNotifyType_onPublicGroupDisolvedNotify,
        PublicGroupNotifyType_onInvitedToPublicGroupNotify,
	}PublicGroupNotifyType;

	struct PublicGroupNotifyPara
	{
		PublicGroupNotifyType aNotifyType;
		int result;
		string aRosterURI;
		string aMemberURI;
		USSDK_Group* aPublicGroup;
		GroupModifyType aGroupModifyType;
	};

	typedef enum
	{
		PublicGroupMemberOperateType_invite,
		PublicGroupMemberOperateType_remove,			
	}PublicGroupMemberOperateType;

	struct PublicGroupMemberChangedNotifyPara
	{
		string aRosterURI;
		PublicGroupMemberOperateType aOperateType;
		RosterBriefInfo operaterInfo;
		RosterBriefInfo memberInfo;
	};

}

#endif /* USSDK_RosterServiceCallBack_h */
