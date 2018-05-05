//
//  USSDK_GroupModel.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//
#include "USSDK_GroupModel.h"
#include "../../../Foundation/USSDKBitOperation/USSDK_BitOperation.hpp"

#include "../../../LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"

#ifdef _WIN32
#include "../../../../stdafx.h"
#endif

#include "../../../clt_include/OcxInterface.h"
#include "../../../clt_include/ussdk_Typedefs.h"

#include <string>
using namespace std;
using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_Foundation;
using namespace NS_USSDK_LogService;

bool USSDK_Group::isValid()
{
	return rosterRelationShip == RosterRelationShip_Strong;
}

void USSDK_Group::setValid(bool isValid)
{
	if (isValid)
	{
		rosterRelationShip = RosterRelationShip_Strong;
	}
	else
	{
		rosterRelationShip = RosterRelationShip_Weak;
	}
}

bool USSDK_Group::isOwner()
{
	return GroupMember_owner == myRole;
}

void USSDK_Group::setIsOwner(string myURI)
{
	if (!myURI.compare(ownerURI))
	{
		myRole = GroupMember_owner;
	}
	else
	{
		myRole = GroupMember_member;
	}
}

void NS_USSDK_RosterService::USSDK_Group::addOneAttribute( GroupAttribute aGroupAttribute)
{
	groupAttribute = BitAdd(groupAttribute, (int)aGroupAttribute);
}

void NS_USSDK_RosterService::USSDK_Group::removeOneAttribute( GroupAttribute aGroupAttribute )
{
	groupAttribute = BitRemoveB(groupAttribute, (int)aGroupAttribute);
}

bool NS_USSDK_RosterService::USSDK_Group::isAllowMemberInvite()
{
	return BitHasB(groupAttribute, (int)GroupAttribute_AllowMemberInvite);
}

bool NS_USSDK_RosterService::USSDK_Group::isForbidShareQRCode()
{
	return BitHasB(groupAttribute, (int)GroupAttribute_ForbidShareQRCode);
}

bool USSDK_Group::addExpectMember(string URI, string aName)
{
	RosterBriefInfo para = RosterBriefInfo(URI, aName);

	expectMembers.add(URI, para);
	return true;
}

bool USSDK_Group::removeExpectMember(string URI)
{
	expectMembers.remove(URI);
	return true;
}

bool USSDK_Group::addMember(string URI, string aName)
{
	RosterBriefInfo para = RosterBriefInfo(URI, aName);

	members.add(URI, para);
	return true;
}

bool USSDK_Group::addMembers(vector<string> URIs,  vector<string> names)
{
	return true;
}

bool USSDK_Group::removeMember(string URI)
{
	members.remove(URI);
	return true;
}

bool USSDK_Group::removeMembers(vector<string> URIs)
{
	return true;
}

bool USSDK_Group::removeAllMember()
{
	members.clear();
	return true;
}

bool USSDK_Group::existMember(string URI)
{
	return members.hasElement(URI);
}

bool USSDK_Group::merger( USSDK_Group &aCompare, bool isBriefInfo )
{
	bool isEqual = true;
	if (name.compare(aCompare.name))
	{
		isEqual = false;
		name = aCompare.name;
	}

	if (ownerURI.compare(aCompare.ownerURI))
	{
		isEqual = false;
		ownerURI = aCompare.ownerURI;
		myRole = aCompare.myRole;
	}

	if (rosterRelationShip != aCompare.rosterRelationShip)
	{
		isEqual = false;
		rosterRelationShip = aCompare.rosterRelationShip;
	}

	if (isBriefInfo)
	{
		return isEqual;
	}
    
    return isEqual;
}

