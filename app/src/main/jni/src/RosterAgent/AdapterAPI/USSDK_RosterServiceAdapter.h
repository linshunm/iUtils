//
//  USSDK_RosterServiceAdapter.hpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_RosterServiceAdapter_h
#define USSDK_RosterServiceAdapter_h

#include <stdio.h>
#include <string>
#include <vector>

#include "../../USSDKMethod.h"

#include "../RosterService/Model/USSDK_RosterModel.h"
#include "../RosterService/Model/USSDK_SingleContactModel.h"
#include "../RosterService/Model/USSDK_GroupModel.h"
#include "../RosterService/Model/USSDK_PubaccModel.h"
#include "../RosterService/Model/USSDK_EmployeeModel.h"

#include "../../Foundation/NotificationCenter/USSDK_NotificationCenter.h"

using namespace std;
using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_Foundation;

namespace NS_USSDK_RosterService
{
	class USSDK_API USSDK_RosterServiceAdapter
    {
	public:
		static void addObserver(USSDK_UNotifyMethodClass * observer, string anEventName, int callBackThreadID);
		static void addObserver(USSDK_UNotifyMethodClass * observer, int callBackThreadID);

		static void removeObserver(USSDK_UNotifyMethodClass * observer, string anEventName);
		static void removeObserver(USSDK_UNotifyMethodClass * observer);

		//test DB function for android
	public:
		static bool databaseFunctionTest();

		//direct invoke, not execue at rosterAgent thread
	public:
		static string logoPathOfRoster(string rosterURI, RosterType aRosterType = RosterType_SingleContact);
		static USSDK_SingleContactMap getFriendList();
		static bool getOneSingleContact(string aRosterURI, USSDK_SingleContact &out);
		static int friendCount();
		//static USSDK_SingleContactMap getNewFriendsList();
		//static int getNewFriendsCount();

		static USSDK_GroupMap getPublicGroupList();
		static bool getOnePublicGroup(string aRosterURI, USSDK_Group &out);

	//down
	public:
		static bool loadCacheData(string anUserURI);
		static bool checkRoster(string aRosterURI, RosterType aRosterType = RosterType_SingleContact, RosterSource aRosterSource = RosterSource_Default);
		static bool addOneSingleContact(string aRosterURI);
		static bool removeOneSingleContact(string aRosterURI);
		//static bool agreeOrRejectNonFriendReq(string aRosterURI, string aRosterName,  NonFriend_Operate anOperate);

		//group
		//create
		static bool createPublicGroup(string aGroupName, vector<RosterBriefInfo *> &aGroupMembersInfo, string anOperateID);
		static bool deletePublicGroupCreateCache(string anOperateID);
	
		//add, remove
		static bool inviteSomeoneToPublicGroup(string aGroupURI, RosterBriefInfo &aMemberInfo);
		static bool inviteMembersToPublicGroup(string aGroupURI, vector<RosterBriefInfo *> &aMemberInfo);
		static bool deleteSomeOneFromPublicGroup(string aGroupURI, string aMemberURI);
		static bool deleteMembersFromPublicGroup(string aGroupURI, vector<string> &aMembersURI);
		
		//modify
		static bool modifyPublicGroupAttribute(string aGroupURI, GroupModifyType aModifyType, int aNewIntValue, string aNewStrValue,  string anOperateID);

		//leave
		static bool leavePublicGroup(string aGroupURI, string anOperateID);
		static bool deletePublicGroup(string aGroupURI);	
		//QRCode to join group
		static bool requestToJoinPublicGroup(string aGroupURI, string groupIdentifier, string myName, string shareUserURI, string shareUserName);

	public:
		//up
		//USER_INFO--void*
		static bool onGotOneSingleContactInfo(int result, string aRosterURI, void *aRosterInfo, bool isMyself = false);
		static bool onGotOneSingleContactSubInfo(int result, int type, string aRosterURI, void *info, bool isMyself = false);
		static bool onGotSingleContactListInfo(string aRosterURI, string aRosterName, int result, int count, void *info);
		static bool onWatcherSubscribeMe(string aWatcherURI, string aWatcherName, string requestInfo, string status);
		static bool onAddSingleContactResult(int result, string aRosterURI, string aRosterName);
		static bool onRemoveSingleContactResult(int result, string aRosterURI, string aRosterName);

		//group
		static bool onGotGroupListInfo(void *info);
		static bool onGotOnePublicGroupInfo(string aGroupName, string aGroupURI, string aOperUser, short *pAttribute, long iListNum, short *pList);
		static bool onPublicGroupMemberNotify(long iType, string aGroupURI, string anOwnerURI, string aOperUser, string aMemberURI, string aMemberDisplay, string aMemberInfo, string aMemberPhone, string aMemberType, string aMemberCaps, long iStatus, string aMsgID); 
	
		//req-->res
		static bool onCreatePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID);
		static bool onInviteSomeoneToPublicGroupResult(int result, string aGroupURI, string anMemberURI);
		static bool ondeleteMemberFromPublicGroupResult(int result, string aGroupURI, string anMemberURI, string anOperateID);
		
		static bool onModifyPublicGroupAttributeResult(int result, string aGroupURI, string aType, string aNewValue, string anOperateID);
		static bool onModifyPublicGroupResult(int result, string aGroupURI, int modifyType, string aNewStrValue, int aNewIntValue, string anOperateID);
		
		static bool onLeavePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID);
		static bool onDeletePublicGroupResult(int result, string aGroupURI);
		static bool onRequestToJoinPublicGroupResult(string aGroupURI, string groupIdentifier, int result);

		//notify
		static bool onDeletedFromPublicGroupNotify(string aGroupURI, int type);
		//GROUP_INVITE
		static bool onInvitedToPublicGroupNotify(void * invitition);
    };
}
#endif /* USSDK_RosterServiceAdapter_h */
