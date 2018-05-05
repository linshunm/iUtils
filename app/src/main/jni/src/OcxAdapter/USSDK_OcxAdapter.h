//
//  USSDK_OcxAdapter.hpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_OcxAdapter_h
#define USSDK_OcxAdapter_h

#include <stdio.h>
#include <string>
#include "USSDK_OcxAdapterModel.h"
#include "../USSDKMethod.h"
#include "../clt_include/OcxInterface.h"
#include "../clt_include/OcxZXInterface.h"

#include "../clt_include/CtlEventWrapper.h"

using namespace std;
using namespace NS_USSDK_OcxAdapter;

namespace NS_USSDK_OcxAdapter
{
    class USSDK_API USSDK_OcxAdapter : public CCTLEventWrapper
    {
    public:
        static USSDK_OcxAdapter * shareInstance();
        
	public:
		//timer
		static UINT16 setTimer(UINT32 second, TimerNo aTimerNo, bool isLoop);
		static UINT8 killTimer(TimerNo aTimerNo);
		static UINT16 setExtraTimer(UINT32 second, TimerNo aTimerNo, UINT32 Extra);
		static UINT8 killExtraTimer(UINT16 tid);

	public:
		static bool login(OcxLogonPara *aPara);
		static bool logout(bool bJustClearData = false);

	public:
		//bIMSAddOneAddressList
		static bool addOneAddressList(string aRosterURI, string aDisplayName, string info);
		static bool deleteOneAddressList(string aRosterURI);

		static bool getOneRosterDetailInfo(string aRosterURI);
		static bool getOneRosterPhotoInfo(string aRosterURI, string photoIndex);

		static bool getOnePublicDetailInfo(string aRosterURI);
		static bool createOnePublicInfo(string ownerName);
		static bool inviteSomeoneToPublicGroup(string aGroupURI, string aMemberURI, string aMemberName);
		static bool deleteSomeOneFromPublicGroup(string aGroupURI, string aMemberURI);
		/*!
		*  @brief 群主转让-"owner" 群组名-"name" 二维码属性-"forbid_2D_join" 加成员权限-"member_invite_enable" 群公告内容- "bulletin"	
		*		"true" "false" 
		*/
		static bool modifyPublicGroupAttribute(string aGroupURI, string aModifyType, string aNewValue);
		static bool leavePublicGroup(string aGroupURI);
		static bool deletePublicGroup(string aGroupURI);
		static bool requestToJoinPublicGroup(string aGroupURI, string groupIdentifier, string myName, string shareUserURI, string shareUserName);

	public:
		//初始化
		static bool bInitControl();
		static void bSetEventWrapper(CCTLEventWrapper *pEventWrapper);
		//static void bSetUCallback(void *pObj, CTLCALLBACK_Ptr pCallback); //暂时未用

		//登录注销
		static bool bLogon(long lType, short * pPara, long lSpecialScene);
		static bool bGoOnLogonServer();
		static bool bForceLogonServer();
		static bool bLogoutServer(long bForce);
		static bool bServerLinkRecover();
		static bool bCloseServerLink();

		//呼叫
		static bool bLogonSS();
		static bool bLogoutSS();
		static bool bLogonSS2(LPCTSTR pConfuri, LPCTSTR pUserName, LPCTSTR pPassword, LPCTSTR pServerIP,LPCTSTR pServerPort);
		static bool bCreateMultiCall(LPCTSTR lpJID, LPCTSTR lpCallee, long lType);
		static bool bAcceptCall(LPCTSTR lpCaller);
		static bool bRejectCall(LPCTSTR lpCaller, int iCause);
		static bool bLeaveCall(int iCause);
		static void bSecondDialNum(LPCTSTR lpNum);
		static bool bCallHold(LPCTSTR pPhoneNumber, long iType);
		static bool bDoNewCallWhenBusy2(long iDoType, LPCTSTR pCaller);
		static char * bGetAudioProps();
		static char * bGetVideoProps();
		static void bSetAudioProps(LPCTSTR pCodecName, long nPacketPeriod);
		static void bSetVideoProps(LPCTSTR pCodecName, long nRate);

