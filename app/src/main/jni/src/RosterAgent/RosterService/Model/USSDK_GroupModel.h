//
//  USSDK_GroupModel.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_GroupModel_h
#define USSDK_GroupModel_h

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "USSDK_RosterModel.h"
#include "USSDK_RosterListModel.h"
#include "USSDK_MergerModel.h"

using namespace std;
using namespace NS_USSDK_RosterService;

namespace NS_USSDK_RosterService
{
	class USSDK_Group;

	typedef map<string, USSDK_Group> USSDK_GroupMap;

	typedef enum
	{
		GroupModifyType_GroupName           = 0,  
		//    MPGType_Duration    = 1,    
		//    MPGType_Visibitily  = 2,   
		//    MPGType_NeedPermit  = 3, 
		//    MPGType_Max         = 4,    
		//    MPGType_Subject     = 5,   
		GroupModifyType_GroupTopic            = 6, 
		//    MPGType_WebUrl      = 7,    
		//    MPGType_ClassID     = 8,    
		GroupModifyType_forbidShareQRCode     = 9,
		GroupModifyType_AllowmemberInvite     = 10,
		GroupModifyType_GroupOwnerTansfer     = 20,
		GroupModifyType_Invalid     = 21, 
	}GroupModifyType;

	typedef enum
	{
		GroupMember_member,
		GroupMember_owner,
	}GroupMemberRole;

	typedef enum
	{
		GroupAttribute_AllowMemberInvite = 1 << 0,
		GroupAttribute_ForbidShareQRCode = 1 << 1,
	}GroupAttribute;

	class USSDK_Group : public USSDK_Roster
	{
	public:
		string ownerURI;
		string ownerName;
		string topic;
		string subject;
		GroupMemberRole myRole;
		int groupAttribute;
		int maxGroupMember;

		USSDK_RosterList<RosterBriefInfo> expectMembers;
		USSDK_RosterList<RosterBriefInfo> members;
		
		bool autoLoadAttachment;
		
		RosterBriefInfo QRCodeShareMember;
		RosterBriefInfo bulletinEditor;
		string bulletinEditTime;

	public:
		bool isValid();
		void setValid(bool isValid);

	public:
		bool isOwner();
		void setIsOwner(string myURI);
		
	public:
		void addOneAttribute(GroupAttribute aGroupAttribute);
		void removeOneAttribute(GroupAttribute aGroupAttribute);
		bool isAllowMemberInvite();
		bool isForbidShareQRCode();


		string groupName();

		RosterBriefInfo infoOfGroupMember(string URI);

	public:
		//group create
		bool addExpectMember(string URI, string aName);
		bool addExpectMembers(vector<string> URIs,  vector<string> names);

		bool removeExpectMember(string URI);
		bool removeExpectMembers(vector<string> URIs);
		bool removeAllExpectMember();

		bool existExpectMember(string URI);

	public:
		//group member
		bool addMember(string URI, string aName);
		bool addMembers(vector<string> URIs,  vector<string> names);

		bool removeMember(string URI);
		bool removeMembers(vector<string> URIs);
		bool removeAllMember();

		bool existMember(string URI);

	public:
		bool merger(USSDK_Group &aCompare, bool isBriefInfo);
		bool merger(string aGroupURI, string aGroupName, void *pAttributeInfo,  void* pMemberInfo, int memberNum, CompareResult &anOutCompareResult, vector<RosterBriefInfo> &anOutNewMember);
        bool merger(void* anInfo, CompareResult &anOutCompareResult);
	};
}
#endif /* USSDK_GroupModel_h */
