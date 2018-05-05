/*******************************************************************************/
/*
OCX控件层接口头文件定义
*/
/*******************************************************************************/
#ifndef __CTLMETHOD_H
#define __CTLMETHOD_H

#include "CtlEventWrapper.h"

#ifdef _WIN32
	#ifdef SOFTDACTRL_EXPORTS
		#define SOFTDACTRL_API __declspec(dllexport)
	#else
		#define SOFTDACTRL_API __declspec(dllimport)
	#endif
#else
	#define SOFTDACTRL_API  
#endif

 #ifdef __cplusplus
 extern "C"
 {
 #endif

//初始化
SOFTDACTRL_API BOOL bCtl_InitControl();
SOFTDACTRL_API BOOL bCtl_ExitControl();
SOFTDACTRL_API void bCtl_SetEventWrapper(CCTLEventWrapper *pEventWrapper);

//登录注销
SOFTDACTRL_API BOOL bCtl_Logon(long lType, short * pPara, long lSpecialScene);
SOFTDACTRL_API BOOL bCtl_GoOnLogonServer();
SOFTDACTRL_API BOOL bCtl_ForceLogonServer();
SOFTDACTRL_API BOOL bCtl_LogoutServer(long bForce);
SOFTDACTRL_API BOOL bCtl_ServerLinkRecover();
SOFTDACTRL_API BOOL bCtl_CloseServerLink();
SOFTDACTRL_API BOOL bCtl_MOAImmediatelyReReg();

//个人
SOFTDACTRL_API BOOL bCtl_SetXCAPAccount(LPCTSTR pURI, LPCTSTR pPasswd);
SOFTDACTRL_API BOOL bCtl_IMSSetUserInfo(short* pUserInfo);
SOFTDACTRL_API BOOL bCtl_IMSSetMyPhoto(LPCTSTR pIndex, short* pFileContent, long iLen);
SOFTDACTRL_API BOOL bCtl_UserModifyPasswdReq(int iType, LPCTSTR pUserURI, LPCTSTR pOldPassword, LPCTSTR pNewPassword);
SOFTDACTRL_API BOOL bCtl_IMSPsMethod(long iType, short* pPara);
SOFTDACTRL_API BOOL bCtl_IMSGetDeptAddrList(long iType, LPCTSTR pAddrList);
SOFTDACTRL_API BOOL bCtl_IMSGetPersonSubInfo(long lType, short* pRosterMethod);
SOFTDACTRL_API BOOL bCtl_GetCheckCode(LPCTSTR lpUserCode);
SOFTDACTRL_API BOOL bCtl_UserRegistReq(int iType, LPCTSTR pUserName, LPCTSTR pUserURI, LPCTSTR pPassword, LPCTSTR pCompany, LPCTSTR pCheckCode);
SOFTDACTRL_API BOOL bCtl_GetUserStatus();

//联系人
SOFTDACTRL_API BOOL bCtl_IMSAddOneAddressList(LPCTSTR pURI, LPCTSTR pDisplayName, LPCTSTR pInfo);
SOFTDACTRL_API BOOL bCtl_IMSDeleteOneAddressList(LPCTSTR pURI);
SOFTDACTRL_API BOOL bCtl_IMSSubWatcher();
SOFTDACTRL_API BOOL bCtl_IMSDoWatcherSubSeq(LPCTSTR pWatcherURI, long iAccept);
SOFTDACTRL_API BOOL bCtl_IMSSearchOneListInfo(LPCTSTR pListURI);
SOFTDACTRL_API BOOL bCtl_IMSSearchRLS();
SOFTDACTRL_API BOOL bCtl_IMSAddRLSMember(LPCTSTR pURI, LPCTSTR pListName, LPCTSTR pListMemberURI, LPCTSTR pInfo);
SOFTDACTRL_API BOOL bCtl_IMSDeleteRLSMember(LPCTSTR pURI, LPCTSTR pListMemberURI);
SOFTDACTRL_API BOOL bCtl_IMSSubscribeOneList(long iType, LPCTSTR pListURI, long lURIType);
SOFTDACTRL_API BOOL bCtl_IMSSearchUserOnline(long iPage);
SOFTDACTRL_API BOOL bCtl_IMSSearchUserByURI(LPCTSTR pURI, long iPage);
SOFTDACTRL_API BOOL bCtl_IMSSearchUserByDName(LPCTSTR pDName, long iPage);
SOFTDACTRL_API BOOL bCtl_IMSSearchUserByPhone(LPCTSTR pPhone,  long iPage);
SOFTDACTRL_API BOOL bCtl_IMSSearchUserByEmail(LPCTSTR pEmail, long iPage);
SOFTDACTRL_API BOOL bCtl_IMSGetPresenceRules();
SOFTDACTRL_API BOOL bCtl_IMSSubscribeOneRLS(LPCTSTR pURI);
SOFTDACTRL_API BOOL bCtl_IMSSearchAddrList(long iType, short* pSearch, long iPage);
SOFTDACTRL_API BOOL bCtl_IMSLoadAddrList(long iType, short*pAddrList, long iNum, long iPage);
SOFTDACTRL_API BOOL bCtl_CheckURIIsMOA(LPCTSTR lpcsURI);
SOFTDACTRL_API BOOL bCtl_GetUserLogonStatus(LPCTSTR lpURI);
SOFTDACTRL_API BOOL bCtl_RequestLastLogInfo(short* pPara);
SOFTDACTRL_API BOOL bCtl_GetRecommendContact(int pageNo, int pageSize);
SOFTDACTRL_API BOOL bCtl_SyncUserCodeMoaInfo(int iType, LPCTSTR lpMsgId, LPCTSTR lpDeviceId, LPCTSTR lpXmlBody);

//消息
SOFTDACTRL_API BOOL bCtl_IMSSendAttachMessage(long iType, LPCTSTR pFilePath, LPCTSTR pFileType, LPCTSTR pMessage, LPCTSTR pReceiverURI, LPCTSTR pLocalMsgID, int iReceipt);
SOFTDACTRL_API BOOL bCtl_IMSSendMessage(int iType, int iReport, LPCTSTR pMessage, LPCTSTR pDeliverTime, LPCTSTR pReceiverURI, LPCTSTR pLocalMsgID, LPCTSTR pTMSubject, DWORD dwFileSeekSize, int iReceipt=0);
SOFTDACTRL_API BOOL bCtl_IsLargeMessage(LPCTSTR pMessage);
SOFTDACTRL_API BOOL bCtl_GetOfflineMsgReq(LPCTSTR pSeq, int maxCount);

//群组相关
SOFTDACTRL_API BOOL bCtl_IMSCreateOnePrivateGroup(LPCTSTR pGroupName);
SOFTDACTRL_API BOOL bCtl_IMSDeleteOnePrivateGroup(LPCTSTR pGroupName);
SOFTDACTRL_API BOOL bCtl_IMSGetPublicGroup(LPCTSTR lpIdentifier, int iOperFlag);
SOFTDACTRL_API BOOL bCtl_IMSDoPublicGroupInvite(LPCTSTR lpcsOwnerName, LPCTSTR pInviterURI, LPCTSTR pGroupName, long iResult, LPCTSTR pGroupURI, LPCTSTR pReturn);
SOFTDACTRL_API BOOL bCtl_IMSModifyPrivateGroupName(LPCTSTR pOldName, LPCTSTR pNewName);
SOFTDACTRL_API BOOL bCtl_IMSGetPrivateGroupMember(LPCTSTR lpIdentifier);
SOFTDACTRL_API BOOL bCtl_IMSCopyListToPrivateGroup(LPCTSTR pListURI, LPCTSTR pGroupName, long iGroupType, LPCTSTR pDisplayName, long iSeq);
SOFTDACTRL_API BOOL bCtl_IMSDeleteListFromPrivateGroup(LPCTSTR pListURI, LPCTSTR pGroupName);
SOFTDACTRL_API BOOL bCtl_IMSCreateOnePublicGroup(short* pPara);
SOFTDACTRL_API BOOL bCtl_IMSDeleteOnePublicGroup(LPCTSTR pGroupURI);
SOFTDACTRL_API BOOL bCtl_IMSDoPublicGroupJoinReq(LPCTSTR pReqMemberName, LPCTSTR pReqURI, long iResult, LPCTSTR pReturn1, LPCTSTR pReturn2) ;
SOFTDACTRL_API BOOL bCtl_IMSModifyOneListDpName(LPCTSTR pURI, LPCTSTR pDisplayName);
SOFTDACTRL_API BOOL bCtl_IMSReqJoinPublicGroup(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, LPCTSTR pMyDisplayName, LPCTSTR pShareUserURI, LPCTSTR pShareUserName);
SOFTDACTRL_API BOOL bCtl_IMSLeavePublicGroup(LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex);
SOFTDACTRL_API BOOL bCtl_ModifyGroupAttributeReq(LPCTSTR pGroupURI, LPCTSTR pModType, LPCTSTR pNewValue, LPCTSTR operateId);
SOFTDACTRL_API BOOL bCtl_IMSDeleteOneFromMyPublicGroup(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex);
SOFTDACTRL_API BOOL bCtl_IMSInviteSomeoneToMyPublicGroup(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, LPCTSTR pOperaterIndex);
SOFTDACTRL_API BOOL bCtl_GetIMPrivateGroupMember(LPCTSTR  lpGroupIdentifier);
SOFTDACTRL_API BOOL bCtl_GetAllIMPrivateGroupList();

//呼叫
SOFTDACTRL_API BOOL bCtl_LogonSS();
SOFTDACTRL_API BOOL bCtl_LogoutSS();
SOFTDACTRL_API BOOL bCtl_LogonSS2(LPCTSTR pConfuri, LPCTSTR pUserName, LPCTSTR pPassword, LPCTSTR pServerIP,LPCTSTR pServerPort);
SOFTDACTRL_API BOOL bCtl_CreateMultiCall(LPCTSTR lpJID, LPCTSTR lpCallee, long lType);
SOFTDACTRL_API BOOL bCtl_InviteToMultiCall(LPCTSTR lpJID, LPCTSTR lpCallee, long lType);
SOFTDACTRL_API BOOL bCtl_AcceptCall(LPCTSTR lpCaller);
SOFTDACTRL_API BOOL bCtl_RejectCall(LPCTSTR lpCaller, int iCause);
SOFTDACTRL_API BOOL bCtl_LeaveCall(int iCause);
SOFTDACTRL_API void bCtl_SecondDialNum(LPCTSTR lpNum);
SOFTDACTRL_API BOOL bCtl_CallHold(LPCTSTR pPhoneNumber, long iType);
SOFTDACTRL_API BOOL bCtl_DoNewCallWhenBusy2(long iDoType, LPCTSTR pCaller);
SOFTDACTRL_API char *bCtl_GetAudioProps();
SOFTDACTRL_API char *bCtl_GetVideoProps();
SOFTDACTRL_API void bCtl_SetAudioProps(LPCTSTR pCodecName, long nPacketPeriod);
SOFTDACTRL_API void bCtl_SetVideoProps(LPCTSTR pCodecName, long nRate);
SOFTDACTRL_API BOOL bCtl_IMSCallMethod(long lType, short* pCallMethod);
SOFTDACTRL_API BOOL bCtl_StartRecord(LPCTSTR lpFilePathName);
SOFTDACTRL_API BOOL bCtl_StopRecord();
SOFTDACTRL_API BOOL bCtl_SelectTalker(LPCTSTR lpCallee);
SOFTDACTRL_API BOOL bCtl_ReleaseTalker(LPCTSTR lpCallee);
SOFTDACTRL_API BOOL bCtl_CtdCallMethod(long lType, short* pPara);
SOFTDACTRL_API BOOL bCtl_AudioConnectDevice();
SOFTDACTRL_API BOOL bCtl_AudioDisconnectDevice();
SOFTDACTRL_API BOOL bCtl_SetAudioMute();
SOFTDACTRL_API BOOL bCtl_SetAudioNotMute();
SOFTDACTRL_API BOOL bCtl_OpenAudioMedia();

//公众号
SOFTDACTRL_API BOOL bCtl_SearchPubAccount(int iType, LPCTSTR pubAccName, int pageNo, int pageSize);
SOFTDACTRL_API BOOL bCtl_GetHistoryPubMsg(int count, LPCTSTR pPubAccId, int seq, LPCTSTR pFlag);
SOFTDACTRL_API BOOL bCtl_GetPublicMsg(LPCTSTR pMsgID,LPCTSTR lpURL);
SOFTDACTRL_API BOOL bCtl_SendPubAccountReq(int iType, LPCTSTR pPubAccId);
SOFTDACTRL_API BOOL bCtl_GetPubAccountMenu(LPCTSTR pPubAccId, LPCTSTR etag);
SOFTDACTRL_API BOOL bCtl_SendPubAccMsg(LPCTSTR pPubAccId, LPCTSTR pSender, int iMsgType, LPCTSTR pPutMsgId, LPCTSTR pMenuId, LPCTSTR pContent);


//个人设置
SOFTDACTRL_API BOOL bCtl_MessageMuteNotifyOperateReq(int iType, LPCTSTR pURI, LPCTSTR operateId);
SOFTDACTRL_API BOOL bCtl_MessageReceiptOperateReq(int iOperType, LPCTSTR cSessionID, int isGroup, LPCTSTR pURI, LPCTSTR pMsgIDs);
SOFTDACTRL_API BOOL bCtl_ConcernGroupOperateReq(int iType, LPCTSTR eTag, LPCTSTR gURI);
SOFTDACTRL_API BOOL bCtl_ContactMarkReq(int iType, LPCTSTR eTag, LPCTSTR pMarkEx);

//文件传输
SOFTDACTRL_API BOOL bCtl_UploadFileByXcapReq(int iType, LPCTSTR msgId, LPCTSTR filePath);
SOFTDACTRL_API BOOL bCtl_UploadFileReq(LPCTSTR pSessionID, LPCTSTR pFilePathName, int iOperType, LPCTSTR pFileID);
SOFTDACTRL_API BOOL bCtl_DownloadFileReq(LPCTSTR pSessionID, LPCTSTR pFilePathName, int iOperType, LPCTSTR pFileID);

//上报接口
SOFTDACTRL_API BOOL bCtl_LoginInfoReport(LPCTSTR lpRecord);
SOFTDACTRL_API BOOL bCtl_SendBroadcastMsgReq(LPCTSTR cMsgID, LPCTSTR pURIs, LPCTSTR gURIs, LPCTSTR filePath);
SOFTDACTRL_API BOOL bCtl_ClientLoginNotify(int type);
SOFTDACTRL_API BOOL bCtl_UserBindOperateReq(int iOperType, LPCTSTR pBindNumber, int iNumberType, LPCTSTR pPasswd, LPCTSTR pCompanyId);

//通用
SOFTDACTRL_API BOOL bCtl_PatSpring(LPCTSTR lpDestNumber);
SOFTDACTRL_API BOOL bCtl_WhetherEncrypt(long lSetValue);
SOFTDACTRL_API BOOL bCtl_IMSDispUICmdMethod(long lType, short* pCmdPara);
SOFTDACTRL_API BOOL bCtl_GetSensitiveWord();
SOFTDACTRL_API BOOL bCtl_Get2DCodeInfo(int iType, LPCTSTR strKeyWord, int validType, int clientExist, int codeType, LPCTSTR msgId);
SOFTDACTRL_API BOOL bCtl_IMSRuleSetMethod(long lType, short* pPara);
SOFTDACTRL_API BOOL bCtl_DispUICmdForEcp2Method(long lType, short* pCmdPara);
SOFTDACTRL_API BOOL bCtl_UpdateMsgIndication(short* pPara);
SOFTDACTRL_API BOOL bCtl_SendSoapMessageByXcap(int iType, LPCTSTR lpcsRequestLine, LPCTSTR lpcstrAction, LPCTSTR lpcsBody);
SOFTDACTRL_API BOOL bCtl_SendRequestMsgToHttpAP(LPCTSTR cSessionID, LPCTSTR cRequestLine, LPCTSTR cMethod, LPCTSTR cContentType, LPCTSTR cEtag, char* pBody);
SOFTDACTRL_API BOOL bCtl_PushAPNs(LPCTSTR pOsType, LPCTSTR pToken, LPCTSTR pTokenVoIP);
SOFTDACTRL_API BOOL bCtl_VideoData2beEnc(LPCTSTR pData, long len);
SOFTDACTRL_API BOOL bCtl_SetSoftdaEnv(short far* pPara);
SOFTDACTRL_API BOOL bCtl_SetSipConnectType(int type);

//检查
SOFTDACTRL_API BOOL bCtl_CheckOsTimer(int pno);
SOFTDACTRL_API BOOL bCtl_CheckSocket();
SOFTDACTRL_API BOOL bCtl_CheckConnection();
SOFTDACTRL_API INT32 bCtl_CheckSipServer(LPCTSTR pServerIP, unsigned short lpPort, int sec);
SOFTDACTRL_API INT32 bCtl_CheckHttpServer(LPCTSTR pServerIP, unsigned short lpPort, int sec);

//灰度版本
SOFTDACTRL_API BOOL bCtl_GetGrayVersion(LPCTSTR lpVersion);

//
SOFTDACTRL_API BOOL bCtl_Aes128EcbEncHex(unsigned char* in, int inlen, unsigned char *out, int* outlen, unsigned char * key);
SOFTDACTRL_API BOOL bCtl_Aes128EcbDecHex(unsigned char* in, int inlen, unsigned char *out, int* outlen, unsigned char * key);
SOFTDACTRL_API void bCtl_AesBase64Encrypt(int type, char *str_in, char *str_out);
SOFTDACTRL_API int bCtl_Base64Encode(int type, UINT8 *inbuf, int inlen, UINT8 *outbuf, int *poutlen);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif
