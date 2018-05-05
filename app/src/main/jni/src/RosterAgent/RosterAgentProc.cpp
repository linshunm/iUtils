//
//  RosterAgentProc.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include "RosterAgentProc.hpp"
#include "RosterAgentProcTransModel.hpp"

#include "RosterService/Service/USSDK_RosterService.h"
#include "../Foundation/ThreadManager/USSDK_ThreadManager.h"
#include "../LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"


using namespace NS_USSDK_LogService;
using namespace NS_USSDK_RosterService;

namespace NS_USSDK_RosterService
{
	USSDK_MessageQueue<CheckRosterAPITrans> g_CheckRosterAPITrans;
	USSDK_MessageQueue<OnGotOneSingleContactInfoAPITrans> g_OnGotOneSingleContactInfoAPITrans;
	USSDK_MessageQueue<AddOneSingleContactAPITrans> g_AddOneSingleContactAPITrans;
	USSDK_MessageQueue<OnGotSingleContactListInfoAPITrans> g_OnGotSingleContactListInfoAPITrans;

	USSDK_MessageQueue<OnGotGroupListInfoAPITrans> g_OnGotGroupListInfoAPITrans;
	USSDK_MessageQueue<OnGotOneGroupInfoAPITrans> g_OnGotOneGroupInfoAPITrans;
	USSDK_MessageQueue<OnPublicGroupMemberNotifyAPITrans> g_OnPublicGroupMemberNotifyAPITrans;

	USSDK_MessageQueue<GroupOperateAPITrans> g_GroupOperateAPITrans;
	USSDK_MessageQueue<GroupOperateUpAPITrans> g_GroupOperateUpAPITrans;
	USSDK_MessageQueue<GROUP_INVITE> g_GROUP_INVITEAPITrans;

	USSDK_RosterService *CorrespondService2()
	{
		return USSDK_RosterService::shareInstance();
	}

	void far rosterAgentProc(UINT8 far * lpMsg, UINT8 far * lpNULL0, UINT8 far * lpNULL1)
	{
		unsigned short event, msgLen;

		event  = bCtl_ZXGetEvent();
		msgLen = bCtl_ZXGetLength();

		switch(event)
		{
		case InitAllProcessEvent:
			LOG_DEBUGGING(rosterAgentName, "rosterAgentProc start success");
			break;
		case QuitAllProcessEvent:
			{
				LOG_DEBUGGING(rosterAgentName, "rosterAgentProc stop success");
				break;
			}
		case Event_rosterAdapterReq:
			{
				LOG_DEBUGGING(rosterAgentName, "receive Event_rosterAdapterReq");
				dealRosterAdapterReq(lpMsg);
				break;
			}
		default:
			{
				LOG_DEBUGGING(rosterAgentName, "rosterAgentProc unknow event %d from %d", event, bCtl_ZXGetSenderPno);
				break;
			}
		}
	}

