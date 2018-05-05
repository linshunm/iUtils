//
//  USSDK_EventWrapper.h
//  SYDUSSDK
//
//  Created by moa_mini2 on 2018/5/2.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_EventWrapper_h
#define USSDK_EventWrapper_h

#include "../USSDKMethod.h"

#include "../clt_include/OcxZXInterface.h"
#include "../LoginAgent/LoginService/NotifyAPI/USSDK_LoginServiceCallBack.h"
#include "../RosterAgent/RosterService/NotifyAPI/USSDK_RosterServiceCallBack.h"

namespace NS_USSDK_DEFAULT
{
    class USSDK_API USSDK_EventWrapper
    {
//    public:
//        USSDK_EventWrapper() = 0;
//        USSDK_EventWrapper() = 0;
        
    public:
        //初始化
        virtual void OnInitControlResult(long lResult) = 0; //保留
        
        //登录注销
        virtual void OnLogonResult(long lResult, long lReason, long extraCode) = 0;
        virtual void OnRegisterSoftPhoneToSS(char * lpSoftPhone, int lType) = 0;
        virtual void OnAlreadyLogout(long lReason) = 0;
        virtual void OnServerLinkLost(int itype) = 0;
        virtual void OnLogoutServerSuccess() = 0;
        virtual void OnServerLinkOK() = 0;
        virtual void OnRegisterSoftPhoneToSSNew(short far* pInfo) = 0;
        virtual void OnTry403FlowBegin() = 0;
        virtual void OnTry403FlowResult(int iResult) = 0;
        