		static bool bIMSCallMethod(long lType, short* pCallMethod);
		static bool bStartRecord(LPCTSTR lpFilePathName);
		static bool bStopRecord();
		static bool bSelectTalker(LPCTSTR lpCallee);
		static bool bInviteToMultiCall(LPCTSTR lpJID, LPCTSTR lpCallee, long lType);

		static bool bReleaseTalker(LPCTSTR lpCallee);

		//个人
		static bool bSetXCAPAccount(LPCTSTR pURI, LPCTSTR pPasswd);
		static bool bIMSGetDeptAddrList(long iType, LPCTSTR pAddrList);
		static bool bIMSSubscribeOneList(long iType, LPCTSTR pListURI, long lURIType);

		//公共
		static bool bPatSpring(LPCTSTR lpDestNumber);
		static bool bWhetherEncrypt(long lSetValue);


		//消息
		static bool bIMSSendAttachMessage(long iType, LPCTSTR pFilePath, LPCTSTR pFileType, LPCTSTR pMessage, LPCTSTR pReceiverURI, LPCTSTR pLocalMsgID, int iReceipt);
		static bool bIMSSendMessage(int iType, int iReport, LPCTSTR pMessage, LPCTSTR pDeliverTime, LPCTSTR pReceiverURI, LPCTSTR pLocalMsgID, LPCTSTR pTMSubject, DWORD dwFileSeekSize, int iReceipt=0);
		static bool bIsLargeMessage(LPCTSTR pMessage);

		//订阅
		static bool bIMSPsMethod(long iType, short* pPara);
		static bool bIMSSearchRLS();
		static bool bIMSGetPresenceRules();
		static bool bIMSSubWatcher();
		static bool bIMSSubscribeOneRLS(LPCTSTR pURI);

		static bool bIMSGetPublicGroup(LPCTSTR lpIdentifier, int iOperFlag);

		static bool bIMSDoPublicGroupInvite(LPCTSTR lpcsOwnerName, LPCTSTR pInviterURI, LPCTSTR pGroupName, long iResult, LPCTSTR pGroupURI, LPCTSTR pReturn);
		static bool bIMSSetUserInfo(short* pUserInfo);
		static bool bIMSSetMyPhoto(LPCTSTR pIndex, short* pFileContent, long iLen);
		static bool bIMSGetPersonSubInfo(long lType, short* pRosterMethod);

		static bool bDispUICmdForEcp2Method(long lType, short* pCmdPara);
		static bool bCtdCallMethod(long lType, short* pPara);
		static bool bIMSSearchOneListInfo(LPCTSTR pListURI);
		static bool bIMSRuleSetMethod(long lType, short* pPara);

		//搜索
		static bool bIMSSearchUserByURI(LPCTSTR pURI, long iPage);
		static bool bIMSSearchUserByDName(LPCTSTR pDName, long iPage);
		static bool bIMSSearchUserOnline(long iPage);
		static bool bIMSSearchUserByPhone(LPCTSTR pPhone,  long iPage);
		static bool bIMSSearchUserByEmail(LPCTSTR pEmail, long iPage);

		//联系人
		static bool bIMSAddOneAddressList(LPCTSTR pURI, LPCTSTR pDisplayName, LPCTSTR pInfo);
		static bool bIMSDeleteOneAddressList(LPCTSTR pURI);
		static bool bIMSDoWatcherSubSeq(LPCTSTR pWatcherURI, long iAccept);
		static bool bIMSAddRLSMember(LPCTSTR pURI, LPCTSTR pListName, LPCTSTR pListMemberURI, LPCTSTR pInfo);
		static bool bIMSDeleteRLSMember(LPCTSTR pURI, LPCTSTR pListMemberURI);

		static bool bIMSLoadAddrList(long iType, short*pAddrList, long iNum, long iPage);