	//dealRosterAdapterReq
	void dealRosterAdapterReq(void *aReq)
	{
		RosterAgentEventReq *req = (RosterAgentEventReq *)aReq;

		LOG_DEBUGGING(rosterAgentName, "receive subEvent[%d]", req->aSubEventType);

		switch(req->aSubEventType)
		{
		case SubEvent_loadCacheData:
			{
				CheckRosterAPITrans aPara = g_CheckRosterAPITrans.get();
				CorrespondService2()->loadCacheData(aPara.aRosterURI);
				break;
			}
		case SubEvent_checkRoster:
			{
				CheckRosterAPITrans aPara = g_CheckRosterAPITrans.get();
				CorrespondService2()->checkRoster(aPara.aRosterURI, aPara.aRosterType, aPara.aRosterSource);
				break;
			}
		case SubEvent_addOneSingleContact:
			{
				AddOneSingleContactAPITrans aPara = g_AddOneSingleContactAPITrans.get();
				CorrespondService2()->addOneSingleContact(aPara.aRosterURI);
				break;
			}
		case SubEvent_removeOneSingleContact:
			{
				AddOneSingleContactAPITrans aPara = g_AddOneSingleContactAPITrans.get();
				CorrespondService2()->removeOneSingleContact(aPara.aRosterURI);
				break;
			}
		case SubEvent_createPublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->createPublicGroup(aPara.groupName, aPara.expectGroupMembersInfo, aPara.operateID);
				break;
			}
		case SubEvent_deletePublicGroupCreateCache:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->deletePublicGroupCreateCache(aPara.operateID);
				break;
			}
		case SubEvent_inviteSomeoneToPublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->inviteSomeoneToPublicGroup(aPara.groupURI, aPara.expectGroupMembersInfo[0]);
				break;
			}
		case SubEvent_inviteMembersToPublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->inviteMembersToPublicGroup(aPara.groupURI, aPara.expectGroupMembersInfo);
				break;
			}
		case SubEvent_deleteSomeOneFromPublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->deleteSomeOneFromPublicGroup(aPara.groupURI, aPara.expectGroupMembersURI[0]);
				break;
			}
		case SubEvent_deleteMembersFromPublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->deleteMembersFromPublicGroup(aPara.groupURI, aPara.expectGroupMembersURI);
				break;
			}
		case SubEvent_modifyPublicGroupAttribute:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->modifyPublicGroupAttribute(aPara.groupURI, aPara.modifyType, aPara.newIntValue, aPara.newStrValue, aPara.operateID);
				break;
			}
		case SubEvent_leavePublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->leavePublicGroup(aPara.groupURI, aPara.operateID);
				break;
			}
		case SubEvent_deletePublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->deletePublicGroup(aPara.groupURI);
				break;
			}
		case SubEvent_requestToJoinPublicGroup:
			{
				GroupOperateAPITrans aPara = g_GroupOperateAPITrans.get();
				CorrespondService2()->requestToJoinPublicGroup(aPara.groupURI, aPara.groupIdentifier, aPara.myName, aPara.shareUserURI, aPara.shareUserName);
				break;
			}
		case SubEvent_onGotOneSingleContactInfo:
			{
				OnGotOneSingleContactInfoAPITrans aPara = g_OnGotOneSingleContactInfoAPITrans.get();
				CorrespondService2()->onGotOneSingleContactInfo(aPara.result, aPara.aRosterURI, &aPara.aRosterInfo, aPara.isMyself);
				break;
			}
		case SubEvent_onGotOneSingleContactSubInfo:
			{
				OnGotOneSingleContactInfoAPITrans aPara = g_OnGotOneSingleContactInfoAPITrans.get();
				CorrespondService2()->onGotOneSingleContactSubInfo(aPara.result,  aPara.type, aPara.aRosterURI, &aPara.aRosterInfo, aPara.isMyself);
				break;
			}
		case SubEvent_onGotSingleContactListInfo:
			{
				OnGotSingleContactListInfoAPITrans aPara = g_OnGotSingleContactListInfoAPITrans.get();
				CorrespondService2()->onGotSingleContactListInfo(aPara.aURI, aPara.aName, aPara.result, aPara.count, &aPara.info);
				break;
			}
		case SubEvent_onAddSingleContactResult:
			{
				AddOneSingleContactAPITrans aPara = g_AddOneSingleContactAPITrans.get();
				CorrespondService2()->onAddSingleContactResult(aPara.result, aPara.aRosterURI, aPara.aRosterName);
				break;
			}
		case SubEvent_onRemoveSingleContactResult:
			{
				AddOneSingleContactAPITrans aPara = g_AddOneSingleContactAPITrans.get();
				CorrespondService2()->onRemoveSingleContactResult(aPara.result, aPara.aRosterURI, aPara.aRosterName);
				break;
			}
		case SubEvent_onGotGroupListInfo:
			{
				OnGotGroupListInfoAPITrans aPara = g_OnGotGroupListInfoAPITrans.get();
				CorrespondService2()->onGotGroupListInfo(aPara.num, aPara.pList);
				
				if (aPara.pList)
				{
					free(aPara.pList);
					aPara.pList = NULL;
				}
				break;
			}
		case SubEvent_onGotOneGroupInfo:
			{
				OnGotOneGroupInfoAPITrans aPara = g_OnGotOneGroupInfoAPITrans.get();
				
				CorrespondService2()->onGotOneGroupInfo(aPara.groupName, aPara.groupURI, aPara.operUser, &aPara.attribute, aPara.iListNum, aPara.pList);

				if (aPara.pList)
				{
					free(aPara.pList);
					aPara.pList = NULL;
				}
				break;
			}
		case SubEvent_onPublicGroupMemberNotify:
			{
				OnPublicGroupMemberNotifyAPITrans para = g_OnPublicGroupMemberNotifyAPITrans.get();
				CorrespondService2()->onPublicGroupMemberNotify(para.iType, para.aGroupURI, para.anOwnerURI, para.aOperUser, para.aMemberURI, para.aMemberDisplay, para.aMemberInfo, para.aMemberPhone, para.aMemberType, para.aMemberCaps, para.iStatus, para.aMsgID);
				break;
			}
		case SubEvent_onCreatePublicGroupResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onCreatePublicGroupResult(aPara.result, aPara.groupURI, aPara.groupName, aPara.operateID);
				break;
			}
		case SubEvent_onInviteSomeoneToPublicGroupResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onInviteSomeoneToPublicGroupResult(aPara.result, aPara.groupURI, aPara.memberURI);
				break;
			}
		case SubEvent_ondeleteMemberFromPublicGroupResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->ondeleteMemberFromPublicGroupResult(aPara.result, aPara.groupURI, aPara.memberURI, aPara.operateID);
				break;
			}
		case SubEvent_onModifyPublicGroupAttributeResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onModifyPublicGroupAttributeResult(aPara.result, aPara.groupURI, aPara.strType, aPara.newStrValue, aPara.operateID);
				break;
			}
		case SubEvent_onModifyPublicGroupResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onModifyPublicGroupResult(aPara.result, aPara.groupURI, aPara.type, aPara.newStrValue, aPara.newIntValue, aPara.operateID);
				break;
			}
		case SubEvent_onLeavePublicGroupResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onLeavePublicGroupResult(aPara.result, aPara.groupURI, aPara.groupName, aPara.operateID);
				break;
			}
		case SubEvent_onDeletePublicGroupResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onDeletePublicGroupResult(aPara.result, aPara.groupURI);
				break;
			}
		case SubEvent_onRequestToJoinPublicGroupResult:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onRequestToJoinPublicGroupResult(aPara.groupURI, aPara.groupIdentifier, aPara.result);
				break;
			}
		case SubEvent_onDeletedFromPublicGroupNotify:
			{
				GroupOperateUpAPITrans aPara = g_GroupOperateUpAPITrans.get();
				CorrespondService2()->onDeletedFromPublicGroupNotify( aPara.groupURI, aPara.type);
				break;
			}
		case SubEvent_onInvitedToPublicGroupNotify:
			{
				GROUP_INVITE aPara = g_GROUP_INVITEAPITrans.get();
				CorrespondService2()->onInvitedToPublicGroupNotify(&aPara);
				break;
			}
		default:
			{
				LOG_ERROR(rosterAgentName,"unknow subEventType[%d]", req->aSubEventType);
				break;
			}
		}
	}

	void rosterAgentThreadInit()
	{
		USSDK_ThreadManager *aManager = USSDK_ThreadManager::shareInstance();
		aManager->startOneAgent(rosterAgentName, rosterAgentProc);
	}

	void rosterAgentThreadUnInit()
	{
		USSDK_ThreadManager *aManager = USSDK_ThreadManager::shareInstance();
		aManager->stopOneAgent(rosterAgentName);
	}
}


