//
//  USSDK_OcxAdapter.cpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//
#ifdef _WIN32
#include "../../stdafx.h"
#endif

#include "USSDK_OcxAdapter.h"

#include "../clt_include/CtlZXMethod.h"
#include "../clt_include/CtlMethod.h"

#include "../clt_include/ussdk_Typedefs.h"
#include "../USSDK_UpNotify.hpp"

using namespace NS_USSDK_OcxAdapter;

namespace NS_USSDK_OcxAdapter
{
    using namespace NS_USSDK_DEFAULT;
    USSDK_EventWrapper *CorrespondEventWrapper()
    {
        return USSDK_UpNotify::shareInstance()->eventWrapper;
    }
}

USSDK_OcxAdapter * USSDK_OcxAdapter::shareInstance()
{
    static USSDK_OcxAdapter instance;
    return &instance;
}

UINT16 USSDK_OcxAdapter::setTimer(UINT32 second, TimerNo aTimerNo, bool isLoop)
{
	if (isLoop)
	{
		return bCtl_ZXSET_LOOP_TIMER(second*10, aTimerNo);
	}
	else
	{
		return bCtl_ZXSET_TIMER(second*10, aTimerNo);
	}
}

UINT8 USSDK_OcxAdapter::killTimer(TimerNo aTimerNo)
{
	return bCtl_ZXKILL_TIMER(aTimerNo);
}

UINT16 USSDK_OcxAdapter::setExtraTimer(UINT32 second, TimerNo aTimerNo, UINT32 Extra)
{
	return bCtl_ZXEXTRA_SET_TIMER(second*10, aTimerNo, Extra);
}

UINT8 USSDK_OcxAdapter::killExtraTimer(UINT16 tid)
{
	return bCtl_ZXEXTRA_KILL_TIMER(tid);
}

bool USSDK_OcxAdapter::login(OcxLogonPara *aPara)
{
	LOGON_PARA_NGN_DAS logonPara = {0};

#if _WIN32
 	USES_CONVERSION;
 	CString aAccount = A2CT(aPara->chSoftPhone.c_str());
 	CString aPassword = A2CT(aPara->chPassword.c_str());
 	CString aSipServer = A2CT(aPara->chSipServerIP.c_str());
 	CString anAppVersion =  A2CT(aPara->chVersion.c_str());
 
 	logonPara.nLogonType =  aPara->nLogonType;
 	_tcsncpy(logonPara.chSoftPhone, aAccount, MAX_LOG_USER_LEN);
 	_tcsncpy(logonPara.chPassword, aPassword, MAX_LOG_PSW_LEN);
 	_tcsncpy(logonPara.chSSIP, _T(""), MAX_LOG_IP_LEN);
 	logonPara.lSSPort = aPara->lSipServerPort;
 	_tcsncpy(logonPara.chSipServerIP, aSipServer, MAX_LOG_IP_LEN);
 	logonPara.lSipServerPort = aPara->lSipServerPort;
 	_tcsncpy(logonPara.chVersion, anAppVersion, MAX_LOG_VER_LEN);

	return bCtl_Logon(logonType_NGN_DAS, reinterpret_cast<short*>(&logonPara), 5);
    
#endif
    return true;
}

bool USSDK_OcxAdapter::logout(bool bJustClearData)
{
	return bCtl_LogoutServer(0);
}

bool USSDK_OcxAdapter::addOneAddressList(string aRosterURI, string aDisplayName, string info)
{
	USES_CONVERSION;
	return bCtl_IMSAddOneAddressList(A2CT(aRosterURI.c_str()), A2CT(aDisplayName.c_str()), A2CT(info.c_str()));
}

bool USSDK_OcxAdapter::deleteOneAddressList(string aRosterURI)
{
	USES_CONVERSION;
	return bCtl_IMSDeleteOneAddressList(A2CT(aRosterURI.c_str()));
}

bool USSDK_OcxAdapter::getOneRosterDetailInfo(string aRosterURI)
{
	USES_CONVERSION;
	return bCtl_IMSSearchOneListInfo(A2CT(aRosterURI.c_str()));
}

bool USSDK_OcxAdapter::getOneRosterPhotoInfo(string aRosterURI, string photoIndex)
{
#if _WIN32
	USES_CONVERSION;
	SUBINFO_PHOTO_PARA para;
	_tcsncpy(para.cPhotoIndex, A2CT(photoIndex.c_str()), MAX_PHOTO_INDEX_LEN);
	_tcsncpy(para.cURI, A2CT(aRosterURI.c_str()), MAX_IMS_URI_LEN);
		
	return bCtl_IMSGetPersonSubInfo(0, (short *)&para);
#endif
    return true;
}

bool USSDK_OcxAdapter::getOnePublicDetailInfo(string aRosterURI)
{	
	USES_CONVERSION;
#if _WIN32
	return bCtl_IMSGetPublicGroup(A2CT(aRosterURI.c_str()));
#endif
    return true;
}

bool USSDK_OcxAdapter::createOnePublicInfo(string ownerName)
{
#if _WIN32
	USES_CONVERSION;
	PUBLIC_GROUP_INPUT_PARA CreateGroup = {0};
	_tcsncpy(CreateGroup.cGroupName, _T(""), MAX_NAME_LEN);
	_tcsncpy(CreateGroup.cMyDisplayName,  A2CT(ownerName.c_str()), MAX_NAME_LEN); 
	CreateGroup.lDuration = 0;
	CreateGroup.lMax = 250;
	CreateGroup.lNeedPermit = 0;
	CreateGroup.lVisibility = 3;
	CreateGroup.forbid_2D_join = 0;
	CreateGroup.member_invite_enable = 1;
	_tcsncpy(CreateGroup.cSubject, _T(""), MAX_GROUP_SUBJECT_LEN);
	_tcsncpy(CreateGroup.cTopic, _T(""), MAX_GROUP_BULLETIN_LEN);
	_tcsncpy(CreateGroup.cGroupClassID, _T(""), MAX_GROUPCLASS_LEN); 
	_tcsncpy(CreateGroup.cType, _T("temporary"), MAX_SYSGROUP_TYPE_LEN); 

	return bCtl_IMSCreateOnePublicGroup((short *)&CreateGroup);
#endif
    return true;
}

bool USSDK_OcxAdapter::inviteSomeoneToPublicGroup(string aGroupURI, string aMemberURI, string aMemberName)
{
	USES_CONVERSION;
	return bCtl_IMSInviteSomeoneToMyPublicGroup(A2CT(aMemberURI.c_str()), A2CT(aMemberName.c_str()), A2CT(aGroupURI.c_str()), "");
}

bool USSDK_OcxAdapter::deleteSomeOneFromPublicGroup(string aGroupURI, string aMemberURI)
{
	USES_CONVERSION;
	return bCtl_IMSDeleteOneFromMyPublicGroup(A2CT(aMemberURI.c_str()), A2CT(aGroupURI.c_str()), "");	
}

bool USSDK_OcxAdapter::modifyPublicGroupAttribute(string aGroupURI, string aModifyType, string aNewValue)
{
	USES_CONVERSION;
	return bCtl_ModifyGroupAttributeReq(A2CT(aGroupURI.c_str()), A2CT(aModifyType.c_str()), A2CT(aNewValue.c_str()), "");
}

bool USSDK_OcxAdapter::leavePublicGroup(string aGroupURI)
{
	USES_CONVERSION;
	return bCtl_IMSLeavePublicGroup(_T(""), A2CT(aGroupURI.c_str()), "");
}

bool USSDK_OcxAdapter::deletePublicGroup(string aGroupURI)
{
	USES_CONVERSION;
	return bCtl_IMSDeleteOnePublicGroup(A2CT(aGroupURI.c_str()));	
}

bool USSDK_OcxAdapter::requestToJoinPublicGroup(string aGroupURI, string groupIdentifier, string myName, string shareUserURI, string shareUserName)
{
#if _WIN32
	USES_CONVERSION;
	return bCtl_IMSReqJoinPublicGroup(A2CT(aGroupURI.c_str()), A2CT(groupIdentifier.c_str()), A2CT(myName.c_str()), A2CT(shareUserURI.c_str()));
#endif
	return false;
}