		//群组相关
		static bool bIMSCreateOnePrivateGroup(LPCTSTR pGroupName);
		static bool bIMSModifyPrivateGroupName(LPCTSTR pOldName, LPCTSTR pNewName);
		static bool bIMSDeleteOnePrivateGroup(LPCTSTR pGroupName);
		static bool bIMSGetPrivateGroupMember(LPCTSTR lpIdentifier);
		static bool bIMSCopyListToPrivateGroup(LPCTSTR pListURI, LPCTSTR pGroupName, long iGroupType, LPCTSTR pDisplayName, long iSeq);
		static bool bIMSDeleteListFromPrivateGroup(LPCTSTR pListURI, LPCTSTR pGroupName);

		static bool bIMSSearchAddrList(long iType, short* pSearch, long iPage);
		static bool bIMSDoPublicGroupJoinReq(LPCTSTR pReqMemberName, LPCTSTR pReqURI, long iResult, LPCTSTR pReturn1, LPCTSTR pReturn2) ;
		static bool bIMSModifyOneListDpName(LPCTSTR pURI, LPCTSTR pDisplayName);
		static bool bIMSReqJoinPublicGroup(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, LPCTSTR pMyDisplayName, LPCTSTR pShareUserURI, LPCTSTR pShareUserName);
		static bool bIMSLeavePublicGroup(LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex);
		static bool bIMSDeleteOneFromMyPublicGroup(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex);
		static bool bIMSDeleteOnePublicGroup(LPCTSTR pGroupURI);
		static bool bIMSDispUICmdMethod(long lType, short* pCmdPara);
		static bool bModifyGroupAttributeReq(LPCTSTR pGroupURI, LPCTSTR pModType, LPCTSTR pNewValue);
		static bool bGetIMPrivateGroupMember(LPCTSTR  lpGroupIdentifier);

		//输入状态
		static bool bUpdateMsgIndication(short* pPara);

		//查询好友最后一次登录信息
		static bool bRequestLastLogInfo(short* pPara);

		static bool bIMSCreateOnePublicGroup(short* pPara);
		static bool bIMSInviteSomeoneToMyPublicGroup(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex);

		//MOA接口
		static bool bGetAllIMPrivateGroupList();

		//通用接口
		static bool bSendSoapMessageByXcap(int iType, LPCTSTR lpcsRequestLine, LPCTSTR lpcstrAction, LPCTSTR lpcsBody);
		static bool bCheckURIIsMOA(LPCTSTR lpcsURI);

		//公众帐号相关处理。iType: 1-查询自己关注的公众号
		static bool bSendPubAccountReq(int iType, LPCTSTR pPubAccId);
		static bool bGetHistoryPubMsg(int count, LPCTSTR pPubAccId, int seq, LPCTSTR pFlag);
		static bool bSearchPubAccount(int iType, LPCTSTR pubAccName, int pageNo, int pageSize);
		static bool bGetPubAccountMenu(LPCTSTR pPubAccId, LPCTSTR etag);

		//好友推荐功能
		static bool bGetRecommendContact(int pageNo, int pageSize);

		//bCheckOsTimer
		static bool bCheckOsTimer(int pno);
		//登陆通知
		static bool bClientLoginNotify(int type);
		static bool bMOAImmediatelyReReg();

		//检测与DAS间的连接
		static bool bCheckConnection();
		static bool bLoginInfoReport(LPCTSTR lpRecord);
		//查询本地通讯录联系人是否MOA用户
		static bool bSyncUserCodeMoaInfo(int iType, LPCTSTR lpMsgId, LPCTSTR lpDeviceId, LPCTSTR lpXmlBody);
		//下载敏感词
		static bool bGetSensitiveWord();

		//用户开户接口
		static bool bGetCheckCode(LPCTSTR lpUserCode);
		static bool bUserRegistReq(int iType, LPCTSTR pUserName, LPCTSTR pUserURI, LPCTSTR pPassword, LPCTSTR pCompany, LPCTSTR pCheckCode);
		static bool bUserModifyPasswdReq(int iType, LPCTSTR pUserURI, LPCTSTR pOldPassword, LPCTSTR pNewPassword);

