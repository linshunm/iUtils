#ifndef RCS_CONTROLLER_H
#define RCS_CONTROLLER_H
#include "../util/androidLog.h"
#include "../os/zxinfunc.h"
#include <jni.h>
#include <../os/List.h>

#ifdef __cplusplus
extern "C" {
#endif
	void java_FireLogonResult(long lResult, long lReason, long extraCode);
	void java_FireReportMemberStatus(char *lpJID, char *lpCallee, long lStatus, long lCallType);
	void java_FireSomeOneCallMe(char *lpJID, char *lpCaller, long lType);	
	void java_FireRegisterSoftPhoneToSS(char * lpSoftPhone, int lType)	;   
	void java_FireAlreadyLogout(long lReason);
	void java_FireLogoutServerSuccess();
	void java_FireServerLinkLost(int itype);
	void java_FireServerLinkOK();
	void java_FireIMSSetUserInfoResult(long iResult);
	void java_FireIMSPersonSubInfoChanged(long lType, short far* pPersonSubInfoEvent);  
	void java_FireIMSGotPersonSubInfo(int retCode, long lType, short far*pPersonSubInfoEvent);
	void java_FireIMSCallEvent(long lType, LPCTSTR lpCaller, short far* pCallEvent);
	void java_FireMultiRing();
	void java_FireLostPacketNotify(long lTotalPacketNum, long lLostPacketNum, float fLostPacketRate);
	void java_FireVideoCaptureAsMirrorStopped();
	void java_FireNewCallWhenBusy2(LPCTSTR pCaller, long iType, LPCTSTR pURL, long iCallType);
	void java_FireCalleeForward(LPCTSTR pCalleeNumber, LPCTSTR pForwardNumber);
	void java_FireCreateConfCallSuccess(LPCTSTR lpConfURI);
	void java_FireColorImageURLArrived(LPCTSTR lpURL, LPCTSTR lpCaller, LPCTSTR lpRoomID, long lType);
	void java_FirePhoneConnected(long lVideo);
	void java_FirePhoneIdle();
	void java_FireReportMultiCallError(long lError);
	void java_FirePhoneStatus(long lType, long lValue, LPCTSTR lpMsg);
	void java_FireCallOver();	   
	void java_FireIMSMessageArrived(int iType, LPCTSTR pChatRoomURI, LPCTSTR pSenderURI, LPCTSTR pMessage, LPCTSTR pTime, LPCTSTR pMessageId, int iReceipt, LPCTSTR pSipTime);
	void java_FireIMSAttachMessageArrived(int iType, LPCTSTR pSenderURI, LPCTSTR pChatroomURI, LPCTSTR pSendTime, LPCTSTR pAttachFilePath, LPCTSTR pAttachFileType, LPCTSTR pMessage, LPCTSTR pMessageId);
	void java_FireIMSSendMessageResult(short far* pNotify);
	void java_FireIMSSendLargeMessageResult(short far* pNotify);
	void java_FireIMSRecvFileTransReq(LPCTSTR pSenderURI, LPCTSTR pFilePathName, int iFileSize, LPCTSTR pSessionID, LPCTSTR pFileID);
	void java_FireIMSFileTransRate(LPCTSTR pSessionID, int iRate, DWORD dwTransedSize);
	void java_FireIMSSenderCancelSend(long iCancelType, LPCTSTR pSessionID);
	void java_FireIMSReceiverCancelReceive(long iCancelType, LPCTSTR pSessionID);
	void java_FireIMSAssociatedIdentifyArrived(long iNum, short far* pIdentify);
	void java_FireIMSGotOneWatcher(LPCTSTR pWatcherURI, LPCTSTR pStatus);
	void java_FireIMSWatcherSubscribeMe(LPCTSTR pWatcherURI, LPCTSTR pDisplayName, LPCTSTR pInfo, LPCTSTR pStatus);
	void java_FireIMSRecvFile(LPCTSTR pSenderURI, LPCTSTR pFilePathName, DWORD dwFileSize, LPCTSTR pSessionID, LPCTSTR pFileID,int IsUtf8,DWORD dwSeekSize,LPCTSTR pcFilePath);
	void java_FireIMSConfEvent(long iType, long iNum, short far*pConfEvent);
	void java_FireIMSAttachMessageArrivedImmediately(LPCTSTR pSenderURI, LPCTSTR pChatroomURI, LPCTSTR pAttachFileType, LPCTSTR pMessageId);

    //ucs
	void java_FireGetXCAPCapsInfo(short far* pNotify);
	void java_FireIMSGotUserInfo(short far* pNotify);
	void java_FireIMSGotGroupInfo(int index, int count, short far* pNotify);
	void java_FireIMSPsEvent(long iType, short far* pNotify);
	void java_FireIMSBatchPsEvent(int iNum, int iCode, short far* pNotify);
//	void java_FireIMSGotAddressList(LPCTSTR pURI, LPCTSTR pDisplayName, short far* pNotify);
	void java_FireIMSGotAddressList(int iNum, short far* pNotify);
	void java_FireIMSGotOneRLS(int iResult, LPCTSTR pURI, LPCTSTR pGroupName, LPCTSTR pListName, int iListNum, short far* pListMemberURI);
	void java_FireIMSGotMyPresenceRule(long iType, long iNum, short far* pURIList);
	void java_FireIMSSendMessageReport(LPCTSTR pReportID, LPCTSTR pReportType, LPCTSTR pDoneTime, LPCTSTR pErrorCode, LPCTSTR pTerminal);
	void java_FireIMSListPresenceArrived(LPCTSTR pSipFromURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone, long iTimestamp);
	void java_FireIMSGetPublicGroupInfoListFailed(LPCTSTR lpGroupURI, long iResult);
	void java_FireIMSGotOnePublicGroup(int flag, LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperUser, long iMyRole, short far* pAttribute, long iMemberNum, void **pMemberList, long iBlacklistNum, short far* pBlacklist);
	void java_FireIMSCreatePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, long iResult, long iGroupType);
	void java_FireIMSModifyPublicGroupResult(LPCTSTR pGroupURI, long iType, LPCTSTR pNewName, long iNewValue, long iResult);
	void java_FireIMSInviteToMyPuGroupSendResult(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, long iResult);

	void java_FireIMSPGroupListPresenceArrived(LPCTSTR pPublicGroupURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone);
	void java_FirePublicGroupAttributeChanged(LPCTSTR pURI, long iType, LPCTSTR pName, long iValue);
	void java_FireIMSInvitedToPublicGroup(short far* pPara);
	void java_FireIMSPublicGroupInviteResult(LPCTSTR pGroupURI, LPCTSTR pInvitedUserURI, long iResult);
	void java_FireIMSSearchPuGroupResult(long iResultNum, LPCTSTR pSearchString, short far* pNotify);
	void java_FireIMSSearchPublicGroupFailed(LPCTSTR pGroupURI);
	void java_FireIMSReqJoinMyPublicGroup(short far* pPara);
	void java_FireIMSInvitedToTmpPublicGroup(short far* pPara);
	void java_FireIMSPublicGroupMemberNotify(short far* pPara);
	void java_FireIMSPublicGroupDeleted(LPCTSTR pGroupURI);
	void java_FireIMSDeleteMeInPublicGroup(LPCTSTR pGroupURI, int iType);
	void java_FireIMSDoPublicGroupInviteResult(long lDoType, LPCTSTR pGroupURI, LPCTSTR pGroupName, LPCTSTR lpcsOwnerName, long lCode);
	void java_FireIMSPersonSubInfoChanged(long lType, short far* pPersonSubInfoEvent);
	void java_FireIMSSetUserInfoResult(long lResult);//设置本人详细信息结果事件
	void java_FireIMSGotOneListInfo(short far* pListInfo,int iReturnCode, LPCTSTR pURI);
	void java_FireIMSGotListInfo(List *pListInfo); //批量上报联系人详细信息
	long java_FireFIndFileSize(LPCTSTR pFilePath);
	void java_FireCtdCallEvent(long lType, short far* pPara);
	void java_FireDispUICmdForEcp2Result(long lType, short far* pPara);
	void java_FireIMSDispUICmdResult(int iType, short far* pPara);
	void java_FireIMSRuleSetEvent(long lType, short far* pPara);
    void java_FireIMSLoadAddrListResult(long iType, long iCode, long iNum, short far* pAddrList);

	void java_FireIMSCreatePrivateGroupResult(LPCTSTR szGroupName, long iResult);
	void java_FireIMSModifyPrivateGroupNameResult(LPCTSTR szOldName, LPCTSTR szpNewName, long iResult);
	void java_FireIMSDeletePrivateGroupResult(LPCTSTR szGroupName, long iResult);
	void java_FireIMSGotOnePrivateList(LPCTSTR szGroupName, int iListNum, short far *pListURI);
	void java_FireIMSCopyPrivateGroupListResult(LPCTSTR pGroupName, long iGroupType, LPCTSTR pListURI, long iResult);
	void java_FireIMSDelPrivateGroupListResult(LPCTSTR pGroupName, LPCTSTR pListURI, long iResult);

	//联系人相关
	void java_FireIMSSearchUserByURIResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
	void java_FireIMSSearchUserByDNameResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
	void java_FireIMSSearchOneUserOnlineResult(int iPos, LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity);
	void java_FireIMSSearchUserByPhoneResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
	void java_FireIMSSearchUserByEmailResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline);
	void  java_FireIMSAddRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult);
    void java_FireIMSDeleteRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult);

	void java_FireIMSDelListResult(LPCTSTR pURI, int iResult);
	void java_FireIMSAddListResult(LPCTSTR pURI, LPCTSTR pName, int iResult);

	void java_FireIMSSetPresenceRulesResult(LPCTSTR pURI, int iType);
	
	// 聊天室相关
	void java_FireIMSCreateChatRoomResult(LPCTSTR pDlgID, long iResult, LPCTSTR pSubject, LPCTSTR pChatRoomURI);
	void java_FireIMSGotChatRoomUserInfo(short *info);
	void java_FireIMSInvitedToChatRoom(LPCTSTR pCreaterURI, LPCTSTR pChatRoomURI, LPCTSTR pSubject);
	void java_FireIMSJoinChatRoomResult(long iResult, LPCTSTR pSubject, LPCTSTR pChatRoomURI);
	void java_FireIMSLeaveChatRoom(LPCTSTR pChatRoomURI);
	
	// 通讯录相关
	void java_FireIMSGotDeptAddrListResult(long iType, long iCode, long iNum, short far* pAddrList, LPCTSTR pNodeURI);
	void java_FireIMSSearchAddrListResult(long iType, long iCode, short far* pAddrList);

	void java_FireIMSDoGroupJoinReqResult(LPCTSTR pGroupURI, LPCTSTR pMemberURI, LPCTSTR pMemberName, long lResult);
	void java_FireIMSModifyOneListDpNameResult(LPCTSTR pURI, LPCTSTR pDisplayName, int iResult);
	void java_FireIMSReqJoinPublicGroupResult(LPCTSTR pGroupURI, LPCTSTR pGroupName, long iResult);
	void java_FireIMSLeavePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, int iResult);
	void java_FireIMSReqJoinPublicGroupSendResult(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, long iResult);
	void java_FireIMSDeletePublicGroupResult(LPCTSTR pGroupURI, long iResult);
	void java_FireIMSDelFromMyPuGroupResult(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, long iResult);

	//输入状态
	void java_FireMsgIndicationNotify(short far* pPara);

	//最后一次登录信息
	void java_FireGotRegInfoNotify(int IResult, short far* pPara);

	void java_FireIMSSubResult(int iType, LPCTSTR pURI, int iResult);//订阅结果上报
	void java_FireRegisterSoftPhoneToSSNew(short far* pInfo);

       //会议相关
       void java_FireDataConfEvent(int iType,  short far* pConfEvent);
	void java_FireRecvFromMrsEvent(int iType, short far* pRecvMessage);
    
       void java_FireInitControlResult(long lResult);
       void java_FireConfBridgeControlResult(int iType, short far* pRecvMessage);

	//MOA 事件
	void java_FireGotIMAllPrivateGroupListResult(int iResult, List *pGroupList);
	void java_FireGotIMPrivateGroupMemberResult(LPCTSTR pGroupIdendifier, int iResult, List *pMemberList);
	void java_FireCheckURIIsMOAResult(int iResult, List *pCheckList);

	void java_FireSendSoapMessageByXcapResult(int iCode, int iType, LPCTSTR lpcsBody);
	void java_FireGetCheckCodeResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc);
	void java_FireIMSMessageListArrived(int nNum, void **pMsgList);

	//403流程上报界面
	void java_FireTry403FlowBegin();
	void java_FireTry403FlowResult(int iResult);
	void java_FireCheckConnectResult(int iResult, int delaysec);

	void java_FireIMSGroupOfflineMsgNotify(int iMsgNum, LPCTSTR cGroupURI, LPCTSTR pMessage, LPCTSTR timestamp);
	void java_FireIMSGotGroupOfflineMsgResult(int iReturnCode, int nMethodType, int nOverFlag, LPCTSTR cGroupURI, LPCTSTR cMaxTimeQue, int nNum, void **pMsgList);

	//公众号
	void java_FireGotPubAccountListResult(int iResult, int nNum, void **pPubAccountList);
	void java_FireIMSGotPublishMsgResult(int msgType, LPCTSTR localMsgID, LPCTSTR pubAccId, LPCTSTR cTime, LPCTSTR msgid, int seq, int nNum, void **pMsgList);
	void java_FireAddOnePubAccountResult(int iResult, int nNum, void **pPubAccountList);
	void java_FireDeleteOnePubAccountResult(int iResult, LPCTSTR pubAccId);
	void java_FireSearchPubAccountResult(int iResult, int iType, int pageNo, int pageSize, int nNum, void **pPubAccountList);
	void java_FireUserCodeMoaInfoResult(int iType, int iResult, LPCTSTR msgId, LPCTSTR etag, int nNum, void **pUserMoaInfo);
	void java_FireGotPubAccountMenuResult(int iResult, LPCTSTR pubAccId, LPCTSTR etag, int nNum, void **pPubAccountMenu);
	void java_FireSendPubAccMsgResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc, int iMsgType, LPCTSTR lpPutMsgId, LPCTSTR lpMenuId, LPCTSTR lpDate);
    //推荐好友
	void java_FireGetRecommendContactResult(int iResult, int pageNo, int pageSize, int nNum, void **pContactInfoList);
	//敏感词
	void java_FireGetSensWordListResult(int nNum, void *pWordList);
	//号码绑定相关操作结果
	void java_FireUserBindOperResult(int iResultCode, int iOperType, LPCTSTR lpURI, LPCTSTR lpCompanyID);
	//群组属性修改结果
	void java_FireModifyGroupAttributeResult(LPCTSTR lpGroupURI, LPCTSTR lpModType, LPCTSTR lpNewValue, int iResultCode);
	void java_FireMessageMuteNotifyOperateResult(int iResultCode, int iType, LPCTSTR lpURI, int iGroupNum, void* lpGroupURI, int iContactNum, void* lpContactURI
		, int iPubAccNum, void* lpPubAccURI);
	void java_FireMessageReceiptOperateResult(int iOperType, LPCTSTR sessionID, int iResultCode, LPCTSTR pMsgIDs, void* qryRsp);	
	void java_FireMessageReceiptNotify(int isGroup, LPCTSTR lpURI, int iListNum, void* lpMsgIDList, char* pMsgIDs);
	//灰度版本
	void java_FireGetGrayVersionResult(int iHttpCode, LPCTSTR lpGrayVersion, LPCTSTR lpDownloadUri, LPCTSTR lpDownloadSize, LPCTSTR lpGrayVerInfo);
	//二维码获取结果
	void java_FireGet2DCodeInfoResult(int iType, int iReturnCode, LPCTSTR pDataStr, LPCTSTR jsonKey, LPCTSTR validTime, int codeType, LPCTSTR msgId);

    //lmd20141204，会议相关的
    void java_FireStartVNCDLLEvent(LPCTSTR pLocalIP, int iLocalPort,LPCTSTR pIP, int iPort,LPCTSTR pDPassword, LPCTSTR plinkid);
	void java_FireDataConfFileDownload(long iOperateType, long iSuccess);
	void java_FireDataConfFileUploadResult(long iOperateType, long iSuccess);
	void java_FireTransScheduleNotify(long iOperateType, long iType,long iTotalSize, long iFinishedSize);

	void java_FireAudioStatNotify(short *pNotify);

	//file trans notify	
    void java_FireUploadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iRetCode, int iRate, DWORD iTransedSize);
	void java_FireDownloadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iRetCode, int iRate, DWORD iTransedSize);

	void java_FireResponseMsgFromHttpAP(LPCTSTR pSessionID, LPCTSTR pRequestLine, int iRetCode, LPCTSTR pContentType, LPCTSTR pBody);
	void java_FireGetOfflineMsgRsp(int iRetCode, LPCTSTR cSeq, int endFlag, int nNum, void **pMsgList);
	void java_FireSendBroadcastMsgRsp(int iRetCode, LPCTSTR cMsgID, LPCTSTR pURIs, LPCTSTR gURIs, LPCTSTR filePath);

	void java_FireGetUserLogonStatusResult(short *pNotify);
	void java_FireConcernGroupOperateResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR gURIList);
	void java_FireUploadFileByXcapRsp(int iRetCode, int iType, LPCTSTR msgId, LPCTSTR filePath);
#ifdef __cplusplus
}
#endif

#endif