bool USSDK_OcxAdapter::bInitControl()
{
	return bCtl_InitControl();
}

void USSDK_OcxAdapter::bSetEventWrapper(CCTLEventWrapper *pEventWrapper)
{
	return bCtl_SetEventWrapper(pEventWrapper);
}

//void USSDK_OcxAdapter::bSetUCallback(void *pObj, CTLCALLBACK_Ptr pCallback)
//{
//    return bCtl_SetUCallback(pObj, pCallback);
//}

bool USSDK_OcxAdapter::bLogon(long lType, short * pPara, long lSpecialScene)
{
	return bCtl_Logon(lType, pPara, lSpecialScene);
}

bool USSDK_OcxAdapter::bGoOnLogonServer()
{
	return bCtl_GoOnLogonServer();
}

bool USSDK_OcxAdapter::bForceLogonServer()
{
	return bCtl_ForceLogonServer();
}

bool USSDK_OcxAdapter::bLogoutServer(long bForce)
{
	return bCtl_LogoutServer(bForce);
}

bool USSDK_OcxAdapter::bServerLinkRecover()
{
	return bCtl_ServerLinkRecover();
}

bool USSDK_OcxAdapter::bCloseServerLink()
{
	return bCtl_CloseServerLink();
}

bool USSDK_OcxAdapter::bLogonSS()
{
	return bCtl_LogonSS();
}

bool USSDK_OcxAdapter::bLogoutSS()
{
	return bCtl_LogoutSS();
}

bool USSDK_OcxAdapter::bLogonSS2(LPCTSTR pConfuri, LPCTSTR pUserName, LPCTSTR pPassword, LPCTSTR pServerIP,LPCTSTR pServerPort)
{
	return bCtl_LogonSS2(pConfuri, pUserName, pPassword, pServerIP, pServerPort);
}

bool USSDK_OcxAdapter::bCreateMultiCall(LPCTSTR lpJID, LPCTSTR lpCallee, long lType)
{
	return bCtl_CreateMultiCall(lpJID, lpCallee, lType);
}

bool USSDK_OcxAdapter::bAcceptCall(LPCTSTR lpCaller)
{
	return bCtl_AcceptCall(lpCaller);
}

bool USSDK_OcxAdapter::bRejectCall(LPCTSTR lpCaller, int iCause)
{
	return bCtl_RejectCall(lpCaller, iCause);
}

bool USSDK_OcxAdapter::bLeaveCall(int iCause)
{
	return bCtl_LeaveCall(iCause);
}

void USSDK_OcxAdapter::bSecondDialNum(LPCTSTR lpNum)
{
	return bCtl_SecondDialNum(lpNum);
}

bool USSDK_OcxAdapter::bCallHold(LPCTSTR pPhoneNumber, long iType)
{
	return bCtl_CallHold(pPhoneNumber, iType);
}

bool USSDK_OcxAdapter::bDoNewCallWhenBusy2(long iDoType, LPCTSTR pCaller)
{
	return bCtl_DoNewCallWhenBusy2(iDoType, pCaller);
}

char * USSDK_OcxAdapter::bGetAudioProps()
{
	return bCtl_GetAudioProps();
}

char * USSDK_OcxAdapter::bGetVideoProps()
{
	return bCtl_GetVideoProps();
}

void USSDK_OcxAdapter::bSetAudioProps(LPCTSTR pCodecName, long nPacketPeriod)
{
	return bCtl_SetAudioProps(pCodecName, nPacketPeriod);
}

void USSDK_OcxAdapter::bSetVideoProps(LPCTSTR pCodecName, long nRate)
{
	return bCtl_SetVideoProps(pCodecName, nRate);
}


bool USSDK_OcxAdapter::bIMSCallMethod(long lType, short* pCallMethod)
{
	return bCtl_IMSCallMethod(lType, pCallMethod);
}

bool USSDK_OcxAdapter::bStartRecord(LPCTSTR lpFilePathName)
{
	return bCtl_StartRecord(lpFilePathName);
}

bool USSDK_OcxAdapter::bStopRecord()
{
	return bCtl_StopRecord();
}

bool USSDK_OcxAdapter::bSelectTalker(LPCTSTR lpCallee)
{
	return bCtl_SelectTalker(lpCallee);
}

bool USSDK_OcxAdapter::bInviteToMultiCall(LPCTSTR lpJID, LPCTSTR lpCallee, long lType)
{
	return bCtl_InviteToMultiCall(lpJID, lpCallee, lType);
}

bool USSDK_OcxAdapter::bReleaseTalker(LPCTSTR lpCallee)
{
	return bCtl_ReleaseTalker(lpCallee);
}

//个人
bool USSDK_OcxAdapter::bSetXCAPAccount(LPCTSTR pURI, LPCTSTR pPasswd)
{
	return bCtl_SetXCAPAccount(pURI, pPasswd);
}

bool USSDK_OcxAdapter::bIMSGetDeptAddrList(long iType, LPCTSTR pAddrList)
{
	return bCtl_IMSGetDeptAddrList(iType, pAddrList);
}

bool USSDK_OcxAdapter::bIMSSubscribeOneList(long iType, LPCTSTR pListURI, long lURIType)
{
	return bCtl_IMSSubscribeOneList(iType, pListURI, lURIType);
}

//公共
bool USSDK_OcxAdapter::bPatSpring(LPCTSTR lpDestNumber)
{
	return bCtl_PatSpring(lpDestNumber);
}

bool USSDK_OcxAdapter::bWhetherEncrypt(long lSetValue)
{
	return bCtl_WhetherEncrypt(lSetValue);
}

bool USSDK_OcxAdapter::bIMSSendAttachMessage(long iType, LPCTSTR pFilePath, LPCTSTR pFileType, LPCTSTR pMessage, LPCTSTR pReceiverURI, LPCTSTR pLocalMsgID, int iReceipt)
{
	return bCtl_IMSSendAttachMessage(iType, pFilePath, pFileType, pMessage, pReceiverURI, pLocalMsgID, iReceipt);
}

bool USSDK_OcxAdapter::bIMSSendMessage(int iType, int iReport, LPCTSTR pMessage, LPCTSTR pDeliverTime, LPCTSTR pReceiverURI, LPCTSTR pLocalMsgID, LPCTSTR pTMSubject, DWORD dwFileSeekSize, int iReceipt)
{
	return bCtl_IMSSendMessage(iType, iReport, pMessage, pDeliverTime, pReceiverURI, pLocalMsgID, pTMSubject, dwFileSeekSize, iReceipt);
}

bool USSDK_OcxAdapter::bIsLargeMessage(LPCTSTR pMessage)
{
	return bCtl_IsLargeMessage(pMessage);
}

bool USSDK_OcxAdapter::bIMSPsMethod(long iType, short* pPara)
{
	return bCtl_IMSPsMethod(iType, pPara);
}

bool USSDK_OcxAdapter::bIMSSearchRLS()
{
	return bCtl_IMSSearchRLS();
}

bool USSDK_OcxAdapter::bIMSGetPresenceRules()
{
	return bCtl_IMSGetPresenceRules();
}

bool USSDK_OcxAdapter::bIMSSubWatcher()
{
	return bCtl_IMSSubWatcher();
}

bool USSDK_OcxAdapter::bIMSSubscribeOneRLS(LPCTSTR pURI)
{
	return bCtl_IMSSubscribeOneRLS(pURI);
}

bool USSDK_OcxAdapter::bIMSGetPublicGroup(LPCTSTR lpIdentifier, int iOperFlag)
{
	return bCtl_IMSGetPublicGroup(lpIdentifier, iOperFlag);
}

bool USSDK_OcxAdapter::bIMSDoPublicGroupInvite(LPCTSTR lpcsOwnerName, LPCTSTR pInviterURI, LPCTSTR pGroupName, long iResult, LPCTSTR pGroupURI, LPCTSTR pReturn)
{
	return bCtl_IMSDoPublicGroupInvite(lpcsOwnerName, pInviterURI, pGroupName, iResult, pGroupURI, pReturn);
}

bool USSDK_OcxAdapter::bIMSSetUserInfo(short* pUserInfo)
{
	return bCtl_IMSSetUserInfo(pUserInfo);
}

bool USSDK_OcxAdapter::bIMSSetMyPhoto(LPCTSTR pIndex, short* pFileContent, long iLen)
{
	return bCtl_IMSSetMyPhoto(pIndex, pFileContent, iLen);
}

bool USSDK_OcxAdapter::bIMSGetPersonSubInfo(long lType, short* pRosterMethod)
{
	return bCtl_IMSGetPersonSubInfo(lType, pRosterMethod);
}

bool USSDK_OcxAdapter::bDispUICmdForEcp2Method(long lType, short* pCmdPara)
{
	return bCtl_DispUICmdForEcp2Method(lType, pCmdPara);
}

bool USSDK_OcxAdapter::bCtdCallMethod(long lType, short* pPara)
{
	return bCtl_CtdCallMethod(lType, pPara);
}

bool USSDK_OcxAdapter::bIMSSearchOneListInfo(LPCTSTR pListURI)
{
	return bCtl_IMSSearchOneListInfo(pListURI);
}

bool USSDK_OcxAdapter::bIMSRuleSetMethod(long lType, short* pPara)
{
	return bCtl_IMSRuleSetMethod(lType, pPara);
}

//搜索
bool USSDK_OcxAdapter::bIMSSearchUserByURI(LPCTSTR pURI, long iPage)
{
	return bCtl_IMSSearchUserByURI(pURI, iPage);
}

bool USSDK_OcxAdapter::bIMSSearchUserByDName(LPCTSTR pDName, long iPage)
{
	return bCtl_IMSSearchUserByDName(pDName, iPage);
}

bool USSDK_OcxAdapter::bIMSSearchUserOnline(long iPage)
{
	return bCtl_IMSSearchUserOnline(iPage);
}

bool USSDK_OcxAdapter::bIMSSearchUserByPhone(LPCTSTR pPhone,  long iPage)
{
	return bCtl_IMSSearchUserByPhone(pPhone, iPage);
}

bool USSDK_OcxAdapter::bIMSSearchUserByEmail(LPCTSTR pEmail, long iPage)
{
	return bCtl_IMSSearchUserByEmail(pEmail, iPage);
}

//联系人
bool USSDK_OcxAdapter::bIMSAddOneAddressList(LPCTSTR pURI, LPCTSTR pDisplayName, LPCTSTR pInfo)
{
	return bCtl_IMSAddOneAddressList(pURI, pDisplayName, pInfo);
}

bool USSDK_OcxAdapter::bIMSDeleteOneAddressList(LPCTSTR pURI)
{
	return bCtl_IMSDeleteOneAddressList(pURI);
}

bool USSDK_OcxAdapter::bIMSDoWatcherSubSeq(LPCTSTR pWatcherURI, long iAccept)
{
	return bCtl_IMSDoWatcherSubSeq(pWatcherURI, iAccept);
}

bool USSDK_OcxAdapter::bIMSAddRLSMember(LPCTSTR pURI, LPCTSTR pListName, LPCTSTR pListMemberURI, LPCTSTR pInfo)
{
	return bCtl_IMSAddRLSMember(pURI, pListName, pListMemberURI, pInfo);
}

bool USSDK_OcxAdapter::bIMSDeleteRLSMember(LPCTSTR pURI, LPCTSTR pListMemberURI)
{
	return bCtl_IMSDeleteRLSMember(pURI, pListMemberURI);
}

bool USSDK_OcxAdapter::bIMSLoadAddrList(long iType, short*pAddrList, long iNum, long iPage)
{
	return bCtl_IMSLoadAddrList(iType, pAddrList, iNum, iPage);
}

//群组相关
bool USSDK_OcxAdapter::bIMSCreateOnePrivateGroup(LPCTSTR pGroupName)
{
	return bCtl_IMSCreateOnePrivateGroup(pGroupName);
}

bool USSDK_OcxAdapter::bIMSModifyPrivateGroupName(LPCTSTR pOldName, LPCTSTR pNewName)
{
	return bCtl_IMSModifyPrivateGroupName(pOldName, pNewName);
}

bool USSDK_OcxAdapter::bIMSDeleteOnePrivateGroup(LPCTSTR pGroupName)
{
	return bCtl_IMSDeleteOnePrivateGroup(pGroupName);
}

bool USSDK_OcxAdapter::bIMSGetPrivateGroupMember(LPCTSTR lpIdentifier)
{
	return bCtl_IMSGetPrivateGroupMember(lpIdentifier);
}

bool USSDK_OcxAdapter::bIMSCopyListToPrivateGroup(LPCTSTR pListURI, LPCTSTR pGroupName, long iGroupType, LPCTSTR pDisplayName, long iSeq)
{
	return bCtl_IMSCopyListToPrivateGroup(pListURI, pGroupName, iGroupType, pDisplayName, iSeq);
}

bool USSDK_OcxAdapter::bIMSDeleteListFromPrivateGroup(LPCTSTR pListURI, LPCTSTR pGroupName)
{
	return bCtl_IMSDeleteListFromPrivateGroup(pListURI, pGroupName);
}

bool USSDK_OcxAdapter::bIMSSearchAddrList(long iType, short* pSearch, long iPage)
{
	return bCtl_IMSSearchAddrList(iType, pSearch, iPage);
}

bool USSDK_OcxAdapter::bIMSDoPublicGroupJoinReq(LPCTSTR pReqMemberName, LPCTSTR pReqURI, long iResult, LPCTSTR pReturn1, LPCTSTR pReturn2)
{
	return bCtl_IMSDoPublicGroupJoinReq(pReqMemberName, pReqURI, iResult, pReturn1, pReturn2);
}

bool USSDK_OcxAdapter::bIMSModifyOneListDpName(LPCTSTR pURI, LPCTSTR pDisplayName)
{
	return bCtl_IMSModifyOneListDpName(pURI, pDisplayName);
}

bool USSDK_OcxAdapter::bIMSReqJoinPublicGroup(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, LPCTSTR pMyDisplayName, LPCTSTR pShareUserURI, LPCTSTR pShareUserName)
{
	return bCtl_IMSReqJoinPublicGroup(pGroupURI, pGroupIdentifier, pMyDisplayName, pShareUserURI, pShareUserName);
}

bool USSDK_OcxAdapter::bIMSLeavePublicGroup(LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex)
{
	return bCtl_IMSLeavePublicGroup(pGroupName, pGroupURI, pOperaterIndex);
}

bool USSDK_OcxAdapter::bIMSDeleteOneFromMyPublicGroup(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex)
{
	return bCtl_IMSDeleteOneFromMyPublicGroup(pSomeoneURI, pGroupURI, pOperaterIndex);
}

bool USSDK_OcxAdapter::bIMSDeleteOnePublicGroup(LPCTSTR pGroupURI)
{
	return bCtl_IMSDeleteOnePublicGroup(pGroupURI);
}

bool USSDK_OcxAdapter::bIMSDispUICmdMethod(long lType, short* pCmdPara)
{
	return bCtl_IMSDispUICmdMethod(lType, pCmdPara);
}

bool USSDK_OcxAdapter::bModifyGroupAttributeReq(LPCTSTR pGroupURI, LPCTSTR pModType, LPCTSTR pNewValue)
{
	return bCtl_ModifyGroupAttributeReq(pGroupURI, pModType, pNewValue, "");
}

bool USSDK_OcxAdapter::bGetIMPrivateGroupMember(LPCTSTR  lpGroupIdentifier)
{
	return bCtl_GetIMPrivateGroupMember(lpGroupIdentifier);
}

bool USSDK_OcxAdapter::bUpdateMsgIndication(short* pPara)
{
	return bCtl_UpdateMsgIndication(pPara);
}

bool USSDK_OcxAdapter::bRequestLastLogInfo(short* pPara)
{
	return bCtl_RequestLastLogInfo(pPara);
}

bool USSDK_OcxAdapter::bIMSCreateOnePublicGroup(short* pPara)
{
	return bCtl_IMSCreateOnePublicGroup(pPara);
}