        //个人
        virtual void OnGetXCAPCapsInfo(short far* pNotify) = 0;//XCAP_CAPS_INFO
        virtual void OnIMSGotUserInfo(short far* pNotify) = 0;//USER_INFO
        virtual void OnIMSPersonSubInfoChanged(long lType, short far* pPersonSubInfoEvent) = 0;
        virtual void OnIMSGotPersonSubInfo(int retCode, long lType, short far*pPersonSubInfoEvent) = 0;
        virtual void OnIMSGotPersonSubInfoFailed(long lType, LPCTSTR lpUserURI, long lResult) = 0;
        virtual void OnIMSPsEvent(long iType, short far* pNotify) = 0;
        virtual void OnIMSBatchPsEvent(int iNum, int iCode, short far* pNotify) = 0;
        virtual void OnXcapTokenInvalid() = 0;
        
        
        //联系人
        virtual void OnIMSGotOneWatcher(LPCTSTR pWatcherURI, LPCTSTR pStatus) = 0;
        virtual void OnIMSWatcherSubscribeMe(LPCTSTR pWatcherURI, LPCTSTR pDisplayName, LPCTSTR pInfo, LPCTSTR pStatus) = 0;
        virtual void OnIMSGotMyPresenceRule(long iType, long iNum, short far* pURIList) = 0;
        virtual void OnIMSGotAddressList(int iNum, short far* pAddrList) = 0;
        virtual void OnIMSGotOneRLS(int iResult, LPCTSTR pURI, LPCTSTR pGroupName, LPCTSTR pListName, int iListNum, short far* pListMemberURI) = 0;
        virtual void OnIMSListPresenceArrived(LPCTSTR pSipFromURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone, long iTimestamp) = 0;
        virtual void OnIMSSetUserInfoResult(long lResult) = 0;
        virtual void OnIMSGotOneListInfo(short far* pListInfo,int iReturnCode, LPCTSTR pURI) = 0;//USER_INFO
        virtual void OnIMSGotOneListInfoFailed(LPCTSTR lpUserURI, long lResult) = 0;
        virtual void OnIMSGotListInfo(long num, short far* pListInfo) = 0;
        virtual void OnIMSGotUserInfoFailed(long lResult) = 0;
        virtual void OnIMSSearchUserByURIResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline) = 0;
        virtual void OnIMSSearchUserByDNameResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline) = 0;
        virtual void OnIMSSearchOneUserOnlineResult(int iPos, LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity) = 0;
        virtual void OnIMSSearchUserByPhoneResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline) = 0;
        virtual void OnIMSSearchUserByEmailResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline) = 0;
        virtual void OnIMSAddListResult(LPCTSTR pURI, LPCTSTR pName, int iResult) = 0;
        virtual void OnIMSAddRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult) = 0;
        virtual void OnIMSDeleteRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult) = 0;
        virtual void OnIMSDelListResult(LPCTSTR pURI, int iResult) = 0;
        virtual void OnIMSSetPresenceRulesResult(LPCTSTR pURI, int iType) = 0;
        virtual void OnIMSLoadAddrListResult(long iType, long iCode, long iNum, short far* pAddrList) = 0;
        virtual void OnIMSGotDeptAddrListResult(long iType, long iCode, long iNum, short far* pAddrList, LPCTSTR pNodeURI) = 0;
        virtual void OnIMSSearchAddrListResult(long iType, long iCode, short far* pAddrList) = 0;
        virtual void OnIMSSubResult(int iType, LPCTSTR pURI, int iResult) = 0;
        virtual void OnCheckURIIsMOAResult(int iResult, long num, short far* pCheckList) = 0;
        virtual void OnGetUserLogonStatusResult(short far *pResult) = 0; //GetUserLogonStatusResult_T
        
        //消息
        virtual void OnIMSSendMessageResult(short far* pNotify) = 0;
        virtual void OnIMSMessageArrived(int iType, LPCTSTR pChatRoomURI, LPCTSTR pSenderURI, LPCTSTR pMessage, LPCTSTR pTime, LPCTSTR pMessageId, int iReceipt, LPCTSTR pSipTime) = 0;
        virtual void OnIMSAttachMessageArrived(int iType, LPCTSTR pSenderURI, LPCTSTR pChatroomURI, LPCTSTR pSendTime, LPCTSTR pAttachFilePath, LPCTSTR pAttachFileType, LPCTSTR pMessage, LPCTSTR pMessageId) = 0;
        virtual void OnIMSSendLargeMessageResult(short far* pNotify) = 0;
        virtual void OnIMSAssociatedIdentifyArrived(long iNum, short far* pIdentify) = 0;
        virtual void OnIMSSendMessageReport(LPCTSTR pReportID, LPCTSTR pReportType, LPCTSTR pDoneTime, LPCTSTR pErrorCode, LPCTSTR pTerminal) = 0;
        virtual void OnIMSMessageListArrived(int nNum, void **pMsgList) = 0;
        virtual void OnIMSGroupOfflineMsgNotify(int iMsgNum, LPCTSTR cGroupURI, LPCTSTR pMessage, LPCTSTR timestamp) = 0;
        virtual void OnIMSGotGroupOfflineMsgResult(short far *pResult) = 0;
        virtual void OnGetOfflineMsgRsp(short far *pResult) = 0;
        
        //群组
        virtual void OnIMSGotGroupInfo(int index, int count, short far* pNotify) = 0;//IMSGotGroupInfo_T or FIRE_GROUP_INFO
        virtual void OnIMSGetPublicGroupInfoListFailed(LPCTSTR lpGroupURI, long lResult) = 0;
        virtual void OnIMSGotOnePublicGroup(int flag, LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperUser, long iMyRole, short far* pAttribute, long iMemberNum, void **pMemberList, long iBlacklistNum, short far* pBlacklist, LPCTSTR pOperUserName) = 0;
        virtual void OnIMSCreatePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, long iResult, long iGroupType, LPCTSTR pGroupOperateindex) = 0;
        virtual void OnIMSModifyPublicGroupResult(LPCTSTR pGroupURI, long iType, LPCTSTR pNewName, long iNewValue, long iResult, LPCTSTR pGroupOperateindex) = 0;
        virtual void OnIMSInviteToMyPuGroupSendResult(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, long iResult, LPCTSTR pOperateIndex) = 0;
        virtual void OnIMSPGroupListPresenceArrived(LPCTSTR pPublicGroupURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone) = 0;
        virtual void OnIMSInvitedToPublicGroup(short far* pPara) = 0;//IMSInvitedToPublicGroup_T
        virtual void OnIMSPublicGroupInviteResult(LPCTSTR pGroupURI, LPCTSTR pInvitedUserURI, long iResult) = 0;
        virtual void OnIMSReqJoinMyPublicGroup(short far* pPara) = 0;//IMSReqJoinMyPublicGroup_T
        virtual void OnIMSInvitedToTmpPublicGroup(short far* pPara) = 0;
        virtual void OnIMSPublicGroupMemberNotify(short far* pPara) = 0;
        virtual void OnIMSDeleteMeInPublicGroup(LPCTSTR pGroupURI, int iType) = 0;
        virtual void OnIMSDoPublicGroupInviteResult(long lDoType, LPCTSTR pGroupURI, LPCTSTR pGroupName, LPCTSTR lpcsOwnerName, long lCode) = 0;
        virtual void OnIMSDoGroupJoinReqResult(LPCTSTR pGroupURI, LPCTSTR pMemberURI, LPCTSTR pMemberName, long lResult) = 0;
        virtual void OnIMSModifyOneListDpNameResult(LPCTSTR pURI, LPCTSTR pDisplayName, int iResult) = 0;
        virtual void OnIMSReqJoinPublicGroupResult(LPCTSTR pGroupURI, LPCTSTR pGroupName, long iResult) = 0;
        virtual void OnIMSLeavePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, int iResult, LPCTSTR pGroupOperateindex) = 0;
        virtual void OnIMSReqJoinPublicGroupSendResult(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, long iResult) = 0;
        virtual void OnIMSDeletePublicGroupResult(LPCTSTR pGroupURI, long iResult) = 0;
        virtual void OnIMSDelFromMyPuGroupResult(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, long iResult, LPCTSTR pGroupOperateindex) = 0;
        virtual void OnIMSCreatePrivateGroupResult(LPCTSTR pGroupName, long iResult) = 0;
        virtual void OnIMSModifyPrivateGroupNameResult(LPCTSTR pOldName, LPCTSTR pNewName, long iResult) = 0;
        virtual void OnIMSDeletePrivateGroupResult(LPCTSTR pGroupName, long iResult) = 0;
        virtual void OnIMSGotOnePrivateList(LPCTSTR szGroupName, int iListNum, short far *pListURI) = 0;
        virtual void OnIMSCopyPrivateGroupListResult(LPCTSTR pGroupName, long iGroupType, LPCTSTR pListURI, long iResult) = 0;
        virtual void OnIMSDelPrivateGroupListResult(LPCTSTR pGroupName, LPCTSTR pListURI, long iResult) = 0;
        virtual void OnGotIMAllPrivateGroupListResult(int iResult, long num, short far* pGroupList) = 0;
        virtual void OnGotIMPrivateGroupMemberResult(LPCTSTR pGroupIdentifier, int iResult, long num, short far* pMemberList) = 0;
        virtual void OnModifyGroupAttributeResult(short far *pResult) = 0;//ModifyGroupAttribute_T
        
        //呼叫
        virtual void OnReportMemberStatus(LPCTSTR lpJID, LPCTSTR lpCallee, long lStatus, long lCallType) = 0;
        virtual void OnSomeOneCallMe(LPCTSTR lpJID, LPCTSTR lpCaller, long lType) = 0;
        virtual void OnIMSCallEvent(long lType, LPCTSTR lpCaller, short far* pCallEvent) = 0;
        virtual void OnMultiRing() = 0;
        virtual void OnNewCallWhenBusy2(LPCTSTR pCaller, long iType, LPCTSTR pURL, long iCallType) = 0;
        virtual void OnCalleeForward(LPCTSTR pCalleeNumber, LPCTSTR pForwardNumber) = 0;
        virtual void OnPhoneConnected(long lVideo) = 0;
        virtual void OnPhoneIdle() = 0;
        virtual void OnPhoneStatus(long lType, long lValue, LPCTSTR lpMsg) = 0;
        virtual void OnCallOver() = 0;
        virtual void OnCtdCallEvent(long lType, short far* pPara) = 0;
        
        
        //公众号
        virtual void OnGotPubAccountListResult(int iResult, int nNum, void **pPubAccountList) = 0;
        virtual void OnIMSGotPublishMsgResult(int msgType, LPCTSTR pLocalMsgID, short far *pResult) = 0;//PublishMsgList_T
        virtual void OnAddOnePubAccountResult(int iResult, int nNum, void **pPubAccountList) = 0;
        virtual void OnDeleteOnePubAccountResult(int iResult, LPCTSTR pubAccId) = 0;
        virtual void OnSearchPubAccountResult(int iResult, int iType, int pageNo, int pageSize, int nNum, void **pPubAccountList) = 0;
        virtual void OnUserCodeMoaInfoResult(int iType, int iResult, LPCTSTR msgId, LPCTSTR etag, int nNum, void **pUserMoaInfo) = 0;
        virtual void OnGotPubAccountMenuResult(int iResult, LPCTSTR pubAccId, LPCTSTR etag, int nNum, void **pPubAccountMenu) = 0;
        virtual void OnSendPubAccMsgResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc, int iMsgType, LPCTSTR lpPutMsgId, LPCTSTR lpMenuId, LPCTSTR lpDate) = 0;
        
        //个人设置
        virtual void OnMessageMuteNotifyOperateResult(short far *pResult) = 0; //MuteNotifyOperate_T
        virtual void OnMessageReceiptOperateResult(int iOperType, LPCTSTR sessionID, int iResultCode, LPCTSTR pMsgIDs, void* pQryRsp) = 0;    //MsgReceiptQryRsp_T
        virtual void OnMessageReceiptNotify(short far *pResult) = 0; //MsgReceiptNotify_T
        virtual void OnConcernGroupOperateResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR uriList) = 0;
        virtual void OnContactMarkResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR markList) = 0;
        
        //文件传输
        virtual void OnUploadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iResultCode, int iRate, DWORD iTransedSize) = 0;
        virtual void OnDownloadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iResultCode, int iRate, DWORD iTransedSize) = 0;
        
        //上报接口
        virtual void OnSendBroadcastMsgRsp(short far *pResult) = 0; //BroadcastMsgRsp_T
        
        //通用
        virtual void OnDispUICmdForEcp2Result(long lType, short far* pPara) = 0;
        virtual void OnIMSDispUICmdResult(long lType, short far* pPara) = 0;
        virtual void OnIMSRuleSetEvent(long lType, short far* pPara) = 0;
        virtual void OnMsgIndicationNotify(short far* pPara) = 0;//MsgIndication_T
        virtual void OnGotRegInfoNotify(int iResult, short far* pPara) = 0;
        virtual void OnSendSoapMessageByXcapResult(int iCode, int iType, LPCTSTR lpcsBody) = 0;
        virtual void OnGetCheckCodeResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc) = 0;
        virtual void OnGetRecommendContactResult(int iResult, int pageNo, int pageSize, int nNum, void **pContactInfoList) = 0;
        virtual void OnGetSensWordListResult(int nNum, void *pWordList) = 0;
        virtual void OnUserBindOperResult(int iResultCode, int iOperType, LPCTSTR lpURI, LPCTSTR lpCompanyID) = 0;
        virtual void OnGet2DCodeInfoResult(short far *pResult) = 0;//Get2DCodeInfoResult_T
        virtual void OnResponseMsgFromHttpAP(LPCTSTR pSessionID, LPCTSTR pRequestLine, int iRetCode, LPCTSTR pContentType, LPCTSTR pBody, LPCTSTR pEtag) = 0;
        virtual void OnUploadFileByXcapRsp(int iRetCode, int iType, LPCTSTR msgId, LPCTSTR filePath) = 0;
        virtual void OnPushAPNsResult(LPCTSTR pToken, LPCTSTR pTokenVoIP, long iResult) = 0;
        
        //检查
        virtual void OnCheckConnectResult(int iResult, int delaysec) = 0;
        
        //版本灰度
        virtual void OnGetGrayVersionResult(int iHttpCode, LPCTSTR lpGrayVersion, LPCTSTR lpDownloadUri, LPCTSTR lpDownloadSize, LPCTSTR lpGrayVerInfo) = 0;
        
        //new
        virtual void onLoginMatterNotify(NS_USSDK_LoginService::LoginMattersNotifyPara *pNotify) = 0;
        virtual void onSingleContactNotify(SingleContactNotifyPara *pNotify) = 0;
        virtual void onPublicGroupNotify(PublicGroupNotifyPara *pNotify) = 0;
        virtual void onPublicGroupMemberChangedNotify(PublicGroupMemberChangedNotifyPara *pNotify) = 0;
    };
}

#endif /* USSDK_EventWrapper_h */