		//公众号消息上行
		static bool bSendPubAccMsg(LPCTSTR pPubAccId, LPCTSTR pSender, int iMsgType, LPCTSTR pPutMsgId, LPCTSTR pMenuId, LPCTSTR pContent);
		//bind操作请求
		static bool bUserBindOperateReq(int iOperType, LPCTSTR pBindNumber, int iNumberType, LPCTSTR pPasswd, LPCTSTR pCompanyId);

		//灰度版本
		static bool bGetGrayVersion(LPCTSTR lpVersion);
		static INT32 bCheckHttpServer(LPCTSTR pServerIP, int port, int sec);
		static INT32 bCheckSipServer(LPCTSTR pServerIP, int port, int sec);

		//二维码相关
		static bool bGet2DCodeInfo(int iType, LPCTSTR strKeyWord, int validType, int clientExist, int codeType, LPCTSTR msgId);
		//消息免扰相关
		static bool bMessageMuteNotifyOperateReq(int iType, LPCTSTR pURI, LPCTSTR operateId);
		static bool bMessageReceiptOperateReq(int iOperType, LPCTSTR cSessionID, int isGroup, LPCTSTR pURI, LPCTSTR pMsgIDs);
		//公众号消息获取
		static bool bGetPublicMsg(LPCTSTR pMsgID,LPCTSTR lpURL);
		//与HTTPAP交互接口
		static bool bSendRequestMsgToHttpAP(LPCTSTR cSessionID, LPCTSTR cRequestLine, LPCTSTR cMethod, LPCTSTR cContentType, LPCTSTR cEtag, char* pBody);
		//查询用户登录状态(for voip)
		static bool bGetUserLogonStatus(LPCTSTR lpURI);
		//星标群组
		static bool bConcernGroupOperateReq(int iType, LPCTSTR eTag, LPCTSTR gURI);
		static bool bUploadFileByXcapReq(int iType, LPCTSTR msgId, LPCTSTR filePath);

		static bool bDownloadFileReq(LPCTSTR pSessionID, LPCTSTR pFilePathName, int iOperType, LPCTSTR pFileID);
		static bool bUploadFileReq(LPCTSTR pSessionID, LPCTSTR pFilePathName, int iOperType, LPCTSTR pFileID);

		static bool bSendBroadcastMsgReq(LPCTSTR cMsgID, LPCTSTR pURIs, LPCTSTR gURIs, LPCTSTR filePath);
		static bool bGetOfflineMsgReq(LPCTSTR pSeq, int maxCount);
        
    //up
    public:
        void OnInitControlResult(long lResult); //保留
        
        //登录注销
        void OnLogonResult(long lResult, long lReason, long extraCode);
        void OnRegisterSoftPhoneToSS(char * lpSoftPhone, int lType);
        void OnAlreadyLogout(long lReason);
        void OnServerLinkLost(int itype);
        void OnLogoutServerSuccess();
        void OnServerLinkOK();
        void OnRegisterSoftPhoneToSSNew(short far* pInfo);
        void OnTry403FlowBegin();
        void OnTry403FlowResult(int iResult);
        