bool USSDK_OcxAdapter::bIMSInviteSomeoneToMyPublicGroup(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex)
{
	return bCtl_IMSInviteSomeoneToMyPublicGroup(pSomeoneURI, pDisplayName, pGroupURI, pOperaterIndex);
}

bool USSDK_OcxAdapter::bGetAllIMPrivateGroupList()
{
	return bCtl_GetAllIMPrivateGroupList();
}

bool USSDK_OcxAdapter::bSendSoapMessageByXcap(int iType, LPCTSTR lpcsRequestLine, LPCTSTR lpcstrAction, LPCTSTR lpcsBody)
{
	return bCtl_SendSoapMessageByXcap(iType, lpcsRequestLine, lpcstrAction, lpcsBody);
}

bool USSDK_OcxAdapter::bCheckURIIsMOA(LPCTSTR lpcsURI)
{
	return bCtl_CheckURIIsMOA(lpcsURI);
}

bool USSDK_OcxAdapter::bSendPubAccountReq(int iType, LPCTSTR pPubAccId)
{
	return bCtl_SendPubAccountReq(iType, pPubAccId);
}

bool USSDK_OcxAdapter::bGetHistoryPubMsg(int count, LPCTSTR pPubAccId, int seq, LPCTSTR pFlag)
{
	return bCtl_GetHistoryPubMsg(count, pPubAccId, seq, pFlag);
}

bool USSDK_OcxAdapter::bSearchPubAccount(int iType, LPCTSTR pubAccName, int pageNo, int pageSize)
{
	return bCtl_SearchPubAccount(iType, pubAccName, pageNo, pageSize);
}

bool USSDK_OcxAdapter::bGetPubAccountMenu(LPCTSTR pPubAccId, LPCTSTR etag)
{
	return bCtl_GetPubAccountMenu(pPubAccId, etag);
}

bool USSDK_OcxAdapter::bGetRecommendContact(int pageNo, int pageSize)
{
	return bCtl_GetRecommendContact(pageNo, pageSize);
}

bool USSDK_OcxAdapter::bCheckOsTimer(int pno)
{
	return bCtl_CheckOsTimer(pno);
}

bool USSDK_OcxAdapter::bClientLoginNotify(int type)
{
	return bCtl_ClientLoginNotify(type);
}

bool USSDK_OcxAdapter::bMOAImmediatelyReReg()
{
	return bCtl_MOAImmediatelyReReg();
}

bool USSDK_OcxAdapter::bCheckConnection()
{
	return bCtl_CheckConnection();
}

bool USSDK_OcxAdapter::bLoginInfoReport(LPCTSTR lpRecord)
{
	return bCtl_LoginInfoReport(lpRecord);
}

bool USSDK_OcxAdapter::bSyncUserCodeMoaInfo(int iType, LPCTSTR lpMsgId, LPCTSTR lpDeviceId, LPCTSTR lpXmlBody)
{
	return bCtl_SyncUserCodeMoaInfo(iType, lpMsgId, lpDeviceId, lpXmlBody);
}

bool USSDK_OcxAdapter::bGetSensitiveWord()
{
	return bCtl_GetSensitiveWord();
}

bool USSDK_OcxAdapter::bGetCheckCode(LPCTSTR lpUserCode)
{
	return bCtl_GetCheckCode(lpUserCode);
}

bool USSDK_OcxAdapter::bUserRegistReq(int iType, LPCTSTR pUserName, LPCTSTR pUserURI, LPCTSTR pPassword, LPCTSTR pCompany, LPCTSTR pCheckCode)
{
	return bCtl_UserRegistReq(iType, pUserName, pUserURI, pPassword, pCompany, pCheckCode);
}

bool USSDK_OcxAdapter::bUserModifyPasswdReq(int iType, LPCTSTR pUserURI, LPCTSTR pOldPassword, LPCTSTR pNewPassword)
{
	return bCtl_UserModifyPasswdReq(iType, pUserURI, pOldPassword, pNewPassword);
}

bool USSDK_OcxAdapter::bSendPubAccMsg(LPCTSTR pPubAccId, LPCTSTR pSender, int iMsgType, LPCTSTR pPutMsgId, LPCTSTR pMenuId, LPCTSTR pContent)
{
	return bCtl_SendPubAccMsg(pPubAccId, pSender, iMsgType, pPutMsgId, pMenuId, pContent);
}

bool USSDK_OcxAdapter::bUserBindOperateReq(int iOperType, LPCTSTR pBindNumber, int iNumberType, LPCTSTR pPasswd, LPCTSTR pCompanyId)
{
	return bCtl_UserBindOperateReq(iOperType, pBindNumber, iNumberType, pPasswd, pCompanyId);
}

bool USSDK_OcxAdapter::bGetGrayVersion(LPCTSTR lpVersion)
{
	return bCtl_GetGrayVersion(lpVersion);
}

INT32 USSDK_OcxAdapter::bCheckHttpServer(LPCTSTR pServerIP, int port, int sec)
{
	return bCtl_CheckHttpServer(pServerIP, port, sec);
}

INT32 USSDK_OcxAdapter::bCheckSipServer(LPCTSTR pServerIP, int port, int sec)
{
	return bCtl_CheckSipServer(pServerIP, port, sec);
}

bool USSDK_OcxAdapter::bGet2DCodeInfo(int iType, LPCTSTR strKeyWord, int validType, int clientExist, int codeType, LPCTSTR msgId)
{
	return bCtl_Get2DCodeInfo(iType, strKeyWord, validType, clientExist, codeType, msgId);
}

bool USSDK_OcxAdapter::bMessageMuteNotifyOperateReq(int iType, LPCTSTR pURI, LPCTSTR operateId)
{
	return bCtl_MessageMuteNotifyOperateReq(iType, pURI, operateId);
}

bool USSDK_OcxAdapter::bMessageReceiptOperateReq(int iOperType, LPCTSTR cSessionID, int isGroup, LPCTSTR pURI, LPCTSTR pMsgIDs)
{
	return bCtl_MessageReceiptOperateReq(iOperType, cSessionID, isGroup, pURI, pMsgIDs);
}

bool USSDK_OcxAdapter::bGetPublicMsg(LPCTSTR pMsgID,LPCTSTR lpURL)
{
	return bCtl_GetPublicMsg(pMsgID, lpURL);
}

bool USSDK_OcxAdapter::bSendRequestMsgToHttpAP(LPCTSTR cSessionID, LPCTSTR cRequestLine, LPCTSTR cMethod, LPCTSTR cContentType, LPCTSTR cEtag, char* pBody)
{
	return bCtl_SendRequestMsgToHttpAP(cSessionID, cRequestLine, cMethod, cContentType, cEtag, pBody);
}

bool USSDK_OcxAdapter::bGetUserLogonStatus(LPCTSTR lpURI)
{
	return bCtl_GetUserLogonStatus(lpURI);
}

bool USSDK_OcxAdapter::bConcernGroupOperateReq(int iType, LPCTSTR eTag, LPCTSTR gURI)
{
	return bCtl_ConcernGroupOperateReq(iType, eTag, gURI);
}

bool USSDK_OcxAdapter::bUploadFileByXcapReq(int iType, LPCTSTR msgId, LPCTSTR filePath)
{
	return bCtl_UploadFileByXcapReq(iType, msgId, filePath);
}

bool USSDK_OcxAdapter::bDownloadFileReq(LPCTSTR pSessionID, LPCTSTR pFilePathName, int iOperType, LPCTSTR pFileID)
{
	return bCtl_DownloadFileReq(pSessionID, pFilePathName, iOperType, pFileID);
}

bool USSDK_OcxAdapter::bUploadFileReq(LPCTSTR pSessionID, LPCTSTR pFilePathName, int iOperType, LPCTSTR pFileID)
{
	return bCtl_UploadFileReq(pSessionID, pFilePathName, iOperType, pFileID);
}

bool USSDK_OcxAdapter::bSendBroadcastMsgReq(LPCTSTR cMsgID, LPCTSTR pURIs, LPCTSTR gURIs, LPCTSTR filePath)
{
	return bCtl_SendBroadcastMsgReq(cMsgID, pURIs, gURIs, filePath);
}

bool USSDK_OcxAdapter::bGetOfflineMsgReq(LPCTSTR pSeq, int maxCount)
{
	return bCtl_GetOfflineMsgReq(pSeq, maxCount);
}

