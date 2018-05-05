//
//  USSDK_RosterService.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_RosterService_h
#define USSDK_RosterService_h

#include <stdio.h>
#include <string>
#include <map>

//#include "../../../DatabaseAgent/sqlite3.h"
//#include "../../../DatabaseAgent/DatabaseAgentHeader.h"

#include "../Model/USSDK_RosterModel.h"
#include "../Model/USSDK_SingleContactModel.h"
#include "../Model/USSDK_GroupModel.h"
#include "../Model/USSDK_PubaccModel.h"
#include "../Model/USSDK_EmployeeModel.h"
#include "../Model/USSDK_RosterListModel.h"

#include "../../../Foundation/USSDKBase/USSDK_BaseClass.hpp"

#include "USSDK_RosterLogoManager.h"
#include "USSDK_RosterDBManager.h"

#ifdef _WIN32
#include "../../../../stdafx.h"
#endif

#include "../../../clt_include/OcxInterface.h"

using namespace std;
using namespace NS_USSDK_Foundation;
using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_DatabaseService;

namespace NS_USSDK_RosterService
{
    class USSDK_RosterService : public USSDK_BaseClass, public USSDK_UNotifyMethodClass
    {
	private:
		string userURI;

		USSDK_SingleContact myInfo;
		USSDK_RosterList<USSDK_SingleContact> friendList;
		USSDK_RosterList<USSDK_SingleContact> nonFriendList;
		USSDK_RosterList<string> tmpFriendList;

		USSDK_RosterList<USSDK_Group> publicGroupList;
		USSDK_RosterList<USSDK_Group> nonValidPublicGroupList;
		USSDK_RosterList<string> tmpPublicGroupList;

		map<string, vector<RosterBriefInfo>> publicGroupCreateCache;

	private:
		USSDK_RosterLogoManager rosterLogoManager;
		USSDK_RosterDBManager rosterDBManager;

    public:
		string userDataPath;
        string dbDataPath;
        static USSDK_RosterService *shareInstance();

    public:
        void threadInit();
        void threadUnInit();

	public:
		bool databaseFunctionTest();

	public:
		//direct invoke, not execue at rosterAgent thread
		string logoPathOfRoster(string rosterURI, RosterType aRosterType = RosterType_SingleContact);
		USSDK_SingleContactMap getFriendList();
		bool getOneSingleContact(string aRosterURI, USSDK_SingleContact &out);
		int friendCount();

		USSDK_SingleContactMap getNewFriendsList();
		int getNewFriendsCount();
	
		USSDK_GroupMap getPublicGroupList();
		bool getOnePublicGroup(string aRosterURI, USSDK_Group &out);

		//down
    public:
		void loadCacheData(string anUserURI);
		void checkRoster(string aRosterURI, RosterType aRosterType, RosterSource aRosterSource = RosterSource_Default);
		bool addOneSingleContact(string aRosterURI);
		bool removeOneSingleContact(string aRosterURI);
		//bool agreeOrRejectNonFriendReq(string aRosterURI, string aRosterName,  NonFriend_Operate anOperate);

		//group
		//create
		bool createPublicGroup(string aGroupName, vector<RosterBriefInfo> &aGroupMembersInfo, string anOperateID);
		bool deletePublicGroupCreateCache(string anOperateID);

		//add, remove
		bool inviteSomeoneToPublicGroup(string aGroupURI, RosterBriefInfo &aMemberInfo);
		bool inviteMembersToPublicGroup(string aGroupURI, vector<RosterBriefInfo> &aMemberInfo);
		bool deleteSomeOneFromPublicGroup(string aGroupURI, string aMemberURI);
		bool deleteMembersFromPublicGroup(string aGroupURI, vector<string> &aMembersURI);

		//modify
		bool modifyPublicGroupAttribute(string aGroupURI, GroupModifyType aModifyType, int aNewIntValue, string aNewStrValue,  string anOperateID);

		//leave
		bool leavePublicGroup(string aGroupURI, string anOperateID);
		bool deletePublicGroup(string aGroupURI);	
		//QRCode to join group
		bool requestToJoinPublicGroup(string aGroupURI, string groupIdentifier, string myName, string shareUserURI, string shareUserName);

		//up
	public:
		void onGotOneSingleContactInfo(int result, string aRosterURI, void *aRosterInfo, bool isMyself = false);
		void onGotOneSingleContactSubInfo(int result, int type, string aRosterURI, void *info, bool isMyself = false);
		void onGotSingleContactListInfo(string aRosterURI, string aRosterName, int result, int count, void *info);
		void onWatcherSubscribeMe(string aWatcherURI, string aWatcherName, string requestInfo, string status);
		void onAddSingleContactResult(int result, string aRosterURI, string aRosterName);
		void onRemoveSingleContactResult(int result, string aRosterURI, string aRosterName);

		//group
		void onGotGroupListInfo(int num, void *info);
		void onGotOneGroupInfo(string aGroupName, string aGroupURI, string aOperUser, void *pAttribute, long iListNum, void *pList);
		void onPublicGroupMemberNotify(long iType, string aGroupURI, string anOwnerURI, string aOperUser, string aMemberURI, string aMemberDisplay, string aMemberInfo, string aMemberPhone, string aMemberType, string aMemberCaps, long iStatus, string aMsgID); 

		//req-->res
		void onCreatePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID);
		void onInviteSomeoneToPublicGroupResult(int result, string aGroupURI, string anMemberURI);
		void ondeleteMemberFromPublicGroupResult(int result, string aGroupURI, string anMemberURI, string anOperateID);

		void onModifyPublicGroupAttributeResult(int result, string aGroupURI, string aType, string aNewValue, string anOperateID);
		void onModifyPublicGroupResult(int result, string aGroupURI, int modifyType, string aNewStrValue, int aNewIntValue, string anOperateID);

		void onLeavePublicGroupResult(int result, string aGroupURI, string aGroupName, string anOperateID);
		void onDeletePublicGroupResult(int result, string aGroupURI);
		void onRequestToJoinPublicGroupResult(string aGroupURI, string groupIdentifier, int result);

		//notify
		void onDeletedFromPublicGroupNotify(string aGroupURI, int type);
		//GROUP_INVITE
		void onInvitedToPublicGroupNotify(void * invitition);

	//intra method
	private:
		void RemoveCacheData();
		void loadSingleContactsInfo();
		bool isMyself(string aRosterURI);
		bool hasOneSingleContact(string aRosterURI);
		void saveOneSingleContactToMemory(USSDK_SingleContact& aSingleContact);
		bool generateSingleContactInfo(USER_INFO *info, USSDK_SingleContact &aSingleContactInfo, bool isDetailInfo = true);

		void loadPublicGroupInfo();
		bool generatePublicGroupInfo(FIRE_ALL_GROUP *info, USSDK_Group &aOutPublicGroupInfo);
	public:
		void onNotify(string anEventName, void* aPara, string sourceName);

    };
}

#endif /* USSDK_RosterService_h */