bool USSDK_Group::merger(string aGroupURI, string aGroupName, void *pAttributeInfo,  void* pMemberInfo, int memberNum, CompareResult &anOutCompareResult, vector<RosterBriefInfo> &anOutNewMember)
{
	if (URI.length() == 0)
	{
		anOutCompareResult = CompareResult_LeftNULL;
	}

	bool isEqual = true;
	if (URI.compare(aGroupURI))
	{
		isEqual = false;
		URI = aGroupURI;
	}

	if (name.compare(aGroupName))
	{
		isEqual = false;
		name = aGroupName;
	}

	ATTRIBUTE_TYPE *attributeInfo = (ATTRIBUTE_TYPE *)pAttributeInfo;
	ENTRY_TYPE *memberInfo = (ENTRY_TYPE *)pMemberInfo;

	USES_CONVERSION;

	string aTopic = T2CA(attributeInfo->cTopic);
	if (topic.compare(aTopic))
	{
		isEqual = false;
		topic = aTopic;
	}

	string bulletinEditorURI = T2CA(attributeInfo->bulletinSetterUri);
	if (bulletinEditor.URI.compare(bulletinEditorURI))
	{
		isEqual = false;
		bulletinEditor.URI = bulletinEditorURI;
	}

	string bulletinEditorName = T2CA(attributeInfo->bulletinSetterName);
	if (bulletinEditor.name.compare(bulletinEditorName))
	{
		isEqual = false;
		bulletinEditor.name = bulletinEditorName;
	}

	string bulletinEditorTime = T2CA(attributeInfo->bulletinSettime);
	if (bulletinEditTime.compare(bulletinEditorTime))
	{
		isEqual = false;
		bulletinEditTime = bulletinEditorTime;
	}

	string aSubject = T2CA(attributeInfo->cSubject);
	if (subject.compare(aSubject))
	{
		isEqual = false;
		subject = aSubject;
	}

	string aIdentifier = T2CA(attributeInfo->cIdentifier);
	if (identifier.compare(aIdentifier))
	{
		isEqual = false;
		identifier = aIdentifier;
	}
	
	bool forbidQRCode = attributeInfo->forbid_2D_join == 1 ? true : false;
	if (forbidQRCode != isForbidShareQRCode())
	{
		isEqual = false;
		if (isForbidShareQRCode())
		{
			removeOneAttribute(GroupAttribute_ForbidShareQRCode);
		}
		else
		{
			addOneAttribute(GroupAttribute_ForbidShareQRCode);
		}
	}

	bool memberinvite = attributeInfo->member_invite_enable == 1 ? true : false;
	if (memberinvite != isAllowMemberInvite())
	{
		isEqual = false;
		if (isAllowMemberInvite())
		{
			removeOneAttribute(GroupAttribute_AllowMemberInvite);
		}
		else
		{
			addOneAttribute(GroupAttribute_AllowMemberInvite);
		}
	}

	string anOwnerURI = T2CA(attributeInfo->cOwner);
	if (ownerURI.compare(anOwnerURI))
	{
		isEqual = false;
		ownerURI = anOwnerURI;
	}

	USSDK_RosterList<string> nowMembers;
	for (int i = 0; i < memberNum; i++)
	{
		string memberURI = T2CA(memberInfo->cURI);
		string memberName = T2CA(memberInfo->cDisplayName);
		LOG_DEBUGGING("USSDK_Group", "generatePublicGroupInfo, memberInfo[%s][%s][%d]", memberURI.c_str(), memberName.c_str(), memberInfo->iMemberStatus);
		
		if (4 == memberInfo->iMemberStatus)
		{
			isEqual = false;
			removeMember(memberURI);
		}
		else if(1 == memberInfo->iMemberStatus)
		{
			nowMembers.add(memberURI, memberURI);
			if (!existMember(memberURI))
			{
				isEqual = false;
				addMember(memberURI, memberName);

				RosterBriefInfo aRosterBriefInfo = RosterBriefInfo(memberURI, memberName);
				anOutNewMember.push_back(aRosterBriefInfo);
			}
		}

		memberInfo++;
	}

	vector<RosterBriefInfo> notAtList;
	members.getNotAtList(nowMembers, notAtList);

	vector<RosterBriefInfo>::iterator it = notAtList.begin();   
	for(; it != notAtList.end(); ++it)  
	{  
		isEqual = false;
		RosterBriefInfo *info = (RosterBriefInfo *)&it;
		members.remove(info->URI);
	}  


	if (anOutCompareResult != CompareResult_LeftNULL)
	{
		if (isEqual)
		{
			anOutCompareResult = CompareResult_Equal;
		}
		else
		{
			anOutCompareResult = CompareResult_NotEqual;
		}
	}

	return true;

}

bool USSDK_Group::merger(void* anInfo, CompareResult &anOutCompareResult)
{
    GROUP_INVITE *info = (GROUP_INVITE *)info;
    if (URI.length() == 0)
    {
        anOutCompareResult = CompareResult_LeftNULL;
    }
    
    bool isEqual = true;
    if (URI.compare(info->cGURI))
    {
        isEqual = false;
        URI = info->cGURI;
    }
    
    if (URI.compare(info->cGName))
    {
        isEqual = false;
        name = info->cGName;
    }
    
    if (URI.compare(info->cGOwnerName))
    {
        isEqual = false;
        ownerName = info->cGOwnerName;
    }
    
    if (URI.compare(info->cGOwnerURI))
    {
        isEqual = false;
        ownerURI = info->cGOwnerURI;
    }
    
    if (URI.compare(info->cGIdentifier))
    {
        isEqual = false;
        identifier = info->cGIdentifier;
    }
    
    if (rosterRelationShip != RosterRelationShip_Strong)
    {
        isEqual = false;
        rosterRelationShip = RosterRelationShip_Strong;
    }
    
    if (anOutCompareResult != CompareResult_LeftNULL)
    {
        if (isEqual)
        {
            anOutCompareResult = CompareResult_Equal;
        }
        else
        {
            anOutCompareResult = CompareResult_NotEqual;
        }
    }
    
    return true;
}