void USSDK_OcxAdapter::OnInitControlResult(long lResult)
{
    CorrespondEventWrapper()->OnInitControlResult(lResult);
}

//登录注销
void USSDK_OcxAdapter::OnLogonResult(long lResult, long lReason, long extraCode)
{
    CorrespondEventWrapper()->OnLogonResult(lResult, lReason, extraCode);
}

void USSDK_OcxAdapter::OnRegisterSoftPhoneToSS(char * lpSoftPhone, int lType)
{
    CorrespondEventWrapper()->OnRegisterSoftPhoneToSS(lpSoftPhone, lType);
}

void USSDK_OcxAdapter::OnAlreadyLogout(long lReason)
{
    CorrespondEventWrapper()->OnAlreadyLogout(lReason);
}

void USSDK_OcxAdapter::OnServerLinkLost(int itype)
{
    CorrespondEventWrapper()->OnServerLinkLost(itype);
}

void USSDK_OcxAdapter::OnLogoutServerSuccess()
{
    CorrespondEventWrapper()->OnLogoutServerSuccess();
}

void USSDK_OcxAdapter::OnServerLinkOK()
{
    CorrespondEventWrapper()->OnServerLinkOK();
}

void USSDK_OcxAdapter::OnRegisterSoftPhoneToSSNew(short far* pInfo)
{
    CorrespondEventWrapper()->OnRegisterSoftPhoneToSSNew(pInfo);
}

void USSDK_OcxAdapter::OnTry403FlowBegin()
{
    CorrespondEventWrapper()->OnTry403FlowBegin();
}

void USSDK_OcxAdapter::OnTry403FlowResult(int iResult)
{
    CorrespondEventWrapper()->OnTry403FlowResult(iResult);
}

//个人
void USSDK_OcxAdapter::OnGetXCAPCapsInfo(short far* pNotify)
{
    CorrespondEventWrapper()->OnGetXCAPCapsInfo(pNotify);
}

//XCAP_CAPS_INFO
void USSDK_OcxAdapter::OnIMSGotUserInfo(short far* pNotify)
{
    CorrespondEventWrapper()->OnIMSGotUserInfo(pNotify);
}

//USER_INFO
void USSDK_OcxAdapter::OnIMSPersonSubInfoChanged(long lType, short far* pPersonSubInfoEvent)
{
    CorrespondEventWrapper()->OnIMSPersonSubInfoChanged(lType, pPersonSubInfoEvent);
}

void USSDK_OcxAdapter::OnIMSGotPersonSubInfo(int retCode, long lType, short far*pPersonSubInfoEvent)
{
    CorrespondEventWrapper()->OnIMSGotPersonSubInfo(retCode, lType, pPersonSubInfoEvent);
}

void USSDK_OcxAdapter::OnIMSGotPersonSubInfoFailed(long lType, LPCTSTR lpUserURI, long lResult)
{
    CorrespondEventWrapper()->OnIMSGotPersonSubInfoFailed(lType, lpUserURI, lResult);
}

void USSDK_OcxAdapter::OnIMSPsEvent(long iType, short far* pNotify)
{
    CorrespondEventWrapper()->OnIMSPsEvent(iType, pNotify);
}

void USSDK_OcxAdapter::OnIMSBatchPsEvent(int iNum, int iCode, short far* pNotify)
{
    CorrespondEventWrapper()->OnIMSBatchPsEvent(iNum, iCode, pNotify);
}

void USSDK_OcxAdapter::OnXcapTokenInvalid()
{
    CorrespondEventWrapper()->OnXcapTokenInvalid();
}

//联系人
void USSDK_OcxAdapter::OnIMSGotOneWatcher(LPCTSTR pWatcherURI, LPCTSTR pStatus)
{
    CorrespondEventWrapper()->OnIMSGotOneWatcher(pWatcherURI, pStatus);
}

void USSDK_OcxAdapter::OnIMSWatcherSubscribeMe(LPCTSTR pWatcherURI, LPCTSTR pDisplayName, LPCTSTR pInfo, LPCTSTR pStatus)
{
    CorrespondEventWrapper()->OnIMSWatcherSubscribeMe(pWatcherURI, pDisplayName, pInfo, pStatus);
}

void USSDK_OcxAdapter::OnIMSGotMyPresenceRule(long iType, long iNum, short far* pURIList)
{
    CorrespondEventWrapper()->OnIMSGotMyPresenceRule(iType, iNum, pURIList);
}

void USSDK_OcxAdapter::OnIMSGotAddressList(int iNum, short far* pAddrList)
{
    CorrespondEventWrapper()->OnIMSGotAddressList(iNum, pAddrList);
}

void USSDK_OcxAdapter::OnIMSGotOneRLS(int iResult, LPCTSTR pURI, LPCTSTR pGroupName, LPCTSTR pListName, int iListNum, short far* pListMemberURI)
{
    CorrespondEventWrapper()->OnIMSGotOneRLS(iResult, pURI, pGroupName, pListName, iListNum, pListMemberURI);
}

void USSDK_OcxAdapter::OnIMSListPresenceArrived(LPCTSTR pSipFromURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone, long iTimestamp)
{
    CorrespondEventWrapper()->OnIMSListPresenceArrived(pSipFromURI, pListURI, pStatus, pPhone, iTimestamp);
}

void USSDK_OcxAdapter::OnIMSSetUserInfoResult(long lResult)
{
    CorrespondEventWrapper()->OnIMSSetUserInfoResult(lResult);
}

void USSDK_OcxAdapter::OnIMSGotOneListInfo(short far* pListInfo,int iReturnCode, LPCTSTR pURI)
{
    CorrespondEventWrapper()->OnIMSGotOneListInfo(pListInfo, iReturnCode, pURI);
}

//USER_INFO
void USSDK_OcxAdapter::OnIMSGotOneListInfoFailed(LPCTSTR lpUserURI, long lResult)
{
    CorrespondEventWrapper()->OnIMSGotOneListInfoFailed(lpUserURI, lResult);
}

void USSDK_OcxAdapter::OnIMSGotListInfo(long num, short far* pListInfo)
{
    CorrespondEventWrapper()->OnIMSGotListInfo(num, pListInfo);
}

void USSDK_OcxAdapter::OnIMSGotUserInfoFailed(long lResult)
{
    CorrespondEventWrapper()->OnIMSGotUserInfoFailed(lResult);
}

void USSDK_OcxAdapter::OnIMSSearchUserByURIResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    CorrespondEventWrapper()->OnIMSSearchUserByURIResult(pURI, pDName, pSex, iAge, pCity, iOnline);
}

void USSDK_OcxAdapter::OnIMSSearchUserByDNameResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    CorrespondEventWrapper()->OnIMSSearchUserByDNameResult(pURI, pDName, pSex, iAge, pCity, iOnline);
}

void USSDK_OcxAdapter::OnIMSSearchOneUserOnlineResult(int iPos, LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity)
{
    CorrespondEventWrapper()->OnIMSSearchOneUserOnlineResult(iPos, pURI, pDName, pSex, iAge, pCity);
}

void USSDK_OcxAdapter::OnIMSSearchUserByPhoneResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    CorrespondEventWrapper()->OnIMSSearchUserByPhoneResult(pURI, pDName, pSex, iAge, pCity, iOnline);
}

void USSDK_OcxAdapter::OnIMSSearchUserByEmailResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    CorrespondEventWrapper()->OnIMSSearchUserByEmailResult(pURI, pDName, pSex, iAge, pCity, iOnline);
}

void USSDK_OcxAdapter::OnIMSAddListResult(LPCTSTR pURI, LPCTSTR pName, int iResult)
{
    CorrespondEventWrapper()->OnIMSAddListResult(pURI, pName, iResult);
}

void USSDK_OcxAdapter::OnIMSAddRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult)
{
    CorrespondEventWrapper()->OnIMSAddRLSMemberResult(pURI, pListMemberURI, iResult);
}

void USSDK_OcxAdapter::OnIMSDeleteRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult)
{
    CorrespondEventWrapper()->OnIMSDeleteRLSMemberResult(pURI, pListMemberURI, iResult);
}