        //个人
        void OnGetXCAPCapsInfo(short far* pNotify);//XCAP_CAPS_INFO
        void OnIMSGotUserInfo(short far* pNotify);//USER_INFO
        void OnIMSPersonSubInfoChanged(long lType, short far* pPersonSubInfoEvent);
        void OnIMSGotPersonSubInfo(int retCode, long lType, short far*pPersonSubInfoEvent);
        void OnIMSGotPersonSubInfoFailed(long lType, LPCTSTR lpUserURI, long lResult);
        void OnIMSPsEvent(long iType, short far* pNotify);
        void OnIMSBatchPsEvent(int iNum, int iCode, short far* pNotify);
        void OnXcapTokenInvalid();
        
        
        //联系人
        void OnIMSGotOneWatcher(LPCTSTR pWatcherURI, LPCTSTR pStatus);
        void OnIMSWatcherSubscribeMe(LPCTSTR pWatcherURI, LPCTSTR pDisplayName, LPCTSTR pInfo, LPCTSTR pStatus);
        void OnIMSGotMyPresenceRule(long iType, long iNum, short far* pURIList);
        void OnIMSGotAddressList(int iNum, short far* pAddrList);
        void OnIMSGotOneRLS(int iResult, LPCTSTR pURI, LPCTSTR pGroupName, LPCTSTR pListName, int iListNum, short far* pListMemberURI);
        void OnIMSListPresenceArrived(LPCTSTR pSipFromURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone, long iTimestamp);
        void OnIMSSetUserInfoResult(long lResult);
        void OnIMSGotOneListInfo(short far* pListInfo,int iReturnCode, LPCTSTR pURI);//USER_INFO
        void OnIMSGotOneListInfoFailed(LPCTSTR lpUserURI, long lResult);
        void OnIMSGotListInfo(long num, short far* pListInfo);
        void OnIMSGotUserInfoFailed(long lResult);
        void OnIMSSearchUserByURIResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
        void OnIMSSearchUserByDNameResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
        void OnIMSSearchOneUserOnlineResult(int iPos, LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity);
        void OnIMSSearchUserByPhoneResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
        void OnIMSSearchUserByEmailResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
        void OnIMSAddListResult(LPCTSTR pURI, LPCTSTR pName, int iResult);
        void OnIMSAddRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult);
        void OnIMSDeleteRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult);
        void OnIMSDelListResult(LPCTSTR pURI, int iResult);
        void OnIMSSetPresenceRulesResult(LPCTSTR pURI, int iType);
        void OnIMSLoadAddrListResult(long iType, long iCode, long iNum, short far* pAddrList);
        void OnIMSGotDeptAddrListResult(long iType, long iCode, long iNum, short far* pAddrList, LPCTSTR pNodeURI);
        void OnIMSSearchAddrListResult(long iType, long iCode, short far* pAddrList);
        void OnIMSSubResult(int iType, LPCTSTR pURI, int iResult);
        void OnCheckURIIsMOAResult(int iResult, long num, short far* pCheckList);
        void OnGetUserLogonStatusResult(short far *pResult); //GetUserLogonStatusResult_T
        
        //消息
        void OnIMSSendMessageResult(short far* pNotify);
        void OnIMSMessageArrived(int iType, LPCTSTR pChatRoomURI, LPCTSTR pSenderURI, LPCTSTR pMessage, LPCTSTR pTime, LPCTSTR pMessageId, int iReceipt, LPCTSTR pSipTime);
        void OnIMSAttachMessageArrived(int iType, LPCTSTR pSenderURI, LPCTSTR pChatroomURI, LPCTSTR pSendTime, LPCTSTR pAttachFilePath, LPCTSTR pAttachFileType, LPCTSTR pMessage, LPCTSTR pMessageId);
        void OnIMSSendLargeMessageResult(short far* pNotify);
        void OnIMSAssociatedIdentifyArrived(long iNum, short far* pIdentify);
        void OnIMSSendMessageReport(LPCTSTR pReportID, LPCTSTR pReportType, LPCTSTR pDoneTime, LPCTSTR pErrorCode, LPCTSTR pTerminal);
        void OnIMSMessageListArrived(int nNum, void **pMsgList);
        void OnIMSGroupOfflineMsgNotify(int iMsgNum, LPCTSTR cGroupURI, LPCTSTR pMessage, LPCTSTR timestamp);
        void OnIMSGotGroupOfflineMsgResult(short far *pResult);
        void OnGetOfflineMsgRsp(short far *pResult);
        
        //群组
        void OnIMSGotGroupInfo(int index, int count, short far* pNotify);//IMSGotGroupInfo_T or FIRE_GROUP_INFO
        void OnIMSGetPublicGroupInfoListFailed(LPCTSTR lpGroupURI, long lResult);
        void OnIMSGotOnePublicGroup(int flag, LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperUser, long iMyRole, short far* pAttribute, long iMemberNum, void **pMemberList, long iBlacklistNum, short far* pBlacklist, LPCTSTR pOperUserName);
        void OnIMSCreatePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, long iResult, long iGroupType, LPCTSTR pGroupOperateindex);
        void OnIMSModifyPublicGroupResult(LPCTSTR pGroupURI, long iType, LPCTSTR pNewName, long iNewValue, long iResult, LPCTSTR pGroupOperateindex);
        void OnIMSInviteToMyPuGroupSendResult(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, long iResult, LPCTSTR pOperateIndex);
        void OnIMSPGroupListPresenceArrived(LPCTSTR pPublicGroupURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone);
        void OnIMSInvitedToPublicGroup(short far* pPara);//IMSInvitedToPublicGroup_T
        void OnIMSPublicGroupInviteResult(LPCTSTR pGroupURI, LPCTSTR pInvitedUserURI, long iResult);
        void OnIMSReqJoinMyPublicGroup(short far* pPara);//IMSReqJoinMyPublicGroup_T
        void OnIMSInvitedToTmpPublicGroup(short far* pPara);
        void OnIMSPublicGroupMemberNotify(short far* pPara);
        void OnIMSDeleteMeInPublicGroup(LPCTSTR pGroupURI, int iType);
        void OnIMSDoPublicGroupInviteResult(long lDoType, LPCTSTR pGroupURI, LPCTSTR pGroupName, LPCTSTR lpcsOwnerName, long lCode);
        void OnIMSDoGroupJoinReqResult(LPCTSTR pGroupURI, LPCTSTR pMemberURI, LPCTSTR pMemberName, long lResult);
        void OnIMSModifyOneListDpNameResult(LPCTSTR pURI, LPCTSTR pDisplayName, int iResult);
        void OnIMSReqJoinPublicGroupResult(LPCTSTR pGroupURI, LPCTSTR pGroupName, long iResult);
        void OnIMSLeavePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, int iResult, LPCTSTR pGroupOperateindex);
        void OnIMSReqJoinPublicGroupSendResult(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, long iResult);
        void OnIMSDeletePublicGroupResult(LPCTSTR pGroupURI, long iResult);
        void OnIMSDelFromMyPuGroupResult(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, long iResult, LPCTSTR pGroupOperateindex);
        void OnIMSCreatePrivateGroupResult(LPCTSTR pGroupName, long iResult);
        void OnIMSModifyPrivateGroupNameResult(LPCTSTR pOldName, LPCTSTR pNewName, long iResult);
        void OnIMSDeletePrivateGroupResult(LPCTSTR pGroupName, long iResult);
        void OnIMSGotOnePrivateList(LPCTSTR szGroupName, int iListNum, short far *pListURI);
        void OnIMSCopyPrivateGroupListResult(LPCTSTR pGroupName, long iGroupType, LPCTSTR pListURI, long iResult);
        void OnIMSDelPrivateGroupListResult(LPCTSTR pGroupName, LPCTSTR pListURI, long iResult);
        void OnGotIMAllPrivateGroupListResult(int iResult, long num, short far* pGroupList);
        void OnGotIMPrivateGroupMemberResult(LPCTSTR pGroupIdentifier, int iResult, long num, short far* pMemberList);
        void OnModifyGroupAttributeResult(short far *pResult);//ModifyGroupAttribute_T
        
        //呼叫
        void OnReportMemberStatus(LPCTSTR lpJID, LPCTSTR lpCallee, long lStatus, long lCallType);
        void OnSomeOneCallMe(LPCTSTR lpJID, LPCTSTR lpCaller, long lType);
        void OnIMSCallEvent(long lType, LPCTSTR lpCaller, short far* pCallEvent);
        void OnMultiRing();
        void OnNewCallWhenBusy2(LPCTSTR pCaller, long iType, LPCTSTR pURL, long iCallType);
        void OnCalleeForward(LPCTSTR pCalleeNumber, LPCTSTR pForwardNumber);
        void OnPhoneConnected(long lVideo);
        void OnPhoneIdle();
        void OnPhoneStatus(long lType, long lValue, LPCTSTR lpMsg);
        void OnCallOver();
        void OnCtdCallEvent(long lType, short far* pPara);
        
        
        //公众号
        void OnGotPubAccountListResult(int iResult, int nNum, void **pPubAccountList);
        void OnIMSGotPublishMsgResult(int msgType, LPCTSTR pLocalMsgID, short far *pResult);//PublishMsgList_T
        void OnAddOnePubAccountResult(int iResult, int nNum, void **pPubAccountList);
        void OnDeleteOnePubAccountResult(int iResult, LPCTSTR pubAccId);
        void OnSearchPubAccountResult(int iResult, int iType, int pageNo, int pageSize, int nNum, void **pPubAccountList);
        void OnUserCodeMoaInfoResult(int iType, int iResult, LPCTSTR msgId, LPCTSTR etag, int nNum, void **pUserMoaInfo);
        void OnGotPubAccountMenuResult(int iResult, LPCTSTR pubAccId, LPCTSTR etag, int nNum, void **pPubAccountMenu);
        void OnSendPubAccMsgResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc, int iMsgType, LPCTSTR lpPutMsgId, LPCTSTR lpMenuId, LPCTSTR lpDate);
        
        //个人设置
        void OnMessageMuteNotifyOperateResult(short far *pResult); //MuteNotifyOperate_T
        void OnMessageReceiptOperateResult(int iOperType, LPCTSTR sessionID, int iResultCode, LPCTSTR pMsgIDs, void* pQryRsp);    //MsgReceiptQryRsp_T
        void OnMessageReceiptNotify(short far *pResult); //MsgReceiptNotify_T
        void OnConcernGroupOperateResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR uriList);
        void OnContactMarkResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR markList);
        
        //文件传输
        void OnUploadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iResultCode, int iRate, DWORD iTransedSize);
        void OnDownloadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iResultCode, int iRate, DWORD iTransedSize);
        
        //上报接口
        void OnSendBroadcastMsgRsp(short far *pResult); //BroadcastMsgRsp_T
        
        //通用
        void OnDispUICmdForEcp2Result(long lType, short far* pPara);
        void OnIMSDispUICmdResult(long lType, short far* pPara);
        void OnIMSRuleSetEvent(long lType, short far* pPara);
        void OnMsgIndicationNotify(short far* pPara);//MsgIndication_T
        void OnGotRegInfoNotify(int iResult, short far* pPara);
        void OnSendSoapMessageByXcapResult(int iCode, int iType, LPCTSTR lpcsBody);
        void OnGetCheckCodeResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc);
        void OnGetRecommendContactResult(int iResult, int pageNo, int pageSize, int nNum, void **pContactInfoList);
        void OnGetSensWordListResult(int nNum, void *pWordList);
        void OnUserBindOperResult(int iResultCode, int iOperType, LPCTSTR lpURI, LPCTSTR lpCompanyID);
        void OnGet2DCodeInfoResult(short far *pResult);//Get2DCodeInfoResult_T
        void OnResponseMsgFromHttpAP(LPCTSTR pSessionID, LPCTSTR pRequestLine, int iRetCode, LPCTSTR pContentType, LPCTSTR pBody, LPCTSTR pEtag);
        void OnUploadFileByXcapRsp(int iRetCode, int iType, LPCTSTR msgId, LPCTSTR filePath);
        void OnPushAPNsResult(LPCTSTR pToken, LPCTSTR pTokenVoIP, long iResult);
        
        //检查
        void OnCheckConnectResult(int iResult, int delaysec);
        
        //版本灰度
        void OnGetGrayVersionResult(int iHttpCode, LPCTSTR lpGrayVersion, LPCTSTR lpDownloadUri, LPCTSTR lpDownloadSize, LPCTSTR lpGrayVerInfo);
    };

}
#endif /* USSDK_OcxAdapter_h */