void USSDK_OcxAdapter::OnIMSDelListResult(LPCTSTR pURI, int iResult)
{
    CorrespondEventWrapper()->OnIMSDelListResult(pURI, iResult);
}

void USSDK_OcxAdapter::OnIMSSetPresenceRulesResult(LPCTSTR pURI, int iType)
{
    CorrespondEventWrapper()->OnIMSSetPresenceRulesResult(pURI, iType);
}

void USSDK_OcxAdapter::OnIMSLoadAddrListResult(long iType, long iCode, long iNum, short far* pAddrList)
{
    CorrespondEventWrapper()->OnIMSLoadAddrListResult(iType, iCode, iNum, pAddrList);
}

void USSDK_OcxAdapter::OnIMSGotDeptAddrListResult(long iType, long iCode, long iNum, short far* pAddrList, LPCTSTR pNodeURI)
{
    CorrespondEventWrapper()->OnIMSGotDeptAddrListResult(iType, iCode, iNum, pAddrList, pNodeURI);
}

void USSDK_OcxAdapter::OnIMSSearchAddrListResult(long iType, long iCode, short far* pAddrList)
{
    CorrespondEventWrapper()->OnIMSSearchAddrListResult(iType, iCode, pAddrList);
}

void USSDK_OcxAdapter::OnIMSSubResult(int iType, LPCTSTR pURI, int iResult)
{
    CorrespondEventWrapper()->OnIMSSubResult(iType, pURI, iResult);
}

void USSDK_OcxAdapter::OnCheckURIIsMOAResult(int iResult, long num, short far* pCheckList)
{
    CorrespondEventWrapper()->OnCheckURIIsMOAResult(iResult, num, pCheckList);
}

void USSDK_OcxAdapter::OnGetUserLogonStatusResult(short far *pResult)
{
    CorrespondEventWrapper()->OnGetUserLogonStatusResult(pResult);
} //GetUserLogonStatusResult_T

//消息
void USSDK_OcxAdapter::OnIMSSendMessageResult(short far* pNotify)
{
    CorrespondEventWrapper()->OnIMSSendMessageResult(pNotify);
}

void USSDK_OcxAdapter::OnIMSMessageArrived(int iType, LPCTSTR pChatRoomURI, LPCTSTR pSenderURI, LPCTSTR pMessage, LPCTSTR pTime, LPCTSTR pMessageId, int iReceipt, LPCTSTR pSipTime)
{
    CorrespondEventWrapper()->OnIMSMessageArrived(iType, pChatRoomURI, pSenderURI, pMessage, pTime, pMessageId, iReceipt, pSipTime);
}

void USSDK_OcxAdapter::OnIMSAttachMessageArrived(int iType, LPCTSTR pSenderURI, LPCTSTR pChatroomURI, LPCTSTR pSendTime, LPCTSTR pAttachFilePath, LPCTSTR pAttachFileType, LPCTSTR pMessage, LPCTSTR pMessageId)
{
    CorrespondEventWrapper()->OnIMSAttachMessageArrived(iType, pSenderURI, pChatroomURI, pSendTime, pAttachFilePath, pAttachFileType, pMessage, pMessageId);
}

void USSDK_OcxAdapter::OnIMSSendLargeMessageResult(short far* pNotify)
{
    CorrespondEventWrapper()->OnIMSSendLargeMessageResult(pNotify);
}

void USSDK_OcxAdapter::OnIMSAssociatedIdentifyArrived(long iNum, short far* pIdentify)
{
    CorrespondEventWrapper()->OnIMSAssociatedIdentifyArrived(iNum, pIdentify);
}

void USSDK_OcxAdapter::OnIMSSendMessageReport(LPCTSTR pReportID, LPCTSTR pReportType, LPCTSTR pDoneTime, LPCTSTR pErrorCode, LPCTSTR pTerminal)
{
    CorrespondEventWrapper()->OnIMSSendMessageReport(pReportID, pReportType, pDoneTime, pErrorCode, pTerminal);
}

void USSDK_OcxAdapter::OnIMSMessageListArrived(int nNum, void **pMsgList)
{
    CorrespondEventWrapper()->OnIMSMessageListArrived(nNum, pMsgList);
}

void USSDK_OcxAdapter::OnIMSGroupOfflineMsgNotify(int iMsgNum, LPCTSTR cGroupURI, LPCTSTR pMessage, LPCTSTR timestamp)
{
    CorrespondEventWrapper()->OnIMSGroupOfflineMsgNotify(iMsgNum, cGroupURI, pMessage, timestamp);
}

void USSDK_OcxAdapter::OnIMSGotGroupOfflineMsgResult(short far *pResult)
{
    CorrespondEventWrapper()->OnIMSGotGroupOfflineMsgResult(pResult);
}

void USSDK_OcxAdapter::OnGetOfflineMsgRsp(short far *pResult)
{
    CorrespondEventWrapper()->OnGetOfflineMsgRsp(pResult);
}

//群组
void USSDK_OcxAdapter::OnIMSGotGroupInfo(int index, int count, short far* pNotify)
{
    CorrespondEventWrapper()->OnIMSGotGroupInfo(index, count, pNotify);
}//IMSGotGroupInfo_T or FIRE_GROUP_INFO

void USSDK_OcxAdapter::OnIMSGetPublicGroupInfoListFailed(LPCTSTR lpGroupURI, long lResult)
{
    CorrespondEventWrapper()->OnIMSGetPublicGroupInfoListFailed(lpGroupURI, lResult);
}

void USSDK_OcxAdapter::OnIMSGotOnePublicGroup(int flag, LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperUser, long iMyRole, short far* pAttribute, long iMemberNum, void **pMemberList, long iBlacklistNum, short far* pBlacklist, LPCTSTR pOperUserName)
{
    CorrespondEventWrapper()->OnIMSGotOnePublicGroup(flag, pGroupName, pGroupURI, pOperUser, iMyRole, pAttribute, iMemberNum, pMemberList, iBlacklistNum, pBlacklist, pOperUserName);
}

void USSDK_OcxAdapter::OnIMSCreatePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, long iResult, long iGroupType, LPCTSTR pGroupOperateindex)
{
    CorrespondEventWrapper()->OnIMSCreatePublicGroupResult(pGroupName, pGroupURI, iResult, iGroupType, pGroupOperateindex);
}

void USSDK_OcxAdapter::OnIMSModifyPublicGroupResult(LPCTSTR pGroupURI, long iType, LPCTSTR pNewName, long iNewValue, long iResult, LPCTSTR pGroupOperateindex)
{
    CorrespondEventWrapper()->OnIMSModifyPublicGroupResult(pGroupURI, iType, pNewName, iNewValue, iResult, pGroupOperateindex);
}

void USSDK_OcxAdapter::OnIMSInviteToMyPuGroupSendResult(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, long iResult, LPCTSTR pOperateIndex)
{
    CorrespondEventWrapper()->OnIMSInviteToMyPuGroupSendResult(pSomeoneURI, pDisplayName, pGroupURI, iResult, pOperateIndex);
}

void USSDK_OcxAdapter::OnIMSPGroupListPresenceArrived(LPCTSTR pPublicGroupURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone)
{
    CorrespondEventWrapper()->OnIMSPGroupListPresenceArrived(pPublicGroupURI, pListURI, pStatus, pPhone);
}

void USSDK_OcxAdapter::OnIMSInvitedToPublicGroup(short far* pPara)
{
    CorrespondEventWrapper()->OnIMSInvitedToPublicGroup(pPara);
}//IMSInvitedToPublicGroup_T

void USSDK_OcxAdapter::OnIMSPublicGroupInviteResult(LPCTSTR pGroupURI, LPCTSTR pInvitedUserURI, long iResult)
{
    CorrespondEventWrapper()->OnIMSPublicGroupInviteResult(pGroupURI, pInvitedUserURI, iResult);
}

void USSDK_OcxAdapter::OnIMSReqJoinMyPublicGroup(short far* pPara)
{
    CorrespondEventWrapper()->OnIMSReqJoinMyPublicGroup(pPara);
}//IMSReqJoinMyPublicGroup_T

void USSDK_OcxAdapter::OnIMSInvitedToTmpPublicGroup(short far* pPara)
{
    CorrespondEventWrapper()->OnIMSInvitedToTmpPublicGroup(pPara);
}

void USSDK_OcxAdapter::OnIMSPublicGroupMemberNotify(short far* pPara)
{
    CorrespondEventWrapper()->OnIMSPublicGroupMemberNotify(pPara);
}

void USSDK_OcxAdapter::OnIMSDeleteMeInPublicGroup(LPCTSTR pGroupURI, int iType)
{
    CorrespondEventWrapper()->OnIMSDeleteMeInPublicGroup(pGroupURI, iType);
}

void USSDK_OcxAdapter::OnIMSDoPublicGroupInviteResult(long lDoType, LPCTSTR pGroupURI, LPCTSTR pGroupName, LPCTSTR lpcsOwnerName, long lCode)
{
    CorrespondEventWrapper()->OnIMSDoPublicGroupInviteResult(lDoType, pGroupURI, pGroupName, lpcsOwnerName, lCode);
}

void USSDK_OcxAdapter::OnIMSDoGroupJoinReqResult(LPCTSTR pGroupURI, LPCTSTR pMemberURI, LPCTSTR pMemberName, long lResult)
{
    CorrespondEventWrapper()->OnIMSDoGroupJoinReqResult(pGroupURI, pMemberURI, pMemberName, lResult);
}

void USSDK_OcxAdapter::OnIMSModifyOneListDpNameResult(LPCTSTR pURI, LPCTSTR pDisplayName, int iResult)
{
    CorrespondEventWrapper()->OnIMSModifyOneListDpNameResult(pURI, pDisplayName, iResult);
}

void USSDK_OcxAdapter::OnIMSReqJoinPublicGroupResult(LPCTSTR pGroupURI, LPCTSTR pGroupName, long iResult)
{
    CorrespondEventWrapper()->OnIMSReqJoinPublicGroupResult(pGroupURI, pGroupName, iResult);
}

void USSDK_OcxAdapter::OnIMSLeavePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, int iResult, LPCTSTR pGroupOperateindex)
{
    CorrespondEventWrapper()->OnIMSLeavePublicGroupResult(pGroupName, pGroupURI, iResult, pGroupOperateindex);
}

void USSDK_OcxAdapter::OnIMSReqJoinPublicGroupSendResult(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, long iResult)
{
    CorrespondEventWrapper()->OnIMSReqJoinPublicGroupSendResult(pGroupURI, pGroupIdentifier, iResult);
}

void USSDK_OcxAdapter::OnIMSDeletePublicGroupResult(LPCTSTR pGroupURI, long iResult)
{
    CorrespondEventWrapper()->OnIMSDeletePublicGroupResult(pGroupURI, iResult);
}

void USSDK_OcxAdapter::OnIMSDelFromMyPuGroupResult(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, long iResult, LPCTSTR pGroupOperateindex)
{
    CorrespondEventWrapper()->OnIMSDelFromMyPuGroupResult(pSomeoneURI, pGroupURI, iResult, pGroupOperateindex);
}

void USSDK_OcxAdapter::OnIMSCreatePrivateGroupResult(LPCTSTR pGroupName, long iResult)
{
    CorrespondEventWrapper()->OnIMSCreatePrivateGroupResult(pGroupName, iResult);
}

void USSDK_OcxAdapter::OnIMSModifyPrivateGroupNameResult(LPCTSTR pOldName, LPCTSTR pNewName, long iResult)
{
    CorrespondEventWrapper()->OnIMSModifyPrivateGroupNameResult(pOldName, pNewName, iResult);
}

void USSDK_OcxAdapter::OnIMSDeletePrivateGroupResult(LPCTSTR pGroupName, long iResult)
{
    CorrespondEventWrapper()->OnIMSDeletePrivateGroupResult(pGroupName, iResult);
}

void USSDK_OcxAdapter::OnIMSGotOnePrivateList(LPCTSTR szGroupName, int iListNum, short far *pListURI)
{
    CorrespondEventWrapper()->OnIMSGotOnePrivateList(szGroupName, iListNum, pListURI);
}

void USSDK_OcxAdapter::OnIMSCopyPrivateGroupListResult(LPCTSTR pGroupName, long iGroupType, LPCTSTR pListURI, long iResult)
{
    CorrespondEventWrapper()->OnIMSCopyPrivateGroupListResult(pGroupName, iGroupType, pListURI, iResult);
}

void USSDK_OcxAdapter::OnIMSDelPrivateGroupListResult(LPCTSTR pGroupName, LPCTSTR pListURI, long iResult)
{
    CorrespondEventWrapper()->OnIMSDelPrivateGroupListResult(pGroupName, pListURI, iResult);
}

void USSDK_OcxAdapter::OnGotIMAllPrivateGroupListResult(int iResult, long num, short far* pGroupList)
{
    CorrespondEventWrapper()->OnGotIMAllPrivateGroupListResult(iResult, num, pGroupList);
}

void USSDK_OcxAdapter::OnGotIMPrivateGroupMemberResult(LPCTSTR pGroupIdentifier, int iResult, long num, short far* pMemberList)
{
    CorrespondEventWrapper()->OnGotIMPrivateGroupMemberResult(pGroupIdentifier, iResult, num, pMemberList);
}

void USSDK_OcxAdapter::OnModifyGroupAttributeResult(short far *pResult)
{
    CorrespondEventWrapper()->OnModifyGroupAttributeResult(pResult);
}//ModifyGroupAttribute_T

//呼叫
void USSDK_OcxAdapter::OnReportMemberStatus(LPCTSTR lpJID, LPCTSTR lpCallee, long lStatus, long lCallType)
{
    CorrespondEventWrapper()->OnReportMemberStatus(lpJID, lpCallee, lStatus, lCallType);
}

void USSDK_OcxAdapter::OnSomeOneCallMe(LPCTSTR lpJID, LPCTSTR lpCaller, long lType)
{
    CorrespondEventWrapper()->OnSomeOneCallMe(lpJID, lpCaller, lType);
}

void USSDK_OcxAdapter::OnIMSCallEvent(long lType, LPCTSTR lpCaller, short far* pCallEvent)
{
    CorrespondEventWrapper()->OnIMSCallEvent(lType, lpCaller, pCallEvent);
}

void USSDK_OcxAdapter::OnMultiRing()
{
    CorrespondEventWrapper()->OnMultiRing();
}

void USSDK_OcxAdapter::OnNewCallWhenBusy2(LPCTSTR pCaller, long iType, LPCTSTR pURL, long iCallType)
{
    CorrespondEventWrapper()->OnNewCallWhenBusy2(pCaller, iType, pURL, iCallType);
}

void USSDK_OcxAdapter::OnCalleeForward(LPCTSTR pCalleeNumber, LPCTSTR pForwardNumber)
{
    CorrespondEventWrapper()->OnCalleeForward(pCalleeNumber, pForwardNumber);
}

void USSDK_OcxAdapter::OnPhoneConnected(long lVideo)
{
    CorrespondEventWrapper()->OnPhoneConnected(lVideo);
}

void USSDK_OcxAdapter::OnPhoneIdle()
{
    CorrespondEventWrapper()->OnPhoneIdle();
}

void USSDK_OcxAdapter::OnPhoneStatus(long lType, long lValue, LPCTSTR lpMsg)
{
    CorrespondEventWrapper()->OnPhoneStatus(lType, lValue, lpMsg);
}

void USSDK_OcxAdapter::OnCallOver()
{
    CorrespondEventWrapper()->OnCallOver();
}

void USSDK_OcxAdapter::OnCtdCallEvent(long lType, short far* pPara)
{
    CorrespondEventWrapper()->OnCtdCallEvent(lType, pPara);
}


//公众号
void USSDK_OcxAdapter::OnGotPubAccountListResult(int iResult, int nNum, void **pPubAccountList)
{
    CorrespondEventWrapper()->OnGotPubAccountListResult(iResult, nNum, pPubAccountList);
}

void USSDK_OcxAdapter::OnIMSGotPublishMsgResult(int msgType, LPCTSTR pLocalMsgID, short far *pResult)
{
    CorrespondEventWrapper()->OnIMSGotPublishMsgResult(msgType, pLocalMsgID, pResult);
}//PublishMsgList_T

void USSDK_OcxAdapter::OnAddOnePubAccountResult(int iResult, int nNum, void **pPubAccountList)
{
    CorrespondEventWrapper()->OnAddOnePubAccountResult(iResult, nNum, pPubAccountList);
}

void USSDK_OcxAdapter::OnDeleteOnePubAccountResult(int iResult, LPCTSTR pubAccId)
{
    CorrespondEventWrapper()->OnDeleteOnePubAccountResult(iResult, pubAccId);
}

void USSDK_OcxAdapter::OnSearchPubAccountResult(int iResult, int iType, int pageNo, int pageSize, int nNum, void **pPubAccountList)
{
    CorrespondEventWrapper()->OnSearchPubAccountResult(iResult, iType, pageNo, pageSize, nNum, pPubAccountList);
}

void USSDK_OcxAdapter::OnUserCodeMoaInfoResult(int iType, int iResult, LPCTSTR msgId, LPCTSTR etag, int nNum, void **pUserMoaInfo)
{
    CorrespondEventWrapper()->OnUserCodeMoaInfoResult(iType, iResult, msgId, etag, nNum, pUserMoaInfo);
}

void USSDK_OcxAdapter::OnGotPubAccountMenuResult(int iResult, LPCTSTR pubAccId, LPCTSTR etag, int nNum, void **pPubAccountMenu)
{
    CorrespondEventWrapper()->OnGotPubAccountMenuResult(iResult, pubAccId, etag, nNum, pPubAccountMenu);
}

void USSDK_OcxAdapter::OnSendPubAccMsgResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc, int iMsgType, LPCTSTR lpPutMsgId, LPCTSTR lpMenuId, LPCTSTR lpDate)
{
    CorrespondEventWrapper()->OnSendPubAccMsgResult(iHttpCode, iResultCode, lpResultDesc, iMsgType, lpPutMsgId, lpMenuId, lpDate);
}

//个人设置
void USSDK_OcxAdapter::OnMessageMuteNotifyOperateResult(short far *pResult)
{
    CorrespondEventWrapper()->OnMessageMuteNotifyOperateResult(pResult);
} //MuteNotifyOperate_T

void USSDK_OcxAdapter::OnMessageReceiptOperateResult(int iOperType, LPCTSTR sessionID, int iResultCode, LPCTSTR pMsgIDs, void* pQryRsp)
{
    CorrespondEventWrapper()->OnMessageReceiptOperateResult(iOperType, sessionID, iResultCode, pMsgIDs, pQryRsp);
}    //MsgReceiptQryRsp_T

void USSDK_OcxAdapter::OnMessageReceiptNotify(short far *pResult)
{
    CorrespondEventWrapper()->OnMessageReceiptNotify(pResult);
} //MsgReceiptNotify_T

void USSDK_OcxAdapter::OnConcernGroupOperateResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR uriList)
{
    CorrespondEventWrapper()->OnConcernGroupOperateResult(iRetCode, iType, eTag, uriList);
}

void USSDK_OcxAdapter::OnContactMarkResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR markList)
{
    CorrespondEventWrapper()->OnContactMarkResult(iRetCode, iType, eTag, markList);
}

//文件传输
void USSDK_OcxAdapter::OnUploadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iResultCode, int iRate, DWORD iTransedSize)
{
    CorrespondEventWrapper()->OnUploadFileRsp(pSessionID, pFileID, iResultCode, iRate, iTransedSize);
}

void USSDK_OcxAdapter::OnDownloadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iResultCode, int iRate, DWORD iTransedSize)
{
    CorrespondEventWrapper()->OnDownloadFileRsp(pSessionID, pFileID, iResultCode, iRate, iTransedSize);
}

//上报接口
void USSDK_OcxAdapter::OnSendBroadcastMsgRsp(short far *pResult)
{
    CorrespondEventWrapper()->OnSendBroadcastMsgRsp(pResult);
} //BroadcastMsgRsp_T

//通用
void USSDK_OcxAdapter::OnDispUICmdForEcp2Result(long lType, short far* pPara)
{
    CorrespondEventWrapper()->OnDispUICmdForEcp2Result(lType, pPara);
}

void USSDK_OcxAdapter::OnIMSDispUICmdResult(long lType, short far* pPara)
{
    CorrespondEventWrapper()->OnIMSDispUICmdResult(lType, pPara);
}

void USSDK_OcxAdapter::OnIMSRuleSetEvent(long lType, short far* pPara)
{
    CorrespondEventWrapper()->OnIMSRuleSetEvent(lType, pPara);
}

void USSDK_OcxAdapter::OnMsgIndicationNotify(short far* pPara)
{
    CorrespondEventWrapper()->OnMsgIndicationNotify(pPara);
}//MsgIndication_T

void USSDK_OcxAdapter::OnGotRegInfoNotify(int iResult, short far* pPara)
{
    CorrespondEventWrapper()->OnGotRegInfoNotify(iResult, pPara);
}

void USSDK_OcxAdapter::OnSendSoapMessageByXcapResult(int iCode, int iType, LPCTSTR lpcsBody)
{
    CorrespondEventWrapper()->OnSendSoapMessageByXcapResult(iCode, iType, lpcsBody);
}

void USSDK_OcxAdapter::OnGetCheckCodeResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc)
{
    CorrespondEventWrapper()->OnGetCheckCodeResult(iHttpCode, iResultCode, lpResultDesc);
}

void USSDK_OcxAdapter::OnGetRecommendContactResult(int iResult, int pageNo, int pageSize, int nNum, void **pContactInfoList)
{
    CorrespondEventWrapper()->OnGetRecommendContactResult(iResult, pageNo, pageSize, nNum, pContactInfoList);
}

void USSDK_OcxAdapter::OnGetSensWordListResult(int nNum, void *pWordList)
{
    CorrespondEventWrapper()->OnGetSensWordListResult(nNum, pWordList);
}

void USSDK_OcxAdapter::OnUserBindOperResult(int iResultCode, int iOperType, LPCTSTR lpURI, LPCTSTR lpCompanyID)
{
    CorrespondEventWrapper()->OnUserBindOperResult(iResultCode, iOperType, lpURI, lpCompanyID);
}

void USSDK_OcxAdapter::OnGet2DCodeInfoResult(short far *pResult)
{
    CorrespondEventWrapper()->OnGet2DCodeInfoResult(pResult);
}//Get2DCodeInfoResult_T

void USSDK_OcxAdapter::OnResponseMsgFromHttpAP(LPCTSTR pSessionID, LPCTSTR pRequestLine, int iRetCode, LPCTSTR pContentType, LPCTSTR pBody, LPCTSTR pEtag)
{
    CorrespondEventWrapper()->OnResponseMsgFromHttpAP(pSessionID, pRequestLine, iRetCode, pContentType, pBody, pEtag);
}

void USSDK_OcxAdapter::OnUploadFileByXcapRsp(int iRetCode, int iType, LPCTSTR msgId, LPCTSTR filePath)
{
    CorrespondEventWrapper()->OnUploadFileByXcapRsp(iRetCode, iType, msgId, filePath);
}

void USSDK_OcxAdapter::OnPushAPNsResult(LPCTSTR pToken, LPCTSTR pTokenVoIP, long iResult)
{
    CorrespondEventWrapper()->OnPushAPNsResult(pToken, pTokenVoIP, iResult);
}

//检查
void USSDK_OcxAdapter::OnCheckConnectResult(int iResult, int delaysec)
{
    CorrespondEventWrapper()->OnCheckConnectResult(iResult, delaysec);
}

//版本灰度
void USSDK_OcxAdapter::OnGetGrayVersionResult(int iHttpCode, LPCTSTR lpGrayVersion, LPCTSTR lpDownloadUri, LPCTSTR lpDownloadSize, LPCTSTR lpGrayVerInfo)
{
    CorrespondEventWrapper()->OnGetGrayVersionResult(iHttpCode, lpGrayVersion, lpDownloadUri, lpDownloadSize, lpGrayVerInfo);
}

