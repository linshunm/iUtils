#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include "rcscontroller.h"

#include "JObjectWrapper.hpp"

typedef union
{
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

typedef unsigned char byte;
static JavaVM* gs_jvm = NULL;
static const char *classPathName = "com/zte/softda/ocx/OcxNative";
static const char *classPathName_postagent = "com/zte/softda/ocx/OcxEventCallBack";
static char TAG[64] = "rcscontroller";
static jclass gpostClass = NULL;

char *getStringField(JNIEnv* env, jobject jobj, jclass jcs,  const char * fieldName, char *result, int ilen);
char *safeGetStringUTFChars(JNIEnv* env, jstring string, char* dest, int size, jboolean *iscopy);
char *safeAllocStringUTFCharsFromJString(JNIEnv* env, jstring string);

char* safeAllocStringUTFchars(JNIEnv* env, jobject jobj, jclass jcs,  const char * fieldName);

int getStaticJniMethod(char *pFuncName, char *paraList, JNIEnv **pEnv, jmethodID *pJinMethod)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller getStaticJniMethod funcname[%s], paraList[%s]", pFuncName, paraList);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "getStaticJniMethod funcname[%s], paraList[%s]", pFuncName, paraList);

    if (NULL == gs_jvm)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontrollers gs_jvm is NULL");
        else if(g_WriteLogType == 1)
            LOGE(TAG, "gs_jvm is NULL");
        return JNI_ERR;
    }
    if (NULL == gpostClass)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller gpostClass is NULL");
        else if(g_WriteLogType == 1)
            LOGE(TAG, "gpostClass is NULL");

        return JNI_ERR;
    }
    if (gs_jvm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller ERROR: GetEnv failed");
        else if(g_WriteLogType == 1)
            LOGE(TAG, "ERROR: GetEnv failed");

        return JNI_ERR;
    }
    env = uenv.env;

    /* notes: if this , when subthread invoking, will not find this clsss , so get the jclass's globalref    */
    jclass  JniClass = env->FindClass(classPathName_postagent);
    if (JniClass == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller JniClass is NULL");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "JniClass is NULL");

        return 1;
    }

    gJinMethod = env->GetStaticMethodID(JniClass, pFuncName, paraList);
    if(gJinMethod == 0 || gJinMethod == NULL)
    {
        LOGE(TAG, "GetStaticMethodID Failed, funcname[%s], paraList[%s]", pFuncName, paraList);
        return JNI_ERR;
    }

    *pJinMethod = gJinMethod;
    *pEnv = env;

    return JNI_OK;
}
/*===============================================
*begin callback java method
*===============================================*/
void java_FireLogonResult(long lResult, long lReason, long extraCode)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireLogonResult lResult[%d], lReason[%d]", lResult, lReason);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireLogonResult lResult[%d], lReason[%d]", lResult, lReason);

    iresult = getStaticJniMethod((char *)"java_FireLogonResult", (char *)"(III)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireLogonResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireLogonResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lResult, lReason, extraCode);
}
void java_FireReportMemberStatus(char *lpJID, char *lpCallee, long lStatus, long lCallType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireReportMemberStatus lpJID[%s] lpCallee[%s] lResult[%d], lReason[%d]",
                        lpJID, lpCallee, lStatus, lCallType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireReportMemberStatus pJID[%s] lpCallee[%s] lResult[%d], lReason[%d]", lpJID, lpCallee, lStatus, lCallType);

    iresult = getStaticJniMethod((char *)"java_FireReportMemberStatus", (char *)"(Ljava/lang/String;Ljava/lang/String;II)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireReportMemberStatus");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireReportMemberStatus");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,lpJID), JNI_STR(env,lpCallee), lStatus, lCallType);
}

void java_FireSomeOneCallMe(char *lpJID, char *lpCaller, long lType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireSomeOneCallMe lpJID[%s] lpCaller[%s] lType[%d]",
                        lpJID, lpCaller, lType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireSomeOneCallMe lpJID[%s] lpCaller[%s] lType[%d]", lpJID, lpCaller, lType);

    iresult = getStaticJniMethod((char *)"java_FireSomeOneCallMe", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireSomeOneCallMe");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireSomeOneCallMe");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,lpJID), JNI_STR(env,lpCaller), lType);
}
void java_FireRegisterSoftPhoneToSS(char * lpSoftPhone, int lType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireRegisterSoftPhoneToSS lpSoftPhone[%s] lType[%d]",
                        lpSoftPhone, lType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireRegisterSoftPhoneToSS lpSoftPhone[%s] lType[%d]", lpSoftPhone, lType);

    iresult = getStaticJniMethod((char *)"java_FireRegisterSoftPhoneToSS", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireRegisterSoftPhoneToSS");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireRegisterSoftPhoneToSS");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,lpSoftPhone), lType);
}
void java_FireRegisterSoftPhoneToSSNew(short far* pInfo)
{
    jmethodID gJinMethod;	 
    JNIEnv *env;
    int iresult = -1;
	jfieldID id_fieldID;

	Reg_SoftPhoneToSS_RESULT* pResult = NULL;

	pResult = (Reg_SoftPhoneToSS_RESULT*)pInfo;

	if(pResult == NULL)
	{
        LOGI(TAG,"not found java_FireRegisterSoftPhoneToSSNew  pResult == NULL");  
        return;		
	}

    LOGI(TAG,"java_FireRegisterSoftPhoneToSSNew lpSoftPhone[%s] lType[%d]", 
		pResult->cPhone, pResult->lType); 
	
    iresult = getStaticJniMethod((char *)"java_FireRegisterSoftPhoneToSSNew", (char *)"(Lcom/zte/softda/ocx/RegisterSoftPhoneToSS_T;)V", &env, &gJinMethod);	

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        LOGI(TAG,"not found java_FireRegisterSoftPhoneToSSNew");  
        return;	
    }
	
	jclass class_RegSSResult = env->FindClass("com/zte/softda/ocx/RegisterSoftPhoneToSS_T");
	jmethodID id_RegSSResult  = env->GetMethodID(class_RegSSResult, "<init>", "()V");
	jobject obj_RegSSResult = env->NewObject(class_RegSSResult, id_RegSSResult);  
	
	id_fieldID = env->GetFieldID(class_RegSSResult, "cConfuri", "Ljava/lang/String;");
	env->SetObjectField(obj_RegSSResult, id_fieldID, JNI_STR(env,pResult->cConfuri));  
	
	id_fieldID = env->GetFieldID(class_RegSSResult, "cPhone", "Ljava/lang/String;");
	env->SetObjectField(obj_RegSSResult, id_fieldID, JNI_STR(env,pResult->cPhone));  
	
	id_fieldID = env->GetFieldID(class_RegSSResult, "cPhonepwd", "Ljava/lang/String;");
	env->SetObjectField(obj_RegSSResult, id_fieldID, JNI_STR(env,pResult->cPhonepwd));	
	
	id_fieldID = env->GetFieldID(class_RegSSResult, "cIpaddr", "Ljava/lang/String;");
	env->SetObjectField(obj_RegSSResult, id_fieldID, JNI_STR(env,pResult->cIpaddr));  
	
	id_fieldID = env->GetFieldID(class_RegSSResult, "cPort", "Ljava/lang/String;");
	env->SetObjectField(obj_RegSSResult, id_fieldID, JNI_STR(env,pResult->cPort)); 
	
	id_fieldID = env->GetFieldID(class_RegSSResult, "lType", "J");
	env->SetLongField(obj_RegSSResult, id_fieldID, pResult->lType);	  
	
	env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_RegSSResult); 

    return;
}
void java_FireAlreadyLogout(long lReason)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireAlreadyLogout lReason[%d]", lReason);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireAlreadyLogout lReason[%d]", lReason);

    iresult = getStaticJniMethod((char *)"java_FireAlreadyLogout", (char *)"(I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireAlreadyLogout");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireAlreadyLogout");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lReason);
}
void java_FireLogoutServerSuccess()
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireLogoutServerSuccess");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireLogoutServerSuccess");

    iresult = getStaticJniMethod((char *)"java_FireLogoutServerSuccess", (char *)"()V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireLogoutServerSuccess");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireLogoutServerSuccess");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod);
}
void java_FireServerLinkLost(int itype)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireServerLinkLost type[%d]", itype);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireServerLinkLost[%d]", itype);

    iresult = getStaticJniMethod((char *)"java_FireServerLinkLost", (char *)"(I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireServerLinkLost");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireServerLinkLost");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, (jint)itype);
}
void java_FireServerLinkOK()
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireServerLinkOK");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireServerLinkOK");
    iresult = getStaticJniMethod((char *)"java_FireServerLinkOK", (char *)"()V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireServerLinkOK");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireServerLinkOK");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod);
}
void java_FireIMSCallEvent(long lType, LPCTSTR lpCaller, short far* pCallEvent)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jobject callEvent = NULL;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSCallEvent lType[%d] lpCaller[%s]", lType, lpCaller);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSCallEvent lType[%d] lpCaller[%s]", lType, lpCaller);

    iresult = getStaticJniMethod((char *)"java_FireIMSCallEvent", (char *)"(ILjava/lang/String;Ljava/lang/Object;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSCallEvent");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSCallEvent");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, JNI_STR(env,lpCaller), callEvent);
}
void java_FireMultiRing()
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireMultiRing");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireMultiRing");

    iresult = getStaticJniMethod((char *)"java_FireMultiRing", (char *)"()V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireMultiRing");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireMultiRing");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod);
}
void java_FireLostPacketNotify(long lTotalPacketNum, long lLostPacketNum, float fLostPacketRate)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireLostPacketNotify lTotalPacketNum[%d] lLostPacketNum[%d] fLostPacketRate[%f]",
                        lTotalPacketNum, lLostPacketNum, fLostPacketRate);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireLostPacketNotify lTotalPacketNum[%d] lLostPacketNum[%d] fLostPacketRate[%f]", lTotalPacketNum, lLostPacketNum, fLostPacketRate);

    iresult = getStaticJniMethod((char *)"java_FireLostPacketNotify", (char *)"(IIF)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireLostPacketNotify");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireLostPacketNotify");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lTotalPacketNum, lLostPacketNum, fLostPacketRate);
}
void java_FireVideoCaptureAsMirrorStopped()
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireVideoCaptureAsMirrorStopped");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireVideoCaptureAsMirrorStopped");

    iresult = getStaticJniMethod((char *)"java_FireVideoCaptureAsMirrorStopped", (char *)"()V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireVideoCaptureAsMirrorStopped");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireVideoCaptureAsMirrorStopped");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod);
}
void java_FireNewCallWhenBusy2(LPCTSTR pCaller, long iType, LPCTSTR pURL, long iCallType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireNewCallWhenBusy2 pCaller[%s] iType[%d] pURL[%s] iCallType[%d]", pCaller, iType, pURL, iCallType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireNewCallWhenBusy2 pCaller[%s] iType[%d] pURL[%s] iCallType[%d]", pCaller, iType, pURL, iCallType);

    iresult = getStaticJniMethod((char *)"java_FireNewCallWhenBusy2", (char *)"(Ljava/lang/String;ILjava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireNewCallWhenBusy2");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireNewCallWhenBusy2");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pCaller), iType, JNI_STR(env,pURL), iCallType);
}
void java_FireCalleeForward(LPCTSTR pCalleeNumber, LPCTSTR pForwardNumber)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireCalleeForward  begin pCalleeNumber[%s], pForwardNumber[%s]", pCalleeNumber, pForwardNumber);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireCalleeForward  begin pCalleeNumber[%s], pForwardNumber[%s]", pCalleeNumber, pForwardNumber);

    iresult = getStaticJniMethod((char *)"java_FireCalleeForward", (char *)"(Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireCalleeForward");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireCalleeForward");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pCalleeNumber), JNI_STR(env,pForwardNumber));
}
void java_FireCreateConfCallSuccess(LPCTSTR lpConfURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireCreateConfCallSuccess begin lpConfURI[%s]", lpConfURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireCreateConfCallSuccess begin lpConfURI[%s]", lpConfURI);

    iresult = getStaticJniMethod((char *)"java_FireCreateConfCallSuccess", (char *)"(Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireCreateConfCallSuccess");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireCreateConfCallSuccess");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,lpConfURI));
}
void java_FireColorImageURLArrived(LPCTSTR lpURL, LPCTSTR lpCaller, LPCTSTR lpRoomID, long lType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireColorImageURLArrived begin lpURL[%s], lpCaller[%s] lpRoomID[%s] lType[%d]",
                        lpURL, lpCaller, lpRoomID, lType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireColorImageURLArrived begin lpURL[%s], lpCaller[%s] lpRoomID[%s] lType[%d]", lpURL, lpCaller, lpRoomID, lType);

    iresult = getStaticJniMethod((char *)"java_FireColorImageURLArrived", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireColorImageURLArrived");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireColorImageURLArrived");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,lpURL), JNI_STR(env,lpCaller), JNI_STR(env,lpRoomID), lType);
}
void java_FirePhoneConnected(long lVideo)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FirePhoneConnected lpConfURI[%d]", lVideo);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FirePhoneConnected lpConfURI[%d]", lVideo);

    iresult = getStaticJniMethod((char *)"java_FirePhoneConnected", (char *)"(I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FirePhoneConnected");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FirePhoneConnected");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod,  lVideo);
}
void java_FirePhoneIdle()
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FirePhoneIdle begin");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FirePhoneIdle begin");

    iresult = getStaticJniMethod((char *)"java_FirePhoneIdle", (char *)"()V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FirePhoneIdle");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FirePhoneIdle");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod);
}
void java_FireReportMultiCallError(long lError)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireReportMultiCallError lpConfURI[%d]", lError);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireReportMultiCallError lpConfURI[%d]", lError);

    iresult = getStaticJniMethod((char *)"java_FireReportMultiCallError", (char *)"(I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireReportMultiCallError");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireReportMultiCallError");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod,  lError);
}
void java_FirePhoneStatus(long lType, long lValue, LPCTSTR lpMsg)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FirePhoneStatus lType[%d], lValue[%d] lpMsg[%s]", lType, lValue, lpMsg);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FirePhoneStatus lType[%d], lValue[%d] lpMsg[%s]", lType, lValue, lpMsg);

    iresult = getStaticJniMethod((char *)"java_FirePhoneStatus", (char *)"(IILjava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FirePhoneStatus");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FirePhoneStatus");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, lValue, JNI_STR(env,lpMsg));
}
void java_FireCallOver()
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireCallOver");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireCallOver");

    iresult = getStaticJniMethod((char *)"java_FireCallOver", (char *)"()V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireCallOver");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireCallOver");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod);
}

void java_FireIMSMessageArrived(int iType, LPCTSTR pChatRoomURI, LPCTSTR pSenderURI, LPCTSTR pMessage, LPCTSTR pTime, LPCTSTR pMessageId, int iReceipt, LPCTSTR pSipTime)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    TraceMsgWindow1(1, "rcscontroller java_FireIMSMessageArrived iType[%d] pChatRoomURI[%s] pSenderURI[%s] pMessage[***] pTime[%s] pMessageId[%s] iReceipt[%d] pSipTime[%s]",
                    iType, pChatRoomURI, pSenderURI, pTime, pMessageId, iReceipt, pSipTime);

    iresult = getStaticJniMethod((char *)"java_FireIMSMessageArrived", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {      
        TraceMsgWindow1(1, "rcscontroller not found java_FireIMSMessageArrived");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, JNI_STR(env,pChatRoomURI), JNI_STR(env,pSenderURI), JNI_STR(env,pMessage), JNI_STR(env,pTime), JNI_STR(env,pMessageId), iReceipt, JNI_STR(env,pSipTime));
}
void java_FireIMSAttachMessageArrived(int iType, LPCTSTR pSenderURI, LPCTSTR pChatroomURI, LPCTSTR pSendTime, LPCTSTR pAttachFilePath, LPCTSTR pAttachFileType, LPCTSTR pMessage, LPCTSTR pMessageId)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    //jstring jChatroomURI = NULL, jSendTime = NULL, jSenderURI = NULL, jAttachFilePath = NULL, jAttachFileType = NULL, jMessage = NULL, jpMessageId = NULL;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSAttachMessageArrived iType[%d] pSenderURI[%s] pChatroomURI[%s] pSendTime[%s] pAttachFilePath[%s] pAttachFileType[%s] pMessage[%s] jpMessageId[%s]",
                        iType, pSenderURI, pChatroomURI, pSendTime, pAttachFilePath, pAttachFileType, pMessage, pMessageId);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSAttachMessageArrived iType[%d] pSenderURI[%s] pChatroomURI[%s] pSendTime[%s] pAttachFilePath[%s] pAttachFileType[%s] pMessage[%s] pMessageId[%s]", iType, pSenderURI, pChatroomURI, pSendTime, pAttachFilePath, pAttachFileType, pMessage, pMessageId);

    iresult = getStaticJniMethod((char *)"java_FireIMSAttachMessageArrived", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSAttachMessageArrived");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSAttachMessageArrived");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, JNI_STR(env,pSenderURI), JNI_STR(env,pChatroomURI), JNI_STR(env,pSendTime), JNI_STR(env,pAttachFilePath), JNI_STR(env,pAttachFileType), JNI_STR(env,pMessage), JNI_STR(env,pMessageId));
    return;
}

void java_FireIMSAttachMessageArrivedImmediately(LPCTSTR pSenderURI, LPCTSTR pChatroomURI, LPCTSTR pAttachFileType, LPCTSTR pMessageId)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    //jstring jSenderURI = NULL, jpChatroomURI = NULL, jAttachFileType = NULL, jMessageId = NULL;
	
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSAttachMessageArrivedImmediately pSenderURI[%s] pChatroomURI[%s] pAttachFileType[%s] pMessageId[%s]",pSenderURI,pChatroomURI,pAttachFileType,pMessageId);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSAttachMessageArrivedImmediately pSenderURI[%s] pChatroomURI[%s] pAttachFileType[%s] pMessageId[%s]", pSenderURI, pChatroomURI, pAttachFileType, pMessageId);
	
    iresult = getStaticJniMethod((char *)"java_FireIMSAttachMessageArrivedImmediately", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
	
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSAttachMessageArrivedImmediately");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSAttachMessageArrivedImmediately");
		
        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSenderURI), JNI_STR(env,pChatroomURI), JNI_STR(env,pAttachFileType), JNI_STR(env,pMessageId));
    return;
}

void java_FireIMSSendMessageResult(short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    IMSSendMessageResult_T *pResult = NULL;

    pResult = (IMSSendMessageResult_T *)pNotify;
    if(pResult == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSSendMessageResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSSendMessageResult");

    }

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSendMessageResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSendMessageResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSSendMessageResult", (char *)"(Lcom/zte/softda/ocx/FireSendMessageResultPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSendMessageResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSendMessageResult");

        return;
    }

    jclass class_SendMessageResult = env->FindClass("com/zte/softda/ocx/FireSendMessageResultPara");
    jmethodID id_SendMessageResult  = env->GetMethodID(class_SendMessageResult, "<init>", "()V");
    jobject obj_SendMessageResult = env->NewObject(class_SendMessageResult, id_SendMessageResult);

    id_fieldID = env->GetFieldID(class_SendMessageResult, "nType", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->nType);

    jstring pLocalMsgID = env->NewStringUTF(pResult->pLocalMsgID);
    id_fieldID = env->GetFieldID(class_SendMessageResult, "pLocalMsgID", "Ljava/lang/String;");
    env->SetObjectField(obj_SendMessageResult, id_fieldID, pLocalMsgID);

    jstring pReceiverURI = env->NewStringUTF(pResult->pReceiverURI);
    id_fieldID = env->GetFieldID(class_SendMessageResult, "pReceiverURI", "Ljava/lang/String;");
    env->SetObjectField(obj_SendMessageResult, id_fieldID, pReceiverURI);

    id_fieldID = env->GetFieldID(class_SendMessageResult, "iResult", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->iResult);

    id_fieldID = env->GetFieldID(class_SendMessageResult, "nSequence", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->nSequence);

    id_fieldID = env->GetFieldID(class_SendMessageResult, "nSum", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->nSum);

    jstring pDate = env->NewStringUTF(pResult->strDate);
    id_fieldID = env->GetFieldID(class_SendMessageResult, "pDate", "Ljava/lang/String;");
    env->SetObjectField(obj_SendMessageResult, id_fieldID, pDate);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_SendMessageResult);

    env->DeleteLocalRef(pLocalMsgID);
    env->DeleteLocalRef(pReceiverURI);
    env->DeleteLocalRef(pDate);
}

void java_FireIMSSendLargeMessageResult(short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    IMSSendMessageResult_T *pResult = NULL;
	
    pResult = (IMSSendMessageResult_T *)pNotify;
    if(pResult == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSSendLargeMessageResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSSendLargeMessageResult");
		
    }
	
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSendLargeMessageResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSendMessageResult");
	
    iresult = getStaticJniMethod((char *)"java_FireIMSSendLargeMessageResult", (char *)"(Lcom/zte/softda/ocx/FireSendMessageResultPara;)V", &env, &gJinMethod);
	
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSendLargeMessageResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSendMessageResult");
		
        return;
    }
	
    jclass class_SendMessageResult = env->FindClass("com/zte/softda/ocx/FireSendMessageResultPara");
    jmethodID id_SendMessageResult  = env->GetMethodID(class_SendMessageResult, "<init>", "()V");
    jobject obj_SendMessageResult = env->NewObject(class_SendMessageResult, id_SendMessageResult);
	
    id_fieldID = env->GetFieldID(class_SendMessageResult, "nType", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->nType);
	
    jstring pLocalMsgID = env->NewStringUTF(pResult->pLocalMsgID);
    id_fieldID = env->GetFieldID(class_SendMessageResult, "pLocalMsgID", "Ljava/lang/String;");
    env->SetObjectField(obj_SendMessageResult, id_fieldID, pLocalMsgID);
	
    jstring pReceiverURI = env->NewStringUTF(pResult->pReceiverURI);
    id_fieldID = env->GetFieldID(class_SendMessageResult, "pReceiverURI", "Ljava/lang/String;");
    env->SetObjectField(obj_SendMessageResult, id_fieldID, pReceiverURI);
	
    id_fieldID = env->GetFieldID(class_SendMessageResult, "iResult", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->iResult);
	
    id_fieldID = env->GetFieldID(class_SendMessageResult, "nSequence", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->nSequence);
	
    id_fieldID = env->GetFieldID(class_SendMessageResult, "nSum", "J");
    env->SetLongField(obj_SendMessageResult, id_fieldID, (jlong)pResult->nSum);

    jstring pDate = env->NewStringUTF(pResult->strDate);
    id_fieldID = env->GetFieldID(class_SendMessageResult, "pDate", "Ljava/lang/String;");
    env->SetObjectField(obj_SendMessageResult, id_fieldID, pDate);
	
    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_SendMessageResult);
	
    env->DeleteLocalRef(pLocalMsgID);
    env->DeleteLocalRef(pReceiverURI);
    env->DeleteLocalRef(pDate);
}

void java_FireIMSRecvFileTransReq(LPCTSTR pSenderURI, LPCTSTR pFilePathName, int iFileSize, LPCTSTR pSessionID, LPCTSTR pFileID)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSRecvFileTransReq pSenderURI[%s] pFilePathName[%s]", pSenderURI, pFilePathName);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSRecvFileTransReq pSenderURI[%s] pFilePathName[%s]", pSenderURI, pFilePathName);

    iresult = getStaticJniMethod((char *)"java_FireIMSRecvFileTransReq", (char *)"(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSRecvFileTransReq");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSRecvFileTransReq");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSenderURI), JNI_STR(env,pFilePathName), (jint)iFileSize, JNI_STR(env,pSessionID), JNI_STR(env,pFileID));
}
void java_FireIMSRecvFile(LPCTSTR pSenderURI, LPCTSTR pFilePathName, DWORD dwFileSize, LPCTSTR pSessionID, LPCTSTR pFileID, int IsUtf8,DWORD dwSeekSize,LPCTSTR pFilePath)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSRecvFile pSenderURI[%s] pFilePathName[%s] dwSeekSize[%u] pFilePath[%d]....", pSenderURI, pFilePathName,dwSeekSize,pFilePath);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSRecvFile pSenderURI[%s] pFilePathName[%s]....", pSenderURI, pFilePathName);

    iresult = getStaticJniMethod((char *)"java_FireIMSRecvFile", (char *)"(Ljava/lang/String;[BILjava/lang/String;Ljava/lang/String;II[B)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSRecvFile");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSRecvFile");

        return;
    }
    jbyte *by = (jbyte*)pFilePathName;
    jbyteArray jarray = env->NewByteArray(strlen(pFilePathName));
    env->SetByteArrayRegion(jarray, 0, strlen(pFilePathName), by);
	 jbyte *by2 = (jbyte*)pFilePath;
    jbyteArray jarray2 = env->NewByteArray(strlen(pFilePath));
    env->SetByteArrayRegion(jarray2, 0, strlen(pFilePath), by2);
//	env->CallStaticVoidMethod(gpostClass,gJinMethod, JNI_STR(env,pSenderURI), JNI_STR(env,pFilePathName), (jdouble)dbFileSize, JNI_STR(env,pSessionID), JNI_STR(env,pFileID));
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSenderURI), jarray, (jint)dwFileSize, JNI_STR(env,pSessionID), JNI_STR(env,pFileID), (jint)IsUtf8,(jint)dwSeekSize,jarray2);
}
void java_FireIMSFileTransRate(LPCTSTR pSessionID, int iRate, DWORD dwTransedSize)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSFileTransRate pSessionID[%s]", pSessionID);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSFileTransRate pSessionID[%s]", pSessionID);

    iresult = getStaticJniMethod((char *)"java_FireIMSFileTransRate", (char *)"(Ljava/lang/String;II)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSFileTransRate");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSFileTransRate");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSessionID), (jint)iRate, (jint)dwTransedSize);
}
void java_FireIMSSenderCancelSend(long iCancelType, LPCTSTR pSessionID)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSenderCancelSend iCancelType[%d] pSessionID[%s]", iCancelType, pSessionID);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSenderCancelSend iCancelType[%d] pSessionID[%s]", iCancelType, pSessionID);

    iresult = getStaticJniMethod((char *)"java_FireIMSSenderCancelSend", (char *)"(ILjava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSenderCancelSend");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSenderCancelSend");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, (jint)iCancelType, JNI_STR(env,pSessionID));
}
void java_FireIMSReceiverCancelReceive(long iCancelType, LPCTSTR pSessionID)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSReceiverCancelReceive iCancelType[%d] pSessionID[%s]", iCancelType, pSessionID);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSReceiverCancelReceive iCancelType[%d] pSessionID[%s]", iCancelType, pSessionID);

    iresult = getStaticJniMethod((char *)"java_FireIMSReceiverCancelReceive", (char *)"(ILjava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSReceiverCancelReceive");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSReceiverCancelReceive");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, (jint)iCancelType, JNI_STR(env,pSessionID));
}
void java_FireIMSAssociatedIdentifyArrived(long iNum, short far* pIdentify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    IMSAssociatedIdentifyArrived_T *pMyIdentify = (IMSAssociatedIdentifyArrived_T *)pIdentify;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSAssociatedIdentifyArrived iNum[%d]", pMyIdentify->iNum);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSAssociatedIdentifyArrived iNum[%d]", pMyIdentify->iNum);

    iresult = getStaticJniMethod((char *)"java_FireIMSAssociatedIdentifyArrived", (char *)"(Lcom/zte/softda/ocx/FireAssociatedIdentifyPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSAssociatedIdentifyArrived");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSAssociatedIdentifyArrived");

        return;
    }

    jclass class_Identify = env->FindClass("com/zte/softda/ocx/FireAssociatedIdentifyPara");
    jmethodID id_dentify  = env->GetMethodID(class_Identify, "<init>", "()V");
    jobject obj_Identify = env->NewObject(class_Identify, id_dentify);

    id_fieldID = env->GetFieldID(class_Identify, "iNum", "J");
    env->SetLongField(obj_Identify, id_fieldID, pMyIdentify->iNum);

    jclass class_string = env->FindClass("java/lang/String");
    jobjectArray array_Identify = env->NewObjectArray(pMyIdentify->iNum, class_string, NULL);

    for (i = 0; i < pMyIdentify->iNum; i++)
    {
        env->SetObjectArrayElement (array_Identify, i, JNI_STR(env,pMyIdentify->pIdentify[i]));
    }
    id_fieldID = env->GetFieldID(class_Identify, "IdentifyArray", "[Ljava/lang/String;");
    env->SetObjectField(obj_Identify, id_fieldID,  array_Identify);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_Identify);
}
void java_FireIMSGotOneWatcher(LPCTSTR pWatcherURI, LPCTSTR pStatus)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOneWatcher pWatcherURI[%s] pStatus[%s]", pWatcherURI, pStatus);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotOneWatcher pWatcherURI[%s] pStatus[%s]", pWatcherURI, pStatus);

    iresult = getStaticJniMethod((char *)"java_FireIMSGotOneWatcher", (char *)"(Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotOneWatcher");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotOneWatcher");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pWatcherURI), JNI_STR(env,pStatus));
}
void java_FireIMSWatcherSubscribeMe(LPCTSTR pWatcherURI, LPCTSTR pDisplayName, LPCTSTR pInfo, LPCTSTR pStatus)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSWatcherSubscribeMe pWatcherURI[%s] pDisplayName[%s] pInfo[%s] pStatus[%s]", pWatcherURI, pDisplayName, pInfo, pStatus);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSWatcherSubscribeMe pWatcherURI[%s] pDisplayName[%s] pInfo[%s] pStatus[%s]", pWatcherURI, pDisplayName, pInfo, pStatus);

    iresult = getStaticJniMethod((char *)"java_FireIMSWatcherSubscribeMe", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSWatcherSubscribeMe");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSWatcherSubscribeMe");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pWatcherURI), JNI_STR(env,pDisplayName), JNI_STR(env,pInfo), JNI_STR(env,pStatus));
}
void java_FireGetXCAPCapsInfo(short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    XCAP_CAPS_INFO *pResult = (XCAP_CAPS_INFO *)pNotify;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireGetXCAPCapsInfo");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireGetXCAPCapsInfo");

    iresult = getStaticJniMethod((char *)"java_FireGetXCAPCapsInfo", (char *)"(Lcom/zte/softda/ocx/FireGetXCAPCapsInfoPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireGetXCAPCapsInfo");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireGetXCAPCapsInfo");

        return;
    }

    jclass class_GetXCAPCapsInfo = env->FindClass("com/zte/softda/ocx/FireGetXCAPCapsInfoPara");
    jmethodID id_GetXCAPCapsInfo  = env->GetMethodID(class_GetXCAPCapsInfo, "<init>", "()V");
    jobject obj_GetXCAPCapsInfo = env->NewObject(class_GetXCAPCapsInfo, id_GetXCAPCapsInfo);

    jstring cMcu = env->NewStringUTF(pResult->cMcu);
    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "cMcu", "Ljava/lang/String;");
    env->SetObjectField(obj_GetXCAPCapsInfo, id_fieldID, cMcu);
    env->DeleteLocalRef(cMcu);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_IMS_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_IMS_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_SMS_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_SMS_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_MMS_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_MMS_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_FAX_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_FAX_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_CALL_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_CALL_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_MMC_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_MMC_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_Email_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_Email_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_Schedule_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_Schedule_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_Notice_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_Notice_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "bAUID_Netdisk_PIM", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->bAUID_Netdisk_PIM);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "nIMSendCaps", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->nIMSendCaps);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "nSMSSendCaps", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->nSMSSendCaps);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "nAutoGetMyLocation", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->nAutoGetMyLocation);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "nNeedSet", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->nNeedSet);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "nValDays", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->nValDays);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "nDays", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->nDays);

    jstring cDateTime = env->NewStringUTF(pResult->cDateTime);
    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "cDateTime", "Ljava/lang/String;");
    env->SetObjectField(obj_GetXCAPCapsInfo, id_fieldID, cDateTime);
    env->DeleteLocalRef(cDateTime);

    jstring cPwdDateTime = env->NewStringUTF(pResult->cPwdDateTime);
    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "cPwdDateTime", "Ljava/lang/String;");
    env->SetObjectField(obj_GetXCAPCapsInfo, id_fieldID, cPwdDateTime);
    env->DeleteLocalRef(cPwdDateTime);

    jstring cPwdNotify1 = env->NewStringUTF(pResult->cPwdNotify1);
    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "cPwdNotify1", "Ljava/lang/String;");
    env->SetObjectField(obj_GetXCAPCapsInfo, id_fieldID, cPwdNotify1);
    env->DeleteLocalRef(cPwdNotify1);

    jstring cPwdNotify2 = env->NewStringUTF(pResult->cPwdNotify2);
    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "cPwdNotify2", "Ljava/lang/String;");
    env->SetObjectField(obj_GetXCAPCapsInfo, id_fieldID, cPwdNotify2);
    env->DeleteLocalRef(cPwdNotify2);

    jstring cPwdNotify3 = env->NewStringUTF(pResult->cPwdNotify3);
    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "cPwdNotify3", "Ljava/lang/String;");
    env->SetObjectField(obj_GetXCAPCapsInfo, id_fieldID, cPwdNotify3);
    env->DeleteLocalRef(cPwdNotify3);

    jstring cUserCaps = env->NewStringUTF(pResult->cUserCaps);
    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "cUserCaps", "Ljava/lang/String;");
    env->SetObjectField(obj_GetXCAPCapsInfo, id_fieldID, cUserCaps);
    env->DeleteLocalRef(cUserCaps);

    id_fieldID = env->GetFieldID(class_GetXCAPCapsInfo, "iContact_Rls_Sync", "J");
    env->SetLongField(obj_GetXCAPCapsInfo, id_fieldID, (jlong)pResult->iContact_Rls_Sync);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_GetXCAPCapsInfo);
}
void java_FireIMSGotUserInfo(short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    USER_INFO *pResult = (USER_INFO *)pNotify;
    int i;
    long lDataLen = 0; //��ͷ��Data����

    TraceMsgWindow1(1, "rcscontroller java_FireIMSGotUserInfo begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSGotUserInfo", (char *)"(Lcom/zte/softda/ocx/FireIMSGotUserInfoPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotUserInfo");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotUserInfo");

        return;
    }

    jclass class_IMSGotUserInfo = env->FindClass("com/zte/softda/ocx/FireIMSGotUserInfoPara");
    jmethodID id_IMSGotUserInfo  = env->GetMethodID(class_IMSGotUserInfo, "<init>", "()V");
    jobject obj_IMSGotUserInfo = env->NewObject(class_IMSGotUserInfo, id_IMSGotUserInfo);

    jstring cName = env->NewStringUTF(pResult->cName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cName);
    env->DeleteLocalRef(cName);

    jstring cSex = env->NewStringUTF(pResult->cSex);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSex", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSex);
    env->DeleteLocalRef(cSex);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iOld", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iOld);

    jstring cBirthday = env->NewStringUTF(pResult->cBirthday);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cBirthday", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cBirthday);
    env->DeleteLocalRef(cBirthday);

    jstring cInterest = env->NewStringUTF(pResult->cInterest);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cInterest", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cInterest);
    env->DeleteLocalRef(cInterest);

    jstring cOccupation = env->NewStringUTF(pResult->cOccupation);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOccupation", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOccupation);
    env->DeleteLocalRef(cOccupation);

    jstring cCountry = env->NewStringUTF(pResult->cCountry);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCountry", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCountry);
    env->DeleteLocalRef(cCountry);

    jstring cZone = env->NewStringUTF(pResult->cZone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cZone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cZone);
    env->DeleteLocalRef(cZone);

    jstring cCity = env->NewStringUTF(pResult->cCity);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCity", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCity);
    env->DeleteLocalRef(cCity);

    jstring cUcNum = env->NewStringUTF(pResult->cUcNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cUcNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cUcNum);
    env->DeleteLocalRef(cUcNum);

    jstring cPocNum = env->NewStringUTF(pResult->cPocNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPocNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPocNum);
    env->DeleteLocalRef(cPocNum);

    jstring cMessageNum = env->NewStringUTF(pResult->cMessageNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMessageNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMessageNum);
    env->DeleteLocalRef(cMessageNum);

    jstring cFaxNum = env->NewStringUTF(pResult->cFaxNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxNum);
    env->DeleteLocalRef(cFaxNum);

    jstring cHomeTel = env->NewStringUTF(pResult->cHomeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cHomeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cHomeTel);
    env->DeleteLocalRef(cHomeTel);

    jstring cOfficeTel = env->NewStringUTF(pResult->cOfficeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOfficeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOfficeTel);
    env->DeleteLocalRef(cOfficeTel);

    jstring cMobileTel = env->NewStringUTF(pResult->cMobileTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMobileTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMobileTel);
    env->DeleteLocalRef(cMobileTel);

    jstring cAutoTel = env->NewStringUTF(pResult->cAutoTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAutoTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAutoTel);
    env->DeleteLocalRef(cAutoTel);

    jstring cSHomeTel = env->NewStringUTF(pResult->cSHomeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSHomeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSHomeTel);
    env->DeleteLocalRef(cSHomeTel);

    jstring cSOfficeTel = env->NewStringUTF(pResult->cSOfficeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSOfficeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSOfficeTel);
    env->DeleteLocalRef(cSOfficeTel);

    jstring cSMobileTel = env->NewStringUTF(pResult->cSMobileTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSMobileTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSMobileTel);
    env->DeleteLocalRef(cSMobileTel);

    jstring cHomeAddr = env->NewStringUTF(pResult->cHomeAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cHomeAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cHomeAddr);
    env->DeleteLocalRef(cHomeAddr);

    jstring cOfficeAddr = env->NewStringUTF(pResult->cOfficeAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOfficeAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOfficeAddr);
    env->DeleteLocalRef(cOfficeAddr);

    jstring cOtherAddr = env->NewStringUTF(pResult->cOtherAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOtherAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOtherAddr);
    env->DeleteLocalRef(cOtherAddr);

    jstring cEmailAddr = env->NewStringUTF(pResult->cEmailAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cEmailAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cEmailAddr);
    env->DeleteLocalRef(cEmailAddr);

    jstring cDomain = env->NewStringUTF(pResult->cDomain);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDomain", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDomain);
    env->DeleteLocalRef(cDomain);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iNotifyMode", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iNotifyMode);

    jstring cURI = env->NewStringUTF(pResult->cURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cURI);
    env->DeleteLocalRef(cURI);

    jstring cPhotoIndex = env->NewStringUTF(pResult->cPhotoIndex);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoIndex", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoIndex);
    env->DeleteLocalRef(cPhotoIndex);

//	id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoFile", "[B");//cPhotoFile
//	lDataLen = strlen(pResult->cPhotoFile);
//	jbyteArray ContentArray= env->NewByteArray(lDataLen);
//	env->SetByteArrayRegion(ContentArray, 0, lDataLen, (const jbyte*)pResult->cPhotoFile);
//	env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, ContentArray);
    jstring cPhotoFile = env->NewStringUTF(pResult->cPhotoFile);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoFile", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoFile);
    env->DeleteLocalRef(cPhotoFile);
    if(pResult->cPhotoFile)
    {
        ZX_free(pResult->cPhotoFile);//���������ͷ�(�ڽ����ʱ�������ڴ�)
        pResult->cPhotoFile = NULL;
    }    

    jstring cSoftPhone = env->NewStringUTF(pResult->cSoftPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSoftPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSoftPhone);
    env->DeleteLocalRef(cSoftPhone);

//������ҵ��Ϣ
    jstring cAffiliation = env->NewStringUTF(pResult->cAffiliation);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAffiliation", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAffiliation);
    env->DeleteLocalRef(cAffiliation);

    jstring cDepartment = env->NewStringUTF(pResult->cDepartment);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartment", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartment);
    env->DeleteLocalRef(cDepartment);

    jstring cDepartmentName = env->NewStringUTF(pResult->cDepartmentName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartmentName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartmentName);
    env->DeleteLocalRef(cDepartmentName);

    jstring cDepartmentURI = env->NewStringUTF(pResult->cDepartmentURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartmentURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartmentURI);
    env->DeleteLocalRef(cDepartmentURI);

    jstring cEmployeeNum = env->NewStringUTF(pResult->cEmployeeNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cEmployeeNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cEmployeeNum);
    env->DeleteLocalRef(cEmployeeNum);

    jstring cPersonEmailAddr = env->NewStringUTF(pResult->cPersonEmailAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPersonEmailAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPersonEmailAddr);
    env->DeleteLocalRef(cPersonEmailAddr);

    jstring cPosition = env->NewStringUTF(pResult->cPosition);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPosition", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPosition);
    env->DeleteLocalRef(cPosition);

    jstring cPhotoURI = env->NewStringUTF(pResult->cPhotoURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoURI);
    env->DeleteLocalRef(cPhotoURI);

    jstring cURI2 = env->NewStringUTF(pResult->cURI2);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cURI2", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cURI2);
    env->DeleteLocalRef(cURI2);

//ecp����
    jstring cBusinessPhone = env->NewStringUTF(pResult->cBusinessPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cBusinessPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cBusinessPhone);
    env->DeleteLocalRef(cBusinessPhone);

    jstring cSmsPhone = env->NewStringUTF(pResult->cSmsPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSmsPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSmsPhone);
    env->DeleteLocalRef(cSmsPhone);

    jstring cSmsBusiness = env->NewStringUTF(pResult->cSmsBusiness);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSmsBusiness", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSmsBusiness);
    env->DeleteLocalRef(cSmsBusiness);

    jstring cFaxBusiness = env->NewStringUTF(pResult->cFaxBusiness);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxBusiness", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxBusiness);
    env->DeleteLocalRef(cFaxBusiness);

    jstring cFaxExt = env->NewStringUTF(pResult->cFaxExt);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxExt", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxExt);
    env->DeleteLocalRef(cFaxExt);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iVersionType", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iVersionType);

    jstring cRealName = env->NewStringUTF(pResult->cRealName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cRealName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cRealName);
    env->DeleteLocalRef(cRealName);

    jstring cOtherTel = env->NewStringUTF(pResult->cOtherTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOtherTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOtherTel);
    env->DeleteLocalRef(cOtherTel);

    jstring cFullSpelling = env->NewStringUTF(pResult->cFullSpelling);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFullSpelling", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFullSpelling);
    env->DeleteLocalRef(cFullSpelling);

    jstring cShortSpelling = env->NewStringUTF(pResult->cShortSpelling);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cShortSpelling", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cShortSpelling);
    env->DeleteLocalRef(cShortSpelling);

    jstring cFullSpelling1 = env->NewStringUTF(pResult->cFullSpelling1);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFullSpelling1", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFullSpelling1);
    env->DeleteLocalRef(cFullSpelling1);

    jstring cShortSpelling1 = env->NewStringUTF(pResult->cShortSpelling1);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cShortSpelling1", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cShortSpelling1);
    env->DeleteLocalRef(cShortSpelling1);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iAreaCodeFlag", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iAreaCodeFlag);

    jstring cCompanyUri = env->NewStringUTF(pResult->cCompanyUri);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCompanyUri", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCompanyUri);
    env->DeleteLocalRef(cCompanyUri);

    jstring cUserCapacity = env->NewStringUTF(pResult->cUserCapacity);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cUserCapacity", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, env->NewStringUTF(pResult->cUserCapacity));
    env->DeleteLocalRef(cUserCapacity);

    jstring cAnswerMode = env->NewStringUTF(pResult->cAnswerMode);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAnswerMode", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAnswerMode);
    env->DeleteLocalRef(cAnswerMode);

    jstring cMsgDeliverReport = env->NewStringUTF(pResult->cMsgDeliverReport);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMsgDeliverReport", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, env->NewStringUTF(pResult->cMsgDeliverReport));
    env->DeleteLocalRef(cMsgDeliverReport);

    jstring cOnlyNotify = env->NewStringUTF(pResult->cOnlyNotify);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOnlyNotify", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOnlyNotify);
    env->DeleteLocalRef(cOnlyNotify);

    jstring cAreaCode = env->NewStringUTF(pResult->cAreaCode);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAreaCode", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAreaCode);
    env->DeleteLocalRef(cAreaCode);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iMaxGroupNum", "I");
    env->SetIntField(obj_IMSGotUserInfo, id_fieldID, pResult->iMaxGroupNum);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "IsRecieptEnabled", "I");
    env->SetIntField(obj_IMSGotUserInfo, id_fieldID, pResult->IsRecieptEnabled);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "IsWatermarkEnabled", "I");
    env->SetIntField(obj_IMSGotUserInfo, id_fieldID, pResult->IsWatermarkEnabled);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "IsTransfileEnabled", "I");
    env->SetIntField(obj_IMSGotUserInfo, id_fieldID, pResult->IsTransfileEnabled);

    jstring capabilitySet = env->NewStringUTF(pResult->capabilitySet);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "capabilitySet", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, capabilitySet);
    env->DeleteLocalRef(capabilitySet);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_IMSGotUserInfo);

    TraceMsgWindow1(1, "rcscontroller java_FireIMSGotUserInfo end");
}
void java_FireIMSGotGroupInfo(int index, int count, short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    FIRE_GROUP_INFO *pGroupInfo = (FIRE_GROUP_INFO *)pNotify;
    int i;
//  FIRE_ALL_GROUP* pTemp = NULL;

    TraceMsgWindow1(1, "rcscontroller java_FireIMSGotGroupInfo begin, totalNum=%d, index=%d count=%d", pGroupInfo->iNum, index, count);

    iresult = getStaticJniMethod((char *)"java_FireIMSGotGroupInfo", (char *)"(ILcom/zte/softda/ocx/FireIMSGotGroupInfoPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotGroupInfo");
        return;
    }

    jclass class_GroupInfo = env->FindClass("com/zte/softda/ocx/FireIMSGotGroupInfoPara");
    jmethodID id_GroupInfo  = env->GetMethodID(class_GroupInfo, "<init>", "()V");
    jobject obj_GroupInfo = env->NewObject(class_GroupInfo, id_GroupInfo);

    id_fieldID = env->GetFieldID(class_GroupInfo, "iNum", "I");
//    env->SetIntField(obj_GroupInfo, id_fieldID, pGroupInfo->iNum);
    env->SetIntField(obj_GroupInfo, id_fieldID, count); //��Ϊ�����ϱ�

    jclass class_FireAllGroup = env->FindClass("com/zte/softda/ocx/FireAllGroupPara");
    jmethodID id_FireAllGroup  = env->GetMethodID(class_FireAllGroup, "<init>", "()V");
//    jobjectArray array_group = env->NewObjectArray(pGroupInfo->iNum, class_FireAllGroup, NULL);
    jobjectArray array_group = env->NewObjectArray(count, class_FireAllGroup, NULL); //��Ϊ�����ϱ�
    for(i = index; i < index + count; i++)
    {
        FIRE_ALL_GROUP* pFireGroupInfo = &pGroupInfo->pFireGroupInfo[i];

        jobject obj_FireGroupInfo = env->NewObject(class_FireAllGroup, id_FireAllGroup);

        jstring cGroupURI = env->NewStringUTF(pFireGroupInfo->cGroupURI);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cGroupURI", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cGroupURI);
        env->DeleteLocalRef(cGroupURI);

        jstring cGroupName = env->NewStringUTF(pFireGroupInfo->cGroupName);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cGroupName", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cGroupName);
        env->DeleteLocalRef(cGroupName);

        id_fieldID = env->GetFieldID(class_FireAllGroup, "iGroupType", "I");
        env->SetIntField(obj_FireGroupInfo, id_fieldID, pFireGroupInfo->iGroupType);

        id_fieldID = env->GetFieldID(class_FireAllGroup, "iGroupUsage", "I");
        env->SetIntField(obj_FireGroupInfo, id_fieldID, pFireGroupInfo->iGroupUsage);

        jstring cOwnerURI = env->NewStringUTF(pFireGroupInfo->cOwnerURI);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cOwnerURI", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cOwnerURI);
        env->DeleteLocalRef(cOwnerURI);

        jstring cType = env->NewStringUTF(pFireGroupInfo->cType);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cType", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cType);
        env->DeleteLocalRef(cType);

        id_fieldID = env->GetFieldID(class_FireAllGroup, "iMax", "I");
        env->SetIntField(obj_FireGroupInfo, id_fieldID, pFireGroupInfo->iMax);

        env->SetObjectArrayElement(array_group, i-index, obj_FireGroupInfo);

        env->DeleteLocalRef(obj_FireGroupInfo);
    }
    id_fieldID = env->GetFieldID(class_GroupInfo, "FireGroupInfoArray", "[Lcom/zte/softda/ocx/FireAllGroupPara;");
    env->SetObjectField(obj_GroupInfo, id_fieldID, array_group);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, index, obj_GroupInfo);
    TraceMsgWindow1(1, "rcscontroller java_FireIMSGotGroupInfo over");
}
void java_FireIMSPsEvent(long iType, short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    PS_PRENSENCE_EVENT_PARA *pResult = NULL;
    pResult = (PS_PRENSENCE_EVENT_PARA *)pNotify;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSPsEvent iType[%d]", iType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSPsEvent iType[%d]", iType);

    iresult = getStaticJniMethod((char *)"java_FireIMSPsEvent", (char *)"(ILcom/zte/softda/ocx/FireIMSPsEventPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSPsEvent");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSPsEvent");

        return;
    }
    if(pResult != NULL)
    {
        jclass class_IMSPsEvent = env->FindClass("com/zte/softda/ocx/FireIMSPsEventPara");
        jmethodID id_IMSPsEvent  = env->GetMethodID(class_IMSPsEvent, "<init>", "()V");
        jobject obj_IMSPsEvent = env->NewObject(class_IMSPsEvent, id_IMSPsEvent);

        jstring cURI = env->NewStringUTF(pResult->cURI);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cURI);
        env->DeleteLocalRef(cURI);

        jstring cNote = env->NewStringUTF(pResult->cNote);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cNote", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cNote);
        env->DeleteLocalRef(cNote);

        jstring cLocation = env->NewStringUTF(pResult->cLocation);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cLocation", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cLocation);
        env->DeleteLocalRef(cLocation);

        jstring cCamera = env->NewStringUTF(pResult->cCamera);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cCamera", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cCamera);
        env->DeleteLocalRef(cCamera);

        jstring cMood = env->NewStringUTF(pResult->cMood);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cMood", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cMood);
        env->DeleteLocalRef(cMood);

        jstring cJob = env->NewStringUTF(pResult->cJob);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cJob", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cJob);
        env->DeleteLocalRef(cJob);

        jstring cPhoneStatus = env->NewStringUTF(pResult->cPhoneStatus);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cPhoneStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cPhoneStatus);
        env->DeleteLocalRef(cPhoneStatus);

        jstring CTerminalType = env->NewStringUTF(pResult->CTerminalType);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "CTerminalType", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, CTerminalType);
        env->DeleteLocalRef(CTerminalType);

        jstring COsType = env->NewStringUTF(pResult->COsType);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "COsType", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, COsType);
        env->DeleteLocalRef(COsType);

        jstring cPhone = env->NewStringUTF(pResult->cPhone);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cPhone);
        env->DeleteLocalRef(cPhone);

        jstring cSipFromURI = env->NewStringUTF(pResult->cSipFromURI);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cSipFromURI", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cSipFromURI);
        env->DeleteLocalRef(cSipFromURI);

        id_fieldID = env->GetFieldID(class_IMSPsEvent, "iTimestamp", "I");
        env->SetIntField(obj_IMSPsEvent, id_fieldID, pResult->iTimestamp);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_IMSPsEvent);
    }
}
void java_FireIMSBatchPsEvent(int iNum, int iCode, short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    PS_PRENSENCE_EVENT_PARA *pResult = NULL;
    pResult = (PS_PRENSENCE_EVENT_PARA *)pNotify;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSBatchPsEvent iNum[%d] iCode[%d]", iNum, iCode);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSBatchPsEvent iNum[%d] iCode[%d]", iNum, iCode);

    iresult = getStaticJniMethod((char *)"java_FireIMSBatchPsEvent", (char *)"(II[Lcom/zte/softda/ocx/FireIMSPsEventPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSBatchPsEvent");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSBatchPsEvent");

        return;
    }

    jclass class_IMSPsEvent = env->FindClass("com/zte/softda/ocx/FireIMSPsEventPara");
    jmethodID id_IMSPsEvent  = env->GetMethodID(class_IMSPsEvent, "<init>", "()V");
    jobjectArray array_IMSPsEvent = env->NewObjectArray(iNum, class_IMSPsEvent, NULL);
    for(i = 0; i < iNum; i++)
    {
        PS_PRENSENCE_EVENT_PARA *pServcie = (PS_PRENSENCE_EVENT_PARA *) & (pResult[i]);
        jobject obj_IMSPsEvent = env->NewObject(class_IMSPsEvent, id_IMSPsEvent);

        jstring cURI = env->NewStringUTF(pServcie->cURI);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cURI);
        env->DeleteLocalRef(cURI);

        jstring cNote = env->NewStringUTF(pServcie->cNote);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cNote", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cNote);
        env->DeleteLocalRef(cNote);

        jstring cLocation = env->NewStringUTF(pServcie->cLocation);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cLocation", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cLocation);
        env->DeleteLocalRef(cLocation);

        jstring cCamera = env->NewStringUTF(pServcie->cCamera);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cCamera", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cCamera);
        env->DeleteLocalRef(cCamera);

        jstring cMood = env->NewStringUTF(pServcie->cMood);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cMood", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cMood);
        env->DeleteLocalRef(cMood);

        jstring cJob = env->NewStringUTF(pServcie->cJob);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cJob", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cJob);
        env->DeleteLocalRef(cJob);

        jstring cPhoneStatus = env->NewStringUTF(pServcie->cPhoneStatus);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cPhoneStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cPhoneStatus);
        env->DeleteLocalRef(cPhoneStatus);

        jstring CTerminalType = env->NewStringUTF(pServcie->CTerminalType);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "CTerminalType", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, CTerminalType);
        env->DeleteLocalRef(CTerminalType);

        jstring COsType = env->NewStringUTF(pServcie->COsType);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "COsType", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, COsType);
        env->DeleteLocalRef(COsType);

        jstring cPhone = env->NewStringUTF(pServcie->cPhone);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cPhone);
        env->DeleteLocalRef(cPhone);

        jstring cSipFromURI = env->NewStringUTF(pServcie->cSipFromURI);
        id_fieldID = env->GetFieldID(class_IMSPsEvent, "cSipFromURI", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPsEvent, id_fieldID, cSipFromURI);
        env->DeleteLocalRef(cSipFromURI);

        id_fieldID = env->GetFieldID(class_IMSPsEvent, "iTimestamp", "I");
        env->SetIntField(obj_IMSPsEvent, id_fieldID, pServcie->iTimestamp);

        env->SetObjectArrayElement(array_IMSPsEvent, i, obj_IMSPsEvent);
        env->DeleteLocalRef(obj_IMSPsEvent);
    }
    if(pResult != NULL)
        ZX_free(pResult);

    if(iNum > 0)
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iNum, iCode, array_IMSPsEvent);
}
//void java_FireIMSGotAddressList(LPCTSTR pURI, LPCTSTR pDisplayName, short far* pNotify)
void java_FireIMSGotAddressList(int iNum, short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    IMSGotAddressList_T *pResult = NULL;
    pResult = (IMSGotAddressList_T *)pNotify;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotAddressList iNum[%d]", iNum);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotAddressList iNum[%d]", iNum);

    iresult = getStaticJniMethod((char *)"java_FireIMSGotAddressList", (char *)"(I[Lcom/zte/softda/ocx/FireIMSGotAddressListPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotAddressList");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotAddressList");

        return;
    }

    if(iNum <= 0)
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iNum, NULL);
        return;
    }

    jclass class_IMSGotAddressList = env->FindClass("com/zte/softda/ocx/FireIMSGotAddressListPara");
    jmethodID id_IMSGotAddressList  = env->GetMethodID(class_IMSGotAddressList, "<init>", "()V");
    jobjectArray array_IMSGotAddressList = env->NewObjectArray(iNum, class_IMSGotAddressList, NULL);
    for(i = 0; i < iNum; i++)
    {
        IMSGotAddressList_T *pServcie = (IMSGotAddressList_T *) & (pResult[i]);
        jobject obj_IMSGotAddressList = env->NewObject(class_IMSGotAddressList, id_IMSGotAddressList);

        jstring pURI = env->NewStringUTF(pServcie->pURI);
        id_fieldID = env->GetFieldID(class_IMSGotAddressList, "pURI", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSGotAddressList, id_fieldID, pURI);
        env->DeleteLocalRef(pURI);

        jstring cRealName = env->NewStringUTF(pServcie->sListInfo.cRealName);
        id_fieldID = env->GetFieldID(class_IMSGotAddressList, "cRealName", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSGotAddressList, id_fieldID, cRealName);
        env->DeleteLocalRef(cRealName);

        jstring cName = env->NewStringUTF(pServcie->sListInfo.cName);
        id_fieldID = env->GetFieldID(class_IMSGotAddressList, "cName", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSGotAddressList, id_fieldID, cName);
        env->DeleteLocalRef(cName);

        jstring cDisplayName = env->NewStringUTF(pServcie->pDisplayName);
        id_fieldID = env->GetFieldID(class_IMSGotAddressList, "cDisplayName", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSGotAddressList, id_fieldID, cDisplayName);
        env->DeleteLocalRef(cDisplayName);

        if(g_WriteLogType == 2)//2013.06.05��Ӳ�����־���ڶ�λ����
            TraceMsgWindow1(1, "[java_FireIMSGotAddressList] pURI[%s] cRealName[%s] cName[%s] cDisplayName[%s]", pServcie->pURI, pServcie->sListInfo.cRealName, pServcie->sListInfo.cName, pServcie->pDisplayName);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "[java_FireIMSGotAddressList] pURI[%s] cRealName[%s] cName[%s] cDisplayName[%s]", pServcie->pURI, pServcie->sListInfo.cRealName, pServcie->sListInfo.cName, pServcie->pDisplayName);

        env->SetObjectArrayElement(array_IMSGotAddressList, i, obj_IMSGotAddressList);
        env->DeleteLocalRef(obj_IMSGotAddressList);
    }
    if(pResult != NULL)
        ZX_free(pResult);

/*  if(iNum == 0)
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iNum, NULL);
    }
    else*/
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iNum, array_IMSGotAddressList);
    }
}

static jboolean jni_bLogonSS2(JNIEnv* env, jclass cls, jstring lpConfuri, jstring lpUserName, jstring lpPassword, jstring lpServerIP, jstring lServerPort)
{
//	char cConfuri[MAX_IMS_URL_LEN]= {0};
//  char cUserName[MAX_IMS_URL_LEN]= {0};
//	char cPassword[MAX_PASSWORD_LEN] = {0};
//	char cServerIP[MAX_IMS_URL_LEN] = {0};
//	char iServerPort[10] = {0};
	
	char cConfuri[256+1]= {0};
    char cUserName[256+1]= {0};
	char cPassword[256+1] = {0};
	char cServerIP[128+1] = {0};
	char iServerPort[128+1] = {0};
	     
	safeGetStringUTFChars(env, lpConfuri, cConfuri, sizeof(cConfuri)-1,NULL);		 
	safeGetStringUTFChars(env, lpUserName, cUserName, sizeof(cUserName)-1,NULL);		
	safeGetStringUTFChars(env, lpPassword, cPassword, sizeof(cPassword)-1,NULL);		
	safeGetStringUTFChars(env, lpServerIP, cServerIP, sizeof(cServerIP)-1,NULL);		
	safeGetStringUTFChars(env, lServerPort, iServerPort, sizeof(iServerPort)-1,NULL);		
	     
	LOGI(TAG,"[jni_bLogonSS2()] cConfuri[%s] cUserName[%s] cPassword[%s] cServerIP[%s] iServerPort[%s]\n", 
		cConfuri, cUserName, cPassword, cServerIP, iServerPort);	     
	
    return bLogonSS2(cConfuri, cUserName, cPassword, cServerIP, iServerPort);
}
void java_FireIMSGotOneRLS(int iResult, LPCTSTR pURI, LPCTSTR pGroupName, LPCTSTR pListName, int iListNum, short far* pListMemberURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    char* ListMemberURI = NULL;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOneRLS");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotOneRLS");

    ListMemberURI = (char*)pListMemberURI;

    iresult = getStaticJniMethod((char *)"java_FireIMSGotOneRLS", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotOneRLS");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotOneRLS");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, JNI_STR(env,pURI), JNI_STR(env,pGroupName), JNI_STR(env,pListName), iListNum, JNI_STR(env,ListMemberURI));
}
void java_FireIMSGotMyPresenceRule(long iType, long iNum, short far* pURIList)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    char* cURIList = NULL;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotMyPresenceRule");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotMyPresenceRule");

    cURIList = (char*)pURIList;

    iresult = getStaticJniMethod((char *)"java_FireIMSGotMyPresenceRule", (char *)"(IILjava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotMyPresenceRule");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotMyPresenceRule");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum, JNI_STR(env,cURIList));
}
void java_FireIMSSendMessageReport(LPCTSTR pReportID, LPCTSTR pReportType, LPCTSTR pDoneTime, LPCTSTR pErrorCode, LPCTSTR pTerminal)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSendMessageReport pReportID[%s] pReportType[%s] pDoneTime[%s] pErrorCode[%s] pTerminal[%s]",
                        pReportID, pReportType, pDoneTime, pErrorCode, pTerminal);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSendMessageReport pReportID[%s] pReportType[%s] pDoneTime[%s] pErrorCode[%s] pTerminal[%s]", pReportID, pReportType, pDoneTime, pErrorCode, pTerminal);

    iresult = getStaticJniMethod((char *)"java_FireIMSSendMessageReport", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSendMessageReport");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSendMessageReport");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pReportID), JNI_STR(env,pReportType), JNI_STR(env,pDoneTime), JNI_STR(env,pErrorCode), JNI_STR(env,pTerminal));
}
void java_FireIMSListPresenceArrived(LPCTSTR pSipFromURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone, long iTimestamp)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSListPresenceArrived");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSListPresenceArrived begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSListPresenceArrived", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSListPresenceArrived");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSListPresenceArrived");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSipFromURI), JNI_STR(env,pListURI), JNI_STR(env,pStatus), JNI_STR(env,pPhone), iTimestamp);
}
long java_FireFIndFileSize(LPCTSTR pFilePath)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    long lFileSie = 0;
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    if(gs_jvm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller ERROR: GetEnv failed");
        else if(g_WriteLogType == 1)
            LOGE(TAG, "ERROR: GetEnv failed");

        return 0;
    }
    env = uenv.env;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireFIndFileSize pFilePath[%s]", pFilePath);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireFIndFileSize pFilePath[%s] ", pFilePath);

    jclass  JniClass = env->FindClass(classPathName_postagent);
    gJinMethod = env->GetStaticMethodID(JniClass, "java_FireFIndFileSize", "(Ljava/lang/String;)J");

    lFileSie = (long)env->CallStaticLongMethod(gpostClass, gJinMethod, JNI_STR(env,pFilePath));

    return lFileSie;
}
void java_FireIMSGetPublicGroupInfoListFailed(LPCTSTR lpGroupURI, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGetPublicGroupInfoListFailed lpGroupURI[%s] iResult[%d]", lpGroupURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGetPublicGroupInfoListFailed lpGroupURI[%s] iResult[%d]", lpGroupURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSGetPublicGroupInfoListFailed", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGetPublicGroupInfoListFailed");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGetPublicGroupInfoListFailed");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,lpGroupURI), iResult);
}
void java_FireIMSGotOnePublicGroup(int flag, LPCTSTR pGroupName, LPCTSTR pGroupURI, LPCTSTR pOperUser, long iMyRole, short far* pAttribute, long iMemberNum, void **pMemberList, long iBlacklistNum, short far* pBlacklist)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    ATTRIBUTE_TYPE *pAttr = NULL;
    pAttr = (ATTRIBUTE_TYPE *)pAttribute;
    int i;

    TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOnePublicGroup begin, flag[%d] pGroupURI[%s] iMemberNum[%d]", flag, pGroupURI, iMemberNum);
    if(iMemberNum > MAX_GROUP_MEMBER_NUM)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOnePublicGroup iMemberNum[%d] is abnormal, MAX_GROUP_MEMBER_NUM[%d]", iMemberNum, MAX_GROUP_MEMBER_NUM);
        return;
    }
        
    iresult = getStaticJniMethod((char *)"java_FireIMSGotOnePublicGroup", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILcom/zte/softda/ocx/FireAttributeTypePara;I[Lcom/zte/softda/ocx/EntryTypeArray;I[Lcom/zte/softda/ocx/BlackListArray;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotOnePublicGroup");
        return;
    }
    if(pAttr == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOnePublicGroup pAttr == NULL");
        return;
    }
    if(iMemberNum > 0 && (pMemberList == NULL))
    {
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOnePublicGroup pMembers == NULL, iMemberNum=%d", iMemberNum);
        return;
    }

    jclass class_AttributeType = env->FindClass("com/zte/softda/ocx/FireAttributeTypePara");
    jmethodID id_AttributeType  = env->GetMethodID(class_AttributeType, "<init>", "()V");
    jobject obj_AttributeType = env->NewObject(class_AttributeType, id_AttributeType);

    jstring cOwner = env->NewStringUTF(pAttr->cOwner);
    id_fieldID = env->GetFieldID(class_AttributeType, "cOwner", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cOwner);
    env->DeleteLocalRef(cOwner);

    jstring cCreator = env->NewStringUTF(pAttr->cCreator);
    id_fieldID = env->GetFieldID(class_AttributeType, "cCreator", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cCreator);
    env->DeleteLocalRef(cCreator);

    jstring cTimeStamp = env->NewStringUTF(pAttr->cTimeStamp);
    id_fieldID = env->GetFieldID(class_AttributeType, "cTimeStamp", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cTimeStamp);
    env->DeleteLocalRef(cTimeStamp);

    id_fieldID = env->GetFieldID(class_AttributeType, "iDuration", "I");
    env->SetIntField(obj_AttributeType, id_fieldID, pAttr->iDuration);

    id_fieldID = env->GetFieldID(class_AttributeType, "iVisibitily", "I");
    env->SetIntField(obj_AttributeType, id_fieldID, pAttr->iVisibitily);

    id_fieldID = env->GetFieldID(class_AttributeType, "iNeedPermit", "I");
    env->SetIntField(obj_AttributeType, id_fieldID, pAttr->iNeedPermit);

    id_fieldID = env->GetFieldID(class_AttributeType, "iMax", "I");
    env->SetIntField(obj_AttributeType, id_fieldID, pAttr->iMax);

    id_fieldID = env->GetFieldID(class_AttributeType, "forbid_2D_join", "I");
    env->SetIntField(obj_AttributeType, id_fieldID, pAttr->forbid_2D_join);

    id_fieldID = env->GetFieldID(class_AttributeType, "member_invite_enable", "I");
    env->SetIntField(obj_AttributeType, id_fieldID, pAttr->member_invite_enable);

    jstring cSubject = env->NewStringUTF(pAttr->cSubject);
    id_fieldID = env->GetFieldID(class_AttributeType, "cSubject", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cSubject);
    env->DeleteLocalRef(cSubject);

    jstring cTopic = env->NewStringUTF(pAttr->cTopic);
    id_fieldID = env->GetFieldID(class_AttributeType, "cTopic", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cTopic);
    env->DeleteLocalRef(cTopic);

    jstring cType = env->NewStringUTF(pAttr->cType);
    id_fieldID = env->GetFieldID(class_AttributeType, "cType", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cType);
    env->DeleteLocalRef(cType);

    jstring cWebUrl = env->NewStringUTF(pAttr->cWebUrl);
    id_fieldID = env->GetFieldID(class_AttributeType, "cWebUrl", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cWebUrl);
    env->DeleteLocalRef(cWebUrl);

    jstring cGroupClassID = env->NewStringUTF(pAttr->cGroupClassID);
    id_fieldID = env->GetFieldID(class_AttributeType, "cGroupClassID", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cGroupClassID);
    env->DeleteLocalRef(cGroupClassID);

    jstring cIdentifier = env->NewStringUTF(pAttr->cIdentifier);
    id_fieldID = env->GetFieldID(class_AttributeType, "cIdentifier", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, cIdentifier);
    env->DeleteLocalRef(cIdentifier);

    jstring bulletinSetterUri = env->NewStringUTF(pAttr->bulletinSetterUri);
    id_fieldID = env->GetFieldID(class_AttributeType, "bulletinSetterUri", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, bulletinSetterUri);
    env->DeleteLocalRef(bulletinSetterUri);

    jstring bulletinSetterName = env->NewStringUTF(pAttr->bulletinSetterName);
    id_fieldID = env->GetFieldID(class_AttributeType, "bulletinSetterName", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, bulletinSetterName);
    env->DeleteLocalRef(bulletinSetterName);

    jstring bulletinSettime = env->NewStringUTF(pAttr->bulletinSettime);
    id_fieldID = env->GetFieldID(class_AttributeType, "bulletinSettime", "Ljava/lang/String;");
    env->SetObjectField(obj_AttributeType, id_fieldID, bulletinSettime);
    env->DeleteLocalRef(bulletinSettime);

    jclass class_EntryType = env->FindClass("com/zte/softda/ocx/EntryTypeArray");
    jmethodID id_EntryType  = env->GetMethodID(class_EntryType, "<init>", "()V");
    jobjectArray array_EntryType = env->NewObjectArray(iMemberNum, class_EntryType, NULL);
    int iNum = 0;
    for(i = 0; i < iMemberNum; i++)
    {
        ENTRY_TYPE *pMember = (ENTRY_TYPE *)pMemberList[i];

        if(pMember->iMemberStatus == 4)
        {//ɾ��״̬�Ĳ���UI
            continue;
        }
        
        jobject obj_EntryType = env->NewObject(class_EntryType, id_EntryType);

        jstring cDisplayName = env->NewStringUTF(pMember->cDisplayName);
        id_fieldID = env->GetFieldID(class_EntryType, "cDisplayName", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cDisplayName);
        env->DeleteLocalRef(cDisplayName);

        jstring cFullSpelling = env->NewStringUTF(pMember->cFullSpelling);
        id_fieldID = env->GetFieldID(class_EntryType, "cFullSpelling", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cFullSpelling);
        env->DeleteLocalRef(cFullSpelling);

        jstring cInfo = env->NewStringUTF(pMember->cInfo);
        id_fieldID = env->GetFieldID(class_EntryType, "cInfo", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cInfo);
        env->DeleteLocalRef(cInfo);

        jstring cURI = env->NewStringUTF(pMember->cURI);
        id_fieldID = env->GetFieldID(class_EntryType, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cURI);
        env->DeleteLocalRef(cURI);

        jstring cPhone = env->NewStringUTF(pMember->cPhone);
        id_fieldID = env->GetFieldID(class_EntryType, "cPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cPhone);
        env->DeleteLocalRef(cPhone);

        id_fieldID = env->GetFieldID(class_EntryType, "iMemberStatus", "I");
        env->SetIntField(obj_EntryType, id_fieldID, pMember->iMemberStatus);

        jstring cMemberType = env->NewStringUTF(pMember->cMemberType);
        id_fieldID = env->GetFieldID(class_EntryType, "cMemberType", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cMemberType);
        env->DeleteLocalRef(cMemberType);

        jstring cAllowModifyMyCaps = env->NewStringUTF(pMember->cAllowModifyMyCaps);
        id_fieldID = env->GetFieldID(class_EntryType, "cAllowModifyMyCaps", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cAllowModifyMyCaps);
        env->DeleteLocalRef(cAllowModifyMyCaps);

        env->SetObjectArrayElement(array_EntryType, iNum, obj_EntryType);
        env->DeleteLocalRef(obj_EntryType);
        iNum++; //��¼��ʵ�ĳ�Ա����
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, flag, JNI_STR(env,pGroupName), JNI_STR(env,pGroupURI), JNI_STR(env,pOperUser), iMyRole, obj_AttributeType,
                              iNum, array_EntryType, iBlacklistNum, NULL);
    
    TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOnePublicGroup end");
}
void java_FireIMSCreatePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, long iResult, long iGroupType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSCreatePublicGroupResult pGroupName[%s], pGroupURI[%s] iResult[%d] iGroupType[%d]",
                        pGroupName, pGroupURI, iResult, iGroupType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSCreatePublicGroupResult pGroupName[%s], pGroupURI[%s] iResult[%d] iGroupType[%d]",
             pGroupName, pGroupURI, iResult, iGroupType);

    iresult = getStaticJniMethod((char *)"java_FireIMSCreatePublicGroupResult", (char *)"(Ljava/lang/String;Ljava/lang/String;II)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSCreatePublicGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSCreatePublicGroupResult");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupName), JNI_STR(env,pGroupURI), iResult, iGroupType);
}
void java_FireIMSModifyPublicGroupResult(LPCTSTR pGroupURI, long iType, LPCTSTR pNewName, long iNewValue, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSModifyPublicGroupResult pGroupURI[%s] iType[%d] pNewName[%s] iNewValue[%d] iResult[%d]",
                        pGroupURI, iType, pNewName, iNewValue, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSModifyPublicGroupResult pGroupURI[%s] iType[%d] pNewName[%s] iNewValue[%d] iResult[%d]",
             pGroupURI, iType, pNewName, iNewValue, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSModifyPublicGroupResult", (char *)"(Ljava/lang/String;ILjava/lang/String;II)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSModifyPublicGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSModifyPublicGroupResult");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI), iType, JNI_STR(env,pNewName), iNewValue, iResult);
}
void java_FireIMSInviteToMyPuGroupSendResult(LPCTSTR pSomeoneURI, LPCTSTR pDisplayName, LPCTSTR pGroupURI, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSInviteToMyPuGroupSendResult pSomeoneURI[%s] pDisplayName[%s] pGroupURI[%s] iResult[%d]",
                        pSomeoneURI, pDisplayName, pGroupURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSInviteToMyPuGroupSendResult pSomeoneURI[%s] pDisplayName[%s] pGroupURI[%s] iResult[%d]",
             pSomeoneURI, pDisplayName, pGroupURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSInviteToMyPuGroupSendResult", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSInviteToMyPuGroupSendResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSInviteToMyPuGroupSendResult");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSomeoneURI), JNI_STR(env,pDisplayName), JNI_STR(env,pGroupURI), iResult);
}
void java_FireIMSPGroupListPresenceArrived(LPCTSTR pPublicGroupURI, LPCTSTR pListURI, LPCTSTR pStatus, LPCTSTR pPhone)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSPGroupListPresenceArrived");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSPGroupListPresenceArrived begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSPGroupListPresenceArrived", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSPGroupListPresenceArrived");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSPGroupListPresenceArrived");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pPublicGroupURI), JNI_STR(env,pListURI), JNI_STR(env,pStatus), JNI_STR(env,pPhone));
}
void java_FirePublicGroupAttributeChanged(LPCTSTR pURI, long iType, LPCTSTR pName, long iValue)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FirePublicGroupAttributeChanged begin pURI[%s], iType[%d] pName[%s] iValue[%d]", pURI, iType, pName, iValue);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FirePublicGroupAttributeChanged begin pURI[%s], iType[%d] pName[%s] iValue[%d]", pURI, iType, pName, iValue);

    iresult = getStaticJniMethod((char *)"java_FirePublicGroupAttributeChanged", (char *)"(Ljava/lang/String;ILjava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FirePublicGroupAttributeChanged");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FirePublicGroupAttributeChanged");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), iType, JNI_STR(env,pName), iValue);
}
void java_FireIMSInvitedToPublicGroup(short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    GROUP_INVITE*pInfoPara = (GROUP_INVITE *)pPara;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSInvitedToPublicGroup");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSInvitedToPublicGroup begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSInvitedToPublicGroup", (char *)"(Lcom/zte/softda/ocx/FireGroupInvitePara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSInvitedToPublicGroup");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSInvitedToPublicGroup");

        return;
    }

    jclass class_GroupInvite = env->FindClass("com/zte/softda/ocx/FireGroupInvitePara");
    jmethodID id_GroupInvite  = env->GetMethodID(class_GroupInvite, "<init>", "()V");
    jobject obj_GroupInvite = env->NewObject(class_GroupInvite, id_GroupInvite);

    jstring cGName = env->NewStringUTF(pInfoPara->cGName);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGName", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGName);
    env->DeleteLocalRef(cGName);

    jstring cGOwnerURI = env->NewStringUTF(pInfoPara->cGOwnerURI);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGOwnerURI", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGOwnerURI);
    env->DeleteLocalRef(cGOwnerURI);

    jstring cGOwnerName = env->NewStringUTF(pInfoPara->cGOwnerName);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGOwnerName", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGOwnerName);
    env->DeleteLocalRef(cGOwnerName);

    jstring cGURI = env->NewStringUTF(pInfoPara->cGURI);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGURI", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGURI);
    env->DeleteLocalRef(cGURI);

    jstring cGIdentifier = env->NewStringUTF(pInfoPara->cGIdentifier);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGIdentifier", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGIdentifier);
    env->DeleteLocalRef(cGIdentifier);

    jstring cDesc = env->NewStringUTF(pInfoPara->cDesc);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cDesc", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cDesc);
    env->DeleteLocalRef(cDesc);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_GroupInvite);
}
void java_FireIMSInvitedToTmpPublicGroup(short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    GROUP_INVITE*pInfoPara = (GROUP_INVITE *)pPara;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSInvitedToTmpPublicGroup");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSInvitedToTmpPublicGroup begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSInvitedToTmpPublicGroup", (char *)"(Lcom/zte/softda/ocx/FireGroupInvitePara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSInvitedToTmpPublicGroup");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSInvitedToTmpPublicGroup");

        return;
    }

    jclass class_GroupInvite = env->FindClass("com/zte/softda/ocx/FireGroupInvitePara");
    jmethodID id_GroupInvite  = env->GetMethodID(class_GroupInvite, "<init>", "()V");
    jobject obj_GroupInvite = env->NewObject(class_GroupInvite, id_GroupInvite);

    jstring cGName = env->NewStringUTF(pInfoPara->cGName);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGName", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGName);
    env->DeleteLocalRef(cGName);

    jstring cGOwnerURI = env->NewStringUTF(pInfoPara->cGOwnerURI);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGOwnerURI", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGOwnerURI);
    env->DeleteLocalRef(cGOwnerURI);

    jstring cGOwnerName = env->NewStringUTF(pInfoPara->cGOwnerName);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGOwnerName", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGOwnerName);
    env->DeleteLocalRef(cGOwnerName);

    jstring cGURI = env->NewStringUTF(pInfoPara->cGURI);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGURI", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGURI);
    env->DeleteLocalRef(cGURI);

    jstring cGIdentifier = env->NewStringUTF(pInfoPara->cGIdentifier);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cGIdentifier", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cGIdentifier);
    env->DeleteLocalRef(cGIdentifier);

    jstring cDesc = env->NewStringUTF(pInfoPara->cDesc);
    id_fieldID = env->GetFieldID(class_GroupInvite, "cDesc", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupInvite, id_fieldID, cDesc);
    env->DeleteLocalRef(cDesc);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_GroupInvite);
}

void java_FireIMSPublicGroupInviteResult(LPCTSTR pGroupURI, LPCTSTR pInvitedUserURI, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
	
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSPublicGroupInviteResult pGroupURI[%s] pInvitedUserURI[%s] iResult[%d]",
		pGroupURI, pInvitedUserURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSPublicGroupInviteResult pGroupURI[%s] pInvitedUserURI[%s] iResult[%d]",
		pGroupURI, pInvitedUserURI, iResult);
	
    iresult = getStaticJniMethod((char *)"java_FireIMSPublicGroupInviteResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);
	
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSPublicGroupInviteResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSPublicGroupInviteResult");
		
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI), JNI_STR(env,pInvitedUserURI),iResult);
}

void java_FireIMSSearchPuGroupResult(long iResultNum, LPCTSTR pSearchString, short far* pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    PUBLIC_GROUP_EXPLORER *pResult = (PUBLIC_GROUP_EXPLORER *)pNotify;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchPuGroupResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchPuGroupResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchPuGroupResult", (char *)"(ILjava/lang/String;Lcom/zte/softda/ocx/FirePublicGroupExplorerPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchPuGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchPuGroupResult");

        return;
    }

    jclass class_PublicGroupExplorer = env->FindClass("com/zte/softda/ocx/FirePublicGroupExplorerPara");
    jmethodID id_PublicGroupExplorer  = env->GetMethodID(class_PublicGroupExplorer, "<init>", "()V");
    jobject obj_PublicGroupExplorer = env->NewObject(class_PublicGroupExplorer, id_PublicGroupExplorer);

    if(NULL != pResult)
    {
        jstring cGName = env->NewStringUTF(pResult->cGName);
        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "cGName", "Ljava/lang/String;");
        env->SetObjectField(obj_PublicGroupExplorer, id_fieldID, cGName);
        env->DeleteLocalRef(cGName);

        jstring cGURI = env->NewStringUTF(pResult->cGURI);
        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "cGURI", "Ljava/lang/String;");
        env->SetObjectField(obj_PublicGroupExplorer, id_fieldID, cGURI);
        env->DeleteLocalRef(cGURI);

        jstring cOwnerURI = env->NewStringUTF(pResult->cOwnerURI);
        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "cOwnerURI", "Ljava/lang/String;");
        env->SetObjectField(obj_PublicGroupExplorer, id_fieldID, cOwnerURI);
        env->DeleteLocalRef(cOwnerURI);

        jstring cGSubject = env->NewStringUTF(pResult->cGSubject);
        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "cGSubject", "Ljava/lang/String;");
        env->SetObjectField(obj_PublicGroupExplorer, id_fieldID, cGSubject);
        env->DeleteLocalRef(cGSubject);

        jstring cGBulletin = env->NewStringUTF(pResult->cGBulletin);
        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "cGBulletin", "Ljava/lang/String;");
        env->SetObjectField(obj_PublicGroupExplorer, id_fieldID, cGBulletin);
        env->DeleteLocalRef(cGBulletin);

        jstring cIdentifier = env->NewStringUTF(pResult->cIdentifier);
        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "cIdentifier", "Ljava/lang/String;");
        env->SetObjectField(obj_PublicGroupExplorer, id_fieldID, cIdentifier);
        env->DeleteLocalRef(cIdentifier);

        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "iNeedPermit", "I");
        env->SetIntField(obj_PublicGroupExplorer, id_fieldID, pResult->iNeedPermit);

        jstring cGroupClassID = env->NewStringUTF(pResult->cGroupClassID);
        id_fieldID = env->GetFieldID(class_PublicGroupExplorer, "cGroupClassID", "Ljava/lang/String;");
        env->SetObjectField(obj_PublicGroupExplorer, id_fieldID, cGroupClassID);
        env->DeleteLocalRef(cGroupClassID);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iResultNum, JNI_STR(env,pSearchString), obj_PublicGroupExplorer);
    }
    else
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iResultNum, JNI_STR(env,pSearchString), NULL);
    }
}
void java_FireIMSSearchPublicGroupFailed(LPCTSTR pGroupURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchPublicGroupFailed pGroupURI[%s]", pGroupURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchPublicGroupFailed pGroupURI[%s]", pGroupURI);

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchPublicGroupFailed", (char *)"(Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchPublicGroupFailed");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchPublicGroupFailed");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI));
}
void java_FireIMSReqJoinMyPublicGroup(short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    GROUP_JOIN* pInfoPara = (GROUP_JOIN *)pPara;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSReqJoinMyPublicGroup");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSReqJoinMyPublicGroup begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSReqJoinMyPublicGroup", (char *)"(Lcom/zte/softda/ocx/FireGroupJoinPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSReqJoinMyPublicGroup");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSReqJoinMyPublicGroup");

        return;
    }
    jclass class_GroupJoin = env->FindClass("com/zte/softda/ocx/FireGroupJoinPara");
    jmethodID id_GroupJoin  = env->GetMethodID(class_GroupJoin, "<init>", "()V");
    jobject obj_GroupJoin = env->NewObject(class_GroupJoin, id_GroupJoin);

    jstring cGName = env->NewStringUTF(pInfoPara->cGName);
    id_fieldID = env->GetFieldID(class_GroupJoin, "cGName", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupJoin, id_fieldID, cGName);
    env->DeleteLocalRef(cGName);

    jstring cGProposerURI = env->NewStringUTF(pInfoPara->cGProposerURI);
    id_fieldID = env->GetFieldID(class_GroupJoin, "cGProposerURI", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupJoin, id_fieldID, cGProposerURI);
    env->DeleteLocalRef(cGProposerURI);

    jstring cGProposerName = env->NewStringUTF(pInfoPara->cGProposerName);
    id_fieldID = env->GetFieldID(class_GroupJoin, "cGProposerName", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupJoin, id_fieldID, cGProposerName);
    env->DeleteLocalRef(cGProposerName);

    jstring cGURI = env->NewStringUTF(pInfoPara->cGURI);
    id_fieldID = env->GetFieldID(class_GroupJoin, "cGURI", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupJoin, id_fieldID, cGURI);
    env->DeleteLocalRef(cGURI);

    jstring cGIdentifier = env->NewStringUTF(pInfoPara->cGIdentifier);
    id_fieldID = env->GetFieldID(class_GroupJoin, "cGIdentifier", "Ljava/lang/String;");
    env->SetObjectField(obj_GroupJoin, id_fieldID, cGIdentifier);
    env->DeleteLocalRef(cGIdentifier);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_GroupJoin);
}
void java_FireIMSPublicGroupMemberNotify(short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    IMSPublicGroupMemberNotify_T *pNotify = (IMSPublicGroupMemberNotify_T *)pPara;

    TraceMsgWindow1(1, "rcscontroller java_FireIMSPublicGroupMemberNotify iType[%d] pGroupURI[%s] pOwnerURI[%s] "
        "pMemberURI[%s] pInviterURI[%s] pMemberDisplay[%s] pFullSpelling[%s] pMemberInfo[%s] pMemberPhone[%s] pMemberType[%s] "
        "pMemberCaps[%s] iStatus[%d] pMsgID[%s] pShareUserURI[%s] pShareUserName[%s]",pNotify->iType, pNotify->pGroupURI, pNotify->pOwnerURI,
        pNotify->pMemberURI, pNotify->pInviterURI, pNotify->pMemberDisplayName, pNotify->pFullSpelling, pNotify->lpMemberInfo, pNotify->pMemberPhone, pNotify->pMemberType, 
        pNotify->pMemberCaps, pNotify->iMemberStatus, pNotify->pMsgID, pNotify->cShareUserURI, pNotify->cShareUserName);

    iresult = getStaticJniMethod((char *)"java_FireIMSPublicGroupMemberNotify", (char *)"(ILcom/zte/softda/ocx/GroupMemberNotifyPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireIMSPublicGroupMemberNotify");
        return;
    }

    JOjbectWrapper objEvent(env, "com/zte/softda/ocx/GroupMemberNotifyPara");
	objEvent.NewObject();

    objEvent.SetStringValue("pGroupURI", pNotify->pGroupURI);
    objEvent.SetStringValue("pOwnerURI", pNotify->pOwnerURI);
    objEvent.SetStringValue("pMemberURI", pNotify->pMemberURI);
    objEvent.SetStringValue("pInviterURI", pNotify->pInviterURI);
    objEvent.SetStringValue("pMemberDisplay", pNotify->pMemberDisplayName);
    objEvent.SetStringValue("pFullSpelling", pNotify->pFullSpelling);
    objEvent.SetStringValue("pMemberInfo", pNotify->lpMemberInfo);
    objEvent.SetStringValue("pMemberPhone", pNotify->pMemberPhone);
    objEvent.SetStringValue("pMemberType", pNotify->pMemberType);
    objEvent.SetStringValue("pMemberCaps", pNotify->pMemberCaps);
    objEvent.SetIntValue("iStatus", pNotify->iMemberStatus);
    objEvent.SetStringValue("pMsgID", pNotify->pMsgID);
    objEvent.SetStringValue("pShareUserURI", pNotify->cShareUserURI);
    objEvent.SetStringValue("pShareUserName", pNotify->cShareUserName);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, pNotify->iType, objEvent.GetObject());
    TraceMsgWindow1(1, "rcscontroller java_FireIMSPublicGroupMemberNotify over");
}
void java_FireIMSPublicGroupDeleted(LPCTSTR pGroupURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSPublicGroupDeleted pGroupURI[%s]", pGroupURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSPublicGroupDeleted pGroupURI[%s]", pGroupURI);

    iresult = getStaticJniMethod((char *)"java_FireIMSPublicGroupDeleted", (char *)"(Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSPublicGroupDeleted");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSPublicGroupDeleted");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI));
}
void java_FireIMSDeleteMeInPublicGroup(LPCTSTR pGroupURI, int iType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    TraceMsgWindow1(1, "rcscontroller java_FireIMSDeleteMeInPublicGroup pGroupURI[%s] iType[%d]", pGroupURI, iType);

    iresult = getStaticJniMethod((char *)"java_FireIMSDeleteMeInPublicGroup", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDeleteMeInPublicGroup");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI), iType);
}
void java_FireIMSDoPublicGroupInviteResult(long lDoType, LPCTSTR pGroupURI, LPCTSTR pGroupName, LPCTSTR lpcsOwnerName, long lCode)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDoPublicGroupInviteResult lDoType[%d] pGroupURI[%s] pGroupName[%s] lpcsOwnerName[%s] lCode[%d]",
                        lDoType, pGroupURI, pGroupName, lpcsOwnerName, lCode);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDoPublicGroupInviteResult lDoType[%d] pGroupURI[%s] pGroupName[%s] lpcsOwnerName[%s] lCode[%d]",
             lDoType, pGroupURI, pGroupName, lpcsOwnerName, lCode);

    iresult = getStaticJniMethod((char *)"java_FireIMSDoPublicGroupInviteResult", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDoPublicGroupInviteResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDoPublicGroupInviteResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lDoType, JNI_STR(env,pGroupURI), JNI_STR(env,pGroupName), JNI_STR(env,lpcsOwnerName), lCode);
}
void java_FireIMSPersonSubInfoChanged(long lType, short far* pPersonSubInfoEvent)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;

    PIM_NOTIFY *pResult = NULL;
    int i;

    if(lType == 0)
    {
        pResult = (PIM_NOTIFY *)pPersonSubInfoEvent;
    }

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSPersonSubInfoChanged");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSPersonSubInfoChanged");

    iresult = getStaticJniMethod((char *)"java_FireIMSPersonSubInfoChanged", (char *)"(ILcom/zte/softda/ocx/FireIMSPersonSubInfoChangedPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSPersonSubInfoChanged");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSPersonSubInfoChanged");

        return;
    }

    jclass class_IMSPersonSubInfoChanged = env->FindClass("com/zte/softda/ocx/FireIMSPersonSubInfoChangedPara");
    jmethodID id_IMSPersonSubInfoChanged  = env->GetMethodID(class_IMSPersonSubInfoChanged, "<init>", "()V");
    jobject obj_IMSPersonSubInfoChanged = env->NewObject(class_IMSPersonSubInfoChanged, id_IMSPersonSubInfoChanged);

    if(NULL != pResult)
    {
        jstring cURI = env->NewStringUTF(pResult->cURI);
        id_fieldID = env->GetFieldID(class_IMSPersonSubInfoChanged, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPersonSubInfoChanged, id_fieldID, cURI);
        env->DeleteLocalRef(cURI);

        jstring cChangeType = env->NewStringUTF(pResult->cChangeType);
        id_fieldID = env->GetFieldID(class_IMSPersonSubInfoChanged, "cChangeType", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPersonSubInfoChanged, id_fieldID, cChangeType);
        env->DeleteLocalRef(cChangeType);

        jstring cPhotoIndex = env->NewStringUTF(pResult->cPhotoIndex);
        id_fieldID = env->GetFieldID(class_IMSPersonSubInfoChanged, "cPhotoIndex", "Ljava/lang/String;");
        env->SetObjectField(obj_IMSPersonSubInfoChanged, id_fieldID, cPhotoIndex);
        env->DeleteLocalRef(cPhotoIndex);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_IMSPersonSubInfoChanged);
    }
    else
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, NULL);
    }
}
void java_FireIMSSetUserInfoResult(long lResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSetUserInfoResult lResult[%d]", lResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSetUserInfoResult lResult[%d]", lResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSSetUserInfoResult", (char *)"(I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSetUserInfoResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSetUserInfoResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lResult);
}

void java_FireIMSGotPersonSubInfo(int retCode, long lType, short far*pPersonSubInfoEvent)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    USER_INFO *pResult = (USER_INFO *)pPersonSubInfoEvent;
    int i;
    long lDataLen = 0; //��ͷ��Data����

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotPersonSubInfo begin, lType[%d]", lType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotPersonSubInfo begin, lType[%d]", lType);

    if(lType == GET_USER_ECP_INFO)
    {
        return;
    }

    iresult = getStaticJniMethod((char *)"java_FireIMSGotPersonSubInfo", (char *)"(IILcom/zte/softda/ocx/FireIMSGotUserInfoPara;)V", &env, &gJinMethod);
//FireIMSUserInfo
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotPersonSubInfo");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotPersonSubInfo");

        return;
    }

    jclass class_IMSGotUserInfo = env->FindClass("com/zte/softda/ocx/FireIMSGotUserInfoPara");
    jmethodID id_IMSGotUserInfo  = env->GetMethodID(class_IMSGotUserInfo, "<init>", "()V");
    jobject obj_IMSGotUserInfo = env->NewObject(class_IMSGotUserInfo, id_IMSGotUserInfo);

    jstring cName = env->NewStringUTF(pResult->cName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cName);
    env->DeleteLocalRef(cName);

    jstring cSex = env->NewStringUTF(pResult->cSex);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSex", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSex);
    env->DeleteLocalRef(cSex);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iOld", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iOld);

    jstring cBirthday = env->NewStringUTF(pResult->cBirthday);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cBirthday", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cBirthday);
    env->DeleteLocalRef(cBirthday);

    jstring cInterest = env->NewStringUTF(pResult->cInterest);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cInterest", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cInterest);
    env->DeleteLocalRef(cInterest);

    jstring cOccupation = env->NewStringUTF(pResult->cOccupation);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOccupation", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOccupation);
    env->DeleteLocalRef(cOccupation);

    jstring cCountry = env->NewStringUTF(pResult->cCountry);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCountry", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCountry);
    env->DeleteLocalRef(cCountry);

    jstring cZone = env->NewStringUTF(pResult->cZone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cZone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cZone);
    env->DeleteLocalRef(cZone);

    jstring cCity = env->NewStringUTF(pResult->cCity);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCity", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCity);
    env->DeleteLocalRef(cCity);

    jstring cUcNum = env->NewStringUTF(pResult->cUcNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cUcNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cUcNum);
    env->DeleteLocalRef(cUcNum);

    jstring cPocNum = env->NewStringUTF(pResult->cPocNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPocNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPocNum);
    env->DeleteLocalRef(cPocNum);

    jstring cMessageNum = env->NewStringUTF(pResult->cMessageNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMessageNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMessageNum);
    env->DeleteLocalRef(cMessageNum);

    jstring cFaxNum = env->NewStringUTF(pResult->cFaxNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxNum);
    env->DeleteLocalRef(cFaxNum);

    jstring cHomeTel = env->NewStringUTF(pResult->cHomeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cHomeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cHomeTel);
    env->DeleteLocalRef(cHomeTel);

    jstring cOfficeTel = env->NewStringUTF(pResult->cOfficeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOfficeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOfficeTel);
    env->DeleteLocalRef(cOfficeTel);

    jstring cMobileTel = env->NewStringUTF(pResult->cMobileTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMobileTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMobileTel);
    env->DeleteLocalRef(cMobileTel);

    jstring cAutoTel = env->NewStringUTF(pResult->cAutoTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAutoTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAutoTel);
    env->DeleteLocalRef(cAutoTel);

    jstring cSHomeTel = env->NewStringUTF(pResult->cSHomeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSHomeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSHomeTel);
    env->DeleteLocalRef(cSHomeTel);

    jstring cSOfficeTel = env->NewStringUTF(pResult->cSOfficeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSOfficeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSOfficeTel);
    env->DeleteLocalRef(cSOfficeTel);

    jstring cSMobileTel = env->NewStringUTF(pResult->cSMobileTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSMobileTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSMobileTel);
    env->DeleteLocalRef(cSMobileTel);

    jstring cHomeAddr = env->NewStringUTF(pResult->cHomeAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cHomeAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cHomeAddr);
    env->DeleteLocalRef(cHomeAddr);

    jstring cOfficeAddr = env->NewStringUTF(pResult->cOfficeAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOfficeAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOfficeAddr);
    env->DeleteLocalRef(cOfficeAddr);

    jstring cOtherAddr = env->NewStringUTF(pResult->cOtherAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOtherAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOtherAddr);
    env->DeleteLocalRef(cOtherAddr);

    jstring cEmailAddr = env->NewStringUTF(pResult->cEmailAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cEmailAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cEmailAddr);
    env->DeleteLocalRef(cEmailAddr);

    jstring cDomain = env->NewStringUTF(pResult->cDomain);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDomain", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDomain);
    env->DeleteLocalRef(cDomain);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iNotifyMode", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iNotifyMode);

    jstring cURI = env->NewStringUTF(pResult->cURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cURI);
    env->DeleteLocalRef(cURI);

    jstring cPhotoIndex = env->NewStringUTF(pResult->cPhotoIndex);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoIndex", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoIndex);
    env->DeleteLocalRef(cPhotoIndex);

//	id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoFile", "[B");//cPhotoFile
//	lDataLen = strlen(pResult->cPhotoFile);
//	jbyteArray ContentArray= env->NewByteArray(lDataLen);
//	env->SetByteArrayRegion(ContentArray, 0, lDataLen, (const jbyte*)pResult->cPhotoFile);
//	env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, ContentArray);
    jstring cPhotoFile = env->NewStringUTF(pResult->cPhotoFile ? pResult->cPhotoFile : "");
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoFile", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoFile);
    env->DeleteLocalRef(cPhotoFile);
    if(pResult->cPhotoFile)
    {
        ZX_free(pResult->cPhotoFile);
        pResult->cPhotoFile = NULL;
    }

    jstring cSoftPhone = env->NewStringUTF(pResult->cSoftPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSoftPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSoftPhone);
    env->DeleteLocalRef(cSoftPhone);

//������ҵ��Ϣ
    jstring cAffiliation = env->NewStringUTF(pResult->cAffiliation);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAffiliation", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAffiliation);
    env->DeleteLocalRef(cAffiliation);

    jstring cDepartment = env->NewStringUTF(pResult->cDepartment);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartment", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartment);
    env->DeleteLocalRef(cDepartment);

    jstring cDepartmentName = env->NewStringUTF(pResult->cDepartmentName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartmentName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartmentName);
    env->DeleteLocalRef(cDepartmentName);

    jstring cDepartmentURI = env->NewStringUTF(pResult->cDepartmentURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartmentURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartmentURI);
    env->DeleteLocalRef(cDepartmentURI);

    jstring cEmployeeNum = env->NewStringUTF(pResult->cEmployeeNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cEmployeeNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cEmployeeNum);
    env->DeleteLocalRef(cEmployeeNum);

    jstring cPersonEmailAddr = env->NewStringUTF(pResult->cPersonEmailAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPersonEmailAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPersonEmailAddr);
    env->DeleteLocalRef(cPersonEmailAddr);

    jstring cPosition = env->NewStringUTF(pResult->cPosition);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPosition", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPosition);
    env->DeleteLocalRef(cPosition);

    jstring cPhotoURI = env->NewStringUTF(pResult->cPhotoURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoURI);
    env->DeleteLocalRef(cPhotoURI);

    jstring cURI2 = env->NewStringUTF(pResult->cURI2);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cURI2", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cURI2);
    env->DeleteLocalRef(cURI2);

//ecp����
    jstring cBusinessPhone = env->NewStringUTF(pResult->cBusinessPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cBusinessPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cBusinessPhone);
    env->DeleteLocalRef(cBusinessPhone);

    jstring cSmsPhone = env->NewStringUTF(pResult->cSmsPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSmsPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSmsPhone);
    env->DeleteLocalRef(cSmsPhone);

    jstring cSmsBusiness = env->NewStringUTF(pResult->cSmsBusiness);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSmsBusiness", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSmsBusiness);
    env->DeleteLocalRef(cSmsBusiness);

    jstring cFaxBusiness = env->NewStringUTF(pResult->cFaxBusiness);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxBusiness", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxBusiness);
    env->DeleteLocalRef(cFaxBusiness);

    jstring cFaxExt = env->NewStringUTF(pResult->cFaxExt);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxExt", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxExt);
    env->DeleteLocalRef(cFaxExt);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iVersionType", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iVersionType);

    jstring cRealName = env->NewStringUTF(pResult->cRealName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cRealName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cRealName);
    env->DeleteLocalRef(cRealName);

    jstring cOtherTel = env->NewStringUTF(pResult->cOtherTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOtherTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOtherTel);
    env->DeleteLocalRef(cOtherTel);

    jstring cFullSpelling = env->NewStringUTF(pResult->cFullSpelling);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFullSpelling", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFullSpelling);
    env->DeleteLocalRef(cFullSpelling);

    jstring cShortSpelling = env->NewStringUTF(pResult->cShortSpelling);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cShortSpelling", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cShortSpelling);
    env->DeleteLocalRef(cShortSpelling);

    jstring cFullSpelling1 = env->NewStringUTF(pResult->cFullSpelling1);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFullSpelling1", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFullSpelling1);
    env->DeleteLocalRef(cFullSpelling1);

    jstring cShortSpelling1 = env->NewStringUTF(pResult->cShortSpelling1);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cShortSpelling1", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cShortSpelling1);
    env->DeleteLocalRef(cShortSpelling1);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iAreaCodeFlag", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iAreaCodeFlag);

    jstring cCompanyUri = env->NewStringUTF(pResult->cCompanyUri);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCompanyUri", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCompanyUri);
    env->DeleteLocalRef(cCompanyUri);

    jstring cUserCapacity = env->NewStringUTF(pResult->cUserCapacity);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cUserCapacity", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cUserCapacity);
    env->DeleteLocalRef(cUserCapacity);

    jstring cAnswerMode = env->NewStringUTF(pResult->cAnswerMode);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAnswerMode", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAnswerMode);
    env->DeleteLocalRef(cAnswerMode);

    jstring cMsgDeliverReport = env->NewStringUTF(pResult->cMsgDeliverReport);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMsgDeliverReport", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMsgDeliverReport);
    env->DeleteLocalRef(cMsgDeliverReport);

    jstring cOnlyNotify = env->NewStringUTF(pResult->cOnlyNotify);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOnlyNotify", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOnlyNotify);
    env->DeleteLocalRef(cOnlyNotify);

    jstring cAreaCode = env->NewStringUTF(pResult->cAreaCode);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAreaCode", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAreaCode);
    env->DeleteLocalRef(cAreaCode);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, retCode, lType, obj_IMSGotUserInfo);
}
void java_FireIMSSearchUserByURIResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchUserByURIResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchUserByURIResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchUserByURIResult", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchUserByURIResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchUserByURIResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pDName), JNI_STR(env,pSex),  iAge, JNI_STR(env,pCity), iOnline);
}

void java_FireIMSSearchUserByDNameResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchUserByDNameResult pURI[%s]", pURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchUserByDNameResult pURI[%s]", pURI);

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchUserByDNameResult", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchUserByDNameResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchUserByDNameResult");

        return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pDName), JNI_STR(env,pSex),  iAge, JNI_STR(env,pCity), iOnline);
}
void java_FireIMSSearchOneUserOnlineResult(int iPos, LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchOneUserOnlineResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchOneUserOnlineResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchOneUserOnlineResult", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchOneUserOnlineResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchOneUserOnlineResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iPos, JNI_STR(env,pURI), JNI_STR(env,pDName), JNI_STR(env,pSex),  iAge, JNI_STR(env,pCity));
}

void java_FireIMSSearchUserByPhoneResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchUserByPhoneResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchUserByPhoneResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchUserByPhoneResult", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchOneUserOnlineResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchOneUserOnlineResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pDName), JNI_STR(env,pSex),  iAge, JNI_STR(env,pCity), iOnline);
}

void java_FireIMSSearchUserByEmailResult(LPCTSTR pURI, LPCTSTR pDName, LPCTSTR pSex, int iAge, LPCTSTR pCity, int iOnline)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchUserByEmailResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchUserByEmailResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchUserByEmailResult", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchUserByEmailResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchUserByEmailResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pDName), JNI_STR(env,pSex),  iAge, JNI_STR(env,pCity), iOnline);
}

void java_FireIMSAddListResult(LPCTSTR pURI, LPCTSTR pName, int iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSAddListResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSAddListResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSAddListResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSAddListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSAddListResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pName), iResult);
}
void java_FireIMSDelListResult(LPCTSTR pURI, int iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDelListResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDelListResult  begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSDelListResult", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDelListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDelListResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), iResult);
}

void  java_FireIMSAddRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSAddRLSMemberResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSAddRLSMemberResult  begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSAddRLSMemberResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSAddRLSMemberResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSAddRLSMemberResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pListMemberURI), iResult);
}

void java_FireIMSDeleteRLSMemberResult(LPCTSTR pURI, LPCTSTR pListMemberURI, int iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDeleteRLSMemberResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDeleteRLSMemberResult  begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSDeleteRLSMemberResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDeleteRLSMemberResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDeleteRLSMemberResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pListMemberURI), iResult);
}

void java_FireIMSSetPresenceRulesResult(LPCTSTR pURI, int iType)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSetPresenceRulesResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSetPresenceRulesResult  begin");

    iresult = getStaticJniMethod((char *)"java_FireIMSSetPresenceRulesResult", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSetPresenceRulesResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSetPresenceRulesResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), iType);
}

//void java_FireIMSGotUserInfo(short far* pNotify)
void java_FireIMSGotOneListInfo(short far* pListInfo, int iReturnCode,LPCTSTR pURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    USER_INFO *pResult = NULL;
    int i;
    long lDataLen = 0; //��ͷ��Data����

    pResult = (USER_INFO *)pListInfo;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOneListInfo begin iReturnCode =[%d]",iReturnCode);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotOneListInfo begin iReturnCode =[%d]",iReturnCode);


    iresult = getStaticJniMethod((char *)"java_FireIMSGotOneListInfo", (char *)"(Lcom/zte/softda/ocx/FireIMSGotUserInfoPara;ILjava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotOneListInfo");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotOneListInfo");

        return;
    }

    if(pResult == NULL)//��ϵ��Ϊ��ֱ�ӷ��ز���Ҫ�ϱ�
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOneListInfo pResult == NULL so return");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSGotOneListInfo pResult == NULL so return");
	 env->CallStaticVoidMethod(gpostClass, gJinMethod, NULL, iReturnCode, JNI_STR(env,pURI));
        return;
    }
    jclass class_IMSGotUserInfo = env->FindClass("com/zte/softda/ocx/FireIMSGotUserInfoPara");
    jmethodID id_IMSGotUserInfo  = env->GetMethodID(class_IMSGotUserInfo, "<init>", "()V");
    jobject obj_IMSGotUserInfo = env->NewObject(class_IMSGotUserInfo, id_IMSGotUserInfo);

    jstring cName = env->NewStringUTF(pResult->cName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cName);
    env->DeleteLocalRef(cName);

    jstring cSex = env->NewStringUTF(pResult->cSex);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSex", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSex);
    env->DeleteLocalRef(cSex);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iOld", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iOld);

    jstring cBirthday = env->NewStringUTF(pResult->cBirthday);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cBirthday", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cBirthday);
    env->DeleteLocalRef(cBirthday);

    jstring cInterest = env->NewStringUTF(pResult->cInterest);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cInterest", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cInterest);
    env->DeleteLocalRef(cInterest);

    jstring cOccupation = env->NewStringUTF(pResult->cOccupation);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOccupation", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOccupation);
    env->DeleteLocalRef(cOccupation);

    jstring cCountry = env->NewStringUTF(pResult->cCountry);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCountry", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCountry);
    env->DeleteLocalRef(cCountry);

    jstring cZone = env->NewStringUTF(pResult->cZone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cZone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cZone);
    env->DeleteLocalRef(cZone);

    jstring cCity = env->NewStringUTF(pResult->cCity);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCity", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCity);
    env->DeleteLocalRef(cCity);

    jstring cUcNum = env->NewStringUTF(pResult->cUcNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cUcNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cUcNum);
    env->DeleteLocalRef(cUcNum);

    jstring cPocNum = env->NewStringUTF(pResult->cPocNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPocNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPocNum);
    env->DeleteLocalRef(cPocNum);

    jstring cMessageNum = env->NewStringUTF(pResult->cMessageNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMessageNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMessageNum);
    env->DeleteLocalRef(cMessageNum);

    jstring cFaxNum = env->NewStringUTF(pResult->cFaxNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxNum);
    env->DeleteLocalRef(cFaxNum);

    jstring cHomeTel = env->NewStringUTF(pResult->cHomeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cHomeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cHomeTel);
    env->DeleteLocalRef(cHomeTel);

    jstring cOfficeTel = env->NewStringUTF(pResult->cOfficeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOfficeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOfficeTel);
    env->DeleteLocalRef(cOfficeTel);

    jstring cMobileTel = env->NewStringUTF(pResult->cMobileTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMobileTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMobileTel);
    env->DeleteLocalRef(cMobileTel);

    jstring cAutoTel = env->NewStringUTF(pResult->cAutoTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAutoTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAutoTel);
    env->DeleteLocalRef(cAutoTel);

    jstring cSHomeTel = env->NewStringUTF(pResult->cSHomeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSHomeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSHomeTel);
    env->DeleteLocalRef(cSHomeTel);

    jstring cSOfficeTel = env->NewStringUTF(pResult->cSOfficeTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSOfficeTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSOfficeTel);
    env->DeleteLocalRef(cSOfficeTel);

    jstring cSMobileTel = env->NewStringUTF(pResult->cSMobileTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSMobileTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSMobileTel);
    env->DeleteLocalRef(cSMobileTel);

    jstring cHomeAddr = env->NewStringUTF(pResult->cHomeAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cHomeAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cHomeAddr);
    env->DeleteLocalRef(cHomeAddr);

    jstring cOfficeAddr = env->NewStringUTF(pResult->cOfficeAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOfficeAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOfficeAddr);
    env->DeleteLocalRef(cOfficeAddr);

    jstring cOtherAddr = env->NewStringUTF(pResult->cOtherAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOtherAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOtherAddr);
    env->DeleteLocalRef(cOtherAddr);

    jstring cEmailAddr = env->NewStringUTF(pResult->cEmailAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cEmailAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cEmailAddr);
    env->DeleteLocalRef(cEmailAddr);

    jstring cDomain = env->NewStringUTF(pResult->cDomain);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDomain", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDomain);
    env->DeleteLocalRef(cDomain);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iNotifyMode", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iNotifyMode);

    jstring cURI = env->NewStringUTF(pResult->cURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cURI);
    env->DeleteLocalRef(cURI);

    jstring cPhotoIndex = env->NewStringUTF(pResult->cPhotoIndex);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoIndex", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoIndex);
    env->DeleteLocalRef(cPhotoIndex);

//	id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoFile", "[B");//cPhotoFile
//	lDataLen = strlen(pResult->cPhotoFile);
//	jbyteArray ContentArray= env->NewByteArray(lDataLen);
//	env->SetByteArrayRegion(ContentArray, 0, lDataLen, (const jbyte*)pResult->cPhotoFile);
//	env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, ContentArray);
    jstring cPhotoFile = env->NewStringUTF(pResult->cPhotoFile);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoFile", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoFile);
    env->DeleteLocalRef(cPhotoFile);
    ZX_free(pResult->cPhotoFile);
    pResult->cPhotoFile = NULL;

    jstring cSoftPhone = env->NewStringUTF(pResult->cSoftPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSoftPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSoftPhone);
    env->DeleteLocalRef(cSoftPhone);

//������ҵ��Ϣ
    jstring cAffiliation = env->NewStringUTF(pResult->cAffiliation);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAffiliation", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAffiliation);
    env->DeleteLocalRef(cAffiliation);

    jstring cDepartment = env->NewStringUTF(pResult->cDepartment);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartment", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartment);
    env->DeleteLocalRef(cDepartment);

    jstring cDepartmentName = env->NewStringUTF(pResult->cDepartmentName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartmentName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartmentName);
    env->DeleteLocalRef(cDepartmentName);

    jstring cDepartmentURI = env->NewStringUTF(pResult->cDepartmentURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cDepartmentURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cDepartmentURI);
    env->DeleteLocalRef(cDepartmentURI);

    jstring cEmployeeNum = env->NewStringUTF(pResult->cEmployeeNum);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cEmployeeNum", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cEmployeeNum);
    env->DeleteLocalRef(cEmployeeNum);

    jstring cPersonEmailAddr = env->NewStringUTF(pResult->cPersonEmailAddr);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPersonEmailAddr", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPersonEmailAddr);
    env->DeleteLocalRef(cPersonEmailAddr);

    jstring cPosition = env->NewStringUTF(pResult->cPosition);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPosition", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPosition);
    env->DeleteLocalRef(cPosition);

    jstring cPhotoURI = env->NewStringUTF(pResult->cPhotoURI);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cPhotoURI", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cPhotoURI);
    env->DeleteLocalRef(cPhotoURI);

    jstring cURI2 = env->NewStringUTF(pResult->cURI2);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cURI2", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cURI2);
    env->DeleteLocalRef(cURI2);

//ecp����
    jstring cBusinessPhone = env->NewStringUTF(pResult->cBusinessPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cBusinessPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cBusinessPhone);
    env->DeleteLocalRef(cBusinessPhone);

    jstring cSmsPhone = env->NewStringUTF(pResult->cSmsPhone);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSmsPhone", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSmsPhone);
    env->DeleteLocalRef(cSmsPhone);

    jstring cSmsBusiness = env->NewStringUTF(pResult->cSmsBusiness);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cSmsBusiness", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cSmsBusiness);
    env->DeleteLocalRef(cSmsBusiness);

    jstring cFaxBusiness = env->NewStringUTF(pResult->cFaxBusiness);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxBusiness", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxBusiness);
    env->DeleteLocalRef(cFaxBusiness);

    jstring cFaxExt = env->NewStringUTF(pResult->cFaxExt);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFaxExt", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFaxExt);
    env->DeleteLocalRef(cFaxExt);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iVersionType", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iVersionType);

    jstring cRealName = env->NewStringUTF(pResult->cRealName);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cRealName", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cRealName);
    env->DeleteLocalRef(cRealName);

    jstring cOtherTel = env->NewStringUTF(pResult->cOtherTel);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOtherTel", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOtherTel);
    env->DeleteLocalRef(cOtherTel);

    jstring cFullSpelling = env->NewStringUTF(pResult->cFullSpelling);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFullSpelling", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFullSpelling);
    env->DeleteLocalRef(cFullSpelling);

    jstring cShortSpelling = env->NewStringUTF(pResult->cShortSpelling);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cShortSpelling", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cShortSpelling);
    env->DeleteLocalRef(cShortSpelling);

    jstring cFullSpelling1 = env->NewStringUTF(pResult->cFullSpelling1);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cFullSpelling1", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cFullSpelling1);
    env->DeleteLocalRef(cFullSpelling1);

    jstring cShortSpelling1 = env->NewStringUTF(pResult->cShortSpelling1);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cShortSpelling1", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cShortSpelling1);
    env->DeleteLocalRef(cShortSpelling1);

    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "iAreaCodeFlag", "J");
    env->SetLongField(obj_IMSGotUserInfo, id_fieldID, (jlong)pResult->iAreaCodeFlag);

    jstring cCompanyUri = env->NewStringUTF(pResult->cCompanyUri);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cCompanyUri", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cCompanyUri);
    env->DeleteLocalRef(cCompanyUri);

    jstring cUserCapacity = env->NewStringUTF(pResult->cUserCapacity);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cUserCapacity", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cUserCapacity);
    env->DeleteLocalRef(cUserCapacity);

    jstring cAnswerMode = env->NewStringUTF(pResult->cAnswerMode);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAnswerMode", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAnswerMode);
    env->DeleteLocalRef(cAnswerMode);

    jstring cMsgDeliverReport = env->NewStringUTF(pResult->cMsgDeliverReport);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cMsgDeliverReport", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cMsgDeliverReport);
    env->DeleteLocalRef(cMsgDeliverReport);

    jstring cOnlyNotify = env->NewStringUTF(pResult->cOnlyNotify);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cOnlyNotify", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cOnlyNotify);
    env->DeleteLocalRef(cOnlyNotify);

    jstring cAreaCode = env->NewStringUTF(pResult->cAreaCode);
    id_fieldID = env->GetFieldID(class_IMSGotUserInfo, "cAreaCode", "Ljava/lang/String;");
    env->SetObjectField(obj_IMSGotUserInfo, id_fieldID, cAreaCode);
    env->DeleteLocalRef(cAreaCode);

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "java_FireIMSGotOneListInfo cName[%s] cRealName[%s]", pResult->cName, pResult->cRealName);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotOneListInfo cName[%s] cRealName[%s]", pResult->cName, pResult->cRealName);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_IMSGotUserInfo,iReturnCode, JNI_STR(env,pURI));
}

void java_FireIMSGotListInfo(List *pListInfo)
{
     jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    int i;
    long lDataLen = 0; //��ͷ��Data����
    int nNum = (NULL == pListInfo ? 0 : list_len(pListInfo));

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotListInfo begin num =[%d]",nNum);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotListInfo begin num =[%d]",nNum);

    if (nNum <= 0)
    {
        return;
    }

    iresult = getStaticJniMethod((char *)"java_FireIMSGotListInfo", (char *)"(Lcom/zte/softda/ocx/FireIMSGotUserInfoListPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotListInfo");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotListInfo");

        return;
    }

	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireIMSGotUserInfoListPara");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FireIMSGotUserInfoPara");
    	jobjectArray array_users = env->NewObjectArray(nNum, class_string, NULL);

	USER_INFO *pUserItem = NULL;
	for (int i = 0; i < nNum; i++)
       {
       	pUserItem = (USER_INFO*)list_get(pListInfo, i);
		if (NULL == pUserItem)
			continue;

		JOjbectWrapper objUserItem(env, "com/zte/softda/ocx/FireIMSGotUserInfoPara");
		objUserItem.NewObject();
		objUserItem.SetStringValue("cName", pUserItem->cName);
		objUserItem.SetStringValue("cSex", pUserItem->cSex);
		objUserItem.SetLongValue("iOld", pUserItem->iOld);
		objUserItem.SetStringValue("cBirthday", pUserItem->cBirthday);
		objUserItem.SetStringValue("cInterest", pUserItem->cInterest);
		objUserItem.SetStringValue("cOccupation", pUserItem->cOccupation);
		objUserItem.SetStringValue("cCountry", pUserItem->cCountry);
		objUserItem.SetStringValue("cZone", pUserItem->cZone);
		objUserItem.SetStringValue("cCity", pUserItem->cCity);
		objUserItem.SetStringValue("cUcNum", pUserItem->cUcNum);
		objUserItem.SetStringValue("cPocNum", pUserItem->cPocNum);
		objUserItem.SetStringValue("cMessageNum", pUserItem->cMessageNum);
		objUserItem.SetStringValue("cFaxNum", pUserItem->cFaxNum);
		objUserItem.SetStringValue("cHomeTel", pUserItem->cHomeTel);
		objUserItem.SetStringValue("cOfficeTel", pUserItem->cOfficeTel);
		objUserItem.SetStringValue("cMobileTel", pUserItem->cMobileTel);
		objUserItem.SetStringValue("cAutoTel", pUserItem->cAutoTel);
		objUserItem.SetStringValue("cSHomeTel", pUserItem->cSHomeTel);
		objUserItem.SetStringValue("cSOfficeTel", pUserItem->cSOfficeTel);
		objUserItem.SetStringValue("cSMobileTel", pUserItem->cSMobileTel);
		objUserItem.SetStringValue("cHomeAddr", pUserItem->cHomeAddr);
		objUserItem.SetStringValue("cOfficeAddr", pUserItem->cOfficeAddr);
		objUserItem.SetStringValue("cOtherAddr", pUserItem->cOtherAddr);
		objUserItem.SetStringValue("cEmailAddr", pUserItem->cEmailAddr);
		objUserItem.SetStringValue("cDomain", pUserItem->cDomain);
		objUserItem.SetLongValue("iNotifyMode", pUserItem->iNotifyMode);
		objUserItem.SetStringValue("cURI", pUserItem->cURI);
		objUserItem.SetStringValue("cPhotoIndex", pUserItem->cPhotoIndex);

		objUserItem.SetStringValue("cPhotoFile", pUserItem->cPhotoFile);
		objUserItem.SetStringValue("cSoftPhone", pUserItem->cSoftPhone);
		objUserItem.SetStringValue("cAffiliation", pUserItem->cAffiliation);
		objUserItem.SetStringValue("cDepartment", pUserItem->cDepartment);
		objUserItem.SetStringValue("cDepartmentName", pUserItem->cDepartmentName);
		objUserItem.SetStringValue("cDepartmentURI", pUserItem->cDepartmentURI);
		objUserItem.SetStringValue("cEmployeeNum", pUserItem->cEmployeeNum);
		objUserItem.SetStringValue("cPersonEmailAddr", pUserItem->cPersonEmailAddr);
		objUserItem.SetStringValue("cPosition", pUserItem->cPosition);
		objUserItem.SetStringValue("cPhotoURI", pUserItem->cPhotoURI);
		objUserItem.SetStringValue("cURI2", pUserItem->cURI2);
		objUserItem.SetStringValue("cBusinessPhone", pUserItem->cBusinessPhone);
		objUserItem.SetStringValue("cSmsPhone", pUserItem->cSmsPhone);
		objUserItem.SetStringValue("cSmsBusiness", pUserItem->cSmsBusiness);
		objUserItem.SetStringValue("cFaxBusiness", pUserItem->cFaxBusiness);
		objUserItem.SetStringValue("cFaxExt", pUserItem->cFaxExt);
		objUserItem.SetLongValue("iVersionType", pUserItem->iVersionType);
		objUserItem.SetStringValue("cRealName", pUserItem->cRealName);
		objUserItem.SetStringValue("cOtherTel", pUserItem->cOtherTel);
		objUserItem.SetStringValue("cFullSpelling", pUserItem->cFullSpelling);
		objUserItem.SetStringValue("cShortSpelling", pUserItem->cShortSpelling);
		objUserItem.SetStringValue("cFullSpelling1", pUserItem->cFullSpelling1);
		objUserItem.SetStringValue("cShortSpelling1", pUserItem->cShortSpelling1);
		objUserItem.SetLongValue("iAreaCodeFlag", pUserItem->iAreaCodeFlag);
		objUserItem.SetStringValue("cCompanyUri", pUserItem->cCompanyUri);
		objUserItem.SetStringValue("cUserCapacity", pUserItem->cUserCapacity);
		objUserItem.SetStringValue("cAnswerMode", pUserItem->cAnswerMode);
		objUserItem.SetStringValue("cMsgDeliverReport", pUserItem->cMsgDeliverReport);
		objUserItem.SetStringValue("cOnlyNotify", pUserItem->cOnlyNotify);
		objUserItem.SetStringValue("cAreaCode", pUserItem->cAreaCode);

        	env->SetObjectArrayElement(array_users, i, objUserItem.GetObject());
    	}
	//free list 
	if (NULL != pListInfo)
	{
		list_destroy(pListInfo, (list_item_destructor_t*)APP_RET_MEM);
	}

    objEvent.SetContainsObject("pNotifyList", "[Lcom/zte/softda/ocx/FireIMSGotUserInfoPara;", array_users);
	TraceMsgWindow1(1, "java_FireIMSGotListInfo to ui");
	env->CallStaticVoidMethod(gpostClass, gJinMethod, objEvent.GetObject());
	
}
void java_FireCtdCallEvent(long lType, short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    int i;

    TraceMsgWindow1(1, "rcscontroller java_FireCtdCallEvent, lType[%d]", lType);

    iresult = getStaticJniMethod((char *)"java_FireCtdCallEvent", (char *)"(ILcom/zte/softda/ocx/FireCtdCallEventPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireCtdCallEvent");
        return;
    }
    jclass class_CtdCallEventPara = env->FindClass("com/zte/softda/ocx/FireCtdCallEventPara");
    jmethodID id_CtdCallEventPara  = env->GetMethodID(class_CtdCallEventPara, "<init>", "()V");
    jobject obj_CtdCallEventPara = env->NewObject(class_CtdCallEventPara, id_CtdCallEventPara);
    if(lType == CTD_TYPE_STAT || lType == CTD_TYPE_STAT_EX)
    {
        CTD_MEMBER_STATUS *pCtdStatusInfo = NULL;
        pCtdStatusInfo = (CTD_MEMBER_STATUS *)pPara;
        if(pCtdStatusInfo == NULL)
        {
            TraceMsgWindow1(1, "rcscontroller java_FireCtdCallEvent, pCtdStatus == NULL");
            return;
        }
        jclass class_CtdStatusInfoPara = env->FindClass("com/zte/softda/ocx/CTD_STATUS");
        jmethodID id_CtdStatusInfoPara  = env->GetMethodID(class_CtdStatusInfoPara, "<init>", "()V");
        jobject obj_CtdStatusInfoPara = env->NewObject(class_CtdStatusInfoPara, id_CtdStatusInfoPara);

        id_fieldID = env->GetFieldID(class_CtdStatusInfoPara, "chPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdStatusInfoPara, id_fieldID, JNI_STR(env,pCtdStatusInfo->chPhone));

        id_fieldID = env->GetFieldID(class_CtdStatusInfoPara, "iCallId", "I");
        env->SetIntField(obj_CtdStatusInfoPara, id_fieldID, pCtdStatusInfo->iCallId);

        id_fieldID = env->GetFieldID(class_CtdStatusInfoPara, "iMemberId", "I");
        env->SetIntField(obj_CtdStatusInfoPara, id_fieldID, pCtdStatusInfo->iMemberId);

        id_fieldID = env->GetFieldID(class_CtdStatusInfoPara, "chStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdStatusInfoPara, id_fieldID, JNI_STR(env,pCtdStatusInfo->chStatus));

        id_fieldID = env->GetFieldID(class_CtdCallEventPara, "CtdStatusInfo", "Lcom/zte/softda/ocx/CTD_STATUS;");
        env->SetObjectField(obj_CtdCallEventPara, id_fieldID, obj_CtdStatusInfoPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_CtdCallEventPara);
    }
    else if(lType == CTD_TYPE_BIND)
    {
        CTD_BIND *pCtdBindInfo = NULL;

        pCtdBindInfo = (CTD_BIND*)pPara;
        if(pCtdBindInfo == NULL)
        {
            TraceMsgWindow1(1, "rcscontroller java_FireCtdCallEvent, pCtdBindInfo == NULL");
            return;
        }

        jclass class_CtdBindInfoPara = env->FindClass("com/zte/softda/ocx/CTD_BIND");
        jmethodID id_CtdBindInfoPara  = env->GetMethodID(class_CtdBindInfoPara, "<init>", "()V");
        jobject obj_CtdBindInfoPara = env->NewObject(class_CtdBindInfoPara, id_CtdBindInfoPara);

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "chPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdBindInfoPara, id_fieldID, JNI_STR(env,pCtdBindInfo->chPhone));

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "iResult", "I");
        env->SetIntField(obj_CtdBindInfoPara, id_fieldID, pCtdBindInfo->iResult);

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "chSoftPhoneNum", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdBindInfoPara, id_fieldID, JNI_STR(env,pCtdBindInfo->chSoftPhoneNum));

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "chSoftPhonePwd", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdBindInfoPara, id_fieldID, JNI_STR(env,pCtdBindInfo->chSoftPhonePwd));

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "chSsIpAddr", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdBindInfoPara, id_fieldID, JNI_STR(env,pCtdBindInfo->chSsIpAddr));

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "iSsPort", "I");
        env->SetIntField(obj_CtdBindInfoPara, id_fieldID, pCtdBindInfo->iSsPort);

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "chSsMeetingAddr", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdBindInfoPara, id_fieldID, JNI_STR(env,pCtdBindInfo->chSsMeetingAddr));

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "iSSMeetingPort", "I");
        env->SetIntField(obj_CtdBindInfoPara, id_fieldID, pCtdBindInfo->iSSMeetingPort);

        id_fieldID = env->GetFieldID(class_CtdBindInfoPara, "iDoType", "I");
        env->SetIntField(obj_CtdBindInfoPara, id_fieldID, pCtdBindInfo->iDoType);

        id_fieldID = env->GetFieldID(class_CtdCallEventPara, "CtdBindInfo", "Lcom/zte/softda/ocx/CTD_BIND;");
        env->SetObjectField(obj_CtdCallEventPara, id_fieldID, obj_CtdBindInfoPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_CtdCallEventPara);
    }
    else if(lType == CTD_TYPE_CALL)
    {
        CTD_CALL *pCtdCallInfo = NULL;

        pCtdCallInfo = (CTD_CALL*)pPara;
        if(pCtdCallInfo == NULL)
        {
            TraceMsgWindow1(1, "rcscontroller java_FireCtdCallEvent, pCtdCallInfo == NULL");
            return;
        }
        jclass class_CtdCallInfoPara = env->FindClass("com/zte/softda/ocx/CTD_CALL");
        jmethodID id_CtdCallInfoPara  = env->GetMethodID(class_CtdCallInfoPara, "<init>", "()V");
        jobject obj_CtdCallInfoPara = env->NewObject(class_CtdCallInfoPara, id_CtdCallInfoPara);

        id_fieldID = env->GetFieldID(class_CtdCallInfoPara, "chPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdCallInfoPara, id_fieldID, JNI_STR(env,pCtdCallInfo->chPhone));

        id_fieldID = env->GetFieldID(class_CtdCallInfoPara, "iCallId", "I");
        env->SetIntField(obj_CtdCallInfoPara, id_fieldID, pCtdCallInfo->iCallId);

        id_fieldID = env->GetFieldID(class_CtdCallInfoPara, "iCallType", "I");
        env->SetIntField(obj_CtdCallInfoPara, id_fieldID, pCtdCallInfo->iCallType);

        id_fieldID = env->GetFieldID(class_CtdCallInfoPara, "iResult", "I");
        env->SetIntField(obj_CtdCallInfoPara, id_fieldID, pCtdCallInfo->iResult);

        id_fieldID = env->GetFieldID(class_CtdCallEventPara, "CtdCallInfo", "Lcom/zte/softda/ocx/CTD_CALL;");
        env->SetObjectField(obj_CtdCallEventPara, id_fieldID, obj_CtdCallInfoPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_CtdCallEventPara);
    }
    else if(lType == CTD_TYPE_CTRL)
    {
        CTD_CALL_CTRL *pCtdCallCtrlInfo = NULL;

        pCtdCallCtrlInfo = (CTD_CALL_CTRL*)pPara;
        if(pCtdCallCtrlInfo == NULL)
        {
            TraceMsgWindow1(1, "rcscontroller java_FireCtdCallEvent, pCtdCallCtrlInfo == NULL");
            return;
        }
        jclass class_CtdCallCtrlInfoPara = env->FindClass("com/zte/softda/ocx/CTD_CALL_CTRL");
        jmethodID id_CtdCallCtrlInfoPara  = env->GetMethodID(class_CtdCallCtrlInfoPara, "<init>", "()V");
        jobject obj_CtdCallCtrlInfoPara = env->NewObject(class_CtdCallCtrlInfoPara, id_CtdCallCtrlInfoPara);

        id_fieldID = env->GetFieldID(class_CtdCallCtrlInfoPara, "lCtrlType", "J");
        env->SetLongField(obj_CtdCallCtrlInfoPara, id_fieldID, pCtdCallCtrlInfo->lCtrlType);

        id_fieldID = env->GetFieldID(class_CtdCallCtrlInfoPara, "iCallId", "I");
        env->SetIntField(obj_CtdCallCtrlInfoPara, id_fieldID, pCtdCallCtrlInfo->iCallId);

        id_fieldID = env->GetFieldID(class_CtdCallCtrlInfoPara, "chPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdCallCtrlInfoPara, id_fieldID, JNI_STR(env,pCtdCallCtrlInfo->chPhone));

        id_fieldID = env->GetFieldID(class_CtdCallCtrlInfoPara, "iMemberId", "I");
        env->SetIntField(obj_CtdCallCtrlInfoPara, id_fieldID, pCtdCallCtrlInfo->iMemberId);

        id_fieldID = env->GetFieldID(class_CtdCallCtrlInfoPara, "chForwardPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdCallCtrlInfoPara, id_fieldID, JNI_STR(env,pCtdCallCtrlInfo->chForwardPhone));

        id_fieldID = env->GetFieldID(class_CtdCallCtrlInfoPara, "iResult", "I");
        env->SetIntField(obj_CtdCallCtrlInfoPara, id_fieldID, pCtdCallCtrlInfo->iResult);

        id_fieldID = env->GetFieldID(class_CtdCallEventPara, "CtdCallCtrlInfo", "Lcom/zte/softda/ocx/CTD_CALL_CTRL;");
        env->SetObjectField(obj_CtdCallEventPara, id_fieldID, obj_CtdCallCtrlInfoPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_CtdCallEventPara);
    }
    else if(lType == CTD_TYPE_PROC)
    {
        CTD_IN_CALL_PROC *pCtdInCallProcInfo = NULL;

        pCtdInCallProcInfo = (CTD_IN_CALL_PROC*)pPara;
        if(pCtdInCallProcInfo == NULL)
        {
            TraceMsgWindow1(1, "rcscontroller java_FireCtdCallEvent, pCtdInCallProcInfo == NULL");
            return;
        }
        jclass class_CtdInCallProcInfoPara = env->FindClass("com/zte/softda/ocx/CTD_IN_CALL_PROC");
        jmethodID id_CtdInCallProcInfoPara  = env->GetMethodID(class_CtdInCallProcInfoPara, "<init>", "()V");
        jobject obj_CtdInCallProcInfoPara = env->NewObject(class_CtdInCallProcInfoPara, id_CtdInCallProcInfoPara);

        id_fieldID = env->GetFieldID(class_CtdInCallProcInfoPara, "iCallId", "I");
        env->SetIntField(obj_CtdInCallProcInfoPara, id_fieldID, pCtdInCallProcInfo->iCallId);

        id_fieldID = env->GetFieldID(class_CtdInCallProcInfoPara, "chPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdInCallProcInfoPara, id_fieldID, JNI_STR(env,pCtdInCallProcInfo->chPhone));

        id_fieldID = env->GetFieldID(class_CtdInCallProcInfoPara, "iMemberId", "I");
        env->SetIntField(obj_CtdInCallProcInfoPara, id_fieldID, pCtdInCallProcInfo->iMemberId);

        id_fieldID = env->GetFieldID(class_CtdInCallProcInfoPara, "lProcType", "J");
        env->SetLongField(obj_CtdInCallProcInfoPara, id_fieldID, pCtdInCallProcInfo->lProcType);

        id_fieldID = env->GetFieldID(class_CtdInCallProcInfoPara, "chForwardPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_CtdInCallProcInfoPara, id_fieldID, JNI_STR(env,pCtdInCallProcInfo->chForwardPhone));

        id_fieldID = env->GetFieldID(class_CtdInCallProcInfoPara, "iAnswerModal", "I");
        env->SetIntField(obj_CtdInCallProcInfoPara, id_fieldID, pCtdInCallProcInfo->iAnswerModal);

        id_fieldID = env->GetFieldID(class_CtdCallEventPara, "CtdInCallProcInfo", "Lcom/zte/softda/ocx/CTD_IN_CALL_PROC;");
        env->SetObjectField(obj_CtdCallEventPara, id_fieldID, obj_CtdInCallProcInfoPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_CtdCallEventPara);
    }
    else
    {          
        TraceMsgWindow1(1, "rcscontroller java_FireCtdCallEvent unkown lType[%d]", lType);
    }
}
void java_FireDispUICmdForEcp2Result(long lType, short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireDispUICmdForEcp2Result, lType[%d]", lType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireDispUICmdForEcp2Result, lType[%d]", lType);

    iresult = getStaticJniMethod((char *)"java_FireDispUICmdForEcp2Result", (char *)"(ILcom/zte/softda/ocx/FireDispUICmdForEcp2ResultPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireDispUICmdForEcp2Result");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireDispUICmdForEcp2Result");

        return;
    }

    jclass class_DispUICmdForEcp2ResultPara = env->FindClass("com/zte/softda/ocx/FireDispUICmdForEcp2ResultPara");
    jmethodID id_DispUICmdForEcp2ResultPara  = env->GetMethodID(class_DispUICmdForEcp2ResultPara, "<init>", "()V");
    jobject obj_DispUICmdForEcp2ResultPara = env->NewObject(class_DispUICmdForEcp2ResultPara, id_DispUICmdForEcp2ResultPara);

    if(lType == 1)//ReturnCode_Modify_MyPassword
    {
        ReturnCode_Modify_MyPassword *pReturnModifyMyPassword = NULL;
        pReturnModifyMyPassword = (ReturnCode_Modify_MyPassword *)pPara;
        if(pReturnModifyMyPassword == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireDispUICmdForEcp2Result pReturnModifyMyPassword == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireDispUICmdForEcp2Result pReturnModifyMyPassword == NULL");

            return;
        }
        jclass class_ReturnModifyMyPasswordPara = env->FindClass("com/zte/softda/ocx/FireReturnModifyMyPasswordPara");
        jmethodID id_ReturnModifyMyPasswordPara  = env->GetMethodID(class_ReturnModifyMyPasswordPara, "<init>", "()V");
        jobject obj_ReturnModifyMyPasswordPara = env->NewObject(class_ReturnModifyMyPasswordPara, id_ReturnModifyMyPasswordPara);

        id_fieldID = env->GetFieldID(class_ReturnModifyMyPasswordPara, "lCode", "I");
        env->SetIntField(obj_ReturnModifyMyPasswordPara, id_fieldID, pReturnModifyMyPassword->lCode);

        id_fieldID = env->GetFieldID(class_DispUICmdForEcp2ResultPara, "ReturnModifyMyPasswordPara", "Lcom/zte/softda/ocx/FireReturnModifyMyPasswordPara;");
        env->SetObjectField(obj_DispUICmdForEcp2ResultPara, id_fieldID, obj_ReturnModifyMyPasswordPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_DispUICmdForEcp2ResultPara);
    }
    else if(lType == 8)
    {
        ReturnCode_LoginInfo *pReturnLoginInfo = NULL;
        pReturnLoginInfo = (ReturnCode_LoginInfo *)pPara;

        if(pReturnLoginInfo == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireDispUICmdForEcp2Result pReturnLoginInfo == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireDispUICmdForEcp2Result pReturnLoginInfo == NULL");

            return;
        }
        jclass class_ReturnCodeLoginInfoPara = env->FindClass("com/zte/softda/ocx/FireReturnCodeLoginInfoPara");
        jmethodID id_ReturnCodeLoginInfoPara  = env->GetMethodID(class_ReturnCodeLoginInfoPara, "<init>", "()V");
        jobject obj_ReturnCodeLoginInfoPara = env->NewObject(class_ReturnCodeLoginInfoPara, id_ReturnCodeLoginInfoPara);

        id_fieldID = env->GetFieldID(class_ReturnCodeLoginInfoPara, "iReturnCode", "I");
        env->SetIntField(obj_ReturnCodeLoginInfoPara, id_fieldID, pReturnLoginInfo->iReturnCode);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_ReturnCodeLoginInfoPara);
    }
}
void java_FireIMSDispUICmdResult(int iType, short* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult, lType[%d]", iType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDispUICmdResult, iType[%d]", iType);

    iresult = getStaticJniMethod((char *)"java_FireIMSDispUICmdResult", (char *)"(ILcom/zte/softda/ocx/FireIMSDispUICmdResultPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDispUICmdResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDispUICmdResult");

        return;
    }

    jclass class_IMSDispUICmdResultPara = env->FindClass("com/zte/softda/ocx/FireIMSDispUICmdResultPara");
    jmethodID id_IMSDispUICmdResultPara  = env->GetMethodID(class_IMSDispUICmdResultPara, "<init>", "()V");
    jobject obj_IMSDispUICmdResultPara = env->NewObject(class_IMSDispUICmdResultPara, id_IMSDispUICmdResultPara);

    if (iType == 5)
    {
        IM_PREFERENCE_STRUCT *pImPreStructInfo = NULL;
        pImPreStructInfo = (IM_PREFERENCE_STRUCT *)pPara;

        if(pImPreStructInfo == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pImPreStructInfo == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSDispUICmdResult pImPreStructInfo == NULL");

            return;
        }

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult, iOperType[%d]", pImPreStructInfo->iOperType);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSDispUICmdResult, iOperType[%d]", pImPreStructInfo->iOperType);

        jclass class_ImPreStructInfoPara = env->FindClass("com/zte/softda/ocx/IM_PREFERENCE_STRUCT");
        jmethodID id_ImPreStructInfoPara  = env->GetMethodID(class_ImPreStructInfoPara, "<init>", "()V");
        jobject obj_ImPreStructInfoPara = env->NewObject(class_ImPreStructInfoPara, id_ImPreStructInfoPara);

        id_fieldID = env->GetFieldID(class_ImPreStructInfoPara, "iOperType", "I");
        env->SetIntField(obj_ImPreStructInfoPara, id_fieldID, pImPreStructInfo->iOperType);

        id_fieldID = env->GetFieldID(class_ImPreStructInfoPara, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_ImPreStructInfoPara, id_fieldID, JNI_STR(env,pImPreStructInfo->cURI));

        if((pImPreStructInfo->iOperType == 1) ||
           (pImPreStructInfo->iOperType == 3) ||
           (pImPreStructInfo->iOperType == 4))//��ʾ��ѯ��Ϣ��ҵ�����ѡ�����ý��
        {
            PERSON_IM_RELATED_INFO *pPersonImRelatedInfo = NULL;
            pPersonImRelatedInfo = (PERSON_IM_RELATED_INFO *)pImPreStructInfo->pAddrList;

            if(pPersonImRelatedInfo == NULL)
            {
                if(g_WriteLogType == 2)
                    TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pPersonImRelatedInfo == NULL");
                else if(g_WriteLogType == 1)
                    LOGI(TAG, "java_FireIMSDispUICmdResult pPersonImRelatedInfo == NULL");

                env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_ImPreStructInfoPara);
                return;
            }

            jclass class_PersonImRelatedInfoPara = env->FindClass("com/zte/softda/ocx/PERSON_IM_RELATED_INFO");
            jmethodID id_PersonImRelatedInfoPara  = env->GetMethodID(class_PersonImRelatedInfoPara, "<init>", "()V");
            jobject obj_PersonImRelatedInfoPara = env->NewObject(class_PersonImRelatedInfoPara, id_PersonImRelatedInfoPara);

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "iReturnCode", "I");
            env->SetIntField(obj_PersonImRelatedInfoPara, id_fieldID, pPersonImRelatedInfo->iReturnCode);

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "cBusinessPhone", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonImRelatedInfoPara, id_fieldID, JNI_STR(env,pPersonImRelatedInfo->cBusinessPhone));

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "cURI", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonImRelatedInfoPara, id_fieldID, JNI_STR(env,pPersonImRelatedInfo->cURI));

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "cUserCapacity", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonImRelatedInfoPara, id_fieldID, JNI_STR(env,pPersonImRelatedInfo->cUserCapacity));

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "iTransmitType", "I");
            env->SetIntField(obj_PersonImRelatedInfoPara, id_fieldID, pPersonImRelatedInfo->iTransmitType);

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "cTransmitPhone", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonImRelatedInfoPara, id_fieldID, JNI_STR(env,pPersonImRelatedInfo->cTransmitPhone));

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "cServicePhone", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonImRelatedInfoPara, id_fieldID, JNI_STR(env,pPersonImRelatedInfo->cServicePhone));

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "nStatusNum", "I");
            env->SetIntField(obj_PersonImRelatedInfoPara, id_fieldID, pPersonImRelatedInfo->nStatusNum);

            jclass class_string = env->FindClass("java/lang/String");
            jobjectArray array_TransmitStatus = env->NewObjectArray(pPersonImRelatedInfo->nStatusNum, class_string, NULL);

            for(i = 0; i < pPersonImRelatedInfo->nStatusNum; i++)
            {
                char cStatus[17] = {0};
                strncpy(cStatus, (const char*)&pPersonImRelatedInfo->vTransmitStatus[i], 16);
//			  env->SetObjectArrayElement(array_TransmitStatus, i, JNI_STR(env,pPersonImRelatedInfo->vTransmitStatus[i]));
                env->SetObjectArrayElement(array_TransmitStatus, i, JNI_STR(env,cStatus));
            }
            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "vTransmitStatus", "[Ljava/lang/String;");
            env->SetObjectField(obj_PersonImRelatedInfoPara, id_fieldID,	array_TransmitStatus);

            id_fieldID = env->GetFieldID(class_PersonImRelatedInfoPara, "bStatusChange", "I");
            env->SetIntField(obj_PersonImRelatedInfoPara, id_fieldID, pPersonImRelatedInfo->bStatusChange);
        }
        else if(pImPreStructInfo->iOperType == 2)
        {

            PERSON_IM_RELATED_INFO *pPersonImRelatedInfo = NULL;
            pPersonImRelatedInfo = (PERSON_IM_RELATED_INFO *)pImPreStructInfo->pAddrList;

            if(pPersonImRelatedInfo == NULL)
            {
                if(g_WriteLogType == 2)
                    TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pPersonImRelatedInfo == NULL");
                else if(g_WriteLogType == 1)
                    LOGI(TAG, "java_FireIMSDispUICmdResult pPersonImRelatedInfo == NULL");

                env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_ImPreStructInfoPara);
                return;
            }
        }
        else if(pImPreStructInfo->iOperType == 5)
        {

            PERSON_CALL_INFO *pPersonCallInfo = NULL;
            pPersonCallInfo = (PERSON_CALL_INFO *)pImPreStructInfo->pAddrList;

            if(pPersonCallInfo == NULL)
            {
                if(g_WriteLogType == 2)
                    TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pPersonCallInfo == NULL");
                else if(g_WriteLogType == 1)
                    LOGI(TAG, "java_FireIMSDispUICmdResult pPersonCallInfo == NULL");

                env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_ImPreStructInfoPara);
                return;
            }

            jclass class_PersonCallInfoPara = env->FindClass("com/zte/softda/ocx/PERSON_CALL_INFO");
            jmethodID id_PersonCallInfoPara  = env->GetMethodID(class_PersonCallInfoPara, "<init>", "()V");
            jobject obj_PersonCallInfoPara = env->NewObject(class_PersonCallInfoPara, id_PersonCallInfoPara);

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "iReturnCode", "I");
            env->SetIntField(obj_PersonCallInfoPara, id_fieldID, pPersonCallInfo->iReturnCode);

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cOfficeTel", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cOfficeTel));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cMobileTel", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cMobileTel));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cSecondMobileTel", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cSecondMobileTel));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cBusinessPhone", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cBusinessPhone));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cBusinessSms", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cBusinessSms));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cBusinessFax", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cBusinessFax));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cURI", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cURI));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cName", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cName));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cName1", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cName1));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "iVersionType", "I");
            env->SetIntField(obj_PersonCallInfoPara, id_fieldID, pPersonCallInfo->iVersionType);

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "nBusinessType", "I");
            env->SetIntField(obj_PersonCallInfoPara, id_fieldID, pPersonCallInfo->nBusinessType);

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cAreaCode", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cAreaCode));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cCapacityFlag", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cCapacityFlag));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cRuleCapacity", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cRuleCapacity));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cUserCapacity", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cUserCapacity));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "nAudioConfMaxMember", "I");
            env->SetIntField(obj_PersonCallInfoPara, id_fieldID, pPersonCallInfo->nAudioConfMaxMember);

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "nVideoConfMaxMember", "I");
            env->SetIntField(obj_PersonCallInfoPara, id_fieldID, pPersonCallInfo->nVideoConfMaxMember);

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "cSoftPhone", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonCallInfoPara, id_fieldID, JNI_STR(env,pPersonCallInfo->cSoftPhone));

            id_fieldID = env->GetFieldID(class_PersonCallInfoPara, "iLocalFlag", "I");
            env->SetIntField(obj_PersonCallInfoPara, id_fieldID, pPersonCallInfo->iLocalFlag);

            id_fieldID = env->GetFieldID(class_ImPreStructInfoPara, "PersonCallInfo", "Lcom/zte/softda/ocx/PERSON_CALL_INFO;");
            env->SetObjectField(obj_ImPreStructInfoPara, id_fieldID, obj_PersonCallInfoPara);

            id_fieldID = env->GetFieldID(class_IMSDispUICmdResultPara, "ImpreStruct", "Lcom/zte/softda/ocx/IM_PREFERENCE_STRUCT;");
            env->SetObjectField(obj_IMSDispUICmdResultPara, id_fieldID, obj_ImPreStructInfoPara);
        }
        else if(pImPreStructInfo->iOperType == 6)
        {

            PERSON_MMC_INFO *pPersonMMCInfo = NULL;
            pPersonMMCInfo = (PERSON_MMC_INFO *)pImPreStructInfo->pAddrList;

            if(pPersonMMCInfo == NULL)
            {
                    TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pPersonMMCInfo == NULL");
                env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_ImPreStructInfoPara);
                return;
            }

            TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult, iOperType[%d] iReturnCode[%d] cURI[%s] cAreaCode[%s] "
                "cUserCapacity[%s] nAudioConfMaxMember[%d] nVideoConfMaxMember[%d] nDataConfMaxMember[%d] nConfMaxDuration[%d] ", 
                pImPreStructInfo->iOperType, pPersonMMCInfo->iReturnCode, pPersonMMCInfo->cURI, pPersonMMCInfo->cAreaCode,
                pPersonMMCInfo->cUserCapacity, pPersonMMCInfo->nAudioConfMaxMember, pPersonMMCInfo->nVideoConfMaxMember, 
                pPersonMMCInfo->nDataConfMaxMember, pPersonMMCInfo->nConfMaxDuration);

            jclass class_PersonMMCInfoPara = env->FindClass("com/zte/softda/ocx/PERSON_MMC_INFO");
            jmethodID id_PersonMMCInfoPara  = env->GetMethodID(class_PersonMMCInfoPara, "<init>", "()V");
            jobject obj_PersonMMCInfoPara = env->NewObject(class_PersonMMCInfoPara, id_PersonMMCInfoPara);

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "iReturnCode", "I");
            env->SetIntField(obj_PersonMMCInfoPara, id_fieldID, pPersonMMCInfo->iReturnCode);

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "cBusinessPhone", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonMMCInfoPara, id_fieldID, JNI_STR(env,pPersonMMCInfo->cBusinessPhone));

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "cURI", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonMMCInfoPara, id_fieldID, JNI_STR(env,pPersonMMCInfo->cURI));

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "cName", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonMMCInfoPara, id_fieldID, JNI_STR(env,pPersonMMCInfo->cName));

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "cAreaCode", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonMMCInfoPara, id_fieldID, JNI_STR(env,pPersonMMCInfo->cAreaCode));

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "cUserCapacity", "Ljava/lang/String;");
            env->SetObjectField(obj_PersonMMCInfoPara, id_fieldID, JNI_STR(env,pPersonMMCInfo->cUserCapacity));

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "nAudioConfMaxMember", "I");
            env->SetIntField(obj_PersonMMCInfoPara, id_fieldID, pPersonMMCInfo->nAudioConfMaxMember);

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "nVideoConfMaxMember", "I");
            env->SetIntField(obj_PersonMMCInfoPara, id_fieldID, pPersonMMCInfo->nVideoConfMaxMember);

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "nDataConfMaxMember", "I");
            env->SetIntField(obj_PersonMMCInfoPara, id_fieldID, pPersonMMCInfo->nDataConfMaxMember);

            id_fieldID = env->GetFieldID(class_PersonMMCInfoPara, "nConfMaxDuration", "I");
            env->SetIntField(obj_PersonMMCInfoPara, id_fieldID, pPersonMMCInfo->nConfMaxDuration);

            id_fieldID = env->GetFieldID(class_ImPreStructInfoPara, "PersonMMCInfo", "Lcom/zte/softda/ocx/PERSON_MMC_INFO;");
            env->SetObjectField(obj_ImPreStructInfoPara, id_fieldID, obj_PersonMMCInfoPara);

            id_fieldID = env->GetFieldID(class_IMSDispUICmdResultPara, "ImpreStruct", "Lcom/zte/softda/ocx/IM_PREFERENCE_STRUCT;");
            env->SetObjectField(obj_IMSDispUICmdResultPara, id_fieldID, obj_ImPreStructInfoPara);
        }
    }
    else if (iType == 10)
    {
        DEPT_FAVORITE_EVENT_PARAM *pDeptFavEventInfo = NULL;
        pDeptFavEventInfo = (DEPT_FAVORITE_EVENT_PARAM *)pPara;

        if(pDeptFavEventInfo == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pDeptFavEventInfo == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSDispUICmdResult pDeptFavEventInfo == NULL");

            return;
        }

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult, nType[%d] iCode[%d] iNum[%d]",
                            pDeptFavEventInfo->nType, pDeptFavEventInfo->iCode, pDeptFavEventInfo->iNum);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSDispUICmdResult, nType[%d] iCode[%d] iNum[%d]",
                 pDeptFavEventInfo->nType, pDeptFavEventInfo->iCode, pDeptFavEventInfo->iNum);

        jclass class_DeptFavEventPara = env->FindClass("com/zte/softda/ocx/DEPT_FAVORITE_EVENT_PARAM");
        jmethodID id_DeptFavEventPara  = env->GetMethodID(class_DeptFavEventPara, "<init>", "()V");
        jobject obj_DeptFavEventPara = env->NewObject(class_DeptFavEventPara, id_DeptFavEventPara);

        id_fieldID = env->GetFieldID(class_DeptFavEventPara, "nType", "I");
        env->SetIntField(obj_DeptFavEventPara, id_fieldID, pDeptFavEventInfo->nType);

        id_fieldID = env->GetFieldID(class_DeptFavEventPara, "iCode", "I");
        env->SetIntField(obj_DeptFavEventPara, id_fieldID, pDeptFavEventInfo->iCode);

        id_fieldID = env->GetFieldID(class_DeptFavEventPara, "iNum", "I");
        env->SetIntField(obj_DeptFavEventPara, id_fieldID, pDeptFavEventInfo->iNum);

        DEPT_FAVORITE *pDeptFavInfo = NULL, *pAllDeptFavInfo = NULL;
        jclass class_FireDeptFavInfo = env->FindClass("com/zte/softda/ocx/DEPT_FAVORITE");
        jmethodID id_FireDeptFavInfo  = env->GetMethodID(class_FireDeptFavInfo, "<init>", "()V");
        jobjectArray array_FireDeptFavInfo = env->NewObjectArray(pDeptFavEventInfo->iNum, class_FireDeptFavInfo, NULL);
        if(pDeptFavEventInfo->iNum)
        {
            pAllDeptFavInfo = (DEPT_FAVORITE *)pDeptFavEventInfo->pData;
            for(i = 0; i < pDeptFavEventInfo->iNum; i++)
            {
                pDeptFavInfo = (DEPT_FAVORITE *) & (pAllDeptFavInfo[i]);

                jobject obj_FireDeptFavInfo = env->NewObject(class_FireDeptFavInfo, id_FireDeptFavInfo);

                id_fieldID = env->GetFieldID(class_FireDeptFavInfo, "nType", "I");
                env->SetIntField(obj_FireDeptFavInfo, id_fieldID, pDeptFavInfo->nType);

                id_fieldID = env->GetFieldID(class_FireDeptFavInfo, "cName", "Ljava/lang/String;");
                env->SetObjectField(obj_FireDeptFavInfo, id_fieldID, JNI_STR(env,pDeptFavInfo->cName));

                id_fieldID = env->GetFieldID(class_FireDeptFavInfo, "cURI", "Ljava/lang/String;");
                env->SetObjectField(obj_FireDeptFavInfo, id_fieldID, JNI_STR(env,pDeptFavInfo->cURI));

                id_fieldID = env->GetFieldID(class_FireDeptFavInfo, "nAutoExpand", "I");
                env->SetIntField(obj_FireDeptFavInfo, id_fieldID, pDeptFavInfo->nAutoExpand);

                env->SetObjectArrayElement(array_FireDeptFavInfo, i, obj_FireDeptFavInfo);
                env->DeleteLocalRef(obj_FireDeptFavInfo);             
            }
        }
        id_fieldID = env->GetFieldID(class_DeptFavEventPara, "pData", "[Lcom/zte/softda/ocx/DEPT_FAVORITE;");
        env->SetObjectField(obj_DeptFavEventPara, id_fieldID, array_FireDeptFavInfo);

        id_fieldID = env->GetFieldID(class_IMSDispUICmdResultPara, "DeptFavEventInfo", "Lcom/zte/softda/ocx/DEPT_FAVORITE_EVENT_PARAM;");
        env->SetObjectField(obj_IMSDispUICmdResultPara, id_fieldID, obj_DeptFavEventPara);
    }
    else if (iType == 18)
    {
        GROUP_CLASS *pGroupClassInfo = NULL;
        pGroupClassInfo = (GROUP_CLASS *)pPara;

        if(pGroupClassInfo == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pGroupClassInfo == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSDispUICmdResult pGroupClassInfo == NULL");

            return;
        }

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult, cGroupURI[%s] cIdentifier[%s] cGroupClassID[%s] cGroupClassName[%s] iResultCode[%d]",
                            pGroupClassInfo->cGroupURI, pGroupClassInfo->cIdentifier, pGroupClassInfo->cGroupClassID,
                            pGroupClassInfo->cGroupClassName, pGroupClassInfo->iResultCode);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSDispUICmdResult, cGroupURI[%s] cIdentifier[%s] cGroupClassID[%s] cGroupClassName[%s] iResultCode[%d]",
                 pGroupClassInfo->cGroupURI, pGroupClassInfo->cIdentifier, pGroupClassInfo->cGroupClassID,
                 pGroupClassInfo->cGroupClassName, pGroupClassInfo->iResultCode);

        jclass class_GroupClassInfoPara = env->FindClass("com/zte/softda/ocx/GROUP_CLASS");
        jmethodID id_GroupClassInfoPara  = env->GetMethodID(class_GroupClassInfoPara, "<init>", "()V");
        jobject obj_GroupClassInfoPara = env->NewObject(class_GroupClassInfoPara, id_GroupClassInfoPara);

        id_fieldID = env->GetFieldID(class_GroupClassInfoPara, "cGroupURI", "Ljava/lang/String;");
        env->SetObjectField(obj_GroupClassInfoPara, id_fieldID, JNI_STR(env,pGroupClassInfo->cGroupURI));

        id_fieldID = env->GetFieldID(class_GroupClassInfoPara, "cIdentifier", "Ljava/lang/String;");
        env->SetObjectField(obj_GroupClassInfoPara, id_fieldID, JNI_STR(env,pGroupClassInfo->cIdentifier));

        id_fieldID = env->GetFieldID(class_GroupClassInfoPara, "cGroupClassID", "Ljava/lang/String;");
        env->SetObjectField(obj_GroupClassInfoPara, id_fieldID, JNI_STR(env,pGroupClassInfo->cGroupClassID));

        id_fieldID = env->GetFieldID(class_GroupClassInfoPara, "cGroupClassName", "Ljava/lang/String;");
        env->SetObjectField(obj_GroupClassInfoPara, id_fieldID, JNI_STR(env,pGroupClassInfo->cGroupClassName));

        id_fieldID = env->GetFieldID(class_GroupClassInfoPara, "iResultCode", "I");
        env->SetIntField(obj_GroupClassInfoPara, id_fieldID, pGroupClassInfo->iResultCode);

        id_fieldID = env->GetFieldID(class_IMSDispUICmdResultPara, "GroupClassInfo", "Lcom/zte/softda/ocx/GROUP_CLASS;");
        env->SetObjectField(obj_IMSDispUICmdResultPara, id_fieldID, obj_GroupClassInfoPara);
    }
    else if (iType == 20) //http��ʽ�ҵ�������
    {
        HTTP_HANGUP_CONF_EVENT_PARAM *pHttpHangUpConfInfo = NULL;
        pHttpHangUpConfInfo = (HTTP_HANGUP_CONF_EVENT_PARAM *)pPara;

        if(pHttpHangUpConfInfo == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult pHttpHangUpConfInfo == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSDispUICmdResult pHttpHangUpConfInfo == NULL");

            return;
        }

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSDispUICmdResult, cConfURI[%s] iResultCode[%d]",
                            pHttpHangUpConfInfo->cConfURI, pHttpHangUpConfInfo->iResultCode);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSDispUICmdResult, cConfURI[%s] iResultCode[%d]",
                 pHttpHangUpConfInfo->cConfURI, pHttpHangUpConfInfo->iResultCode);

        jclass class_HttpHangUpConfInfoPara = env->FindClass("com/zte/softda/ocx/HTTP_HANGUP_CONF_EVENT_PARAM");
        jmethodID id_HttpHangUpConfInfoPara  = env->GetMethodID(class_HttpHangUpConfInfoPara, "<init>", "()V");
        jobject obj_HttpHangUpConfInfoPara = env->NewObject(class_HttpHangUpConfInfoPara, id_HttpHangUpConfInfoPara);

        id_fieldID = env->GetFieldID(class_HttpHangUpConfInfoPara, "cConfURI", "Ljava/lang/String;");
        env->SetObjectField(obj_HttpHangUpConfInfoPara, id_fieldID, JNI_STR(env,pHttpHangUpConfInfo->cConfURI));

        id_fieldID = env->GetFieldID(class_HttpHangUpConfInfoPara, "iResultCode", "I");
        env->SetIntField(obj_HttpHangUpConfInfoPara, id_fieldID, pHttpHangUpConfInfo->iResultCode);

        id_fieldID = env->GetFieldID(class_IMSDispUICmdResultPara, "HttpHangUpConfInfo", "Lcom/zte/softda/ocx/HTTP_HANGUP_CONF_EVENT_PARAM;");
        env->SetObjectField(obj_IMSDispUICmdResultPara, id_fieldID, obj_HttpHangUpConfInfoPara);
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_IMSDispUICmdResultPara);
}
void java_FireIMSCreatePrivateGroupResult(LPCTSTR szGroupName, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSCreatePrivateGroupResult szGroupName[%s] iResult[%d]", szGroupName, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSCreatePrivateGroupResult szGroupName[%s] iResult[%d]", szGroupName, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSCreatePrivateGroupResult", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSCreatePrivateGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSCreatePrivateGroupResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,szGroupName), iResult);
}

void java_FireIMSModifyPrivateGroupNameResult(LPCTSTR szOldName, LPCTSTR szNewName, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSModifyPrivateGroupNameResult szOldName[%s] szNewName[%s] iResult[%d]",
                        szOldName, szNewName, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSModifyPrivateGroupNameResult szOldName[%s] szNewName[%s] iResult[%d]", szOldName, szNewName, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSModifyPrivateGroupNameResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSModifyPrivateGroupNameResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSModifyPrivateGroupNameResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,szOldName), JNI_STR(env,szNewName), iResult);
}
void java_FireIMSDeletePrivateGroupResult(LPCTSTR szGroupName, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDeletePrivateGroupResult szGroupName[%s] iResult[%d]", szGroupName, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDeletePrivateGroupResult szGroupName[%s] iResult[%d]", szGroupName, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSDeletePrivateGroupResult", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDeletePrivateGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDeletePrivateGroupResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,szGroupName), iResult);
}
void java_FireIMSGotOnePrivateList(LPCTSTR szGroupName, int iListNum, short far *pListURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    jstring jstr;

    char *pMemberInfo = NULL, *pMemberInfoTemp = NULL;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOnePrivateList [%d]", iListNum);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotOnePrivateList [%d]", iListNum);

    pMemberInfo = (char*)pListURI;
    if(pMemberInfo == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSGotOnePrivateList pMemberInfo == NULL");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSGotOnePrivateList pMemberInfo == NULL");

        return;
    }

    iresult = getStaticJniMethod((char *)"java_FireIMSGotOnePrivateList", (char *)"(Ljava/lang/String;I[Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotOnePrivateList");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotOnePrivateList");

        return;
    }

    jclass class_string = env->FindClass("java/lang/String");
    jobjectArray array_members = env->NewObjectArray(iListNum, class_string, NULL);

    for (int i = 0; i < iListNum; i++)
    {
        pMemberInfoTemp = pMemberInfo + i * (MAX_IMS_URI_LEN + 1);
        jstr = JNI_STR(env,pMemberInfoTemp);

//		memcpy(pListURI+i*(MAX_IMS_URI_LEN+1), pTemp->cURI, strlen(pTemp->cURI));
#if 0
        jstr = env->NewStringUTF(pMemberInfo);//���µ�¼�е�ʱ��core������
        pMemberInfo += (MAX_IMS_URI_LEN + 1);
#endif
        env->SetObjectArrayElement(array_members, i, jstr);
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,szGroupName), iListNum, array_members);
}
void java_FireIMSCopyPrivateGroupListResult(LPCTSTR pGroupName, long iGroupType, LPCTSTR pListURI, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSCopyPrivateGroupListResult pGroupName[%s] iGroupType[%d] pListURI[%s] iResult[%d]",
                        pGroupName, iGroupType, pListURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSCopyPrivateGroupListResult pGroupName[%s] iGroupType[%d] pListURI[%s] iResult[%d]", pGroupName, iGroupType, pListURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSCopyPrivateGroupListResult", (char *)"(Ljava/lang/String;ILjava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSCopyPrivateGroupListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSCopyPrivateGroupListResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupName), iGroupType, JNI_STR(env,pListURI), iResult);
}
void java_FireIMSDelPrivateGroupListResult(LPCTSTR pGroupName, LPCTSTR pListURI, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDelPrivateGroupListResult pGroupName[%s]	pListURI[%s] iResult[%d]",
                        pGroupName, pListURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDelPrivateGroupListResult pGroupName[%s]	pListURI[%s] iResult[%d]", pGroupName, pListURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSDelPrivateGroupListResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDelPrivateGroupListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDelPrivateGroupListResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupName), JNI_STR(env,pListURI), iResult);
}
void java_FireIMSRuleSetEvent(long lType, short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    int i;
    LP_RULE_SET_EVENT pRuleSetEvent = NULL;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSRuleSetEvent, lType[%d]", lType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSRuleSetEvent, lType[%d]", lType);

    iresult = getStaticJniMethod((char *)"java_FireIMSRuleSetEvent", (char *)"(ILcom/zte/softda/ocx/FireIMSRuleSetEventPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSRuleSetEvent");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSRuleSetEvent");

        return;
    }
    pRuleSetEvent = (LP_RULE_SET_EVENT)pPara;
    if(pRuleSetEvent == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSRuleSetEvent pRuleSetEvent == NULL");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSRuleSetEvent pRuleSetEvent == NULL");

        return;
    }

    jclass class_RuleSetEvent = env->FindClass("com/zte/softda/ocx/FireIMSRuleSetEventPara");
    jmethodID id_RuleSetEvent	= env->GetMethodID(class_RuleSetEvent, "<init>", "()V");
    jobject obj_RuleSetEvent = env->NewObject(class_RuleSetEvent, id_RuleSetEvent);

    id_fieldID = env->GetFieldID(class_RuleSetEvent, "lResultCode", "I");
    env->SetIntField(obj_RuleSetEvent, id_fieldID, pRuleSetEvent->lResultCode);

    id_fieldID = env->GetFieldID(class_RuleSetEvent, "lOpeType", "I");
    env->SetIntField(obj_RuleSetEvent, id_fieldID, pRuleSetEvent->lOpeType);

    id_fieldID = env->GetFieldID(class_RuleSetEvent, "lNum", "I");
    env->SetIntField(obj_RuleSetEvent, id_fieldID, pRuleSetEvent->lNum);

    jclass class_RuleSetPara = env->FindClass("com/zte/softda/ocx/RuleSetPara");
    jmethodID id_RuleSetPara  = env->GetMethodID(class_RuleSetPara, "<init>", "()V");
    jobjectArray array_RuleSetPara = env->NewObjectArray(pRuleSetEvent->lNum, class_RuleSetPara, NULL);
    for(i = 0; i < pRuleSetEvent->lNum; i++)
    {
        RULE_SET_EVENT_INFO* pRuleSetPara = &pRuleSetEvent->pInfo[i];
        jobject obj_RuleSetPara = env->NewObject(class_RuleSetPara, id_RuleSetPara);

        id_fieldID = env->GetFieldID(class_RuleSetPara, "lType", "I");
        env->SetIntField(obj_RuleSetPara, id_fieldID, pRuleSetPara->lType);

        id_fieldID = env->GetFieldID(class_RuleSetPara, "cID", "Ljava/lang/String;");
        env->SetObjectField(obj_RuleSetPara, id_fieldID, JNI_STR(env,pRuleSetPara->cID));

        id_fieldID = env->GetFieldID(class_RuleSetPara, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_RuleSetPara, id_fieldID, JNI_STR(env,pRuleSetPara->cURI));

        env->SetObjectArrayElement(array_RuleSetPara, i, obj_RuleSetPara);
    }
    id_fieldID = env->GetFieldID(class_RuleSetEvent, "pInfo", "[Lcom/zte/softda/ocx/RuleSetPara;");
    env->SetObjectField(obj_RuleSetEvent, id_fieldID, array_RuleSetPara);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, lType, obj_RuleSetEvent);
}
void java_FireIMSCreateChatRoomResult(LPCTSTR pDlgID, long iResult, LPCTSTR pSubject, LPCTSTR pChatRoomURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSCreateChatRoomResult pDlgID[%s] iResult[%d] pSubject[%s] pChatRoomURI[%s]",
                        pDlgID, iResult, pSubject, pChatRoomURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSCreateChatRoomResult pDlgID[%s] iResult[%d] pSubject[%s] pChatRoomURI[%s]", pDlgID, iResult, pSubject, pChatRoomURI);

    iresult = getStaticJniMethod((char *)"java_FireIMSCreateChatRoomResult", (char *)"(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSCreateChatRoomResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSCreateChatRoomResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pDlgID), iResult, JNI_STR(env,pSubject),  JNI_STR(env,pChatRoomURI));
}
void java_FireIMSGotChatRoomUserInfo(short *info)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;

    CONFERENCE_INFO *pConInfo = (CONFERENCE_INFO *)info;
    GET_PUB_CHATROOM_USERS_INFO *pUsers = pConInfo->pChatRoomUsers;
    if(pConInfo == NULL || pUsers == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSGotChatRoomUserInfo pConInfo == NULL || pUsers == NULL");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSGotChatRoomUserInfo pConInfo == NULL || pUsers == NULL ");

        return;
    }

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotChatRoomUserInfo");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGotChatRoomUserInfo");

    iresult = getStaticJniMethod((char *)"java_FireIMSGotChatRoomUserInfo", (char *)"(Lcom/zte/softda/ocx/FireConInfoPara;[Lcom/zte/softda/ocx/FireChatRoomUserInfoPara;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotChatRoomUserInfo");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotChatRoomUserInfo");

        return;
    }

    jclass class_ConInfo = env->FindClass("com/zte/softda/ocx/FireConInfoPara");
    jmethodID id_ConInfo  = env->GetMethodID(class_ConInfo, "<init>", "()V");
    jobject obj_ConInfo = env->NewObject(class_ConInfo, id_ConInfo);

    jstring cDlgId = env->NewStringUTF(pConInfo->cDlgId);
    id_fieldID = env->GetFieldID(class_ConInfo, "cDlgId", "Ljava/lang/String;");
    env->SetObjectField(obj_ConInfo, id_fieldID, cDlgId);
    env->DeleteLocalRef(cDlgId);

    jstring cConfURI = env->NewStringUTF(pConInfo->cConfURI);
    id_fieldID = env->GetFieldID(class_ConInfo, "cConfURI", "Ljava/lang/String;");
    env->SetObjectField(obj_ConInfo, id_fieldID, cConfURI);
    env->DeleteLocalRef(cConfURI);

    jstring cSubject = env->NewStringUTF(pConInfo->cSubject);
    id_fieldID = env->GetFieldID(class_ConInfo, "cSubject", "Ljava/lang/String;");
    env->SetObjectField(obj_ConInfo, id_fieldID, cSubject);
    env->DeleteLocalRef(cSubject);

    id_fieldID = env->GetFieldID(class_ConInfo, "iMaxUser", "I");
    env->SetIntField(obj_ConInfo, id_fieldID, (jint)pConInfo->iMaxUser);

    id_fieldID = env->GetFieldID(class_ConInfo, "iActive", "I");
    env->SetIntField(obj_ConInfo, id_fieldID, (jint)pConInfo->iActive);

    id_fieldID = env->GetFieldID(class_ConInfo, "iLocked", "I");
    env->SetIntField(obj_ConInfo, id_fieldID, (jint)pConInfo->iLocked);

    id_fieldID = env->GetFieldID(class_ConInfo, "iUserNum", "I");
    env->SetIntField(obj_ConInfo, id_fieldID, (jint)pConInfo->iUserNum);

    id_fieldID = env->GetFieldID(class_ConInfo, "iThisUserNum", "I");
    env->SetIntField(obj_ConInfo, id_fieldID, (jint)pConInfo->iThisUserNum);

    int nCount = pConInfo->iThisUserNum;
    jclass class_userInfo = env->FindClass("com/zte/softda/ocx/FireChatRoomUserInfoPara");
    jobjectArray array_members = env->NewObjectArray(nCount, class_userInfo, NULL);

    for (int i = 0; i < nCount; i++)
    {
        jmethodID id_userInfo  = env->GetMethodID(class_userInfo, "<init>", "()V");
        jobject obj_userInfo = env->NewObject(class_userInfo, id_userInfo);

        jstring cConfURI = env->NewStringUTF(pUsers->cConfURI);
        id_fieldID = env->GetFieldID(class_userInfo, "cConfURI", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cConfURI);
        env->DeleteLocalRef(cConfURI);

        jstring cUserURI = env->NewStringUTF(pUsers->cUserURI);
        id_fieldID = env->GetFieldID(class_userInfo, "cUserURI", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cUserURI);
        env->DeleteLocalRef(cUserURI);

        jstring cUserRole = env->NewStringUTF(pUsers->cUserRole);
        id_fieldID = env->GetFieldID(class_userInfo, "cUserRole", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cUserRole);
        env->DeleteLocalRef(cUserRole);

        jstring cUserStatus = env->NewStringUTF(pUsers->cUserStatus);
        id_fieldID = env->GetFieldID(class_userInfo, "cUserStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cUserStatus);
        env->DeleteLocalRef(cUserStatus);

        jstring cWhenJoin = env->NewStringUTF(pUsers->cWhenJoin);
        id_fieldID = env->GetFieldID(class_userInfo, "cWhenJoin", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cWhenJoin);
        env->DeleteLocalRef(cWhenJoin);

        jstring cDiscMethod = env->NewStringUTF(pUsers->cDiscMethod);
        id_fieldID = env->GetFieldID(class_userInfo, "cDiscMethod", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cDiscMethod);
        env->DeleteLocalRef(cDiscMethod);

        jstring cWhenDisc = env->NewStringUTF(pUsers->cWhenDisc);
        id_fieldID = env->GetFieldID(class_userInfo, "cWhenDisc", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cWhenDisc);
        env->DeleteLocalRef(cWhenDisc);

        jstring cInviterURI = env->NewStringUTF(pUsers->cInviterURI);
        id_fieldID = env->GetFieldID(class_userInfo, "cInviterURI", "Ljava/lang/String;");
        env->SetObjectField(obj_userInfo, id_fieldID, cInviterURI);
        env->DeleteLocalRef(cInviterURI);

        env->SetObjectArrayElement(array_members, i, obj_userInfo);
        env->DeleteLocalRef(obj_userInfo);

        pUsers++;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_ConInfo, array_members);
}
void java_FireIMSInvitedToChatRoom( LPCTSTR pCreaterURI, LPCTSTR pChatRoomURI, LPCTSTR pSubject )
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSInvitedToChatRoom pCreaterURI[%s] pChatRoomURI[%s] pSubject[%s]",
                        pCreaterURI, pChatRoomURI, pSubject);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSInvitedToChatRoom pCreaterURI[%s] pChatRoomURI[%s] pSubject[%s]", pCreaterURI, pChatRoomURI, pSubject);

    iresult = getStaticJniMethod((char *)"java_FireIMSInvitedToChatRoom", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSInvitedToChatRoom");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSInvitedToChatRoom");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pCreaterURI), JNI_STR(env,pChatRoomURI), JNI_STR(env,pSubject));
}
void java_FireIMSJoinChatRoomResult( long iResult, LPCTSTR pSubject, LPCTSTR pChatRoomURI )
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSJoinChatRoomResult iResult[%d] pSubject[%s] pChatRoomURI[%s]",
                        iResult, pSubject, pChatRoomURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSJoinChatRoomResult iResult[%d] pSubject[%s] pChatRoomURI[%s]", iResult, pSubject, pChatRoomURI);

    iresult = getStaticJniMethod((char *)"java_FireIMSJoinChatRoomResult", (char *)"(ILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSInvitedToChatRoom");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSInvitedToChatRoom");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, JNI_STR(env,pSubject), JNI_STR(env,pChatRoomURI));
}
void java_FireIMSLeaveChatRoom( LPCTSTR pChatRoomURI )
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSLeaveChatRoom pChatRoomURI[%s]", pChatRoomURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSLeaveChatRoom pChatRoomURI[%s]", pChatRoomURI);

    iresult = getStaticJniMethod((char *)"java_FireIMSLeaveChatRoom", (char *)"(Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSLeaveChatRoom");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSLeaveChatRoom");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pChatRoomURI));
}
void java_FireIMSGotDeptAddrListResult( long iType, long iCode, long iNum, short far* pAddrList, LPCTSTR pNodeURI)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;

    TraceMsgWindow1(1, "rcscontroller java_FireIMSGotDeptAddrListResult iType[%d] iCode[%d] pNodeURI[%s]", 
        iType, iCode, pNodeURI);

    iresult = getStaticJniMethod((char *)"java_FireIMSGotDeptAddrListResult", (char *)"(IIILjava/lang/Object;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotDeptAddrListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGotDeptAddrListResult");

        return;
    }

    jclass class_return = env->FindClass("com/zte/softda/ocx/FireDepartAddPara");
    jmethodID id_return  = env->GetMethodID(class_return, "<init>", "()V");
    jobject obj_return = env->NewObject(class_return, id_return);

    id_fieldID = env->GetFieldID(class_return, "nodeUri", "Ljava/lang/String;");
    env->SetObjectField(obj_return, id_fieldID, JNI_STR(env, pNodeURI));

	if(iCode != 200)
	{	   

	    TraceMsgWindow1(1, "ocx java_FireIMSGotDeptAddrListResult 222 iType[%d] iCode[%d]", iType, iCode);	
	    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iCode, iNum, obj_return);
		return;
	}

    if(DeptAddrListType_Company == iType)
    {
        COMPANY_INFO *pInfo = (COMPANY_INFO *)pAddrList;

        jclass class_companyInfo = env->FindClass("com/zte/softda/ocx/FireCompanyInfoPara");
        jmethodID id_companyInfo  = env->GetMethodID(class_companyInfo, "<init>", "()V");
        jobject obj_companyInfo = env->NewObject(class_companyInfo, id_companyInfo);

        id_fieldID = env->GetFieldID(class_companyInfo, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cURI));

        id_fieldID = env->GetFieldID(class_companyInfo, "cName", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cName));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyNum", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyNum));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyTel", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyTel));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyAddress));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyMailAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyMailAddress));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyPostCode", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyPostCode));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyEmailAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyEmailAddress));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyFax", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyFax));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyUrl", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyUrl));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCompanyStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCompanyStatus));

        id_fieldID = env->GetFieldID(class_companyInfo, "cAffiliationURI", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cAffiliationURI));

        id_fieldID = env->GetFieldID(class_companyInfo, "cHomePageURI", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cHomePageURI));

        id_fieldID = env->GetFieldID(class_companyInfo, "cCapacityFlag", "Ljava/lang/String;");
        env->SetObjectField(obj_companyInfo, id_fieldID, JNI_STR(env,pInfo->cCapacityFlag));

        jclass class_employee = env->FindClass("com/zte/softda/ocx/FireEmployeInfoPara");
        jmethodID id_employe  = env->GetMethodID(class_employee, "<init>", "()V");
        jobject obj_employe = env->NewObject(class_employee, id_employe);
        EMPLOYEE_INFO pEmployee = pInfo->MyDetailInfo;

        id_fieldID = env->GetFieldID(class_employee, "cDepartmentName", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cDepartmentName));

        id_fieldID = env->GetFieldID(class_employee, "cDepartmentURI", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cDepartmentURI));

        id_fieldID = env->GetFieldID(class_employee, "cName", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cName));

        id_fieldID = env->GetFieldID(class_employee, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cURI));

        id_fieldID = env->GetFieldID(class_employee, "chSerialNo", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.chSerialNo));

        id_fieldID = env->GetFieldID(class_employee, "cEtag", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cEtag));

        id_fieldID = env->GetFieldID(class_employee, "cEmployeeNum", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cEmployeeNum));

        id_fieldID = env->GetFieldID(class_employee, "cOfficeTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cOfficeTel));

        id_fieldID = env->GetFieldID(class_employee, "cOfficeTel2", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cOfficeTel2));

        id_fieldID = env->GetFieldID(class_employee, "cMobileTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cMobileTel));

        id_fieldID = env->GetFieldID(class_employee, "cMobileTel2", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cMobileTel2));

        id_fieldID = env->GetFieldID(class_employee, "cTelUri", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cTelUri));

        id_fieldID = env->GetFieldID(class_employee, "cHomeTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cHomeTel));

        id_fieldID = env->GetFieldID(class_employee, "cHomeTel2", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cHomeTel2));

        id_fieldID = env->GetFieldID(class_employee, "cSmsBusiness", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cSmsBusiness));

        id_fieldID = env->GetFieldID(class_employee, "cFaxBusiness", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cFaxBusiness));

        id_fieldID = env->GetFieldID(class_employee, "cWorkAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cWorkAddress));

        id_fieldID = env->GetFieldID(class_employee, "cEmailAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cEmailAddress));

        id_fieldID = env->GetFieldID(class_employee, "cPersonEmailAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cPersonEmailAddress));

        id_fieldID = env->GetFieldID(class_employee, "iEnableCreateGroupNum", "I");
        env->SetIntField(obj_employe, id_fieldID, pEmployee.iEnableCreateGroupNum);

        id_fieldID = env->GetFieldID(class_employee, "cUserCapacity", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cUserCapacity));

        id_fieldID = env->GetFieldID(class_employee, "cPosition", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cPosition));

        id_fieldID = env->GetFieldID(class_employee, "cSex", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cSex));

        id_fieldID = env->GetFieldID(class_employee, "cBirthday", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cBirthday));

        id_fieldID = env->GetFieldID(class_employee, "cMood", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cMood));

        id_fieldID = env->GetFieldID(class_employee, "cJob", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cJob));

        id_fieldID = env->GetFieldID(class_employee, "cHomePageURI", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cHomePageURI));

        id_fieldID = env->GetFieldID(class_employee, "cAffiliationDefaultNum", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cAffiliationDefaultNum));

        id_fieldID = env->GetFieldID(class_employee, "cPhotoURL", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cPhotoURL));

        id_fieldID = env->GetFieldID(class_employee, "cCompanyName", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cCompanyName));

        id_fieldID = env->GetFieldID(class_employee, "cCompanyUri", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cCompanyUri));

        id_fieldID = env->GetFieldID(class_employee, "cStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cStatus));

        id_fieldID = env->GetFieldID(class_employee, "cNote", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cNote));

        id_fieldID = env->GetFieldID(class_employee, "iClientUseType", "I");
        env->SetIntField(obj_employe, id_fieldID, pEmployee.iClientUseType);

        id_fieldID = env->GetFieldID(class_employee, "cWorkAffiliation", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cWorkAffiliation));

        id_fieldID = env->GetFieldID(class_employee, "cOtherTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cOtherTel));

        id_fieldID = env->GetFieldID(class_employee, "cFullSpelling", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cFullSpelling));

        id_fieldID = env->GetFieldID(class_employee, "cShortSpelling", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cShortSpelling));

        id_fieldID = env->GetFieldID(class_employee, "cFullSpelling1", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cFullSpelling1));

        id_fieldID = env->GetFieldID(class_employee, "cShortSpelling1", "Ljava/lang/String;");
        env->SetObjectField(obj_employe, id_fieldID, JNI_STR(env,pEmployee.cShortSpelling1));

        id_fieldID = env->GetFieldID(class_companyInfo, "MyDetailInfo", "Lcom/zte/softda/ocx/FireEmployeInfoPara;");
        env->SetObjectField(obj_companyInfo, id_fieldID, obj_employe);

        id_fieldID = env->GetFieldID(class_return, "fireCompanyInfoPara", "Lcom/zte/softda/ocx/FireCompanyInfoPara;");
        env->SetObjectField(obj_return, id_fieldID, obj_companyInfo);
    }
    else if(DeptAddrListType_Departments == iType || DeptAddrListType_Dept_info == iType)
    {
        DEPARTMENTS_INFO *pDepartment = (DEPARTMENTS_INFO *)pAddrList;
        int nNum = iNum;
        jclass class_department = env->FindClass("com/zte/softda/ocx/FireDepartmentPara");
        jobjectArray arrary_departInfo = env->NewObjectArray(nNum, class_department, NULL);
        jmethodID id_depart  = env->GetMethodID(class_department, "<init>", "()V");

        for (int idx = 0; idx < nNum; idx++)
        {
            jobject obj_depart = env->NewObject(class_department, id_depart);

            jstring cAffiliationName = env->NewStringUTF(pDepartment->cAffiliationName);
            id_fieldID = env->GetFieldID(class_department, "cAffiliationName", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cAffiliationName);
            env->DeleteLocalRef(cAffiliationName);

            jstring cAffiliationURI = env->NewStringUTF(pDepartment->cAffiliationURI);
            id_fieldID = env->GetFieldID(class_department, "cAffiliationURI", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cAffiliationURI);
            env->DeleteLocalRef(cAffiliationURI);

            jstring cDepthPath = env->NewStringUTF(pDepartment->cDepthPath);
            id_fieldID = env->GetFieldID(class_department, "cDepthPath", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepthPath);
            env->DeleteLocalRef(cDepthPath);

            id_fieldID = env->GetFieldID(class_department, "iFatherDepartmentURIType", "I");
            env->SetIntField(obj_depart, id_fieldID, pDepartment->iFatherDepartmentURIType);

            jstring cName = env->NewStringUTF(pDepartment->cName);
            id_fieldID = env->GetFieldID(class_department, "cName", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cName);
            env->DeleteLocalRef(cName);

            jstring cURI = env->NewStringUTF(pDepartment->cURI);
            id_fieldID = env->GetFieldID(class_department, "cURI", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cURI);
            env->DeleteLocalRef(cURI);

            jstring chSerialNo = env->NewStringUTF(pDepartment->chSerialNo);
            id_fieldID = env->GetFieldID(class_department, "chSerialNo", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, chSerialNo);
            env->DeleteLocalRef(chSerialNo);

            jstring cDepartmentNum = env->NewStringUTF(pDepartment->cDepartmentNum);
            id_fieldID = env->GetFieldID(class_department, "cDepartmentNum", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepartmentNum);
            env->DeleteLocalRef(cDepartmentNum);

            jstring cDepartmentTel = env->NewStringUTF(pDepartment->cDepartmentTel);
            id_fieldID = env->GetFieldID(class_department, "cDepartmentTel", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepartmentTel);
            env->DeleteLocalRef(cDepartmentTel);

            jstring cDepartmentTel1 = env->NewStringUTF(pDepartment->cDepartmentTel1);
            id_fieldID = env->GetFieldID(class_department, "cDepartmentTel1", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepartmentTel1);
            env->DeleteLocalRef(cDepartmentTel1);

            jstring cFaxNum1 = env->NewStringUTF(pDepartment->cFaxNum1);
            id_fieldID = env->GetFieldID(class_department, "cFaxNum1", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cFaxNum1);
            env->DeleteLocalRef(cFaxNum1);

            jstring cFaxNum2 = env->NewStringUTF(pDepartment->cFaxNum2);
            id_fieldID = env->GetFieldID(class_department, "cFaxNum2", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cFaxNum2);
            env->DeleteLocalRef(cFaxNum2);

            jstring cDepartmentDetailAddr = env->NewStringUTF(pDepartment->cDepartmentDetailAddr);
            id_fieldID = env->GetFieldID(class_department, "cDepartmentDetailAddr", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepartmentDetailAddr);
            env->DeleteLocalRef(cDepartmentDetailAddr);

            jstring cDepartmentMailAddr = env->NewStringUTF(pDepartment->cDepartmentMailAddr);
            id_fieldID = env->GetFieldID(class_department, "cDepartmentMailAddr", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepartmentMailAddr);
            env->DeleteLocalRef(cDepartmentMailAddr);

            jstring cDepartmentPostCodeAddr = env->NewStringUTF(pDepartment->cDepartmentPostCodeAddr);
            id_fieldID = env->GetFieldID(class_department, "cDepartmentPostCodeAddr", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepartmentPostCodeAddr);
            env->DeleteLocalRef(cDepartmentPostCodeAddr);

            jstring cDepartmentEmailAddr = env->NewStringUTF(pDepartment->cDepartmentEmailAddr);
            id_fieldID = env->GetFieldID(class_department, "cDepartmentEmailAddr", "Ljava/lang/String;");
            env->SetObjectField(obj_depart, id_fieldID, cDepartmentEmailAddr);
            env->DeleteLocalRef(cDepartmentEmailAddr);

            env->SetObjectArrayElement(arrary_departInfo, idx, obj_depart);
            env->DeleteLocalRef(obj_depart);

            pDepartment++;
        }

        id_fieldID = env->GetFieldID(class_return, "fireDepartmentParas", "[Lcom/zte/softda/ocx/FireDepartmentPara;");
        env->SetObjectField(obj_return, id_fieldID, arrary_departInfo);
    }
    else if(DeptAddrListType_EmployeeInfo == iType)
    {
        int nNum = iNum;
        jclass class_employee = env->FindClass("com/zte/softda/ocx/FireEmployeInfoPara");
        jobjectArray arrary_emplyee = env->NewObjectArray(nNum, class_employee, NULL);
        jmethodID id_emplyee = env->GetMethodID(class_employee, "<init>", "()V");

        EMPLOYEE_INFO *pEmployee = (EMPLOYEE_INFO *)pAddrList;
        for (int idx = 0; idx < nNum; idx++)
        {
            jobject obj_employe = env->NewObject(class_employee, id_emplyee);

            jstring cDepartmentName = env->NewStringUTF(pEmployee->cDepartmentName);
            id_fieldID = env->GetFieldID(class_employee, "cDepartmentName", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cDepartmentName);

            jstring cDepartmentURI = env->NewStringUTF(pEmployee->cDepartmentURI);
            id_fieldID = env->GetFieldID(class_employee, "cDepartmentURI", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cDepartmentURI);

            jstring cName = env->NewStringUTF(pEmployee->cName);
            id_fieldID = env->GetFieldID(class_employee, "cName", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cName);

            jstring cURI = env->NewStringUTF(pEmployee->cURI);
            id_fieldID = env->GetFieldID(class_employee, "cURI", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, env->NewStringUTF(pEmployee->cURI));

            jstring chSerialNo = env->NewStringUTF(pEmployee->chSerialNo);
            id_fieldID = env->GetFieldID(class_employee, "chSerialNo", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, chSerialNo);

            jstring cEtag = env->NewStringUTF(pEmployee->cEtag);
            id_fieldID = env->GetFieldID(class_employee, "cEtag", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cEtag);

            jstring cEmployeeNum = env->NewStringUTF(pEmployee->cEmployeeNum);
            id_fieldID = env->GetFieldID(class_employee, "cEmployeeNum", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cEmployeeNum);

            jstring cOfficeTel = env->NewStringUTF(pEmployee->cOfficeTel);
            id_fieldID = env->GetFieldID(class_employee, "cOfficeTel", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cOfficeTel);

            jstring cOfficeTel2 = env->NewStringUTF(pEmployee->cOfficeTel2);
            id_fieldID = env->GetFieldID(class_employee, "cOfficeTel2", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cOfficeTel2);

            jstring cMobileTel = env->NewStringUTF(pEmployee->cMobileTel);
            id_fieldID = env->GetFieldID(class_employee, "cMobileTel", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cMobileTel);

            jstring cMobileTel2 = env->NewStringUTF(pEmployee->cMobileTel2);
            id_fieldID = env->GetFieldID(class_employee, "cMobileTel2", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cMobileTel2);

            jstring cTelUri = env->NewStringUTF(pEmployee->cTelUri);
            id_fieldID = env->GetFieldID(class_employee, "cTelUri", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cTelUri);

            jstring cHomeTel = env->NewStringUTF(pEmployee->cHomeTel);
            id_fieldID = env->GetFieldID(class_employee, "cHomeTel", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cHomeTel);

            jstring cHomeTel2 = env->NewStringUTF(pEmployee->cHomeTel2);
            id_fieldID = env->GetFieldID(class_employee, "cHomeTel2", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cHomeTel2);

            jstring cSmsBusiness = env->NewStringUTF(pEmployee->cSmsBusiness);
            id_fieldID = env->GetFieldID(class_employee, "cSmsBusiness", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cSmsBusiness);

            jstring cFaxBusiness = env->NewStringUTF(pEmployee->cFaxBusiness);
            id_fieldID = env->GetFieldID(class_employee, "cFaxBusiness", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cFaxBusiness);

            jstring cWorkAddress = env->NewStringUTF(pEmployee->cWorkAddress);
            id_fieldID = env->GetFieldID(class_employee, "cWorkAddress", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cWorkAddress);

            jstring cEmailAddress = env->NewStringUTF(pEmployee->cEmailAddress);
            id_fieldID = env->GetFieldID(class_employee, "cEmailAddress", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cEmailAddress);

            jstring cPersonEmailAddress = env->NewStringUTF(pEmployee->cPersonEmailAddress);
            id_fieldID = env->GetFieldID(class_employee, "cPersonEmailAddress", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cPersonEmailAddress);

            id_fieldID = env->GetFieldID(class_employee, "iEnableCreateGroupNum", "I");
            env->SetIntField(obj_employe, id_fieldID, pEmployee->iEnableCreateGroupNum);

            jstring cUserCapacity = env->NewStringUTF(pEmployee->cUserCapacity);
            id_fieldID = env->GetFieldID(class_employee, "cUserCapacity", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cUserCapacity);

            jstring cPosition = env->NewStringUTF(pEmployee->cPosition);
            id_fieldID = env->GetFieldID(class_employee, "cPosition", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cPosition);

            jstring cSex = env->NewStringUTF(pEmployee->cSex);
            id_fieldID = env->GetFieldID(class_employee, "cSex", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cSex);

            jstring cBirthday = env->NewStringUTF(pEmployee->cBirthday);
            id_fieldID = env->GetFieldID(class_employee, "cBirthday", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cBirthday);

            jstring cMood = env->NewStringUTF(pEmployee->cMood);
            id_fieldID = env->GetFieldID(class_employee, "cMood", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cMood);

            jstring cJob = env->NewStringUTF(pEmployee->cJob);
            id_fieldID = env->GetFieldID(class_employee, "cJob", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cJob);

            jstring cHomePageURI = env->NewStringUTF(pEmployee->cHomePageURI);
            id_fieldID = env->GetFieldID(class_employee, "cHomePageURI", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cHomePageURI);

            jstring cAffiliationDefaultNum = env->NewStringUTF(pEmployee->cAffiliationDefaultNum);
            id_fieldID = env->GetFieldID(class_employee, "cAffiliationDefaultNum", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cAffiliationDefaultNum);

            jstring cPhotoURL = env->NewStringUTF(pEmployee->cPhotoURL);
            id_fieldID = env->GetFieldID(class_employee, "cPhotoURL", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cPhotoURL);

            jstring cCompanyName = env->NewStringUTF(pEmployee->cCompanyName);
            id_fieldID = env->GetFieldID(class_employee, "cCompanyName", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cCompanyName);

            jstring cCompanyUri = env->NewStringUTF(pEmployee->cCompanyUri);
            id_fieldID = env->GetFieldID(class_employee, "cCompanyUri", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cCompanyUri);

            jstring cStatus = env->NewStringUTF(pEmployee->cStatus);
            id_fieldID = env->GetFieldID(class_employee, "cStatus", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cStatus);

            jstring cNote = env->NewStringUTF(pEmployee->cNote);
            id_fieldID = env->GetFieldID(class_employee, "cNote", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cNote);

            id_fieldID = env->GetFieldID(class_employee, "iClientUseType", "I");
            env->SetIntField(obj_employe, id_fieldID, pEmployee->iClientUseType);

            jstring cWorkAffiliation = env->NewStringUTF(pEmployee->cWorkAffiliation);
            id_fieldID = env->GetFieldID(class_employee, "cWorkAffiliation", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cWorkAffiliation);

            jstring cOtherTel = env->NewStringUTF(pEmployee->cOtherTel);
            id_fieldID = env->GetFieldID(class_employee, "cOtherTel", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cOtherTel);

            jstring cFullSpelling = env->NewStringUTF(pEmployee->cFullSpelling);
            id_fieldID = env->GetFieldID(class_employee, "cFullSpelling", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cFullSpelling);

            jstring cShortSpelling = env->NewStringUTF(pEmployee->cShortSpelling);
            id_fieldID = env->GetFieldID(class_employee, "cShortSpelling", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cShortSpelling);

            jstring cFullSpelling1 = env->NewStringUTF(pEmployee->cFullSpelling1);
            id_fieldID = env->GetFieldID(class_employee, "cFullSpelling1", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cFullSpelling1);

            jstring cShortSpelling1 = env->NewStringUTF(pEmployee->cShortSpelling1);
            id_fieldID = env->GetFieldID(class_employee, "cShortSpelling1", "Ljava/lang/String;");
            env->SetObjectField(obj_employe, id_fieldID, cShortSpelling1);

            env->SetObjectArrayElement(arrary_emplyee, idx, obj_employe);
            env->DeleteLocalRef(obj_employe);
            
            env->DeleteLocalRef(cDepartmentName);
            env->DeleteLocalRef(cDepartmentURI);
            env->DeleteLocalRef(cName);
            env->DeleteLocalRef(cURI);
            env->DeleteLocalRef(chSerialNo);
            env->DeleteLocalRef(cEtag);
            env->DeleteLocalRef(cEmployeeNum);
            env->DeleteLocalRef(cOfficeTel);
            env->DeleteLocalRef(cOfficeTel2);
            env->DeleteLocalRef(cMobileTel);
            env->DeleteLocalRef(cMobileTel2);
            env->DeleteLocalRef(cTelUri);
            env->DeleteLocalRef(cHomeTel);
            env->DeleteLocalRef(cHomeTel2);
            env->DeleteLocalRef(cSmsBusiness);
            env->DeleteLocalRef(cFaxBusiness);
            env->DeleteLocalRef(cWorkAddress);
            env->DeleteLocalRef(cEmailAddress);
            env->DeleteLocalRef(cPersonEmailAddress);
            env->DeleteLocalRef(cUserCapacity);
            env->DeleteLocalRef(cPosition);
            env->DeleteLocalRef(cSex);
            env->DeleteLocalRef(cBirthday);
            env->DeleteLocalRef(cMood);
            env->DeleteLocalRef(cJob);
            env->DeleteLocalRef(cHomePageURI);
            env->DeleteLocalRef(cAffiliationDefaultNum);
            env->DeleteLocalRef(cPhotoURL);
            env->DeleteLocalRef(cCompanyName);
            env->DeleteLocalRef(cCompanyUri);
            env->DeleteLocalRef(cStatus);
            env->DeleteLocalRef(cNote);
            env->DeleteLocalRef(cWorkAffiliation);
            env->DeleteLocalRef(cOtherTel);
            env->DeleteLocalRef(cFullSpelling);
            env->DeleteLocalRef(cShortSpelling);
            env->DeleteLocalRef(cFullSpelling1);
            env->DeleteLocalRef(cShortSpelling1);

            pEmployee++;

        }
        id_fieldID = env->GetFieldID(class_return, "fireEmployeInfoPara", "[Lcom/zte/softda/ocx/FireEmployeInfoPara;");
        env->SetObjectField(obj_return, id_fieldID, arrary_emplyee);
    }
	
    TraceMsgWindow1(1, "ocx java_FireIMSGotDeptAddrListResult 111 iType[%d] iCode[%d]", iType, iCode);
	
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iCode, iNum, obj_return);
}
void java_FireIMSSearchAddrListResult( long iType, long iCode, short far* pAddrList)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    EMPLOYEE_INFO *pEmployee = NULL;
    pEmployee = (EMPLOYEE_INFO *)pAddrList;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSearchAddrListResult iType[%d] iCode[%d]", iType, iCode);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSearchAddrListResult iType[%d] iCode[%d]", iType, iCode);

    iresult = getStaticJniMethod((char *)"java_FireIMSSearchAddrListResult", (char *)"(IILcom/zte/softda/ocx/FireEmployeInfoPara;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSearchAddrListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSearchAddrListResult");

        return;
    }
    jclass class_employee = env->FindClass("com/zte/softda/ocx/FireEmployeInfoPara");
    jmethodID id_emplyee = env->GetMethodID(class_employee, "<init>", "()V");
    jobject obj_employee = env->NewObject(class_employee, id_emplyee);

    if(pEmployee != NULL)
    {
        jstring cDepartmentName = env->NewStringUTF(pEmployee->cDepartmentName);
        id_fieldID = env->GetFieldID(class_employee, "cDepartmentName", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cDepartmentName);

        jstring cDepartmentURI = env->NewStringUTF(pEmployee->cDepartmentURI);
        id_fieldID = env->GetFieldID(class_employee, "cDepartmentURI", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cDepartmentURI);

        jstring cName = env->NewStringUTF(pEmployee->cName);
        id_fieldID = env->GetFieldID(class_employee, "cName", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cName);

        jstring cURI = env->NewStringUTF(pEmployee->cURI);
        id_fieldID = env->GetFieldID(class_employee, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cURI);

        jstring chSerialNo = env->NewStringUTF(pEmployee->chSerialNo);
        id_fieldID = env->GetFieldID(class_employee, "chSerialNo", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, chSerialNo);

        jstring cEtag = env->NewStringUTF(pEmployee->cEtag);
        id_fieldID = env->GetFieldID(class_employee, "cEtag", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cEtag);

        jstring cEmployeeNum = env->NewStringUTF(pEmployee->cEmployeeNum);
        id_fieldID = env->GetFieldID(class_employee, "cEmployeeNum", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cEmployeeNum);

        jstring cOfficeTel = env->NewStringUTF(pEmployee->cOfficeTel);
        id_fieldID = env->GetFieldID(class_employee, "cOfficeTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cOfficeTel);

        jstring cOfficeTel2 = env->NewStringUTF(pEmployee->cOfficeTel2);
        id_fieldID = env->GetFieldID(class_employee, "cOfficeTel2", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cOfficeTel2);

        jstring cMobileTel = env->NewStringUTF(pEmployee->cMobileTel);
        id_fieldID = env->GetFieldID(class_employee, "cMobileTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cMobileTel);

        jstring cMobileTel2 = env->NewStringUTF(pEmployee->cMobileTel2);
        id_fieldID = env->GetFieldID(class_employee, "cMobileTel2", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cMobileTel2);

        jstring cTelUri = env->NewStringUTF(pEmployee->cTelUri);
        id_fieldID = env->GetFieldID(class_employee, "cTelUri", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cTelUri);

        jstring cHomeTel = env->NewStringUTF(pEmployee->cHomeTel);
        id_fieldID = env->GetFieldID(class_employee, "cHomeTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cHomeTel);

        jstring cHomeTel2 = env->NewStringUTF(pEmployee->cHomeTel2);
        id_fieldID = env->GetFieldID(class_employee, "cHomeTel2", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cHomeTel2);

        jstring cSmsBusiness = env->NewStringUTF(pEmployee->cSmsBusiness);
        id_fieldID = env->GetFieldID(class_employee, "cSmsBusiness", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cSmsBusiness);

        jstring cFaxBusiness = env->NewStringUTF(pEmployee->cFaxBusiness);
        id_fieldID = env->GetFieldID(class_employee, "cFaxBusiness", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cFaxBusiness);

        jstring cWorkAddress = env->NewStringUTF(pEmployee->cWorkAddress);
        id_fieldID = env->GetFieldID(class_employee, "cWorkAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cWorkAddress);

        jstring cEmailAddress = env->NewStringUTF(pEmployee->cEmailAddress);
        id_fieldID = env->GetFieldID(class_employee, "cEmailAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cEmailAddress);

        jstring cPersonEmailAddress = env->NewStringUTF(pEmployee->cPersonEmailAddress);
        id_fieldID = env->GetFieldID(class_employee, "cPersonEmailAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cPersonEmailAddress);

        id_fieldID = env->GetFieldID(class_employee, "iEnableCreateGroupNum", "I");
        env->SetIntField(obj_employee, id_fieldID, pEmployee->iEnableCreateGroupNum);

        jstring cUserCapacity = env->NewStringUTF(pEmployee->cUserCapacity);
        id_fieldID = env->GetFieldID(class_employee, "cUserCapacity", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cUserCapacity);

        jstring cPosition = env->NewStringUTF(pEmployee->cPosition);
        id_fieldID = env->GetFieldID(class_employee, "cPosition", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cPosition);

        jstring cSex = env->NewStringUTF(pEmployee->cSex);
        id_fieldID = env->GetFieldID(class_employee, "cSex", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cSex);

        jstring cBirthday = env->NewStringUTF(pEmployee->cBirthday);
        id_fieldID = env->GetFieldID(class_employee, "cBirthday", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cBirthday);

        jstring cMood = env->NewStringUTF(pEmployee->cMood);
        id_fieldID = env->GetFieldID(class_employee, "cMood", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cMood);

        jstring cJob = env->NewStringUTF(pEmployee->cJob);
        id_fieldID = env->GetFieldID(class_employee, "cJob", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cJob);

        jstring cHomePageURI = env->NewStringUTF(pEmployee->cHomePageURI);
        id_fieldID = env->GetFieldID(class_employee, "cHomePageURI", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cHomePageURI);

        jstring cAffiliationDefaultNum = env->NewStringUTF(pEmployee->cAffiliationDefaultNum);
        id_fieldID = env->GetFieldID(class_employee, "cAffiliationDefaultNum", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cAffiliationDefaultNum);

        jstring cPhotoURL = env->NewStringUTF(pEmployee->cPhotoURL);
        id_fieldID = env->GetFieldID(class_employee, "cPhotoURL", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cPhotoURL);

        jstring cCompanyName = env->NewStringUTF(pEmployee->cCompanyName);
        id_fieldID = env->GetFieldID(class_employee, "cCompanyName", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cCompanyName);

        jstring cCompanyUri = env->NewStringUTF(pEmployee->cCompanyUri);
        id_fieldID = env->GetFieldID(class_employee, "cCompanyUri", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cCompanyUri);

        jstring cStatus = env->NewStringUTF(pEmployee->cStatus);
        id_fieldID = env->GetFieldID(class_employee, "cStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cStatus);

        jstring cNote = env->NewStringUTF(pEmployee->cNote);
        id_fieldID = env->GetFieldID(class_employee, "cNote", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cNote);

        id_fieldID = env->GetFieldID(class_employee, "iClientUseType", "I");
        env->SetIntField(obj_employee, id_fieldID, pEmployee->iClientUseType);

        jstring cWorkAffiliation = env->NewStringUTF(pEmployee->cWorkAffiliation);
        id_fieldID = env->GetFieldID(class_employee, "cWorkAffiliation", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cWorkAffiliation);

        jstring cOtherTel = env->NewStringUTF(pEmployee->cOtherTel);
        id_fieldID = env->GetFieldID(class_employee, "cOtherTel", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cOtherTel);

        jstring cFullSpelling = env->NewStringUTF(pEmployee->cFullSpelling);
        id_fieldID = env->GetFieldID(class_employee, "cFullSpelling", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cFullSpelling);

        jstring cShortSpelling = env->NewStringUTF(pEmployee->cShortSpelling);
        id_fieldID = env->GetFieldID(class_employee, "cShortSpelling", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cShortSpelling);

        jstring cFullSpelling1 = env->NewStringUTF(pEmployee->cFullSpelling1);
        id_fieldID = env->GetFieldID(class_employee, "cFullSpelling1", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cFullSpelling1);

        jstring cShortSpelling1 = env->NewStringUTF(pEmployee->cShortSpelling1);
        id_fieldID = env->GetFieldID(class_employee, "cShortSpelling1", "Ljava/lang/String;");
        env->SetObjectField(obj_employee, id_fieldID, cShortSpelling1);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iCode, obj_employee);

        env->DeleteLocalRef(cDepartmentName);
        env->DeleteLocalRef(cDepartmentURI);
        env->DeleteLocalRef(cName);
        env->DeleteLocalRef(cURI);
        env->DeleteLocalRef(chSerialNo);
        env->DeleteLocalRef(cEtag);
        env->DeleteLocalRef(cEmployeeNum);
        env->DeleteLocalRef(cOfficeTel);
        env->DeleteLocalRef(cOfficeTel2);
        env->DeleteLocalRef(cMobileTel);
        env->DeleteLocalRef(cMobileTel2);
        env->DeleteLocalRef(cTelUri);
        env->DeleteLocalRef(cHomeTel);
        env->DeleteLocalRef(cHomeTel2);
        env->DeleteLocalRef(cSmsBusiness);
        env->DeleteLocalRef(cFaxBusiness);
        env->DeleteLocalRef(cWorkAddress);
        env->DeleteLocalRef(cEmailAddress);
        env->DeleteLocalRef(cPersonEmailAddress);
        env->DeleteLocalRef(cUserCapacity);
        env->DeleteLocalRef(cPosition);
        env->DeleteLocalRef(cSex);
        env->DeleteLocalRef(cBirthday);
        env->DeleteLocalRef(cMood);
        env->DeleteLocalRef(cJob);
        env->DeleteLocalRef(cHomePageURI);
        env->DeleteLocalRef(cAffiliationDefaultNum);
        env->DeleteLocalRef(cPhotoURL);
        env->DeleteLocalRef(cCompanyName);
        env->DeleteLocalRef(cCompanyUri);
        env->DeleteLocalRef(cStatus);
        env->DeleteLocalRef(cNote);
        env->DeleteLocalRef(cWorkAffiliation);
        env->DeleteLocalRef(cOtherTel);
        env->DeleteLocalRef(cFullSpelling);
        env->DeleteLocalRef(cShortSpelling);
        env->DeleteLocalRef(cFullSpelling1);
        env->DeleteLocalRef(cShortSpelling1);
    }
    else
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iCode, NULL);
    }
}
void java_FireIMSLoadAddrListResult(long iType, long iCode, long iNum, short far* pAddrList)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    PERSON_LIST_INFO *pPersonListInfos = NULL;
    pPersonListInfos = (PERSON_LIST_INFO *)pAddrList;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSLoadAddrListResult");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSLoadAddrListResult");

    iresult = getStaticJniMethod((char *)"java_FireIMSLoadAddrListResult", (char *)"(III[Lcom/zte/softda/ocx/PersonListInfoList;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSLoadAddrListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSLoadAddrListResult");

        return;
    }
    if(iNum > 0 && (pPersonListInfos == NULL))
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSLoadAddrListResult pPersonListInfos == NULL, iNum=%d", iNum);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSLoadAddrListResult pPersonListInfos == NULL, iNum=%d", iNum);

        return;
    }
    if((iNum == 0) && (pPersonListInfos == NULL))
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSLoadAddrListResult pPersonListInfos == NULL, iNum=0");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSLoadAddrListResult pPersonListInfos == NULL, iNum=0");

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iCode, 0, NULL);
        return;
    }
    jclass class_PersonListInfo = env->FindClass("com/zte/softda/ocx/PersonListInfoList");
    jmethodID id_PersonListInfo  = env->GetMethodID(class_PersonListInfo, "<init>", "()V");
    jobjectArray array_PersonListInfo = env->NewObjectArray(iNum, class_PersonListInfo, NULL);
    for(i = 0; i < iNum; i++)
    {
        PERSON_LIST_INFO *pServcie = (PERSON_LIST_INFO *) & (pPersonListInfos[i]);
        jobject obj_PersonListInfo = env->NewObject(class_PersonListInfo, id_PersonListInfo);

        jstring cName = env->NewStringUTF(pServcie->cName);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cName", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cName);
        env->DeleteLocalRef(cName);

        jstring cURI = env->NewStringUTF(pServcie->cURI);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cURI);
        env->DeleteLocalRef(cURI);

        jstring cURI2 = env->NewStringUTF(pServcie->cURI2);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cURI2", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cURI2);
        env->DeleteLocalRef(cURI2);

        jstring cOfficeTel = env->NewStringUTF(pServcie->cOfficeTel);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cOfficeTel", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cOfficeTel);
        env->DeleteLocalRef(cOfficeTel);

        jstring cMobileTel = env->NewStringUTF(pServcie->cMobileTel);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cMobileTel", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, env->NewStringUTF(pServcie->cMobileTel));
        env->DeleteLocalRef(cMobileTel);

        jstring cFectionNum = env->NewStringUTF(pServcie->cFectionNum);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cFectionNum", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cFectionNum);
        env->DeleteLocalRef(cFectionNum);

        jstring cFaxNum = env->NewStringUTF(pServcie->cFaxNum);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cFaxNum", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cFaxNum);
        env->DeleteLocalRef(cFaxNum);

        jstring cAutoTel = env->NewStringUTF(pServcie->cAutoTel);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cAutoTel", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cAutoTel);
        env->DeleteLocalRef(cAutoTel);

        jstring cHomeTel = env->NewStringUTF(pServcie->cHomeTel);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cHomeTel", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cHomeTel);
        env->DeleteLocalRef(cHomeTel);

        jstring cSecondOfficeTel = env->NewStringUTF(pServcie->cSecondOfficeTel);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cSecondOfficeTel", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cSecondOfficeTel);
        env->DeleteLocalRef(cSecondOfficeTel);

        jstring cSecondMobileTel = env->NewStringUTF(pServcie->cSecondMobileTel);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cSecondMobileTel", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cSecondMobileTel);
        env->DeleteLocalRef(cSecondMobileTel);

        jstring cOfficeAddress = env->NewStringUTF(pServcie->cOfficeAddress);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cOfficeAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cOfficeAddress);
        env->DeleteLocalRef(cOfficeAddress);

        jstring cOtherAddress = env->NewStringUTF(pServcie->cOtherAddress);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cOtherAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cOtherAddress);
        env->DeleteLocalRef(cOtherAddress);

        jstring cEmailAddress = env->NewStringUTF(pServcie->cEmailAddress);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cEmailAddress", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cEmailAddress);
        env->DeleteLocalRef(cEmailAddress);

        jstring cAffiliation = env->NewStringUTF(pServcie->cAffiliation);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cAffiliation", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cAffiliation);
        env->DeleteLocalRef(cAffiliation);

        jstring cSex = env->NewStringUTF(pServcie->cSex);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cSex", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cSex);
        env->DeleteLocalRef(cSex);

        id_fieldID = env->GetFieldID(class_PersonListInfo, "iOld", "I");
        env->SetIntField(obj_PersonListInfo, id_fieldID, pServcie->iOld);

        jstring cPhotoURL = env->NewStringUTF(pServcie->cPhotoURL);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cPhotoURL", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cPhotoURL);
        env->DeleteLocalRef(cPhotoURL);

        jstring cPosition = env->NewStringUTF(pServcie->cPosition);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cPosition", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cPosition);
        env->DeleteLocalRef(cPosition);

        jstring cEmployeeNum = env->NewStringUTF(pServcie->cEmployeeNum);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cEmployeeNum", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cEmployeeNum);
        env->DeleteLocalRef(cEmployeeNum);

        jstring cPersonalDescription = env->NewStringUTF(pServcie->cPersonalDescription);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cPersonalDescription", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cPersonalDescription);
        env->DeleteLocalRef(cPersonalDescription);

        jstring cGroupName = env->NewStringUTF(pServcie->cGroupName);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cGroupName", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cGroupName);
        env->DeleteLocalRef(cGroupName);

        id_fieldID = env->GetFieldID(class_PersonListInfo, "iCategory", "I");
        env->SetIntField(obj_PersonListInfo, id_fieldID, pServcie->iCategory);

        jstring cEtag0 = env->NewStringUTF(pServcie->cEtag0);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cEtag0", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cEtag0);
        env->DeleteLocalRef(cEtag0);

        jstring cEtag = env->NewStringUTF(pServcie->cEtag);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cEtag", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cEtag);
        env->DeleteLocalRef(cEtag);

        id_fieldID = env->GetFieldID(class_PersonListInfo, "bNeedSynchronize", "I");
        env->SetIntField(obj_PersonListInfo, id_fieldID, pServcie->bNeedSynchronize);

        jstring cWorkAffiliation = env->NewStringUTF(pServcie->cWorkAffiliation);
        id_fieldID = env->GetFieldID(class_PersonListInfo, "cWorkAffiliation", "Ljava/lang/String;");
        env->SetObjectField(obj_PersonListInfo, id_fieldID, cWorkAffiliation);
        env->DeleteLocalRef(cWorkAffiliation);

        env->SetObjectArrayElement(array_PersonListInfo, i, obj_PersonListInfo);
        env->DeleteLocalRef(obj_PersonListInfo);
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iCode, iNum, array_PersonListInfo);
}
void java_FireIMSDoGroupJoinReqResult(LPCTSTR pGroupURI, LPCTSTR pMemberURI, LPCTSTR pMemberName, long lResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDoGroupJoinReqResult pGroupURI[%s] pMemberURI[%s] pMemberName[%s] iResult[%d]",
                        pGroupURI, pMemberURI, pMemberName, lResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDoGroupJoinReqResult pGroupURI[%s] pMemberURI[%s] pMemberName[%s] iResult[%d]",
             pGroupURI, pMemberURI, pMemberName, lResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSDoGroupJoinReqResult", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDoGroupJoinReqResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDoGroupJoinReqResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI), JNI_STR(env,pMemberURI), JNI_STR(env,pMemberName), lResult);
}
void java_FireIMSModifyOneListDpNameResult(LPCTSTR pURI, LPCTSTR pDisplayName, int iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSModifyOneListDpNameResult pURI[%s] pDisplayName[%s] iResult[%d]",
                        pURI, pDisplayName, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSModifyOneListDpNameResult pURI[%s] pDisplayName[%s] iResult[%d]",
             pURI, pDisplayName, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSModifyOneListDpNameResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSModifyOneListDpNameResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSModifyOneListDpNameResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pURI), JNI_STR(env,pDisplayName), iResult);
}
void java_FireIMSReqJoinPublicGroupResult(LPCTSTR pGroupURI, LPCTSTR pGroupName, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSReqJoinPublicGroupResult pGroupURI[%s] pGroupName[%s] iResult[%d]",
                        pGroupURI, pGroupName, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSReqJoinPublicGroupResult pGroupURI[%s] pGroupName[%s] iResult[%d]",
             pGroupURI, pGroupName, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSReqJoinPublicGroupResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSReqJoinPublicGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSReqJoinPublicGroupResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI), JNI_STR(env,pGroupName), iResult);
}
void java_FireIMSConfEvent(long iType, long iNum, short far*pConfEvent)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent  begin iType[%d].", iType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSConfEvent  begin iType[%d].", iType);

    iresult = getStaticJniMethod((char *)"java_FireIMSConfEvent", (char *)"(IILcom/zte/softda/ocx/FireIMSConfEventPara;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDelListResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDelListResult");

        return;
    }
    jclass class_ConfEventPara = env->FindClass("com/zte/softda/ocx/FireIMSConfEventPara");

    jmethodID id_ConfEventPara  = env->GetMethodID(class_ConfEventPara, "<init>", "()V");

    jobject obj_ConfEventPara = env->NewObject(class_ConfEventPara, id_ConfEventPara);

    //�������鷵�صĽ��
    if (GT_EVENT_CONF_CONTROL == iType)
    {
        CONF_CONTROL_EVENT *pConfControlPara = NULL;
        pConfControlPara = (CONF_CONTROL_EVENT*)pConfEvent;
        if (pConfControlPara == NULL)
        {
            return;
        }
        jclass class_ConfControlEventPara =  env->FindClass("com/zte/softda/ocx/FireConfControlEvent");
        jmethodID id_ConfControlEventPara  = env->GetMethodID(class_ConfControlEventPara, "<init>", "()V");
        jobject obj_ConfControlEventPara = env->NewObject(class_ConfControlEventPara, id_ConfControlEventPara);


        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iType", "I");
        env->SetIntField(obj_ConfControlEventPara, id_fieldID, pConfControlPara->iType);
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iResult", "I");
        env->SetIntField(obj_ConfControlEventPara, id_fieldID, pConfControlPara->iResult);

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cConfURI", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cConfURI));
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cSubject", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cSubject));

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iConfType", "I");
        env->SetIntField(obj_ConfControlEventPara, id_fieldID, pConfControlPara->iConfType);

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cMemberURI", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cMemberURI));

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iMaxNum", "I");
        env->SetIntField(obj_ConfControlEventPara, id_fieldID, pConfControlPara->iMaxNum);
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iPeriod", "I");
        env->SetIntField(obj_ConfControlEventPara, id_fieldID, pConfControlPara->iPeriod);

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cStartTime", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cStartTime));
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cDConfID", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cDConfID));
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cDConfPassword", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cDConfPassword));

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iDestType", "I");
        env->SetIntField(obj_ConfControlEventPara, id_fieldID, pConfControlPara->iDestType);

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cSubXconID", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cSubXconID));
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "c183Reason", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->c183Reason));

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cServer", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cServer));

        jintArray array = env->NewIntArray(13);
        jint iStatus[13];

        for (int i = 0; i < 13; i++)
        {
            iStatus[i] = pConfControlPara->iSubConfType[i];
        }
        env->SetIntArrayRegion(array, 0, 13,  iStatus);
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iSubConfType", "[I");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, array);

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "iSubConfNum", "I");
        env->SetIntField(obj_ConfControlEventPara, id_fieldID, pConfControlPara->iSubConfNum);

        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cSubConfWBID", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cSubConfWBID));
        id_fieldID = env->GetFieldID(class_ConfControlEventPara, "cSubConfAppShareID", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfControlEventPara, id_fieldID, JNI_STR(env,pConfControlPara->cSubConfAppShareID));

        id_fieldID = env->GetFieldID(class_ConfEventPara, "fireConfControlEvent", "Lcom/zte/softda/ocx/FireConfControlEvent;");
        env->SetObjectField(obj_ConfEventPara, id_fieldID, obj_ConfControlEventPara);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent pConfControlMethod iType[%d], iResutl[%d], ConfURI[%s]	pConfControlPara->cSubject[%s]",
                            pConfControlPara->iType, pConfControlPara->iResult, pConfControlPara->cConfURI, pConfControlPara->cSubject);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSConfEvent pConfControlMethod iType[%d], iResutl[%d], ConfURI[%s]	pConfControlPara->cSubject[%s]", pConfControlPara->iType, pConfControlPara->iResult, pConfControlPara->cConfURI, pConfControlPara->cSubject);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum,  obj_ConfEventPara);
    }

    //�᳡״̬
    if (GT_EVENT_CONF_STATUS_CHANGE == iType)
    {
        CONF_STATUS_EVENT *pConfStatusEventPara = NULL;

        pConfStatusEventPara = (CONF_STATUS_EVENT *)pConfEvent;

        if (NULL == pConfStatusEventPara)
        {
            return;
        }

        jclass class_ConfStatusEventPara = env->FindClass("com/zte/softda/ocx/FireConfStatusEvent");
        jmethodID id_ConfStatusEventPara  = env->GetMethodID(class_ConfStatusEventPara, "<init>", "()V");
        jobject obj_ConfStatusEventPara = env->NewObject(class_ConfStatusEventPara, id_ConfStatusEventPara);

        id_fieldID = env->GetFieldID(class_ConfStatusEventPara, "iType", "I");
        env->SetIntField(obj_ConfStatusEventPara, id_fieldID, pConfStatusEventPara->iType);

        id_fieldID = env->GetFieldID(class_ConfStatusEventPara, "cConfURI", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfStatusEventPara, id_fieldID, JNI_STR(env,pConfStatusEventPara->cConfURI));

        id_fieldID = env->GetFieldID(class_ConfStatusEventPara, "cSubject", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfStatusEventPara, id_fieldID, JNI_STR(env,pConfStatusEventPara->cSubject));

        id_fieldID = env->GetFieldID(class_ConfStatusEventPara, "cUserURI", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfStatusEventPara, id_fieldID, JNI_STR(env,pConfStatusEventPara->cUserURI));

        id_fieldID = env->GetFieldID(class_ConfStatusEventPara, "cUserURI2", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfStatusEventPara, id_fieldID, JNI_STR(env,pConfStatusEventPara->cUserURI2));

        id_fieldID = env->GetFieldID(class_ConfStatusEventPara, "cStatus", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfStatusEventPara, id_fieldID, JNI_STR(env,pConfStatusEventPara->cStatus));

        id_fieldID = env->GetFieldID(class_ConfStatusEventPara, "cShow", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfStatusEventPara, id_fieldID, JNI_STR(env,pConfStatusEventPara->cShow));

        id_fieldID = env->GetFieldID(class_ConfEventPara, "fireConfStatusEvent", "Lcom/zte/softda/ocx/FireConfStatusEvent;");
        env->SetObjectField(obj_ConfEventPara, id_fieldID, obj_ConfStatusEventPara);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent GT_EVENT_CONF_STATUS_CHANGE [cUserURI = %s]  [cStatus = %s]",
                            pConfStatusEventPara->cUserURI, pConfStatusEventPara->cStatus);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSConfEvent GT_EVENT_CONF_STATUS_CHANGE [cUserURI = %s]  [cStatus = %s]",
                 pConfStatusEventPara->cUserURI, pConfStatusEventPara->cStatus);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum, obj_ConfEventPara);
    }
    //��ȡϵͳʱ��Ľ��
    if(GT_EVENT_ASSISTTANT_CONTROL == iType)
    {
        CONF_ASSISTTANT_CONTROL_EVENT *pConfAssisttantControlPara = NULL;

        pConfAssisttantControlPara = (CONF_ASSISTTANT_CONTROL_EVENT*)pConfEvent;
        if (pConfAssisttantControlPara == NULL)
        {
            return;
        }
        jclass class_ConfAssisttantControlPara = env->FindClass("com/zte/softda/ocx/FireConfAssistantCtrEvent");
        jmethodID id_ConfAssisttantControlPara  = env->GetMethodID(class_ConfAssisttantControlPara, "<init>", "()V");
        jobject obj_ConfAssisttantControlPara = env->NewObject(class_ConfAssisttantControlPara, id_ConfAssisttantControlPara);

        id_fieldID = env->GetFieldID(class_ConfAssisttantControlPara, "iOperateType", "I");
        env->SetIntField(obj_ConfAssisttantControlPara, id_fieldID, pConfAssisttantControlPara->iOperateType);

        id_fieldID = env->GetFieldID(class_ConfAssisttantControlPara, "cSysTime", "Ljava/lang/String;");
        env->SetObjectField(obj_ConfAssisttantControlPara, id_fieldID, JNI_STR(env,pConfAssisttantControlPara->cSysTime));

        id_fieldID = env->GetFieldID(class_ConfEventPara, "fireConfAssistantCtrEvent", "Lcom/zte/softda/ocx/FireConfAssistantCtrEvent;");
        env->SetObjectField(obj_ConfEventPara, id_fieldID, obj_ConfAssisttantControlPara);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent GT_EVENT_ASSISTTANT_CONTROL [iOperateType = %d]  [cSysTime = %s]",
                            pConfAssisttantControlPara->iOperateType, pConfAssisttantControlPara->cSysTime);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSConfEvent GT_EVENT_ASSISTTANT_CONTROL [iOperateType = %d]  [cSysTime = %s]",
                 pConfAssisttantControlPara->iOperateType, pConfAssisttantControlPara->cSysTime);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum, obj_ConfEventPara);
    }
    //��ȡ�����¼���
    if(GT_EVENT_QUERY_CONF == iType)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller GT_EVENT_QUERY_CONF  begin iNum = %d", iNum);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "GT_EVENT_QUERY_CONF  begin iNum = %d", iNum);

        QUERY_CONF_RESULT_INFO *pQueryConfResultInfo = NULL;
        pQueryConfResultInfo = (QUERY_CONF_RESULT_INFO*)pConfEvent;

        if(pQueryConfResultInfo == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller GT_EVENT_QUERY_CONF pQueryConfResultInfo == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "GT_EVENT_QUERY_CONF pQueryConfResultInfo == NULL");

            return;
        }

        jclass class_QueryConfResultPara = env->FindClass("com/zte/softda/ocx/FireQueryConfResutStructEvent");
        jmethodID id_QueryConfResultPara  = env->GetMethodID(class_QueryConfResultPara, "<init>", "()V");
        jobjectArray array_QueryConfResultPara = env->NewObjectArray(iNum, class_QueryConfResultPara, NULL);

        for (int i = 0; i < iNum; i++)
        {
            jobject obj_QueryConfResultInfo = env->NewObject(class_QueryConfResultPara, id_QueryConfResultPara);
            QUERY_CONF_RESULT_STRUCT *pQueryConfResult = (QUERY_CONF_RESULT_STRUCT *) & (pQueryConfResultInfo->pQueryConfResultStruct[i]);

            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "iReturnCode", "I");
            env->SetIntField(obj_QueryConfResultInfo, id_fieldID, pQueryConfResult->iReturnCode);

            jstring cConfURI = env->NewStringUTF(pQueryConfResult->cConfURI);
            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "cConfURI", "Ljava/lang/String;");
            env->SetObjectField(obj_QueryConfResultInfo, id_fieldID, cConfURI);
            env->DeleteLocalRef(cConfURI);

            jstring cMaster = env->NewStringUTF(pQueryConfResult->cMaster);
            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "cMaster", "Ljava/lang/String;");
            env->SetObjectField(obj_QueryConfResultInfo, id_fieldID, cMaster);
            env->DeleteLocalRef(cMaster);

            jstring cSubject = env->NewStringUTF(pQueryConfResult->cSubject);
            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "cSubject", "Ljava/lang/String;");
            env->SetObjectField(obj_QueryConfResultInfo, id_fieldID, cSubject);
            env->DeleteLocalRef(cSubject);

            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "iMediaType", "I");
            env->SetIntField(obj_QueryConfResultInfo, id_fieldID, pQueryConfResult->iMediaType);
            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "iConfType", "I");
            env->SetIntField(obj_QueryConfResultInfo, id_fieldID, pQueryConfResult->iConfType);

            jstring cStartTime = env->NewStringUTF(pQueryConfResult->cStartTime);
            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "cStartTime", "Ljava/lang/String;");
            env->SetObjectField(obj_QueryConfResultInfo, id_fieldID, cStartTime);
            env->DeleteLocalRef(cStartTime);

            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "iPeriod", "I");
            env->SetIntField(obj_QueryConfResultInfo, id_fieldID, pQueryConfResult->iPeriod);
            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "iStatus", "I");
            env->SetIntField(obj_QueryConfResultInfo, id_fieldID, pQueryConfResult->iStatus);
            id_fieldID = env->GetFieldID(class_QueryConfResultPara, "iMaxMember", "I");
            env->SetIntField(obj_QueryConfResultInfo, id_fieldID, pQueryConfResult->iMaxMember);

            env->SetObjectArrayElement(array_QueryConfResultPara, i, obj_QueryConfResultInfo);
            env->DeleteLocalRef(obj_QueryConfResultInfo);

            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent pQueryConfResultPara  [i = %d]- [iReturnCode= %d]-[cConfURI = %s]-[cSubject = %s]  [cMaster = %s] [cStartTime = %s]",
                                i, pQueryConfResult->iReturnCode, pQueryConfResult->cConfURI, pQueryConfResult->cSubject, pQueryConfResult->cMaster, pQueryConfResult->cStartTime);
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSConfEvent pQueryConfResultPara  [i = %d]- [iReturnCode= %d]-[cConfURI = %s]-[cSubject = %s]  [cMaster = %s] [cStartTime = %s]",
                     i, pQueryConfResult->iReturnCode, pQueryConfResult->cConfURI, pQueryConfResult->cSubject, pQueryConfResult->cMaster, pQueryConfResult->cStartTime);
        }

        id_fieldID = env->GetFieldID(class_ConfEventPara, "fireQueryConfResutStructEvent", "[Lcom/zte/softda/ocx/FireQueryConfResutStructEvent;");
        env->SetObjectField(obj_ConfEventPara, id_fieldID, array_QueryConfResultPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum, obj_ConfEventPara);
    }

    //ȡ��ԤԼ������
    if (ST_EVENT_CANCEL_ORDER_CONF == iType)
    {
        CANCEL_ORDER_CONF_RESULT_STRUCT *pCancelOrderResultPara = NULL;

        pCancelOrderResultPara = NULL;
        pCancelOrderResultPara = (CANCEL_ORDER_CONF_RESULT_STRUCT*)pConfEvent;
        if(pCancelOrderResultPara == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller pCancelOrderResultPara == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSConfEvent pCancelOrderResultPara == NULL");

            return;
        }
        jclass class_CancelOrderResultPara = env->FindClass("com/zte/softda/ocx/FireCancelOrderConfResutStructEvent");
        jmethodID id_CancelOrderResultPara  = env->GetMethodID(class_CancelOrderResultPara, "<init>", "()V");
        jobject obj_CancelOrderResultPara = env->NewObject(class_CancelOrderResultPara, id_CancelOrderResultPara);

        id_fieldID = env->GetFieldID(class_CancelOrderResultPara, "cConfURI", "Ljava/lang/String;");
        env->SetObjectField(obj_CancelOrderResultPara, id_fieldID, JNI_STR(env,pCancelOrderResultPara->cConfURI));

        id_fieldID = env->GetFieldID(class_CancelOrderResultPara, "iStatus", "I");
        env->SetIntField(obj_CancelOrderResultPara, id_fieldID, pCancelOrderResultPara->iStatus);

        id_fieldID = env->GetFieldID(class_ConfEventPara, "fireCancelOrderConfResutStructEvent", "Lcom/zte/softda/ocx/FireCancelOrderConfResutStructEvent;");
        env->SetObjectField(obj_ConfEventPara, id_fieldID, obj_CancelOrderResultPara);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent pQueryConfResultPara  return %d", pCancelOrderResultPara->iStatus);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSConfEvent pQueryConfResultPara  return %d", pCancelOrderResultPara->iStatus);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum, obj_ConfEventPara);
    }
    if (ST_EVENT_ORDER_CONF_NOTIFY == iType)
    {
        ORDER_CONF_NOTIFY_STRUCT *pOrderConfNotifyPara = NULL;

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent ST_EVENT_ORDER_CONF_NOTIFY");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSConfEvent ST_EVENT_ORDER_CONF_NOTIFY");

        pOrderConfNotifyPara = NULL;
        pOrderConfNotifyPara = (ORDER_CONF_NOTIFY_STRUCT*)pConfEvent;
        if(pOrderConfNotifyPara == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent pOrderConfNotifyPara == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSConfEvent pOrderConfNotifyPara == NULL");

            return;
        }
        jclass class_OrderConfNotifyPara = env->FindClass("com/zte/softda/ocx/FireOrderConfNotifyStructEvent");
        jmethodID id_OrderConfNotifyPara   = env->GetMethodID(class_OrderConfNotifyPara, "<init>", "()V");
        jobject obj_OrderConfNotifyPara = env->NewObject(class_OrderConfNotifyPara, id_OrderConfNotifyPara);

        id_fieldID = env->GetFieldID(class_OrderConfNotifyPara, "iType", "I");
        env->SetIntField(obj_OrderConfNotifyPara, id_fieldID, pOrderConfNotifyPara->iType);
        id_fieldID = env->GetFieldID(class_OrderConfNotifyPara, "iCallID", "I");
        env->SetIntField(obj_OrderConfNotifyPara, id_fieldID, pOrderConfNotifyPara->iCallID);

        id_fieldID = env->GetFieldID(class_OrderConfNotifyPara, "cConfURI", "Ljava/lang/String;");
        env->SetObjectField(obj_OrderConfNotifyPara, id_fieldID, JNI_STR(env,pOrderConfNotifyPara->cConfURI));
        id_fieldID = env->GetFieldID(class_OrderConfNotifyPara, "cChairPSW", "Ljava/lang/String;");
        env->SetObjectField(obj_OrderConfNotifyPara, id_fieldID, JNI_STR(env,pOrderConfNotifyPara->cChairPSW));
        id_fieldID = env->GetFieldID(class_OrderConfNotifyPara, "cConfSubject", "Ljava/lang/String;");
        env->SetObjectField(obj_OrderConfNotifyPara, id_fieldID, JNI_STR(env,pOrderConfNotifyPara->cConfSubject));

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent ST_EVENT_ORDER_CONF_NOTIFY	[iType = %d] ,[iCallID = %d] ,[cConfURI = %s], [cConfSubject = %s]",
                            pOrderConfNotifyPara->iType, pOrderConfNotifyPara->iCallID, pOrderConfNotifyPara->cConfURI, pOrderConfNotifyPara->cConfSubject);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "java_FireIMSConfEvent ST_EVENT_ORDER_CONF_NOTIFY	[iType = %d] ,[iCallID = %d] ,[cConfURI = %s], [cConfSubject = %s]",
                 pOrderConfNotifyPara->iType, pOrderConfNotifyPara->iCallID, pOrderConfNotifyPara->cConfURI, pOrderConfNotifyPara->cConfSubject);

        id_fieldID = env->GetFieldID(class_ConfEventPara, "fireOrderConfNotifyStructEvent", "Lcom/zte/softda/ocx/FireOrderConfNotifyStructEvent;");
        env->SetObjectField(obj_ConfEventPara, id_fieldID, obj_OrderConfNotifyPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum, obj_ConfEventPara);
    }
    //��ȡ�����Ա��Ϣ���
    if (GT_EVENT_QUERY_CONF_MEMBERLIST == iType)
    {
        PRE_CONF_MEMBER_LIST_EVENT *pPreConfMemberList = NULL;
        pPreConfMemberList = (PRE_CONF_MEMBER_LIST_EVENT*)pConfEvent;
        if(pPreConfMemberList == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent pPreConfMemberList == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "java_FireIMSConfEvent pPreConfMemberList == NULL");

            return;
        }

        jclass class_PreConfMemberLisPara = env->FindClass("com/zte/softda/ocx/FirePreConfMemerList");
        jmethodID id_PreConfMemberLisPara   = env->GetMethodID(class_PreConfMemberLisPara, "<init>", "()V");
        jobject obj_PreConfMemberLisPara = env->NewObject(class_PreConfMemberLisPara, id_PreConfMemberLisPara);

        id_fieldID = env->GetFieldID(class_PreConfMemberLisPara, "cConfURI", "Ljava/lang/String;");
        env->SetObjectField(obj_PreConfMemberLisPara, id_fieldID, JNI_STR(env,pPreConfMemberList->cConfURI));

        id_fieldID = env->GetFieldID(class_PreConfMemberLisPara, "iResultCode", "I");
        env->SetIntField(obj_PreConfMemberLisPara, id_fieldID, pPreConfMemberList->iResultCode);
        id_fieldID = env->GetFieldID(class_PreConfMemberLisPara, "iMemberNum", "I");
        env->SetIntField(obj_PreConfMemberLisPara, id_fieldID, pPreConfMemberList->iMemberNum);

        jclass class_FireMemberInfo = env->FindClass("com/zte/softda/ocx/FireMemberInfo");
        jmethodID id_FireMemberInfo  = env->GetMethodID(class_FireMemberInfo, "<init>", "()V");
        jobjectArray array_FireMemberInfo = env->NewObjectArray(pPreConfMemberList->iMemberNum, class_FireMemberInfo, NULL);
        for(int i = 0; i < pPreConfMemberList->iMemberNum; i++)
        {
            MEMBER_INFO_PARA* FireMemberInfo = &pPreConfMemberList->stMemberList[i];
            jobject obj_FireMemberInfo = env->NewObject(class_FireMemberInfo, id_FireMemberInfo);

            id_fieldID = env->GetFieldID(class_FireMemberInfo, "cMemberURI", "Ljava/lang/String;");
            env->SetObjectField(obj_FireMemberInfo, id_fieldID, JNI_STR(env,FireMemberInfo->cMemberURI));

            id_fieldID = env->GetFieldID(class_FireMemberInfo, "nMemProperty", "I");
            env->SetIntField(obj_FireMemberInfo, id_fieldID, FireMemberInfo->nMemProperty);

            env->SetObjectArrayElement(array_FireMemberInfo, i, obj_FireMemberInfo);
            env->DeleteLocalRef(obj_FireMemberInfo);
            

            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller GT_EVENT_QUERY_CONF_MEMBERLIST   [cMemberURI = %s]   [nMemProperty = %d]",
                                FireMemberInfo->cMemberURI, FireMemberInfo->nMemProperty);
            else if(g_WriteLogType == 1)
                LOGI(TAG, "GT_EVENT_QUERY_CONF_MEMBERLIST   [cMemberURI = %s]   [nMemProperty = %d]",
                     FireMemberInfo->cMemberURI, FireMemberInfo->nMemProperty);
        }

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller java_FireIMSConfEvent GT_EVENT_QUERY_CONF_MEMBERLIST	[cConfURI = %s]   [iResultCode = %d]  [iMemberNum = %d]",
                            pPreConfMemberList->cConfURI, pPreConfMemberList->iResultCode, pPreConfMemberList->iMemberNum);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "GT_EVENT_QUERY_CONF_MEMBERLIST  [cConfURI = %s]	[iResultCode = %d]	[iMemberNum = %d]",
                 pPreConfMemberList->cConfURI, pPreConfMemberList->iResultCode, pPreConfMemberList->iMemberNum);

        id_fieldID = env->GetFieldID(class_PreConfMemberLisPara, "firememerInfo", "[Lcom/zte/softda/ocx/FireMemberInfo;");
        env->SetObjectField(obj_PreConfMemberLisPara, id_fieldID, array_FireMemberInfo);

        id_fieldID = env->GetFieldID(class_ConfEventPara, "firePreConfMemerList", "Lcom/zte/softda/ocx/FirePreConfMemerList;");
        env->SetObjectField(obj_ConfEventPara, id_fieldID, obj_PreConfMemberLisPara);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iNum, obj_ConfEventPara);
    }
}
void java_FireIMSReqJoinPublicGroupSendResult(LPCTSTR pGroupURI, LPCTSTR pGroupIdentifier, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSReqJoinPublicGroupSendResult pGroupURI[%s] pGroupIdentifier[%s] iResult[%d]",
                        pGroupURI, pGroupIdentifier, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSReqJoinPublicGroupSendResult pGroupURI[%s] pGroupIdentifier[%s] iResult[%d]",
             pGroupURI, pGroupIdentifier, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSReqJoinPublicGroupSendResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSReqJoinPublicGroupSendResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSReqJoinPublicGroupSendResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI), JNI_STR(env,pGroupIdentifier), iResult);
}
void java_FireIMSLeavePublicGroupResult(LPCTSTR pGroupName, LPCTSTR pGroupURI, int iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSLeavePublicGroupResult pGroupName[%s] pGroupURI[%s] iResult[%d]",
                        pGroupName, pGroupURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSLeavePublicGroupResult pGroupName[%s] pGroupURI[%s] iResult[%d]",
             pGroupName, pGroupURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSLeavePublicGroupResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSLeavePublicGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSLeavePublicGroupResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupName), JNI_STR(env,pGroupURI), iResult);
}
void java_FireIMSDeletePublicGroupResult(LPCTSTR pGroupURI, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDeletePublicGroupResult pGroupURI[%s] iResult[%d]",
                        pGroupURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDeletePublicGroupResult pGroupURI[%s] iResult[%d]",
             pGroupURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSDeletePublicGroupResult", (char *)"(Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDeletePublicGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDeletePublicGroupResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pGroupURI), iResult);
}
void java_FireIMSDelFromMyPuGroupResult(LPCTSTR pSomeoneURI, LPCTSTR pGroupURI, long iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSDelFromMyPuGroupResult pSomeoneURI[%s] pGroupURI[%s] iResult[%d]",
                        pSomeoneURI, pGroupURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSDelFromMyPuGroupResult pSomeoneURI[%s] pGroupURI[%s] iResult[%d]",
             pSomeoneURI, pGroupURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSDelFromMyPuGroupResult", (char *)"(Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSDelFromMyPuGroupResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSDelFromMyPuGroupResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSomeoneURI), JNI_STR(env,pGroupURI), iResult);
}
void java_FireMsgIndicationNotify(short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    MsgIndication_T* pInfoPara = NULL;

    pInfoPara = (MsgIndication_T *)pPara;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireMsgIndicationNotify begin");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireMsgIndicationNotify begin");

    iresult = getStaticJniMethod((char *)"java_FireMsgIndicationNotify", (char *)"(Lcom/zte/softda/ocx/FireMsgIndicationPara;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireMsgIndicationNotify");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireMsgIndicationNotify");

        return;
    }
    jclass class_FireMsgIndicationPara = env->FindClass("com/zte/softda/ocx/FireMsgIndicationPara");
    jmethodID id_FireMsgIndicationPara  = env->GetMethodID(class_FireMsgIndicationPara, "<init>", "()V");
    jobject obj_FireMsgIndicationPara = env->NewObject(class_FireMsgIndicationPara, id_FireMsgIndicationPara);
    id_fieldID = env->GetFieldID(class_FireMsgIndicationPara, "cMemberURI", "Ljava/lang/String;");
    env->SetObjectField(obj_FireMsgIndicationPara, id_fieldID, JNI_STR(env,pInfoPara->cMemberURI));
    id_fieldID = env->GetFieldID(class_FireMsgIndicationPara, "mask", "I");
    env->SetIntField(obj_FireMsgIndicationPara, id_fieldID, pInfoPara->mask);
    id_fieldID = env->GetFieldID(class_FireMsgIndicationPara, "state", "I");
    env->SetIntField(obj_FireMsgIndicationPara, id_fieldID, pInfoPara->state);
    id_fieldID = env->GetFieldID(class_FireMsgIndicationPara, "tLastactive", "Ljava/lang/String;");
    env->SetObjectField(obj_FireMsgIndicationPara, id_fieldID, JNI_STR(env,pInfoPara->tLastactive));
    id_fieldID = env->GetFieldID(class_FireMsgIndicationPara, "refresh", "I");
    env->SetIntField(obj_FireMsgIndicationPara, id_fieldID, pInfoPara->refresh);
    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_FireMsgIndicationPara);
}

void java_FireGotRegInfoNotify(int IResult, short far* pPara)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    LastLogInfo_T* pInfoPara = NULL;
    pInfoPara = (LastLogInfo_T *)pPara;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireGotRegInfoNotify begin %s", pInfoPara->friendURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireGotRegInfoNotify begin %s", pInfoPara->friendURI);

    iresult = getStaticJniMethod((char *)"java_FireGotRegInfoNotify", (char *)"(ILcom/zte/softda/ocx/FireLastLogInfo;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireGotRegInfoNotify");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireGotRegInfoNotify");

        return;
    }
    jclass class_FireGotRegInfoNotify = env->FindClass("com/zte/softda/ocx/FireLastLogInfo");
    jmethodID id_FireGotRegInfoNotify  = env->GetMethodID(class_FireGotRegInfoNotify, "<init>", "()V");
    jobject obj_FireGotRegInfoNotify = env->NewObject(class_FireGotRegInfoNotify, id_FireGotRegInfoNotify);
    id_fieldID = env->GetFieldID(class_FireGotRegInfoNotify, "friendURI", "Ljava/lang/String;");
    env->SetObjectField(obj_FireGotRegInfoNotify, id_fieldID, JNI_STR(env,pInfoPara->friendURI));
    id_fieldID = env->GetFieldID(class_FireGotRegInfoNotify, "lastlogintime", "Ljava/lang/String;");
    env->SetObjectField(obj_FireGotRegInfoNotify, id_fieldID, JNI_STR(env,pInfoPara->lastlogintime));
    id_fieldID = env->GetFieldID(class_FireGotRegInfoNotify, "lastlogouttime", "Ljava/lang/String;");
    env->SetObjectField(obj_FireGotRegInfoNotify, id_fieldID, JNI_STR(env,pInfoPara->lastlogouttime));
    env->CallStaticVoidMethod(gpostClass, gJinMethod, IResult, obj_FireGotRegInfoNotify);
}
void java_FireIMSSubResult(int iType, LPCTSTR pURI, int iResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSSubResult iType[%d] pURI[%s] iResult[%d]", iType, pURI, iResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSSubResult iType[%d] pURI[%s] iResult[%d]", iType, pURI, iResult);

    iresult = getStaticJniMethod((char *)"java_FireIMSSubResult", (char *)"(ILjava/lang/String;I)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSSubResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSSubResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, JNI_STR(env,pURI), iResult);
}

void WBMakeBaseInfo(JNIEnv *env, jclass cls, jobject obj, WbUnknownPtr_t pUnKnown)
{
	if (NULL == pUnKnown)
	{
		return;
	}
	
	jfieldID id_fieldID;
	id_fieldID = env->GetFieldID(cls, "enumShapeType", "I");
    	env->SetIntField(obj, id_fieldID, (int)pUnKnown->enumShapeType);
	id_fieldID = env->GetFieldID(cls, "chID", "Ljava/lang/String;");
    	env->SetObjectField(obj, id_fieldID, JNI_STR(env,pUnKnown->chID));
	id_fieldID = env->GetFieldID(cls, "iObjectOper", "I");
    	env->SetIntField(obj, id_fieldID, pUnKnown->iObjectOper);
}

jobject WBMakeStyle(JNIEnv *env, WbUnknownPtr_t pUnKnown)
{
	if (NULL == pUnKnown)
	{
		return NULL;
	}

	WbStylePtr_t  style = NULL;
	switch(pUnKnown->enumShapeType)
	{
		case WB_ST_RECT:
		{
			WbRectPtr_t pWBPtr = (WbRectPtr_t)pUnKnown;
			style = pWBPtr->style;
		}
		break;
		case WB_ST_LINE:
		{
			WbLinePtr_t pWBPtr = (WbLinePtr_t)pUnKnown;
			style = pWBPtr->style;	
		}
		break;
		case WB_ST_CIRCLE:
		{
			WbCirclePtr_t pWBPtr = (WbCirclePtr_t)pUnKnown;
			style = pWBPtr->style;	
		}
		break;
		case WB_ST_ELLIPSE:
		{
			WbEllipsePtr_t pWBPtr = (WbEllipsePtr_t)pUnKnown;
			style = pWBPtr->style;	
		}
		break;
		case WB_ST_PATH:
		{
			WbPathPtr_t pWBPtr = (WbPathPtr_t)pUnKnown;
			style = pWBPtr->style;	
		}
		break;
		case WB_ST_TEXT:
		{
			WbTextPtr_t pWBPtr = (WbTextPtr_t)pUnKnown;
			style = pWBPtr->style;	
		}
		break;
		case WB_ST_IMAGE:
		{
			WbImagePtr_t pWBPtr = (WbImagePtr_t)pUnKnown;
			//style = pWBPtr->style;	
		}
		break;
		case WB_ST_DOC_PAGE:
		{
			WbDocPagePtr_t pWBPtr = (WbDocPagePtr_t)pUnKnown;
			//style = pWBPtr->style;	
		}
		break;
		default:break;
	}

	if (NULL != style)
	{
		jfieldID id_fieldID;
		jclass class_wbStyle= env->FindClass("com/zte/softda/ocx/conference/wb/WBStyle");
    		jmethodID id_wbStyle  = env->GetMethodID(class_wbStyle, "<init>", "()V");
    		jobject obj_wbStyle = env->NewObject(class_wbStyle, id_wbStyle);
//		DEBUG_INFO("[WBMakeStyle] class_wbStyle=%x, id_wbStyle=%x, obj_wbStyle=%x"
//			, class_wbStyle, id_wbStyle, obj_wbStyle);
		id_fieldID = env->GetFieldID(class_wbStyle, "chFillColor", "Ljava/lang/String;");
    		env->SetObjectField(obj_wbStyle, id_fieldID, JNI_STR(env,style->chFillColor));
		id_fieldID = env->GetFieldID(class_wbStyle, "chStrokeColor", "Ljava/lang/String;");
    		env->SetObjectField(obj_wbStyle, id_fieldID, JNI_STR(env,style->chStrokeColor));
		id_fieldID = env->GetFieldID(class_wbStyle, "iStrokeWidth", "I");
    		env->SetIntField(obj_wbStyle, id_fieldID, style->iStrokeWidth);
		id_fieldID = env->GetFieldID(class_wbStyle, "fFillOpacity", "F");
    		env->SetFloatField(obj_wbStyle, id_fieldID, style->fFillOpacity);
		if (pUnKnown->enumShapeType == WB_ST_TEXT)
		{
			id_fieldID = env->GetFieldID(class_wbStyle, "iFontSize", "I");
    			env->SetIntField(obj_wbStyle, id_fieldID, style->iFontSize);
			id_fieldID = env->GetFieldID(class_wbStyle, "chFontFamily", "Ljava/lang/String;");
    			env->SetObjectField(obj_wbStyle, id_fieldID, JNI_STR(env,style->chFontFamily));
			id_fieldID = env->GetFieldID(class_wbStyle, "enumTextAnchor", "I");
    			env->SetIntField(obj_wbStyle, id_fieldID, (int)style->enumTextAnchor);
			id_fieldID = env->GetFieldID(class_wbStyle, "iBold", "I");
    			env->SetIntField(obj_wbStyle, id_fieldID, style->iBold);
			id_fieldID = env->GetFieldID(class_wbStyle, "iItalic", "I");
    			env->SetIntField(obj_wbStyle, id_fieldID, style->iItalic);
			id_fieldID = env->GetFieldID(class_wbStyle, "iTextDecorateType", "I");
    			env->SetIntField(obj_wbStyle, id_fieldID, style->iTextDecorateType);
		}

		return obj_wbStyle;
	}

	return NULL;
}

int GetPageCount(WbPageListPtr_t pPageList)
{
	int nSize = 0;
	WbPageListPtr_t pPage = pPageList;
	DEBUG_INFO("[GetPageCount] pPageList=%x", pPageList);
	while (pPage != NULL)
	{
		nSize++;
		pPage = pPage->pPageNext;
	}
	DEBUG_INFO("[GetPageCount] SIZE=%d", nSize);
	return nSize;
}

int GetShapeListSize(WbShapeListPtr_t pShapeList)
{
	int nSize = 0;
	WbShapeListPtr_t pShape = pShapeList;
	DEBUG_INFO("[GetShapeListSize] pShapeList=%x", pShapeList);
	while (pShape != NULL)
	{
		nSize++;
		pShape = pShape->pShapeNext;
	}
	DEBUG_INFO("[GetShapeListSize] SIZE=%d", nSize);
	return nSize;
}

void java_FireDataConfEvent(int iType,  short far* pConfEvent)
{
	jmethodID gJinMethod;
    	JNIEnv *env;
    	int iresult = -1;
    	jfieldID id_fieldID;

    	if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireDataConfEvent begin %d", iType);
    	else if(g_WriteLogType == 1)
        	LOGI(TAG, "java_FireDataConfEventbegin %d", iType);

    	iresult = getStaticJniMethod((char *)"java_FireDataConfEvent", (char *)"(ILcom/zte/softda/ocx/conference/ConfEventPara;)V", &env, &gJinMethod);
    	if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    	{
        	if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireDataConfEvent");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireDataConfEvent");

        	return;
    	}

	//��װjava���ϱ�����
	jclass class_FireDataConfEvent= env->FindClass("com/zte/softda/ocx/conference/ConfEventPara");
    	jmethodID id_FireDataConfEvent  = env->GetMethodID(class_FireDataConfEvent, "<init>", "()V");
    	jobject obj_FireDataConfEvent = env->NewObject(class_FireDataConfEvent, id_FireDataConfEvent);

	switch(iType)
	{
		case GT_EVENT_CONF_CONTROL:
			{
				LP_CONF_CONTROL_EVENT pConfCtrlStruct = (LP_CONF_CONTROL_EVENT)pConfEvent;

				jclass class_ConfCtrlEvent= env->FindClass("com/zte/softda/ocx/conference/ConfCtrlEventPara");
    				jmethodID id_ConfCtrlEvent  = env->GetMethodID(class_ConfCtrlEvent, "<init>", "()V");
    				jobject obj_ConfCtrlEvent = env->NewObject(class_ConfCtrlEvent, id_ConfCtrlEvent);	

				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iType", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iType);
				TraceMsgWindow1(1, "pConfCtrlStruct->iType=%d, pConfCtrlStruct->iConfType=%d, pConfCtrlStruct->iResult=%d"
					, pConfCtrlStruct->iType, pConfCtrlStruct->iConfType, pConfCtrlStruct->iResult);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iResult", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iResult);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cConfURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cConfURI));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cSubject", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cSubject));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iConfType", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iConfType);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cMemberURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cMemberURI));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iStatus", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iStatus);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cMasterURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cMasterURI));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iMaxNum", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iMaxNum);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iPeriod", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iPeriod);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cStartTime", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cStartTime));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cDConfID", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cDConfID));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cDConfPassword", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cDConfPassword));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iDestType", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iDestType);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cSubXconID", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cSubXconID));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "c183Reason", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->c183Reason));
				//iSubConfType					
				jintArray array = env->NewIntArray(13);
        			jint iStatus[13];
       		       for (int i = 0; i < 13; i++)
        			{
            				iStatus[i] = pConfCtrlStruct->iSubConfType[i];
        			}
        			env->SetIntArrayRegion(array, 0, 13,  iStatus);
        			id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iSubConfType", "[I");
        			env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, array);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "iSubConfNum", "I");
				env->SetIntField(obj_ConfCtrlEvent, id_fieldID, pConfCtrlStruct->iSubConfNum);
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cSubConfWBID", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cSubConfWBID));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cSubConfAppShareID", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cSubConfAppShareID));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cServer", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cServer));
				id_fieldID = env->GetFieldID(class_ConfCtrlEvent, "cHttpServerPath", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfCtrlEvent, id_fieldID, JNI_STR(env,pConfCtrlStruct->cHttpServerPath));

				id_fieldID = env->GetFieldID(class_FireDataConfEvent, "confCtrlEventPara", "Lcom/zte/softda/ocx/conference/ConfCtrlEventPara;");
    				env->SetObjectField(obj_FireDataConfEvent, id_fieldID, obj_ConfCtrlEvent);
			}
			break;
		case GT_EVENT_CONF_PROPERTY:
			{
				CONF_ATTR_CONTROL_EVENT *pConfPropertyStruct = NULL;
				pConfPropertyStruct = (CONF_ATTR_CONTROL_EVENT *)pConfEvent;

				jclass class_ConfPropertyEvent= env->FindClass("com/zte/softda/ocx/conference/ConfPropertyEventPara");
    				jmethodID id_ConfPropertyEvent  = env->GetMethodID(class_ConfPropertyEvent, "<init>", "()V");
    				jobject obj_ConfPropertyEvent = env->NewObject(class_ConfPropertyEvent, id_ConfPropertyEvent);	

				id_fieldID = env->GetFieldID(class_ConfPropertyEvent, "iType", "I");
				env->SetIntField(obj_ConfPropertyEvent, id_fieldID, pConfPropertyStruct->iType);
				id_fieldID = env->GetFieldID(class_ConfPropertyEvent, "cConfURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfPropertyEvent, id_fieldID, JNI_STR(env, pConfPropertyStruct->cConfURI));
				id_fieldID = env->GetFieldID(class_ConfPropertyEvent, "iTime", "I");
				env->SetIntField(obj_ConfPropertyEvent, id_fieldID, pConfPropertyStruct->iTime);
				id_fieldID = env->GetFieldID(class_ConfPropertyEvent, "iResult", "I");
				env->SetIntField(obj_ConfPropertyEvent, id_fieldID, pConfPropertyStruct->iResult);

				id_fieldID = env->GetFieldID(class_FireDataConfEvent, "confPropertyEventPara", "Lcom/zte/softda/ocx/conference/ConfPropertyEventPara;");
    				env->SetObjectField(obj_FireDataConfEvent, id_fieldID, obj_ConfPropertyEvent);
			}
			break;
		case GT_EVENT_CONF_STATUS_CHANGE:
			{
				LP_CONF_STATUS_EVENT pConfStatusStruct = (LP_CONF_STATUS_EVENT)pConfEvent;
					
				jclass class_ConfStatusEvent= env->FindClass("com/zte/softda/ocx/conference/ConfStatusEventPara");
    				jmethodID id_ConfStatusEvent  = env->GetMethodID(class_ConfStatusEvent, "<init>", "()V");
    				jobject obj_ConfStatusEvent = env->NewObject(class_ConfStatusEvent, id_ConfStatusEvent);
						
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iType", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iType);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cConfURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cConfURI));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cSubject", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cSubject));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iMax", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iMax);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iCurrentNum", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iCurrentNum);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bActive", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bActive);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bLocked", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bLocked);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cCtrlMode", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cCtrlMode));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bMute", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bMute);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bRecord", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bRecord);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bFile", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bFile);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bLayout", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bLayout);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cVideoSync", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cVideoSync));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bRevolve", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bRevolve);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iMaxVideoNum", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iVideoMaxNum);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iVideoNum", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iVideoNum);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iVideoLayout", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iVideoLayout);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bVideoMode", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bVideoMode);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iCycleStatus", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iCycleStatus);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iCyclePolicy", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iCyclePolicy);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iRemainTime", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iRemainTime);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iExtendTime", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iExtendTime);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iPeriod", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iPeriod);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iBKMusicIndex", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iBKMusicIndex);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iReleaseMode", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iReleaseMode);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cUserURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cUserURI));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cRoles", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cRoles));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iBKMusicIndex", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iBKMusicIndex);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bSpeak", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bSpeak);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "bListen", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->bListen);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cEndpointURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cEndpointURI));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cStatus", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cStatus));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cShow", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cShow));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iOnline", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iOnline);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cRealMedias", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cRealMedias));
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iSubContrlRight", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iSubContrlRight);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "iUserID", "I");
				env->SetIntField(obj_ConfStatusEvent, id_fieldID, pConfStatusStruct->iUserID);
				id_fieldID = env->GetFieldID(class_ConfStatusEvent, "cNickName", "Ljava/lang/String;");
    				env->SetObjectField(obj_ConfStatusEvent, id_fieldID, JNI_STR(env,pConfStatusStruct->cNickName));

				id_fieldID = env->GetFieldID(class_FireDataConfEvent, "confStatusEventPara", "Lcom/zte/softda/ocx/conference/ConfStatusEventPara;");
    				env->SetObjectField(obj_FireDataConfEvent, id_fieldID, obj_ConfStatusEvent);
			}
			break;
		default:break;
	}

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_FireDataConfEvent);
}

/*
void java_FireInitControlResult(long lResult)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireInitControlResult result:%d", lResult);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireInitControlResult result:%d", lResult);

    iresult = getStaticJniMethod((char *)"java_FireInitControlResult", (char *)"(I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireInitControlResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireInitControlResult");

        return;
    }
	
    env->CallStaticVoidMethod(gpostClass, gJinMethod, lResult);
}
*/
void BirdgeEventToString(ConfBridgeControlEvent_T& params, char* buf, size_t bufSize)
{
	snprintf(buf, bufSize, "type=%d,szConfURI=%s,szCreatorURI=%s,iConfType=%d,szSubject=%s,szStartTime=%s"
		"szStatus=%s,szServer=%s,szSoftPhone=%s,szPhonePasswd=%s, szIp=%s,expireTime=%d"
		",szChairPasswd=%s,szMemberPasswd=%s,status=%d,nPort=%d,nTotalPort=%d,nUsedPort=%d,nReserved=%d,errorCode=%d,nConfBridgeInfoCount=%d"
	, params.type,params.szConfURI, params.szCreatorURI, params.iConfType, params.szSubject, params.szStartTime
	, params.szStatus,params.szServer,params.szSoftPhone,params.szPhonePasswd, params.szIp,params.expireTime
	,params.szChairPasswd,params.szMemberPasswd,params.status,params.nPort,params.nTotalPort,params.nUsedPort,params.nReserved,params.errorCode,params.nConfBridgeInfoCount);
}

void java_FireConfBridgeControlResult(int iType, short far* pResult)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	jfieldID id_fieldID;
	ConfBridgeControlEvent_T* pInfoPara = NULL;
	int i = 0;
	pInfoPara = (ConfBridgeControlEvent_T *)pResult;
	char buf[2048] = "";
	BirdgeEventToString(*pInfoPara, buf, 2047);
	
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireConfBridgeControlResult iType=%d, params{%s}", iType, buf);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireConfBridgeControlResult iType=%d, params{%s}", iType, buf);

    iresult = getStaticJniMethod((char *)"java_FireConfBridgeControlResult", (char *)"(ILcom/zte/softda/ocx/confbridge/ConfBridgeControlEvent;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireConfBridgeControlResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireConfBridgeControlResult");

        return;
    }
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/confbridge/ConfBridgeControlEvent");
	objEvent.NewObject();
	objEvent.SetIntValue("type", pInfoPara->type);
	objEvent.SetStringValue("szConfURI", pInfoPara->szConfURI);
	objEvent.SetStringValue("szCreatorURI", pInfoPara->szCreatorURI);
	objEvent.SetIntValue("iConfType", pInfoPara->iConfType);
	objEvent.SetStringValue("szSubject", pInfoPara->szSubject);
	objEvent.SetStringValue("szStartTime", pInfoPara->szStartTime);
	objEvent.SetStringValue("szStatus", pInfoPara->szStatus);
	objEvent.SetStringValue("szServer", pInfoPara->szServer);
	objEvent.SetStringValue("szSoftPhone", pInfoPara->szSoftPhone);
	objEvent.SetStringValue("szPhonePasswd", pInfoPara->szPhonePasswd);
	objEvent.SetStringValue("szIp", pInfoPara->szIp);
	objEvent.SetIntValue("expireTime", pInfoPara->expireTime);
	objEvent.SetStringValue("szChairPasswd", pInfoPara->szChairPasswd);
	objEvent.SetStringValue("szMemberPasswd", pInfoPara->szMemberPasswd);
	objEvent.SetIntValue("status", pInfoPara->status);
	objEvent.SetIntValue("nPort", pInfoPara->nPort);
	objEvent.SetIntValue("nTotalPort", pInfoPara->nTotalPort);
	objEvent.SetIntValue("nUsedPort", pInfoPara->nUsedPort);
	objEvent.SetIntValue("nReserved", pInfoPara->nReserved);
	objEvent.SetIntValue("errorCode", pInfoPara->errorCode);

	if (pInfoPara->nConfBridgeInfoCount > 0)
	{
		jclass classBridgeInfo = env->FindClass("com/zte/softda/ocx/confbridge/ConfBridgeInfo");
		jmethodID initMethod  = env->GetMethodID(classBridgeInfo, "<init>", "()V");
		jobjectArray arrayBridgeInfo = env->NewObjectArray(pInfoPara->nConfBridgeInfoCount, classBridgeInfo, NULL);

		for (i = 0; i < pInfoPara->nConfBridgeInfoCount; i++)
		{
			ConfBridgeInfo_T * pInfo = pInfoPara->pConfBridgeInfoArray + i;
			JOjbectWrapper objBridgeInfo(env, classBridgeInfo, NULL);
			objBridgeInfo.NewObject();
			objBridgeInfo.SetIntValue("iConfType", pInfo->iConfType);
			objBridgeInfo.SetStringValue("szStartTime", pInfo->szStartTime);
			objBridgeInfo.SetIntValue("nMaxMember", pInfo->nMaxMember);
			objBridgeInfo.SetIntValue("nPeriod", pInfo->nPeriod);
			objBridgeInfo.SetIntValue("nLimit", pInfo->nLimit);
			objBridgeInfo.SetStringValue("szSubject", pInfo->szSubject);
			objBridgeInfo.SetStringValue("szChairPasswd", pInfo->szChairPasswd);
			objBridgeInfo.SetStringValue("szMemberPasswd", pInfo->szMemberPasswd);
			objBridgeInfo.SetStringValue("szConfURI", pInfo->szConfURI);
			objBridgeInfo.SetStringValue("szServer", pInfo->szServer);

			env->SetObjectArrayElement(arrayBridgeInfo, i, objBridgeInfo.GetObject());
		}
		objEvent.SetContainsObject("pConfBridgeInfoArray", "[Lcom/zte/softda/ocx/confbridge/ConfBridgeInfo;", arrayBridgeInfo);
	}

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, objEvent.GetObject());
}

//lmd20141204
void java_FireDataConfFileDownload(long iOperateType, long iSuccess)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireDataConfFileDownload iOperateType[%d] iSuccess[%d]", iOperateType, iSuccess);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireDataConfFileDownload iOperateType[%d] iSuccess[%d]", iOperateType, iSuccess);

    iresult = getStaticJniMethod((char *)"java_FireDataConfFileDownload", (char *)"(II)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireDataConfFileDownload");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireDataConfFileDownload");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iOperateType,iSuccess);
}

void java_FireDataConfFileUploadResult(long iOperateType, long iSuccess)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireDataConfFileUploadResult iOperateType[%d] iSuccess[%d]", iOperateType, iSuccess);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireDataConfFileUploadResult iOperateType[%d] iSuccess[%d]", iOperateType, iSuccess);

    iresult = getStaticJniMethod((char *)"java_FireDataConfFileUploadResult", (char *)"(II)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireDataConfFileUploadResult");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireDataConfFileUploadResult");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iOperateType,iSuccess);
}

void java_FireTransScheduleNotify(long iOperateType, long iType,long iTotalSize, long iFinishedSize)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireTransScheduleNotify iOperateType[%d] iType[%d] iTotalSize[%d] iFinishedSize[%d]", iOperateType,iType,iTotalSize, iFinishedSize);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireTransScheduleNotify iOperateType[%d] iType[%d] iTotalSize[%d] iFinishedSize[%d]", iOperateType,iType,iTotalSize, iFinishedSize);

    iresult = getStaticJniMethod((char *)"java_FireTransScheduleNotify", (char *)"(IIII)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireTransScheduleNotify");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireTransScheduleNotify");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iOperateType,iType,iTotalSize, iFinishedSize);
}
//lmd20141204, end

void java_FireRecvFromMrsEvent(int iType, short far* pRecvMessage)
{
	jmethodID gJinMethod;
    	JNIEnv *env;
    	int iresult = -1;
    	jfieldID id_fieldID;

    	if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireRecvFromMrsEvent begin %d", iType);
    	else if(g_WriteLogType == 1)
        	LOGI(TAG, "java_FireRecvFromMrsEvent begin %d", iType);

    	iresult = getStaticJniMethod((char *)"java_FireRecvFromMrsEvent", (char *)"(ILcom/zte/softda/ocx/conference/ConfMrsEventPara;)V", &env, &gJinMethod);
    	if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    	{
        	if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireRecvFromMrsEvent");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireRecvFromMrsEvent");

        	return;
    	}

	//��װjava���ϱ�����
	jclass class_FireRecvFromMrsEvent= env->FindClass("com/zte/softda/ocx/conference/ConfMrsEventPara");
    	jmethodID id_FireRecvFromMrsEvent  = env->GetMethodID(class_FireRecvFromMrsEvent, "<init>", "()V");
    	jobject obj_FireRecvFromMrsEvent = env->NewObject(class_FireRecvFromMrsEvent, id_FireRecvFromMrsEvent);
	
	switch(iType)
	{
	case DATA_CONF_APP_SHARE:
		{
		}
		break;
		case MRS_EVENT_XWB:
			{
				LP_WB_CONTROL_EVENT pWBStruct = (LP_WB_CONTROL_EVENT)pRecvMessage;
			
				jclass class_wbEvent= env->FindClass("com/zte/softda/ocx/conference/wb/WBControlEvent");
    				jmethodID id_wbEvent  = env->GetMethodID(class_wbEvent, "<init>", "()V");
    				jobject obj_wbEvent = env->NewObject(class_wbEvent, id_wbEvent);		
					
    				id_fieldID = env->GetFieldID(class_wbEvent, "iType", "I");
    				env->SetIntField(obj_wbEvent, id_fieldID, pWBStruct->iType);
				id_fieldID = env->GetFieldID(class_wbEvent, "iResult", "I");
    				env->SetIntField(obj_wbEvent, id_fieldID, pWBStruct->iResult);
    				id_fieldID = env->GetFieldID(class_wbEvent, "chConfURI", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chConfURI));
    				id_fieldID = env->GetFieldID(class_wbEvent, "chChannelID", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chChannelID));
				id_fieldID = env->GetFieldID(class_wbEvent, "uXWBID", "I");
    				env->SetIntField(obj_wbEvent, id_fieldID, pWBStruct->uXWBID);
				id_fieldID = env->GetFieldID(class_wbEvent, "chSizeX", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chSizeX));
				id_fieldID = env->GetFieldID(class_wbEvent, "chSizeY", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chSizeY));
				id_fieldID = env->GetFieldID(class_wbEvent, "chPosX", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chPosX));
				id_fieldID = env->GetFieldID(class_wbEvent, "chPosY", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chPosY));
				id_fieldID = env->GetFieldID(class_wbEvent, "chWidth", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chWidth));
				id_fieldID = env->GetFieldID(class_wbEvent, "chHeight", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chHeight));
				id_fieldID = env->GetFieldID(class_wbEvent, "chResolutionX", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chResolutionX));
				id_fieldID = env->GetFieldID(class_wbEvent, "chResolutionY", "Ljava/lang/String;");
    				env->SetObjectField(obj_wbEvent, id_fieldID, JNI_STR(env,pWBStruct->chResolutionY));
				id_fieldID = env->GetFieldID(class_wbEvent, "iPageCount", "I");
    				env->SetIntField(obj_wbEvent, id_fieldID, pWBStruct->iPageCount);
				//make pagelist
				if (NULL != pWBStruct->pPageList)
				{
					int i = 0;
					jclass class_Page = env->FindClass("com/zte/softda/ocx/conference/wb/WBPage");
    					jmethodID id_PageConstructor  = env->GetMethodID(class_Page, "<init>", "()V");
   					jobjectArray array_PageList = env->NewObjectArray(GetPageCount(pWBStruct->pPageList), class_Page, NULL);
					 
					WbPageListPtr_t pCurPage = pWBStruct->pPageList;
					while(pCurPage)
					{
   						jobject obj_Page= env->NewObject(class_Page, id_PageConstructor);
							
						int j = 0;
						jclass class_ShapeList = env->FindClass("com/zte/softda/ocx/conference/wb/WBShape");
    						jmethodID id_ShapeList  = env->GetMethodID(class_ShapeList, "<init>", "()V");
   						jobjectArray array_ShapeList = env->NewObjectArray(GetShapeListSize(pCurPage->pShapeList), class_ShapeList, NULL);
						
						WbShapeListPtr_t pCurShape = pCurPage->pShapeList;
						while(pCurShape)
						{
							WbUnknownPtr_t pUnKnown = (WbUnknownPtr_t)pCurShape->pShapedata;
							if (NULL == pUnKnown)
							{
								continue;
							}
							
							switch(pUnKnown->enumShapeType)
							{
								case WB_ST_RECT:
									{
										WbRectPtr_t pWBPtr = (WbRectPtr_t)pUnKnown;
										jclass class_wbRect= env->FindClass("com/zte/softda/ocx/conference/wb/RectShape");
    										jmethodID id_wbRect  = env->GetMethodID(class_wbRect, "<init>", "()V");
    										jobject obj_wbRect = env->NewObject(class_wbRect, id_wbRect);
									
										//make base info
										WBMakeBaseInfo(env, class_wbRect, obj_wbRect, pUnKnown);
										//make style
										jobject obj_style = WBMakeStyle(env, pUnKnown);
										id_fieldID = env->GetFieldID(class_wbRect, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
    										env->SetObjectField(obj_wbRect, id_fieldID, obj_style);
										//
										id_fieldID = env->GetFieldID(class_wbRect, "chX", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbRect, id_fieldID, JNI_STR(env,pWBPtr->chX));
										id_fieldID = env->GetFieldID(class_wbRect, "chY", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbRect, id_fieldID, JNI_STR(env,pWBPtr->chY));
										id_fieldID = env->GetFieldID(class_wbRect, "chWidth", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbRect, id_fieldID, JNI_STR(env,pWBPtr->chWidth));
										id_fieldID = env->GetFieldID(class_wbRect, "chHeight", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbRect, id_fieldID, JNI_STR(env,pWBPtr->chHeight));
										id_fieldID = env->GetFieldID(class_wbRect, "chRx", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbRect, id_fieldID, JNI_STR(env,pWBPtr->chRx));
										id_fieldID = env->GetFieldID(class_wbRect, "chRy", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbRect, id_fieldID, JNI_STR(env,pWBPtr->chRy));
										//add one shape to shapelist
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbRect);
									}
									break;
								case WB_ST_LINE:
									{
										WbLinePtr_t pWBPtr = (WbLinePtr_t)pUnKnown;
										jclass class_wbLine= env->FindClass("com/zte/softda/ocx/conference/wb/LineShape");
    										jmethodID id_wbLine  = env->GetMethodID(class_wbLine, "<init>", "()V");
    										jobject obj_wbLine = env->NewObject(class_wbLine, id_wbLine);
									
										//make base info
										WBMakeBaseInfo(env, class_wbLine, obj_wbLine, pUnKnown);
										//make style
										jobject obj_style = WBMakeStyle(env, pUnKnown);
										id_fieldID = env->GetFieldID(class_wbLine, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
										DEBUG_INFO("obj_wbLine=%x, id_fieldID=%x, obj_style=%x", obj_wbLine, id_fieldID, obj_style);
    										env->SetObjectField(obj_wbLine, id_fieldID, obj_style);
										//
										DEBUG_INFO("xxxxxxxxxxxxxxxxxxxx");
										id_fieldID = env->GetFieldID(class_wbLine, "chStartX", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbLine, id_fieldID, JNI_STR(env,pWBPtr->chStartX));
										id_fieldID = env->GetFieldID(class_wbLine, "chStartY", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbLine, id_fieldID, JNI_STR(env,pWBPtr->chStartY));
										id_fieldID = env->GetFieldID(class_wbLine, "chEndX", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbLine, id_fieldID, JNI_STR(env,pWBPtr->chEndX));
										id_fieldID = env->GetFieldID(class_wbLine, "chEndY", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbLine, id_fieldID, JNI_STR(env,pWBPtr->chEndY));
										//add one shape to shapelist
										DEBUG_INFO("SetObjectArrayElement, array_ShapeList=%x, j=%d, obj_wbLine=%x", array_ShapeList, obj_wbLine);
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbLine);
										DEBUG_INFO("aaaaaaaaaaaaaaaa");
									}
									break;
								case WB_ST_CIRCLE:
									{
										WbCirclePtr_t pWBPtr = (WbCirclePtr_t)pUnKnown;
										jclass class_wbCircle= env->FindClass("com/zte/softda/ocx/conference/wb/CircleShape");
    										jmethodID id_wbCircle  = env->GetMethodID(class_wbCircle, "<init>", "()V");
    										jobject obj_wbCircle = env->NewObject(class_wbCircle, id_wbCircle);
									
										//make base info
										WBMakeBaseInfo(env, class_wbCircle, obj_wbCircle, pUnKnown);
										//make style
										jobject obj_style = WBMakeStyle(env, pUnKnown);
										id_fieldID = env->GetFieldID(class_wbCircle, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
    										env->SetObjectField(obj_wbCircle, id_fieldID, obj_style);
										//
										id_fieldID = env->GetFieldID(class_wbCircle, "chCx", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbCircle, id_fieldID, JNI_STR(env,pWBPtr->chCx));
										id_fieldID = env->GetFieldID(class_wbCircle, "chCy", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbCircle, id_fieldID, JNI_STR(env,pWBPtr->chCy));
										id_fieldID = env->GetFieldID(class_wbCircle, "chR", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbCircle, id_fieldID, JNI_STR(env,pWBPtr->chR));
										//add one shape to shapelist
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbCircle);
									}
									break;
								case WB_ST_ELLIPSE:
									{
										WbEllipsePtr_t pWBPtr = (WbEllipsePtr_t)pUnKnown;
										jclass class_wbEllipse= env->FindClass("com/zte/softda/ocx/conference/wb/EllipseShape");
    										jmethodID id_wbEllipse  = env->GetMethodID(class_wbEllipse, "<init>", "()V");
    										jobject obj_wbEllipse = env->NewObject(class_wbEllipse, id_wbEllipse);
									
										//make base info
										WBMakeBaseInfo(env, class_wbEllipse, obj_wbEllipse, pUnKnown);
										//make style
										jobject obj_style = WBMakeStyle(env, pUnKnown);
										id_fieldID = env->GetFieldID(class_wbEllipse, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
    										env->SetObjectField(obj_wbEllipse, id_fieldID, obj_style);
										//
										id_fieldID = env->GetFieldID(class_wbEllipse, "chCx", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbEllipse, id_fieldID, JNI_STR(env,pWBPtr->chCx));
										id_fieldID = env->GetFieldID(class_wbEllipse, "chCy", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbEllipse, id_fieldID, JNI_STR(env,pWBPtr->chCy));
										id_fieldID = env->GetFieldID(class_wbEllipse, "chRx", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbEllipse, id_fieldID, JNI_STR(env,pWBPtr->chRx));
										id_fieldID = env->GetFieldID(class_wbEllipse, "chRy", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbEllipse, id_fieldID, JNI_STR(env,pWBPtr->chRy));											
										//add one shape to shapelist
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbEllipse);
									}
									break;
								case WB_ST_PATH:
									{
										WbPathPtr_t pWBPtr = (WbPathPtr_t)pUnKnown;
										jclass class_wbPath= env->FindClass("com/zte/softda/ocx/conference/wb/PathShape");
    										jmethodID id_wbPath  = env->GetMethodID(class_wbPath, "<init>", "()V");
    										jobject obj_wbPath = env->NewObject(class_wbPath, id_wbPath);
									
										//make base info
										WBMakeBaseInfo(env, class_wbPath, obj_wbPath, pUnKnown);
										//make style
										jobject obj_style = WBMakeStyle(env, pUnKnown);
										id_fieldID = env->GetFieldID(class_wbPath, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
    										env->SetObjectField(obj_wbPath, id_fieldID, obj_style);
										//
										id_fieldID = env->GetFieldID(class_wbPath, "pchPoint", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbPath, id_fieldID, JNI_STR(env,pWBPtr->pchPoint));											
										//add one shape to shapelist
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbPath);
									}
									break;
								case WB_ST_TEXT:
									{
										WbTextPtr_t pWBPtr = (WbTextPtr_t)pUnKnown;
										jclass class_wbText= env->FindClass("com/zte/softda/ocx/conference/wb/TextShape");
    										jmethodID id_wbText  = env->GetMethodID(class_wbText, "<init>", "()V");
    										jobject obj_wbText = env->NewObject(class_wbText, id_wbText);
									
										//make base info
										WBMakeBaseInfo(env, class_wbText, obj_wbText, pUnKnown);
										//make style
										jobject obj_style = WBMakeStyle(env, pUnKnown);
										id_fieldID = env->GetFieldID(class_wbText, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
    										env->SetObjectField(obj_wbText, id_fieldID, obj_style);
										//
										id_fieldID = env->GetFieldID(class_wbText, "chX", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbText, id_fieldID, JNI_STR(env,pWBPtr->chX));
										id_fieldID = env->GetFieldID(class_wbText, "chY", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbText, id_fieldID, JNI_STR(env,pWBPtr->chY));
										id_fieldID = env->GetFieldID(class_wbText, "chWidth", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbText, id_fieldID, JNI_STR(env,pWBPtr->chWidth));
										id_fieldID = env->GetFieldID(class_wbText, "chHeight", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbText, id_fieldID, JNI_STR(env,pWBPtr->chHeight));	
										id_fieldID = env->GetFieldID(class_wbText, "pchText", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbText, id_fieldID, JNI_STR(env,pWBPtr->pchText));
										//add one shape to shapelist
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbText);
									}
									break;
								case WB_ST_IMAGE:
									{
										WbImagePtr_t pWBPtr = (WbImagePtr_t)pUnKnown;
										jclass class_wbImage= env->FindClass("com/zte/softda/ocx/conference/wb/ImageShape");
    										jmethodID id_wbImage  = env->GetMethodID(class_wbImage, "<init>", "()V");
    										jobject obj_wbImage = env->NewObject(class_wbImage, id_wbImage);
									
										//make base info
										WBMakeBaseInfo(env, class_wbImage, obj_wbImage, pUnKnown);
										//make style
										jobject obj_style = WBMakeStyle(env, pUnKnown);
										id_fieldID = env->GetFieldID(class_wbImage, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
    										env->SetObjectField(obj_wbImage, id_fieldID, obj_style);
										//
										id_fieldID = env->GetFieldID(class_wbImage, "chX", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbImage, id_fieldID, JNI_STR(env,pWBPtr->chX));
										id_fieldID = env->GetFieldID(class_wbImage, "chY", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbImage, id_fieldID, JNI_STR(env,pWBPtr->chY));
										id_fieldID = env->GetFieldID(class_wbImage, "chWidth", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbImage, id_fieldID, JNI_STR(env,pWBPtr->chWidth));
										id_fieldID = env->GetFieldID(class_wbImage, "chHeight", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbImage, id_fieldID, JNI_STR(env,pWBPtr->chHeight));	
										id_fieldID = env->GetFieldID(class_wbImage, "pChHref", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbImage, id_fieldID, JNI_STR(env,pWBPtr->pchHref));
										//add one shape to shapelist
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbImage);
									}
									break;
								case WB_ST_DOC_PAGE:
									{
										WbDocPagePtr_t pWBPtr = (WbDocPagePtr_t)pUnKnown;
										jclass class_wbDocPage= env->FindClass("com/zte/softda/ocx/conference/wb/DocPageShape");
    										jmethodID id_wbDocPage  = env->GetMethodID(class_wbDocPage, "<init>", "()V");
    										jobject obj_wbDocPage = env->NewObject(class_wbDocPage, id_wbDocPage);
									
										//make base info
										WBMakeBaseInfo(env, class_wbDocPage, obj_wbDocPage, pUnKnown);
										//	
										id_fieldID = env->GetFieldID(class_wbDocPage, "pChHref", "Ljava/lang/String;");
    										env->SetObjectField(obj_wbDocPage, id_fieldID, JNI_STR(env,pWBPtr->pchHref));
										//add one shape to shapelist
										DEBUG_INFO("array_ShapeList=%x, j=%d, obj_wbDocPage=%x", array_ShapeList, j, obj_wbDocPage);
										env->SetObjectArrayElement(array_ShapeList, j, obj_wbDocPage);
									}
									break;
							}
							j++;
							pCurShape = pCurShape->pShapeNext;
						}

						id_fieldID = env->GetFieldID(class_Page, "iPageNum", "I");
    						env->SetIntField(obj_Page, id_fieldID, pCurPage->iPageNum);

						id_fieldID = env->GetFieldID(class_Page, "iPageOper", "I");
    						env->SetIntField(obj_Page, id_fieldID, pCurPage->iPageOper);

						id_fieldID = env->GetFieldID(class_Page, "bActive", "Z");
    						env->SetBooleanField(obj_Page, id_fieldID, pCurPage->bActive);
							
						id_fieldID = env->GetFieldID(class_Page, "shapelist", "[Lcom/zte/softda/ocx/conference/wb/WBShape;");
    						env->SetObjectField(obj_Page, id_fieldID, array_ShapeList);
						//add one page to pagelist
//						DEBUG_INFO("array_PageList=%x, i=%d, obj_Page=%x", array_PageList, i, obj_Page);
						env->SetObjectArrayElement(array_PageList, i, obj_Page);
						i++;
						pCurPage = pCurPage->pPageNext;
					}
					id_fieldID = env->GetFieldID(class_wbEvent, "pPageList", "[Lcom/zte/softda/ocx/conference/wb/WBPage;");
//					DEBUG_INFO("obj_wbEvent=%x, id_fieldID=%x, array_PageList=%x", obj_wbEvent, id_fieldID, array_PageList);
    					env->SetObjectField(obj_wbEvent, id_fieldID, array_PageList);
				}
				id_fieldID = env->GetFieldID(class_FireRecvFromMrsEvent, "wbCtrlEvent", "Lcom/zte/softda/ocx/conference/wb/WBControlEvent;");
    				env->SetObjectField(obj_FireRecvFromMrsEvent, id_fieldID, obj_wbEvent);
			}
			break;
		case MRS_EVENT_DOC_SHARE:
			{
			}
			break;
		case MRS_EVENT_WEB_SHARE:	
			{
			}
			break;
		case MRS_EVENT_SHARE_SPACE:
			{
			}
			break;
		case MRS_EVENT_VOTE:
			{
			}
			break;
		case MRS_EVENT_RAISE_HAND:
			{
			}
			break;
		case MRS_EVENT_AGENDA:
			{
			}
			break;
		case MRS_EVENT_FILE_TRANS:
			{
			}
			break;
		default:
			break;
	}

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, obj_FireRecvFromMrsEvent);
	DEBUG_INFO("[java_FireRecvFromMrsEvent] end");
}

void java_FireGotIMAllPrivateGroupListResult(int iResult, List *pGroupList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	int i = 0;
	int nGroupNum = list_len(pGroupList);
	
       if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireGotIMAllPrivateGroupListResult group number[%d]", nGroupNum);
       else if(g_WriteLogType == 1)
        	LOGI(TAG, "rcscontroller java_FireGotIMAllPrivateGroupListResult group number[%d]", nGroupNum);

       iresult = getStaticJniMethod((char *)"java_FireGotIMAllPrivateGroupListResult", (char *)"(Lcom/zte/softda/ocx/FireGotIMAllPrivateGroupList;)V", &env, &gJinMethod);
       if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireGotIMAllPrivateGroupListResult");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireGotIMAllPrivateGroupListResult");

	      return;
    	}
	
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireGotIMAllPrivateGroupList");
	objEvent.NewObject();
	objEvent.SetIntValue("iResult", iResult);
	objEvent.SetIntValue("iNum", nGroupNum);

	TraceMsgWindow1(1, "java_FireGotIMAllPrivateGroupListResult objEvent[%d] iResult[%d] iNum[%d]", objEvent.GetObject(), iResult, nGroupNum);

	jclass class_string = env->FindClass("com/zte/softda/ocx/IMPrivateGroup");
    	jobjectArray array_groups = env->NewObjectArray(nGroupNum, class_string, NULL);

	Group_T *pGroup = NULL;
	for (int i = 0; i < nGroupNum; i++)
       {
       	pGroup = (Group_T*)list_get(pGroupList, i);
		if (NULL == pGroup)
			continue;

		JOjbectWrapper objGroup(env, "com/zte/softda/ocx/IMPrivateGroup");
		objGroup.NewObject();
		objGroup.SetStringValue("cGroupName", pGroup->cGroupName);
		objGroup.SetStringValue("cGroupIdentifier", pGroup->cIdentifier);
        	env->SetObjectArrayElement(array_groups, i, objGroup.GetObject());
    	}
	//free list 
	if (NULL != pGroupList)
	{
		list_destroy(pGroupList, (list_item_destructor_t*)APP_RET_MEM);		
	}

	objEvent.SetContainsObject("GroupInfoArray", "[Lcom/zte/softda/ocx/IMPrivateGroup;", array_groups);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, objEvent.GetObject());

}

void java_FireGotIMPrivateGroupMemberResult(LPCTSTR pGroupIdendifier, int iResult, List *pMemberList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	int i = 0;
	int nMemberNum = list_len(pMemberList);
	
       if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireGotIMPrivateGroupMemberResult member num[%d]", nMemberNum);
       else if(g_WriteLogType == 1)
        	LOGI(TAG, "rcscontroller java_FireGotIMPrivateGroupMemberResult member num[%d]", nMemberNum);

       iresult = getStaticJniMethod((char *)"java_FireGotIMPrivateGroupMemberResult", (char *)"(Lcom/zte/softda/ocx/FireGotIMPrivateGroupMemberResult;)V", &env, &gJinMethod);
       if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireGotIMPrivateGroupMemberResult");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireGotIMPrivateGroupMemberResult");

	      return;
        }
	
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireGotIMPrivateGroupMemberResult");
	objEvent.NewObject();
	objEvent.SetIntValue("iResult", iResult);
	objEvent.SetIntValue("iNum", nMemberNum);
	objEvent.SetStringValue("cGroupIdentifier", pGroupIdendifier);

	jclass class_string = env->FindClass("com/zte/softda/ocx/IMPrivateGroupMember");
	jobjectArray array_members = env->NewObjectArray(nMemberNum, class_string, NULL);

	IMPrivateGroupMember_T *pMember = NULL;
	for (int i = 0; i < nMemberNum; i++)
   {
       	pMember = (IMPrivateGroupMember_T*)list_get(pMemberList, i);
		if (NULL == pMember)
			continue;

		JOjbectWrapper objMember(env, "com/zte/softda/ocx/IMPrivateGroupMember");
		objMember.NewObject();
		objMember.SetStringValue("cURI", pMember->cURI);
		objMember.SetStringValue("cName", pMember->cName);
        objMember.SetStringValue("cFullSpelling", pMember->cFullSpelling);       
		objMember.SetStringValue("cMobile", pMember->cMobile);
		objMember.SetBoolValue("bMOA", pMember->nMOA);
    	env->SetObjectArrayElement(array_members, i, objMember.GetObject());
	}
	//free list 
	if (NULL != pMemberList)
	{
		list_destroy(pMemberList, (list_item_destructor_t*)APP_RET_MEM);
	}

	objEvent.SetContainsObject("MemberInfoArray", "[Lcom/zte/softda/ocx/IMPrivateGroupMember;", array_members);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, objEvent.GetObject());

}

void java_FireCheckURIIsMOAResult(int iResult, List *pCheckList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	int i = 0;
	int nNum = list_len(pCheckList);
	
       if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireCheckURIIsMOAResult num[%d]", nNum);
       else if(g_WriteLogType == 1)
        	LOGI(TAG, "rcscontroller java_FireCheckURIIsMOAResult member num[%d]", nNum);

       iresult = getStaticJniMethod((char *)"java_FireCheckURIIsMOAResult", (char *)"(Lcom/zte/softda/ocx/FireCheckURIIsMOAResult;)V", &env, &gJinMethod);
       if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireCheckURIIsMOAResult");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireCheckURIIsMOAResult");

	      return;
    	}
	
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireCheckURIIsMOAResult");
	objEvent.NewObject();
	objEvent.SetIntValue("iResult", iResult);
	objEvent.SetIntValue("iNum", nNum);

	jclass class_string = env->FindClass("com/zte/softda/ocx/CheckMOAResult");
    	jobjectArray array_members = env->NewObjectArray(nNum, class_string, NULL);

	CheckURIIsMOAResult_Item_T *pMember = NULL;
	for (int i = 0; i < nNum; i++)
       {
       	pMember = (CheckURIIsMOAResult_Item_T*)list_get(pCheckList, i);
		if (NULL == pMember)
			continue;

		JOjbectWrapper objMember(env, "com/zte/softda/ocx/CheckMOAResult");
		objMember.NewObject();
		objMember.SetStringValue("cURI", pMember->cURI);
		objMember.SetBoolValue("bMOA", pMember->nMOA);
        	env->SetObjectArrayElement(array_members, i, objMember.GetObject());
    	}
	//free list 
	if (NULL != pCheckList)
	{
		list_destroy(pCheckList, (list_item_destructor_t*)APP_RET_MEM);
	}

	objEvent.SetContainsObject("CheckResultArray", "[Lcom/zte/softda/ocx/CheckMOAResult;", array_members);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, objEvent.GetObject());
}

void java_FireSendSoapMessageByXcapResult(int iCode, int iType, LPCTSTR lpcsBody)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
       if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireSendSoapMessageByXcapResult iCode[%d] iType[%d]", iCode, iType);
       else if(g_WriteLogType == 1)
        	LOGI(TAG, "rcscontroller java_FireSendSoapMessageByXcapResult iCode[%d] iType[%d]", iCode, iType);

       iresult = getStaticJniMethod((char *)"java_FireSendSoapMessageByXcapResult", (char *)"(IILjava/lang/String;)V", &env, &gJinMethod);
       if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireSendSoapMessageByXcapResult");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireSendSoapMessageByXcapResult");

	      return;
    	}

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iCode, iType, JNI_STR(env,lpcsBody?lpcsBody:""));

    if(NULL != lpcsBody)
	    ZX_free((UINT8*)lpcsBody);
}

void java_FireResponseMsgFromHttpAP(LPCTSTR pSessionID, LPCTSTR pRequestLine, int iRetCode, LPCTSTR pContentType, LPCTSTR pBody)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireResponseMsgFromHttpAP SessionID[%s] RequestLine[%s] RetCode[%d] ContentType[%s]", 
        pSessionID, pRequestLine, iRetCode, pContentType);
 
    iresult = getStaticJniMethod((char *)"java_FireResponseMsgFromHttpAP", (char *)"(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireResponseMsgFromHttpAP");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env, pSessionID), JNI_STR(env, pRequestLine), iRetCode, JNI_STR(env, pContentType), JNI_STR(env, pBody ? pBody : ""));

    if(NULL != pBody)
	    ZX_free((UINT8*)pBody);
}

void java_FireGetCheckCodeResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireGetCheckCodeResult iHttpCode[%d] iResultCode[%d]", iHttpCode, iResultCode);
  
    iresult = getStaticJniMethod((char *)"java_FireGetCheckCodeResult", (char *)"(IILjava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireGetCheckCodeResult");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iHttpCode, iResultCode, JNI_STR(env,lpResultDesc));
}

void java_FireSendPubAccMsgResult(int iHttpCode, int iResultCode, LPCTSTR lpResultDesc, int iMsgType, LPCTSTR lpPutMsgId, LPCTSTR lpMenuId, LPCTSTR lpDate)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireSendPubAccMsgResult iHttpCode[%d] iResultCode[%d] lpPutMsgId[%s] lpMenuId[%s]", iHttpCode, iResultCode, lpPutMsgId, lpMenuId);
  
    iresult = getStaticJniMethod((char *)"java_FireSendPubAccMsgResult", (char *)"(IILjava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireSendPubAccMsgResult");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iHttpCode, iResultCode, JNI_STR(env,lpResultDesc), iMsgType, JNI_STR(env,lpPutMsgId), JNI_STR(env,lpMenuId), JNI_STR(env,lpDate));
}

void java_FireGetGrayVersionResult(int iHttpCode, LPCTSTR lpGrayVersion, LPCTSTR lpDownloadUri, LPCTSTR lpDownloadSize, LPCTSTR lpGrayVerInfo)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireGetGrayVersionResult iHttpCode[%d] lpGrayVersion[%s] lpDownloadUri[%s] lpDownloadSize[%s]", iHttpCode, lpGrayVersion, lpDownloadUri, lpDownloadSize);
  
    iresult = getStaticJniMethod((char *)"java_FireGetGrayVersionResult", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireGetGrayVersionResult");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iHttpCode, JNI_STR(env,lpGrayVersion), JNI_STR(env,lpDownloadUri), JNI_STR(env,lpDownloadSize), JNI_STR(env,lpGrayVerInfo));
}

void java_FireUserBindOperResult(int iResultCode, int iOperType, LPCTSTR lpURI, LPCTSTR lpCompanyID)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireUserBindOperResult iResultCode[%d] iOperType[%d] lpURI[%s] lpCompanyID[%s]", iResultCode, iOperType, lpURI, lpCompanyID);
  
    iresult = getStaticJniMethod((char *)"java_FireUserBindOperResult", (char *)"(IILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireUserBindOperResult");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResultCode, iOperType, JNI_STR(env,lpURI), JNI_STR(env,lpCompanyID));
}

void java_FireModifyGroupAttributeResult(LPCTSTR lpGroupURI, LPCTSTR lpModType, LPCTSTR lpNewValue, int iResultCode)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireModifyGroupAttributeResult iResultCode[%d] lpGroupURI[%s] lpModType[%s] lpNewValue[%s]", iResultCode, lpGroupURI, lpModType, lpNewValue);
  
    iresult = getStaticJniMethod((char *)"java_FireModifyGroupAttributeResult", (char *)"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireModifyGroupAttributeResult");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,lpGroupURI), JNI_STR(env,lpModType), JNI_STR(env,lpNewValue), iResultCode);
}

void java_FireMessageMuteNotifyOperateResult(int iResultCode, int iType, LPCTSTR lpURI, int iGroupNum, void* lpGroupURI, int iContactNum, void* lpContactURI, int iPubAccNum, void* lpPubAccURI)	
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireMessageMuteNotifyOperateResult iResultCode[%d] iType[%d] iGroupNum[%d] iContactNum[%d] iPubAccNum[%d]", 
        iResultCode, iType, iGroupNum, iContactNum, iPubAccNum);
  
    iresult = getStaticJniMethod((char *)"java_FireMessageMuteNotifyOperateResult", (char *)"(IILjava/lang/String;[Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireMessageMuteNotifyOperateResult");
        return;
    }

    if(iType == 1)
    {
        jclass class_string = env->FindClass("java/lang/String");
        jobjectArray array_Group = env->NewObjectArray(iGroupNum, class_string, NULL);
        jobjectArray array_Contact = env->NewObjectArray(iContactNum, class_string, NULL);
        jobjectArray array_PubAcc = env->NewObjectArray(iPubAccNum, class_string, NULL);
        
        ONEURI *pListURI = (ONEURI *)lpGroupURI;
        int i;
    	for (i = 0; i < iGroupNum; i++)
        {
           	jstring pURI = env->NewStringUTF(pListURI[i]);          
            env->SetObjectArrayElement(array_Group, i, pURI);
            env->DeleteLocalRef(pURI);
        }

        pListURI = (ONEURI *)lpContactURI;
    	for (i = 0; i < iContactNum; i++)
        {
           	jstring pURI = env->NewStringUTF(pListURI[i]);          
            env->SetObjectArrayElement(array_Contact, i, pURI);
            env->DeleteLocalRef(pURI);
        }

        pListURI = (ONEURI *)lpPubAccURI;
    	for (i = 0; i < iPubAccNum; i++)
        {
           	jstring pURI = env->NewStringUTF(pListURI[i]);          
            env->SetObjectArrayElement(array_PubAcc, i, pURI);
            env->DeleteLocalRef(pURI);
        }

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iResultCode, iType, JNI_STR(env,lpURI), array_Group, array_Contact, array_PubAcc);        
    }
    else
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iResultCode, iType, JNI_STR(env,lpURI), NULL, NULL, NULL); 
    }

    TraceMsgWindow1(1, "rcscontroller java_FireMessageMuteNotifyOperateResult Over");
}

void java_FireMessageReceiptNotify(int isGroup, LPCTSTR lpURI, int iListNum, void* lpMsgIDList, char* pMsgIDs)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireMessageReceiptNotify isGroup[%d] lpURI[%s] iListNum[%d]", 
        isGroup, lpURI, iListNum);
  
    iresult = getStaticJniMethod((char *)"java_FireMessageReceiptNotify", (char *)"(ILjava/lang/String;[Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireMessageReceiptNotify");
        return;
    }

    if(isGroup == 1)
    {
        jclass class_string = env->FindClass("java/lang/String");
        jobjectArray array_MsgID = env->NewObjectArray(iListNum, class_string, NULL);
        
        ONEMSGID *pMsgIDList = (ONEMSGID *)lpMsgIDList;
        int i;
    	for (i = 0; i < iListNum; i++)
        {
           	jstring pMsgID = env->NewStringUTF(pMsgIDList[i]);          
            env->SetObjectArrayElement(array_MsgID, i, pMsgID);
            env->DeleteLocalRef(pMsgID);
        }

        env->CallStaticVoidMethod(gpostClass, gJinMethod, isGroup, JNI_STR(env,lpURI), array_MsgID, NULL);        
    }
    else
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, isGroup, JNI_STR(env,lpURI), NULL, JNI_STR(env,pMsgIDs));   
    }

    TraceMsgWindow1(1, "rcscontroller java_FireMessageReceiptNotify Over");
}

void java_FireMessageReceiptOperateResult(int iOperType, LPCTSTR sessionID, int iResultCode, LPCTSTR pMsgIDs, void* qryRsp)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireMessageReceiptOperateResult iOperType[%d] sessionID[%s] iResultCode[%d] pMsgIDs[%s]", 
        iOperType, sessionID, iResultCode, pMsgIDs);
  
    iresult = getStaticJniMethod((char *)"java_FireMessageReceiptOperateResult", (char *)"(ILjava/lang/String;ILjava/lang/String;Lcom/zte/softda/ocx/MessageReceiptQryRsp;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireMessageReceiptOperateResult");
        return;
    }

    if(iOperType == 1)
    {
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iOperType, JNI_STR(env,sessionID), iResultCode, JNI_STR(env,pMsgIDs), NULL);                
    }
    else
    {
        MsgReceiptQryRsp_T *pRsp = (MsgReceiptQryRsp_T *)qryRsp;
        JOjbectWrapper objEvent(env, "com/zte/softda/ocx/MessageReceiptQryRsp");
    	objEvent.NewObject();

        objEvent.SetIntValue("isGroup", pRsp->isGroup);
        objEvent.SetStringValue("cURI", (char*)pRsp->cURI);
        objEvent.SetStringValue("cMsgID", (char*)pRsp->cMsgID);
        objEvent.SetIntValue("isReaded", pRsp->isReaded);
        objEvent.SetStringValue("cReadedList", (char*)pRsp->pReadedList);
        objEvent.SetStringValue("cUnReadedList", (char*)pRsp->pUnReadedList);

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iOperType, JNI_STR(env,sessionID), iResultCode, JNI_STR(env,pMsgIDs), objEvent.GetObject());     
    }

    TraceMsgWindow1(1, "rcscontroller java_FireMessageReceiptOperateResult Over");
}    

void java_FireIMSMessageListArrived(int nNum, void **pMsgList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
    TraceMsgWindow1(1, "rcscontroller java_FireIMSMessageListArrived nNum[%d]", nNum);

    if (NULL == pMsgList)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireIMSMessageListArrived pMsgList is NULL");   
    	return;
    }

	iresult = getStaticJniMethod((char *)"java_FireIMSMessageListArrived", (char *)"(Lcom/zte/softda/ocx/FireMessageList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireIMSMessageListArrived");
        return;
    }
	
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireMessageList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FireMessageItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	IMSMessageItem_T *pMsgItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
       	pMsgItem = (IMSMessageItem_T*)pMsgList[i];
		if (NULL == pMsgItem)
			continue;

		JOjbectWrapper objMsgItem(env, "com/zte/softda/ocx/FireMessageItem");
		objMsgItem.NewObject();
		objMsgItem.SetIntValue("iType", pMsgItem->iType);
        objMsgItem.SetIntValue("iReceipt", pMsgItem->iReceipt);
		objMsgItem.SetStringValue("pChatRoomURI", pMsgItem->pChatRoomURI);
		objMsgItem.SetStringValue("pSenderURI", pMsgItem->pSenderURI);
		objMsgItem.SetStringValue("pMessage", pMsgItem->pMessage);
		objMsgItem.SetStringValue("pTime", pMsgItem->pTime);
		objMsgItem.SetStringValue("pMessageID", pMsgItem->pMessageID);	
        env->SetObjectArrayElement(array_msgs, i, objMsgItem.GetObject());
    }

    objEvent.SetContainsObject("msglist", "[Lcom/zte/softda/ocx/FireMessageItem;", array_msgs);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireIMSMessageListArrived fire over");
}

void java_FireTry403FlowBegin()
{
    	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
       if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireTry403FlowBegin" );
       else if(g_WriteLogType == 1)
        	LOGI(TAG, "rcscontroller java_FireTry403FlowBegin" );

	iresult = getStaticJniMethod((char *)"java_FireTry403FlowBegin", (char *)"()V", &env, &gJinMethod);
       if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireTry403FlowBegin");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireTry403FlowBegin");

	      return;
    	}

	env->CallStaticVoidMethod(gpostClass, gJinMethod);
}
void java_FireTry403FlowResult(int iResult)
{
    jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
   if(g_WriteLogType == 2)
    	TraceMsgWindow1(1, "rcscontroller java_FireTry403FlowResult iResult[%d]", iResult);
   else if(g_WriteLogType == 1)
    	LOGI(TAG, "rcscontroller java_FireTry403FlowResult iResult[%d]", iResult);

	iresult = getStaticJniMethod((char *)"java_FireTry403FlowResult", (char *)"(I)V", &env, &gJinMethod);
       if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireTry403FlowResult");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireTry403FlowResult");

	      return;
    	}

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult);
}

void java_FireCheckConnectResult(int iResult, int delaysec)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
    	TraceMsgWindow1(1, "rcscontroller java_FireCheckConnectResult iResult[%d] delaysec[%d]", iResult, delaysec);
    else if(g_WriteLogType == 1)
    	LOGI(TAG, "rcscontroller java_FireCheckConnectResult iResult[%d] delaysec[%d]", iResult, delaysec);

    iresult = getStaticJniMethod((char *)"java_FireCheckConnectResult", (char *)"(II)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
    	 if(g_WriteLogType == 2)
        		TraceMsgWindow1(1, "rcscontroller not found java_FireCheckConnectResult");
    	else if(g_WriteLogType == 1)
        		LOGI(TAG, "not found java_FireCheckConnectResult");

      return;
    }

    env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, delaysec);
}
//Ⱥ��������Ϣ����֪ͨ
void java_FireIMSGroupOfflineMsgNotify(int iMsgNum, LPCTSTR cGroupURI, LPCTSTR pMessage, LPCTSTR timestamp)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGroupOfflineMsgNotify iMsgNum[%d] cGroupURI[%s] pMessage[%s]",
                        iMsgNum, cGroupURI, pMessage);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireIMSGroupOfflineMsgNotify iMsgNum[%d] cGroupURI[%s] pMessage[%s]", iMsgNum, cGroupURI, pMessage);

    iresult = getStaticJniMethod((char *)"java_FireIMSGroupOfflineMsgNotify", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGroupOfflineMsgNotify");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireIMSGroupOfflineMsgNotify");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, iMsgNum, JNI_STR(env,cGroupURI), JNI_STR(env,pMessage), JNI_STR(env,timestamp));

    TraceMsgWindow1(1, "rcscontroller java_FireIMSGroupOfflineMsgNotify fire over");
}

//Ⱥ��������Ϣ֪ͨ����
//void java_FireIMSGotGroupOfflineMsgResult(int iReturnCode, int nMethodType, int nOverFlag, LPCTSTR cGroupURI, LPCTSTR cMaxTimeQue, int nNum, List *pMsgList)
void java_FireIMSGotGroupOfflineMsgResult(int iReturnCode, int nMethodType, int nOverFlag, LPCTSTR cGroupURI, LPCTSTR cMaxTimeQue, int nNum, void **pMsgList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
       if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller java_FireIMSGotGroupOfflineMsgResult nNum[%d]", nNum);
       else if(g_WriteLogType == 1)
        	LOGI(TAG, "rcscontroller java_FireIMSGotGroupOfflineMsgResult nNum[%d]", nNum);

       if (NULL == pMsgList)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller java_FireIMSGotGroupOfflineMsgResult pMsgList is NULL");
        else if(g_WriteLogType == 1)
            		LOGI(TAG, "rcscontroller java_FireIMSGotGroupOfflineMsgResult pMsgList is NULL");       
//       return;
       }

	iresult = getStaticJniMethod((char *)"java_FireIMSGotGroupOfflineMsgResult", (char *)"(IIILjava/lang/String;Ljava/lang/String;Lcom/zte/softda/ocx/FireOfflineMsgList;)V", &env, &gJinMethod);
       if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
       {
       	 if(g_WriteLogType == 2)
            		TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotGroupOfflineMsgResult");
        	else if(g_WriteLogType == 1)
            		LOGI(TAG, "not found java_FireIMSGotGroupOfflineMsgResult");

	      return;
    	}
	
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireOfflineMsgList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FireOfflineMsgItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	GROUP_OFFLINE_MSG_PARAM *pMsgItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
//      pMsgItem = (GROUP_OFFLINE_MSG_PARAM*)list_get(pMsgList, i);
        pMsgItem = (GROUP_OFFLINE_MSG_PARAM*)pMsgList[i];
		if (NULL == pMsgItem)
			continue;

        //�����Ⱥ���Ա�仯��Ϣ���ɿؼ�����������֪ͨUI
        if(pMsgItem->iType == IM_TYPE_GROUP_MEMBER_CHANGED)
        {//��cID����UI��������
            DoPublicGroupMemberChanged(pMsgItem->pMessage, pMsgItem->cID, FALSE);        
        }
        else
        {
            JOjbectWrapper objMsgItem(env, "com/zte/softda/ocx/FireOfflineMsgItem");
    		objMsgItem.NewObject();
    		objMsgItem.SetStringValue("cID", pMsgItem->cID);
    		objMsgItem.SetStringValue("cSender", pMsgItem->cSender);
    		objMsgItem.SetStringValue("cTime", pMsgItem->cTime);
    		objMsgItem.SetIntValue("iType", pMsgItem->iType);
    		objMsgItem.SetStringValue("pMessage", pMsgItem->pMessage);	
            env->SetObjectArrayElement(array_msgs, i, objMsgItem.GetObject());
        }       
    }
    
    objEvent.SetContainsObject("msglist", "[Lcom/zte/softda/ocx/FireOfflineMsgItem;", array_msgs);
    
	env->CallStaticVoidMethod(gpostClass, gJinMethod, iReturnCode, nMethodType, nOverFlag, JNI_STR(env,cGroupURI), JNI_STR(env,cMaxTimeQue),objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireIMSGotGroupOfflineMsgResult fire over");
}

void java_FireGotPubAccountListResult(int iResult, int nNum, void **pPubAccountList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;

    if(nNum > MAX_PUBACCOUNT_NUM)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireGotPubAccountListResult nNum[%d] is abnormal, MAX_PUBACCOUNT_NUM[%d]", nNum, MAX_PUBACCOUNT_NUM);
        return;
    }
	
	TraceMsgWindow1(1, "rcscontroller java_FireGotPubAccountListResult nNum[%d]", nNum);

    if (NULL == pPubAccountList)
    {
    	 if(g_WriteLogType == 2)
        		TraceMsgWindow1(1, "rcscontroller java_FireGotPubAccountListResult pPubAccountList is NULL");
    	else if(g_WriteLogType == 1)
        		LOGI(TAG, "rcscontroller java_FireGotPubAccountListResult pPubAccountList is NULL");       
    	return;
    }

	iresult = getStaticJniMethod((char *)"java_FireGotPubAccountListResult", (char *)"(ILcom/zte/softda/ocx/FirePubAccountList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
    	 if(g_WriteLogType == 2)
        		TraceMsgWindow1(1, "rcscontroller not found java_FireGotPubAccountListResult");
    	else if(g_WriteLogType == 1)
        		LOGI(TAG, "not found java_FireGotPubAccountListResult");

      return;
    }
    
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FirePubAccountList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FirePubAccountItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	PubAccount_T *pPubAccountItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
       	pPubAccountItem = (PubAccount_T*)pPubAccountList[i];
		if (NULL == pPubAccountItem)
			continue;

		JOjbectWrapper objPubAccountItem(env, "com/zte/softda/ocx/FirePubAccountItem");
		objPubAccountItem.NewObject();

		objPubAccountItem.SetStringValue("PubAccId", pPubAccountItem->PubAccId);
		objPubAccountItem.SetStringValue("PubAccName", pPubAccountItem->PubAccName);
		objPubAccountItem.SetStringValue("PubAccNameEn", pPubAccountItem->PubAccNameEn);
		objPubAccountItem.SetStringValue("PubAccDesc", pPubAccountItem->PubAccDesc);
		objPubAccountItem.SetStringValue("PubAccDescEn", pPubAccountItem->PubAccDescEn);
        objPubAccountItem.SetStringValue("PubAccPhotoUri", pPubAccountItem->PubAccPhotoUri);
        objPubAccountItem.SetStringValue("Acc2DimeCode", pPubAccountItem->Acc2DimeCode);
        objPubAccountItem.SetIntValue("PushType", pPubAccountItem->PushType);
        objPubAccountItem.SetIntValue("PubAccType", pPubAccountItem->PubAccType);
        objPubAccountItem.SetIntValue("IsPrivate", pPubAccountItem->IsPrivate);   
        objPubAccountItem.SetIntValue("SubCount", pPubAccountItem->SubCount); 
        env->SetObjectArrayElement(array_msgs, i, objPubAccountItem.GetObject());
    }
    objEvent.SetContainsObject("pubAccountList", "[Lcom/zte/softda/ocx/FirePubAccountItem;", array_msgs);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireGotPubAccountListResult fire over");
}

void java_FireIMSGotPublishMsgResult(int msgType, LPCTSTR localMsgID, LPCTSTR pubAccId, LPCTSTR cTime, LPCTSTR msgid, int seq, int nNum, void **pMsgList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;

    if(nNum > MAX_PUBMSG_ITEM_NUM)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireIMSGotPublishMsgResult nNum[%d] is abnormal, MAX_PUBMSG_ITEM_NUM[%d]", nNum, MAX_PUBMSG_ITEM_NUM);
        return;
    }
	
	TraceMsgWindow1(1, "rcscontroller java_FireIMSGotPublishMsgResult msgType[%d] localMsgID[%s] nNum[%d] pubAccId[%s] msgid[%s] seq[%d]", 
	    msgType, localMsgID, nNum, pubAccId, msgid, seq);

    if (NULL == pMsgList)
    {
		TraceMsgWindow1(1, "rcscontroller java_FireIMSGotPublishMsgResult pMsgList is NULL");   
        return;
    }

	iresult = getStaticJniMethod((char *)"java_FireIMSGotPublishMsgResult", (char *)"(Lcom/zte/softda/ocx/FirePublishMsgList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
        	TraceMsgWindow1(1, "rcscontroller not found java_FireIMSGotPublishMsgResult");
        else if(g_WriteLogType == 1)
        	LOGI(TAG, "not found java_FireIMSGotPublishMsgResult");

        return;
    }
	
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FirePublishMsgList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FirePublishMsgItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	PublishMsgItem_T *pMsgItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
        pMsgItem = (PublishMsgItem_T*)pMsgList[i];
		if (NULL == pMsgItem)
			continue;
   
        JOjbectWrapper objMsgItem(env, "com/zte/softda/ocx/FirePublishMsgItem");
		objMsgItem.NewObject();
		objMsgItem.SetStringValue("text", pMsgItem->cText);
		objMsgItem.SetStringValue("img", pMsgItem->cImg);
		objMsgItem.SetStringValue("link", pMsgItem->cLink);
		objMsgItem.SetStringValue("summary", pMsgItem->summary);
        objMsgItem.SetStringValue("SubMsgId", pMsgItem->SubMsgId);
        objMsgItem.SetIntValue("AppType", pMsgItem->AppType);
        objMsgItem.SetIntValue("Forward", pMsgItem->Forward);
        objMsgItem.SetIntValue("IsPublic", pMsgItem->IsPublic);
        objMsgItem.SetStringValue("ActiveEntry", pMsgItem->ActiveEntry);
		objMsgItem.SetStringValue("Content", pMsgItem->Content);
		objMsgItem.SetStringValue("AppName", pMsgItem->AppName);
		objMsgItem.SetStringValue("Reserve1", pMsgItem->Reserve1);
        objMsgItem.SetStringValue("Reserve2", pMsgItem->Reserve2);
        objMsgItem.SetStringValue("Reserve3", pMsgItem->Reserve3);
        
        env->SetObjectArrayElement(array_msgs, i, objMsgItem.GetObject());      
    }

    objEvent.SetIntValue("msgType", msgType);
    objEvent.SetStringValue("localMsgID", localMsgID);
    objEvent.SetStringValue("pubAccId", pubAccId);
    objEvent.SetStringValue("time", cTime);
    objEvent.SetStringValue("msgid", msgid);  
    objEvent.SetIntValue("seq", seq); 
    objEvent.SetContainsObject("msglist", "[Lcom/zte/softda/ocx/FirePublishMsgItem;", array_msgs);
    
	env->CallStaticVoidMethod(gpostClass, gJinMethod, objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireIMSGotPublishMsgResult fire over");
}

void java_FireAddOnePubAccountResult(int iResult, int nNum, void **pPubAccountList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
    if(g_WriteLogType == 2)
    	TraceMsgWindow1(1, "rcscontroller java_FireAddOnePubAccountResult nNum[%d]", nNum);
    else if(g_WriteLogType == 1)
    	LOGI(TAG, "rcscontroller java_FireAddOnePubAccountResult nNum[%d]", nNum);

    if (NULL == pPubAccountList)
    {
    	 if(g_WriteLogType == 2)
        		TraceMsgWindow1(1, "rcscontroller java_FireAddOnePubAccountResult pPubAccountList is NULL");
    	else if(g_WriteLogType == 1)
        		LOGI(TAG, "rcscontroller java_FireAddOnePubAccountResult pPubAccountList is NULL");       
    	return;
    }

	iresult = getStaticJniMethod((char *)"java_FireAddOnePubAccountResult", (char *)"(ILcom/zte/softda/ocx/FirePubAccountItem;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
    	 if(g_WriteLogType == 2)
        		TraceMsgWindow1(1, "rcscontroller not found java_FireAddOnePubAccountResult");
    	else if(g_WriteLogType == 1)
        		LOGI(TAG, "not found java_FireAddOnePubAccountResult");

      return;
    }
    
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FirePubAccountItem");
	objEvent.NewObject();

	PubAccount_T *pPubAccountItem = NULL;
	if(nNum >= 1)
    {//�������nNum=1
       	pPubAccountItem = (PubAccount_T*)pPubAccountList[0];
		if (NULL != pPubAccountItem)
        {      
    		objEvent.SetStringValue("PubAccId", pPubAccountItem->PubAccId);
    		objEvent.SetStringValue("PubAccName", pPubAccountItem->PubAccName);
    		objEvent.SetStringValue("PubAccNameEn", pPubAccountItem->PubAccNameEn);
    		objEvent.SetStringValue("PubAccDesc", pPubAccountItem->PubAccDesc);
    		objEvent.SetStringValue("PubAccDescEn", pPubAccountItem->PubAccDescEn);
            objEvent.SetStringValue("PubAccPhotoUri", pPubAccountItem->PubAccPhotoUri);
            objEvent.SetStringValue("Acc2DimeCode", pPubAccountItem->Acc2DimeCode);
            objEvent.SetIntValue("PushType", pPubAccountItem->PushType);
            objEvent.SetIntValue("PubAccType", pPubAccountItem->PubAccType);
            objEvent.SetIntValue("IsPrivate", pPubAccountItem->IsPrivate);
            objEvent.SetIntValue("SubCount", pPubAccountItem->SubCount);
        }
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireAddOnePubAccountResult fire over");
}

void java_FireDeleteOnePubAccountResult(int iResult, LPCTSTR pubAccId)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
    if(g_WriteLogType == 2)
    	TraceMsgWindow1(1, "rcscontroller java_FireDeleteOnePubAccountResult iResult[%d] pubAccId[%s]", iResult, pubAccId);
    else if(g_WriteLogType == 1)
    	LOGI(TAG, "rcscontroller java_FireDeleteOnePubAccountResult iResult[%d] pubAccId[%s]", iResult, pubAccId);

	iresult = getStaticJniMethod((char *)"java_FireDeleteOnePubAccountResult", (char *)"(ILjava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
    	 if(g_WriteLogType == 2)
        		TraceMsgWindow1(1, "rcscontroller not found java_FireDeleteOnePubAccountResult");
    	else if(g_WriteLogType == 1)
        		LOGI(TAG, "not found java_FireDeleteOnePubAccountResult");

      return;
    }
    
	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, JNI_STR(env,pubAccId));
	TraceMsgWindow1(1, "java_FireDeleteOnePubAccountResult fire over");
}

void java_FireGotPubAccountMenuResult(int iResult, LPCTSTR pubAccId, LPCTSTR etag, int nNum, void **pPubAccountMenu)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
    TraceMsgWindow1(1, "rcscontroller java_FireGotPubAccountMenuResult nNum[%d]", nNum);  

    if (NULL == pPubAccountMenu)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireGotPubAccountMenuResult pPubAccountMenu is NULL");   
    	return;
    }

	iresult = getStaticJniMethod((char *)"java_FireGotPubAccountMenuResult", (char *)"(ILjava/lang/String;Ljava/lang/String;Lcom/zte/softda/ocx/FirePubAccountMenuList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireGotPubAccountMenuResult");
        return;
    }
    
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FirePubAccountMenuList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FirePubAccountMenuItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	PubAccountMenu_T *pPubAccMenuItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
       	pPubAccMenuItem = (PubAccountMenu_T*)pPubAccountMenu[i];
		if (NULL == pPubAccMenuItem)
			continue;

		JOjbectWrapper objPubAccMenuItem(env, "com/zte/softda/ocx/FirePubAccountMenuItem");
		objPubAccMenuItem.NewObject();

		objPubAccMenuItem.SetStringValue("MenuId", pPubAccMenuItem->MenuId);
        objPubAccMenuItem.SetIntValue("Seq", pPubAccMenuItem->Seq);
		objPubAccMenuItem.SetStringValue("PMenuId", pPubAccMenuItem->PMenuId);
        objPubAccMenuItem.SetIntValue("MenuType", pPubAccMenuItem->MenuType);
		objPubAccMenuItem.SetStringValue("MenuVersion", pPubAccMenuItem->MenuVersion);
        objPubAccMenuItem.SetIntValue("IsLeaf", pPubAccMenuItem->IsLeaf);
		objPubAccMenuItem.SetStringValue("Text", pPubAccMenuItem->Text);
		objPubAccMenuItem.SetStringValue("Content", pPubAccMenuItem->Content);
        objPubAccMenuItem.SetStringValue("ActiveEntry", pPubAccMenuItem->ActiveEntry);
        objPubAccMenuItem.SetStringValue("AppParam", pPubAccMenuItem->AppParam);
        
        env->SetObjectArrayElement(array_msgs, i, objPubAccMenuItem.GetObject());
    }
    objEvent.SetContainsObject("pubAccountMenuList", "[Lcom/zte/softda/ocx/FirePubAccountMenuItem;", array_msgs);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, JNI_STR(env,pubAccId), JNI_STR(env,etag), objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireGotPubAccountMenuResult fire over");
}

void java_FireSearchPubAccountResult(int iResult, int iType, int pageNo, int pageSize, int nNum, void **pPubAccountList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
    TraceMsgWindow1(1, "rcscontroller java_FireSearchPubAccountResult iResult[%d] iType[%d] pageNo[%d] nNum[%d]", iResult, iType, pageNo, nNum);

    if (NULL == pPubAccountList)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireSearchPubAccountResult pPubAccountList is NULL"); 
    	return;
    }

	iresult = getStaticJniMethod((char *)"java_FireSearchPubAccountResult", (char *)"(IIIILcom/zte/softda/ocx/FirePubAccountList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireSearchPubAccountResult");
        return;
    }
    
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FirePubAccountList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FirePubAccountItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	PubAccount_T *pPubAccountItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
       	pPubAccountItem = (PubAccount_T*)pPubAccountList[i];
		if (NULL == pPubAccountItem)
			continue;

		JOjbectWrapper objPubAccountItem(env, "com/zte/softda/ocx/FirePubAccountItem");
		objPubAccountItem.NewObject();

		objPubAccountItem.SetStringValue("PubAccId", pPubAccountItem->PubAccId);
		objPubAccountItem.SetStringValue("PubAccName", pPubAccountItem->PubAccName);
		objPubAccountItem.SetStringValue("PubAccNameEn", pPubAccountItem->PubAccNameEn);
		objPubAccountItem.SetStringValue("PubAccDesc", pPubAccountItem->PubAccDesc);
		objPubAccountItem.SetStringValue("PubAccDescEn", pPubAccountItem->PubAccDescEn);
        objPubAccountItem.SetStringValue("PubAccPhotoUri", pPubAccountItem->PubAccPhotoUri);
        objPubAccountItem.SetStringValue("Acc2DimeCode", pPubAccountItem->Acc2DimeCode);
        objPubAccountItem.SetIntValue("PushType", pPubAccountItem->PushType);
        objPubAccountItem.SetIntValue("PubAccType", pPubAccountItem->PubAccType);
        objPubAccountItem.SetIntValue("IsPrivate", pPubAccountItem->IsPrivate);
        objPubAccountItem.SetIntValue("SubCount", pPubAccountItem->SubCount);
        env->SetObjectArrayElement(array_msgs, i, objPubAccountItem.GetObject());
    }
    objEvent.SetContainsObject("pubAccountList", "[Lcom/zte/softda/ocx/FirePubAccountItem;", array_msgs);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, iType, pageNo, pageSize, objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireSearchPubAccountResult fire over");
}

void java_FireGetRecommendContactResult(int iResult, int pageNo, int pageSize, int nNum, void **pContactInfoList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
    TraceMsgWindow1(1, "rcscontroller java_FireGetRecommendContactResult iResult[%d] pageNo[%d] nNum[%d]", iResult, pageNo, nNum);
    if (NULL == pContactInfoList)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireGetRecommendContactResult pPubAccountList is NULL"); 
    	return;
    }

	iresult = getStaticJniMethod((char *)"java_FireGetRecommendContactResult", (char *)"(IIILcom/zte/softda/ocx/FireRecommendContactList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireGetRecommendContactResult");
        return;
    }
    
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireRecommendContactList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FireRecommendContactItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	ContactInfo_T *pContactInfoItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
       	pContactInfoItem = (ContactInfo_T*)pContactInfoList[i];
		if (NULL == pContactInfoItem)
			continue;

		JOjbectWrapper objRecommendContactItem(env, "com/zte/softda/ocx/FireRecommendContactItem");
		objRecommendContactItem.NewObject();

		objRecommendContactItem.SetStringValue("Uri", pContactInfoItem->Uri);
		objRecommendContactItem.SetStringValue("Name", pContactInfoItem->Name);
        env->SetObjectArrayElement(array_msgs, i, objRecommendContactItem.GetObject());
    }
    objEvent.SetContainsObject("contactList", "[Lcom/zte/softda/ocx/FireRecommendContactItem;", array_msgs);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, iResult, pageNo, pageSize, objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireGetRecommendContactResult fire over");
}

void java_FireUserCodeMoaInfoResult(int iType, int iResult, LPCTSTR msgId, LPCTSTR etag, int nNum, void **pUserMoaInfo)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireUserCodeMoaInfoResult iType[%d] iResult[%d] etag[%s] nNum[%d]", iType, iResult, etag, nNum);
 
    if (NULL == pUserMoaInfo)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireUserCodeMoaInfoResult pUserMoaInfo is NULL");      
    	return;
    }

	iresult = getStaticJniMethod((char *)"java_FireUserCodeMoaInfoResult", (char *)"(IILjava/lang/String;Ljava/lang/String;Lcom/zte/softda/ocx/FireUserCodeMoaInfoList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireUserCodeMoaInfoResult");
        return;
    }
    
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/FireUserCodeMoaInfoList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/FireUserCodeMoaInfoItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	UserMoaInfo_T *pUserMoaInfoItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
       	pUserMoaInfoItem = (UserMoaInfo_T*)pUserMoaInfo[i];
		if (NULL == pUserMoaInfoItem)
			continue;

		JOjbectWrapper objPubAccountItem(env, "com/zte/softda/ocx/FireUserCodeMoaInfoItem");
		objPubAccountItem.NewObject();

		objPubAccountItem.SetStringValue("UserCode", pUserMoaInfoItem->UserCode);
//      objPubAccountItem.SetIntValue("MoaFlag", pUserMoaInfoItem->MoaFlag);
        env->SetObjectArrayElement(array_msgs, i, objPubAccountItem.GetObject());
    }
    objEvent.SetContainsObject("UserCodeMoaInfoList", "[Lcom/zte/softda/ocx/FireUserCodeMoaInfoItem;", array_msgs);
	env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iResult, JNI_STR(env,msgId), JNI_STR(env,etag), objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireUserCodeMoaInfoResult fire over");
}

void java_FireGet2DCodeInfoResult(int iType, int iReturnCode, LPCTSTR pDataStr, LPCTSTR jsonKey, LPCTSTR validTime, int codeType, LPCTSTR msgId)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;

    TraceMsgWindow1(1, "rcscontroller java_FireGet2DCodeInfoResult iType[%d] iReturnCode[%d] validTime[%s] jsonKey[%s] codeType[%d] msgId[%s]", 
        iType, iReturnCode, validTime, jsonKey, codeType, msgId);

	iresult = getStaticJniMethod((char *)"java_FireGet2DCodeInfoResult", (char *)"(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireGet2DCodeInfoResult");
        return;
    }
	
    if (NULL == pDataStr)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireGet2DCodeInfoResult pDataStr is NULL"); 
        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iReturnCode, JNI_STR(env,""), JNI_STR(env,jsonKey), JNI_STR(env,validTime), codeType, JNI_STR(env,msgId));
    }
    else
    {

        env->CallStaticVoidMethod(gpostClass, gJinMethod, iType, iReturnCode, JNI_STR(env,pDataStr), JNI_STR(env,jsonKey), JNI_STR(env,validTime), codeType, JNI_STR(env,msgId));
    }
    	
	TraceMsgWindow1(1, "java_FireGet2DCodeInfoResult fire over");
}

void java_FireGetSensWordListResult(int nNum, void *pWordList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;

    if(nNum > MAX_SENSWORD_NUM)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireGetSensWordListResult nNum[%d] is abnormal, MAX_SENSWORD_NUM[%d]", nNum, MAX_SENSWORD_NUM);
        return;
    }
	
	TraceMsgWindow1(1, "rcscontroller java_FireGetSensWordListResult nNum[%d]", nNum);    

    if (NULL == pWordList)
    {
		TraceMsgWindow1(1, "rcscontroller java_FireGetSensWordListResult pWordList is NULL");    
    	return;
    }

   //static void java_FireGetSensWordListResult(int nNum, String[] wordlist)
	iresult = getStaticJniMethod((char *)"java_FireGetSensWordListResult", (char *)"(I[Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
		TraceMsgWindow1(1, "rcscontroller not found java_FireGetSensWordListResult");
        return;
    }
    
	jclass class_string = env->FindClass("java/lang/String");
    jobjectArray array_words = env->NewObjectArray(nNum, class_string, NULL);
    OneSensWord_T *pSensWordData = (OneSensWord_T *)pWordList;
	for (int i = 0; i < nNum; i++)
    {
       	jstring sensWord = env->NewStringUTF(pSensWordData[i].sensWord);          
        env->SetObjectArrayElement(array_words, i, sensWord);
        env->DeleteLocalRef(sensWord);
    }
	env->CallStaticVoidMethod(gpostClass, gJinMethod, nNum, array_words);
	TraceMsgWindow1(1, "java_FireGetSensWordListResult fire over");
}

//lmd20141204
void java_FireStartVNCDLLEvent(LPCTSTR pLocalIP, int iLocalPort,LPCTSTR pIP, int iPort,LPCTSTR pDPassword, LPCTSTR plinkid)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireStartVNCDLLEvent pLocalIP[%s] iLocalPort[%d] pIP[%s] iPort[%d] pDPassword[%s] plinkid[%s]",pLocalIP,iLocalPort, pIP, iPort,pDPassword,plinkid);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireStartVNCDLLEvent pLocalIP[%s] iLocalPort[%d] pIP[%s] iPort[%d] pDPassword[%s] plinkid[%s]",pLocalIP,iLocalPort, pIP, iPort,pDPassword,plinkid);

    iresult = getStaticJniMethod((char *)"java_FireStartVNCDLLEvent", (char *)"(Ljava/lang/String;ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireStartVNCDLLEvent");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireStartVNCDLLEvent");

        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pLocalIP), iLocalPort,JNI_STR(env,pIP), iPort, JNI_STR(env,pDPassword), JNI_STR(env,plinkid));
}
//lmd20141204, end

void java_FireAudioStatNotify(short *pNotify)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    jfieldID id_fieldID;
    AudioStatInfo_T *pAudioStatInfo = (AudioStatInfo_T *)pNotify;
    int i;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller java_FireAudioStatNotify begin");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "java_FireAudioStatNotify begin, iNum=");

    //��ʱ����
    return;

    iresult = getStaticJniMethod((char *)"java_FireAudioStatNotify", (char *)"(Lcom/zte/softda/ocx/FireAudioStatInfoPara;)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller not found java_FireAudioStatNotify");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "not found java_FireAudioStatNotify");

        return;
    }

	//AudioStat��
    jclass class_AudioStat = env->FindClass("com/zte/softda/ocx/AudioStat");
    jmethodID id_AudioStat  = env->GetMethodID(class_AudioStat, "<init>", "()V");

	//sendStat
    jobject obj_sendStat = env->NewObject(class_AudioStat, id_AudioStat);

	id_fieldID = env->GetFieldID(class_AudioStat, "iTotalFractionLost", "I");
    env->SetIntField(obj_sendStat, id_fieldID, pAudioStatInfo->sendStat.iTotalFractionLost);

	id_fieldID = env->GetFieldID(class_AudioStat, "iCurJitter", "I");
    env->SetIntField(obj_sendStat, id_fieldID, pAudioStatInfo->sendStat.iCurJitter);

	id_fieldID = env->GetFieldID(class_AudioStat, "iCurBitrate", "I");
    env->SetIntField(obj_sendStat, id_fieldID, pAudioStatInfo->sendStat.iCurBitrate);

	id_fieldID = env->GetFieldID(class_AudioStat, "iAvgBitrate", "I");
    env->SetIntField(obj_sendStat, id_fieldID, pAudioStatInfo->sendStat.iAvgBitrate);

	id_fieldID = env->GetFieldID(class_AudioStat, "iCurRTT", "I");
    env->SetIntField(obj_sendStat, id_fieldID, pAudioStatInfo->sendStat.iCurRTT);

	//recvStat
	jobject obj_recvStat = env->NewObject(class_AudioStat, id_AudioStat);

	id_fieldID = env->GetFieldID(class_AudioStat, "iTotalFractionLost", "I");
    env->SetIntField(obj_recvStat, id_fieldID, pAudioStatInfo->recvStat.iTotalFractionLost);

	id_fieldID = env->GetFieldID(class_AudioStat, "iCurJitter", "I");
    env->SetIntField(obj_recvStat, id_fieldID, pAudioStatInfo->recvStat.iCurJitter);

	id_fieldID = env->GetFieldID(class_AudioStat, "iCurBitrate", "I");
    env->SetIntField(obj_recvStat, id_fieldID, pAudioStatInfo->recvStat.iCurBitrate);

	id_fieldID = env->GetFieldID(class_AudioStat, "iAvgBitrate", "I");
    env->SetIntField(obj_recvStat, id_fieldID, pAudioStatInfo->recvStat.iAvgBitrate);

	id_fieldID = env->GetFieldID(class_AudioStat, "iCurRTT", "I");
    env->SetIntField(obj_recvStat, id_fieldID, pAudioStatInfo->recvStat.iCurRTT);
	/////////////////////

    jclass class_AudioStatInfo = env->FindClass("com/zte/softda/ocx/FireAudioStatInfoPara");
    jmethodID id_AudioStatInfo  = env->GetMethodID(class_AudioStatInfo, "<init>", "()V");
    jobject obj_AudioStatInfo = env->NewObject(class_AudioStatInfo, id_AudioStatInfo);

    id_fieldID = env->GetFieldID(class_AudioStatInfo, "iChannleID", "I");
    env->SetIntField(obj_AudioStatInfo, id_fieldID, pAudioStatInfo->iChannleID);

	jstring strCapName = env->NewStringUTF(pAudioStatInfo->strCapName);
    id_fieldID = env->GetFieldID(class_AudioStatInfo, "strCapName", "Ljava/lang/String;");
    env->SetObjectField(obj_AudioStatInfo, id_fieldID, strCapName);
    env->DeleteLocalRef(strCapName);

	id_fieldID = env->GetFieldID(class_AudioStatInfo, "iBandWidth", "I");
    env->SetIntField(obj_AudioStatInfo, id_fieldID, pAudioStatInfo->iBandWidth);
	
	id_fieldID = env->GetFieldID(class_AudioStatInfo, "sendStat", "Lcom/zte/softda/ocx/AudioStat;");
	env->SetObjectField(obj_AudioStatInfo, id_fieldID, obj_sendStat);

	id_fieldID = env->GetFieldID(class_AudioStatInfo, "recvStat", "Lcom/zte/softda/ocx/AudioStat;");
	env->SetObjectField(obj_AudioStatInfo, id_fieldID, obj_recvStat);

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_AudioStatInfo);
    TraceMsgWindow1(1, "rcscontroller java_FireAudioStatNotify over");
}

void java_FireUploadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iRetCode, int iRate, DWORD iTransedSize)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    TraceMsgWindow1(1, "rcscontroller java_FireUploadFileRsp pSessionID[%s] pFileID[%s] iRetCode[%d] iRate[%d] iTransedSize[%d]",
        pSessionID, pFileID, iRetCode, iRate, iTransedSize);
   
    iresult = getStaticJniMethod((char *)"java_FireUploadFileRsp", (char *)"(Ljava/lang/String;Ljava/lang/String;IIJ)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireUploadFileRsp");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSessionID), JNI_STR(env,pFileID), iRetCode, iRate, (jlong)iTransedSize);
}

void java_FireDownloadFileRsp(LPCTSTR pSessionID, LPCTSTR pFileID, int iRetCode, int iRate, DWORD iTransedSize)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;

    TraceMsgWindow1(1, "rcscontroller java_FireDownloadFileRsp pSessionID[%s] pFileID[%s] iRetCode[%d] iRate[%d] iTransedSize[%d]",
        pSessionID, pFileID, iRetCode, iRate, iTransedSize);
    
    iresult = getStaticJniMethod((char *)"java_FireDownloadFileRsp", (char *)"(Ljava/lang/String;Ljava/lang/String;IIJ)V", &env, &gJinMethod);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireDownloadFileRsp");
        return;
    }
    env->CallStaticVoidMethod(gpostClass, gJinMethod, JNI_STR(env,pSessionID), JNI_STR(env,pFileID), iRetCode, iRate, (jlong)iTransedSize);
}
void java_FireGetOfflineMsgRsp(int iRetCode, LPCTSTR cSeq, int endFlag, int nNum, void **pMsgList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;

    if(nNum > MAX_OFFLINE_MSG_NUM)
    {
        TraceMsgWindow1(1, "rcscontroller java_FireGetOfflineMsgRsp nNum[%d] is abnormal, MAX_OFFLINE_MSG_NUM[%d]", nNum, MAX_OFFLINE_MSG_NUM);
        return;
    }
	
	TraceMsgWindow1(1, "rcscontroller java_FireGetOfflineMsgRsp iRetCode[%d] cSeq[%s] endFlag[%d] nNum[%d]", 
	    iRetCode, cSeq, endFlag, nNum);

    if (NULL == pMsgList)
    {
		TraceMsgWindow1(1, "rcscontroller java_FireGetOfflineMsgRsp pMsgList is NULL");   
        return;
    }

	iresult = getStaticJniMethod((char *)"java_FireGetOfflineMsgRsp", (char *)"(Lcom/zte/softda/ocx/OfflineMessageList;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
    	TraceMsgWindow1(1, "rcscontroller not found java_FireGetOfflineMsgRsp");
        return;
    }
	
	JOjbectWrapper objEvent(env, "com/zte/softda/ocx/OfflineMessageList");
	objEvent.NewObject();

	jclass class_string = env->FindClass("com/zte/softda/ocx/OfflineMessageItem");
    jobjectArray array_msgs = env->NewObjectArray(nNum, class_string, NULL);

	OfflineMsgItem_T *pMsgItem = NULL;
	for (int i = 0; i < nNum; i++)
    {
        pMsgItem = (OfflineMsgItem_T*)pMsgList[i];
		if (NULL == pMsgItem)
			continue;
   
        JOjbectWrapper objMsgItem(env, "com/zte/softda/ocx/OfflineMessageItem");
		objMsgItem.NewObject();
        objMsgItem.SetIntValue("iMsgType", pMsgItem->iMsgType);
		objMsgItem.SetStringValue("cSenderURI", pMsgItem->cSenderURI);
		objMsgItem.SetStringValue("cReceiverURI", pMsgItem->cReceiverURI);
        objMsgItem.SetStringValue("cSendTime", pMsgItem->cSendTime);
		objMsgItem.SetStringValue("cMessageID", pMsgItem->cMessageID);
        objMsgItem.SetIntValue("iReceipt", pMsgItem->iReceipt);
        objMsgItem.SetStringValue("pMessage", pMsgItem->pMessage);
        
        env->SetObjectArrayElement(array_msgs, i, objMsgItem.GetObject());      
    }

    objEvent.SetIntValue("iReturnCode", iRetCode);
    objEvent.SetStringValue("cSeq", cSeq);
    objEvent.SetIntValue("iEndFlag", endFlag);
    objEvent.SetIntValue("itemNum", nNum);
    objEvent.SetContainsObject("msglist", "[Lcom/zte/softda/ocx/OfflineMessageItem;", array_msgs);
    
	env->CallStaticVoidMethod(gpostClass, gJinMethod, objEvent.GetObject());
	TraceMsgWindow1(1, "java_FireGetOfflineMsgRsp fire over");
}

void java_FireSendBroadcastMsgRsp(int iRetCode, LPCTSTR cMsgID, LPCTSTR pURIs, LPCTSTR gURIs, LPCTSTR filePath)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireSendBroadcastMsgRsp iRetCode[%d] cMsgID[%s] pURIs[%s] gURIs[%s] filePath[%s]", 
        iRetCode, cMsgID, pURIs, gURIs, filePath);
 
    iresult = getStaticJniMethod((char *)"java_FireSendBroadcastMsgRsp", (char *)"(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireSendBroadcastMsgRsp");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iRetCode, JNI_STR(env, cMsgID), JNI_STR(env, pURIs), JNI_STR(env, gURIs), JNI_STR(env, filePath));

}

void java_FireGetUserLogonStatusResult(short *pNotify)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	jfieldID id_fieldID;

	GetUserLogonStatusResult_T *pResult = (GetUserLogonStatusResult_T *)pNotify;
	
	TraceMsgWindow1(1, "rcscontroller java_FireGetUserLogonStatusResult iResult[%d] cURI[%s] iPhoneType[%d] cPhoneStatus[%s] iPhoneSecLastMsg[%d] cPhoneLastMsgType[%s] cPhoneOSVer[%s] \
iDesktopType[%d] cDesktopStatus[%s] iDesktopMinLastMsg[%d] cPhoneLastMsgType[%s] cDesktopOSVer[%s]", 
        pResult->iResult, pResult->cURI, pResult->iPhoneType, pResult->cPhoneStatus, pResult->iPhoneSecLastMsg, pResult->cPhoneLastMsgType, pResult->cPhoneOSVer,
		pResult->iDesktopType, pResult->cDesktopStatus, pResult->iDesktopSecLastMsg, pResult->cPhoneLastMsgType, pResult->cDesktopOSVer);
 
    iresult = getStaticJniMethod((char *)"java_FireGetUserLogonStatusResult", (char *)"(Lcom/zte/softda/ocx/FireGetUserLogonStatusResult;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireGetUserLogonStatusResult");
        return;
    }

	jclass class_logonStatusResult = env->FindClass("com/zte/softda/ocx/FireGetUserLogonStatusResult");
    jmethodID id_logonStatusResult  = env->GetMethodID(class_logonStatusResult, "<init>", "()V");
    jobject obj_logonStatusResult = env->NewObject(class_logonStatusResult, id_logonStatusResult);

    id_fieldID = env->GetFieldID(class_logonStatusResult, "iResult", "I");
    env->SetIntField(obj_logonStatusResult, id_fieldID, (jint)pResult->iResult);

    jstring cURI = env->NewStringUTF(pResult->cURI);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cURI", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cURI);

	id_fieldID = env->GetFieldID(class_logonStatusResult, "iPhoneType", "I");
    env->SetIntField(obj_logonStatusResult, id_fieldID, (jint)pResult->iPhoneType);

	jstring cPhoneStatus = env->NewStringUTF(pResult->cPhoneStatus);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cPhoneStatus", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cPhoneStatus);

	id_fieldID = env->GetFieldID(class_logonStatusResult, "iPhoneSecLastMsg", "I");
    env->SetIntField(obj_logonStatusResult, id_fieldID, (jint)pResult->iPhoneSecLastMsg);

	jstring cPhoneLastMsgType = env->NewStringUTF(pResult->cPhoneLastMsgType);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cPhoneLastMsgType", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cPhoneLastMsgType);

	jstring cPhoneOSVer = env->NewStringUTF(pResult->cPhoneOSVer);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cPhoneOSVer", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cPhoneOSVer);

	jstring cPhoneLoginInfo = env->NewStringUTF(pResult->cPhoneLoginInfo);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cPhoneLoginInfo", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cPhoneLoginInfo);

	//
	id_fieldID = env->GetFieldID(class_logonStatusResult, "iDesktopType", "I");
    env->SetIntField(obj_logonStatusResult, id_fieldID, (jint)pResult->iDesktopType);

	jstring cDesktopStatus = env->NewStringUTF(pResult->cDesktopStatus);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cDesktopStatus", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cDesktopStatus);

	id_fieldID = env->GetFieldID(class_logonStatusResult, "iDesktopSecLastMsg", "I");
    env->SetIntField(obj_logonStatusResult, id_fieldID, (jint)pResult->iDesktopSecLastMsg);

	jstring cDesktopLastMsgType = env->NewStringUTF(pResult->cDesktopLastMsgType);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cDesktopLastMsgType", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cDesktopLastMsgType);

	jstring cDesktopOSVer = env->NewStringUTF(pResult->cDesktopOSVer);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cDesktopOSVer", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cDesktopOSVer);

	jstring cDesktopLoginInfo = env->NewStringUTF(pResult->cDesktopLoginInfo);
    id_fieldID = env->GetFieldID(class_logonStatusResult, "cDesktopLoginInfo", "Ljava/lang/String;");
    env->SetObjectField(obj_logonStatusResult, id_fieldID, cDesktopLoginInfo);

	//

    env->CallStaticVoidMethod(gpostClass, gJinMethod, obj_logonStatusResult);

    env->DeleteLocalRef(cURI);
    env->DeleteLocalRef(cPhoneStatus);
    env->DeleteLocalRef(cPhoneLastMsgType);
	env->DeleteLocalRef(cPhoneOSVer);
	env->DeleteLocalRef(cPhoneLoginInfo);

	env->DeleteLocalRef(cDesktopStatus);
    env->DeleteLocalRef(cDesktopLastMsgType);
	env->DeleteLocalRef(cDesktopOSVer);
	env->DeleteLocalRef(cDesktopLoginInfo);
}

void java_FireConcernGroupOperateResult(int iRetCode, int iType, LPCTSTR eTag, LPCTSTR gURIList)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireConcernGroupOperateResult iRetCode[%d] iType[%d] eTag[%s] gURIList[%s]", 
        iRetCode, iType, eTag, gURIList ? gURIList : "");
 
    iresult = getStaticJniMethod((char *)"java_FireConcernGroupOperateResult", (char *)"(IILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireConcernGroupOperateResult");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iRetCode, iType, JNI_STR(env, eTag), JNI_STR(env, gURIList));
}

void java_FireUploadFileByXcapRsp(int iRetCode, int iType, LPCTSTR msgId, LPCTSTR filePath)
{
	jmethodID gJinMethod;
	JNIEnv *env;
	int iresult = -1;
	
	TraceMsgWindow1(1, "rcscontroller java_FireUploadFileByXcapRsp iRetCode[%d] iType[%d] msgId[%s] filePath[%s]", 
        iRetCode, iType, msgId, filePath);
 
    iresult = getStaticJniMethod((char *)"java_FireUploadFileByXcapRsp", (char *)"(IILjava/lang/String;Ljava/lang/String;)V", &env, &gJinMethod);
    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        TraceMsgWindow1(1, "rcscontroller not found java_FireUploadFileByXcapRsp");
        return;
    }

	env->CallStaticVoidMethod(gpostClass, gJinMethod, iRetCode, iType, JNI_STR(env, msgId), JNI_STR(env, filePath));
}

/*===============================================
* end callback java method
*===============================================*/



/*===============================================
* begin jni method
*===============================================*/
/*
get the virtual machine context
*/
static jint setRcsOCXEnv(JNIEnv* env, jclass cls, jint opr)
{
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller begin-register rcsOcx jvm Env");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "begin-register rcsOcx jvm Env");

    //int retGvm=env->GetJavaVM(&gs_jvm);

    jclass	localClass = env->FindClass(classPathName_postagent);
    if (localClass == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller localClass is NULL");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "localClass is NULL");

        return JNI_ERR;
    }
    gpostClass = (jclass)env->NewGlobalRef(localClass);
    env->DeleteLocalRef((jobject)localClass);

    if(gpostClass == NULL)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller gpostClass is NULL");
        else if(g_WriteLogType == 1)
            LOGI(TAG, "gpostClass is NULL");

        return JNI_ERR;
    }

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller end-register rcsOcx jvm Env");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "end-register rcsOcx jvm Env");

    return JNI_OK;
}
static jboolean jni_launcherThreads(JNIEnv* env, jclass cls, jstring jstrcfgName)
{
    const char *str = NULL;
    char cfgName[256 + 1] = {0};

    safeGetStringUTFChars(env, jstrcfgName, cfgName, sizeof(cfgName) - 1, NULL);
    SetCfgfileName(cfgName);//�ӽ���õ�gcCfgFile·����
#if 0
    if(WRITELOG == 1)
        SetLogFileName();//������־��ʼ��·��
#endif
    LoadLogConfig();

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller this cfg path is [%s]", cfgName);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "this cfg path is [%s]", cfgName);

    StartAllAgent();//����zai��֮ǰ��ʼ����־·��,��������so���ز��ֵ���־���ܴ�ӡ�ˡ�

    return JNI_TRUE;
}
static jboolean jni_bLogon(JNIEnv* env, jclass cls, jint iTpye, jobject  LoginParaobject)
{

    LOGON_PARA_NGN_DAS logonPara = {0};

    /* get the class */
    jclass class_LoginPara = env->GetObjectClass(LoginParaobject);

    getStringField(env, LoginParaobject, class_LoginPara, "chSoftPhone", logonPara.chSoftPhone,  sizeof(logonPara.chSoftPhone) - 1);
    getStringField(env, LoginParaobject, class_LoginPara, "chPassword", logonPara.chPassword,  sizeof(logonPara.chPassword) - 1);
    getStringField(env, LoginParaobject, class_LoginPara, "chSSIP", logonPara.chSSIP,  sizeof(logonPara.chSSIP) - 1);
    getStringField(env, LoginParaobject, class_LoginPara, "chSipServerIP", logonPara.chSipServerIP,  sizeof(logonPara.chSipServerIP) - 1);
    getStringField(env, LoginParaobject, class_LoginPara, "chVersion", logonPara.chVersion,  sizeof(logonPara.chVersion) - 1);

    jfieldID  id_lSSPort = env->GetFieldID(class_LoginPara, "lSSPort", "J");
    logonPara.lSSPort = env->GetLongField(LoginParaobject, id_lSSPort);

    jfieldID  id_lSipServerPort = env->GetFieldID(class_LoginPara, "lSipServerPort", "J");
    logonPara.lSipServerPort = env->GetLongField(LoginParaobject, id_lSipServerPort);

    jfieldID  id_LoginType = env->GetFieldID(class_LoginPara, "iLoginType", "I");
    logonPara.iLoginType = env->GetIntField(LoginParaobject, id_LoginType);

    return bLogon(iTpye, (short far*)&logonPara, 0);
}
static jboolean jni_bForceLogonServer(JNIEnv* env, jclass cls)
{
    return bForceLogonServer();
}
static jboolean jni_bGoOnLogonServer(JNIEnv* env, jclass cls)
{
    return bGoOnLogonServer();
}
static jboolean jni_bLogoutServer(JNIEnv* env, jclass cls)
{
    return bLogoutServer();
}
static jboolean jni_bCreateMultiCall(JNIEnv* env, jclass cls, jstring lpJID, jstring lpCallee, jlong lType)
{
    char cJID[256 + 1] = {0};
    char cCallee[256 + 1] = {0};

    safeGetStringUTFChars(env, lpJID, cJID, sizeof(cJID) - 1, NULL);
    safeGetStringUTFChars(env, lpCallee, cCallee, sizeof(cCallee) - 1, NULL);

    return bCreateMultiCall(cJID, cCallee, (long)lType);
}
static jboolean jni_bAcceptCall(JNIEnv* env, jclass cls, jstring lpCaller)
{
    char cCaller[256 + 1] = {0};

    safeGetStringUTFChars(env, lpCaller, cCaller, sizeof(cCaller) - 1, NULL);

    return bAcceptCall(cCaller);
}
static jboolean jni_bRejectCall(JNIEnv* env, jclass cls, jstring lpCaller, jint iCause)
{
    char cCaller[256 + 1] = {0};

    safeGetStringUTFChars(env, lpCaller, cCaller, sizeof(cCaller) - 1, NULL);

    return bRejectCall(cCaller, (int)iCause);
}
static jboolean jni_bLeaveCall(JNIEnv* env, jclass cls, jint iCause)
{
    return bLeaveCall(iCause);
}
static void jni_SecondDialNum(JNIEnv* env, jclass cls, jstring lpNum)
{
    char cNum[256 + 1] = {0};

    safeGetStringUTFChars(env, lpNum, cNum, sizeof(cNum) - 1, NULL);

    SecondDialNum(cNum);

    return;
}
static jboolean jni_bCallHold(JNIEnv* env, jclass cls, jstring pPhoneNumber, jlong iType)
{
    char cPhoneNumber[256 + 1] = {0};

    safeGetStringUTFChars(env, pPhoneNumber, cPhoneNumber, sizeof(cPhoneNumber) - 1, NULL);

    return bCallHold(cPhoneNumber, (long)iType);
}
static jboolean jni_bDoNewCallWhenBusy2(JNIEnv* env, jclass cls, jlong iDoType, jstring pCaller)
{
    char cCaller[256 + 1] = {0};

    safeGetStringUTFChars(env, pCaller, cCaller, sizeof(cCaller) - 1, NULL);

    return bDoNewCallWhenBusy2((long)iDoType, cCaller);
}
static jboolean jni_bCreateConfCall(JNIEnv* env, jclass cls, jlong lDuration, jlong lParties)
{
    return bCreateConfCall((long)lDuration, (long)lParties);
}
static jstring jni_GetAudioProps(JNIEnv* env, jclass cls)
{
    char *pResult = NULL;
    pResult = GetAudioProps();
    jstring jstrResult = env->NewStringUTF(pResult);
    ZX_free(pResult);

    return jstrResult;
}
static jstring jni_GetVideoProps(JNIEnv* env, jclass cls)
{
    char *pResult = NULL;
    pResult = GetVideoProps();
    jstring jstrResult = env->NewStringUTF(pResult);
    ZX_free(pResult);

    return jstrResult;
}
static void jni_SetAudioProps(JNIEnv* env, jclass cls, jstring jstrCodecName, jlong nPacketPeriod)
{
    char cCodecName[50 + 1] = {0};

    safeGetStringUTFChars(env, jstrCodecName, cCodecName, sizeof(cCodecName) - 1, NULL);

    SetAudioProps(cCodecName, (long)nPacketPeriod);

    return;
}
static void jni_SetVideoProps(JNIEnv* env, jclass cls, jstring jstrCodecName, jlong nRate)
{
    char cCodecName[50 + 1] = {0};

    safeGetStringUTFChars(env, jstrCodecName, cCodecName, sizeof(cCodecName) - 1, NULL);

    SetVideoProps(cCodecName, (long)nRate);

    return;
}
static jboolean jni_bIMSCallMethod(JNIEnv* env, jclass cls, jlong lType,  jobject jobj_NormalCallMethod)
{
    NORMAL_CALL_METHOD stNormalCallMethod = {0};

    /* get the class */
    jclass class_CallMethod = env->GetObjectClass(jobj_NormalCallMethod);

    getStringField(env, jobj_NormalCallMethod, class_CallMethod, "cNumber1", stNormalCallMethod.cNumber1,  sizeof(stNormalCallMethod.cNumber1) - 1);
    getStringField(env, jobj_NormalCallMethod, class_CallMethod, "cNumber2", stNormalCallMethod.cNumber2,  sizeof(stNormalCallMethod.cNumber2) - 1);
    getStringField(env, jobj_NormalCallMethod, class_CallMethod, "cNumber3", stNormalCallMethod.cNumber3,  sizeof(stNormalCallMethod.cNumber3) - 1);
    getStringField(env, jobj_NormalCallMethod, class_CallMethod, "cNumber4", stNormalCallMethod.cNumber4,  sizeof(stNormalCallMethod.cNumber4) - 1);


    jfieldID  id_iTye = env->GetFieldID(class_CallMethod, "iType", "I");
    stNormalCallMethod.iType = env->GetIntField(jobj_NormalCallMethod, id_iTye);

    return bIMSCallMethod((long)lType, (short far*)&stNormalCallMethod);
}
static jboolean jni_bWhetherEncrypt(JNIEnv* env, jclass cls, jlong lSetValue)
{
    return bWhetherEncrypt((long)lSetValue);
}
static jboolean jni_bStartRecord(JNIEnv* env, jclass cls,  jstring jstr_FilePathName)
{
    char cFilePathName[264 + 1] = {0};

    safeGetStringUTFChars(env, jstr_FilePathName, cFilePathName, sizeof(cFilePathName) - 1, NULL);

    return bStartRecord(cFilePathName);
}
static jboolean  jni_bStopRecord(JNIEnv* env, jclass cls)
{
    return bStopRecord();
}
static jboolean  jni_bReleaseTalker(JNIEnv* env, jclass cls, jstring jstr_Callee)
{
    char cCallee[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_Callee, cCallee, sizeof(cCallee) - 1, NULL);

    return bReleaseTalker(cCallee);
}
static jboolean  jni_bSelectTalker(JNIEnv* env, jclass cls, jstring jstr_Callee)
{
    char cCallee[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_Callee, cCallee, sizeof(cCallee) - 1, NULL);

    return bSelectTalker(cCallee);
}
static jboolean  jni_bInviteToMultiCall(JNIEnv* env, jclass cls, jstring jstr_JID, jstring jstr_Callee, jlong lType)
{
    char cJID[256 + 1] = {0};
    char cCallee[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_JID, cJID, sizeof(cJID) - 1, NULL);
    safeGetStringUTFChars(env, jstr_Callee, cCallee, sizeof(cCallee) - 1, NULL);

    return bInviteToMultiCall(cJID, cCallee, (long)lType);
}
static jboolean  jni_bIMSSendAttachMessage(JNIEnv* env, jclass cls, jlong iType, jstring jstr_FilePath, jstring jstr_FileType, jstring jstr_Message, jstring jstr_ReceiverURI ,jstring jstr_LocalMsgID, jint iReceipt)
{
    const char *str = NULL;
    char cFilePath[512 + 1] = {0};
    char cFileType[255 + 1] = {0};
    char  *pMessage = NULL;
    char cReceiverURI[256 + 1] = {0};
	char cLocalMsgID[MAX_STRING_LEN_32 + 1] = {0};
    BOOL bReturn = JNI_FALSE;

    safeGetStringUTFChars(env, jstr_FilePath, cFilePath, sizeof(cFilePath) - 1, NULL);
    safeGetStringUTFChars(env, jstr_FileType, cFileType, sizeof(cFileType) - 1, NULL);
    safeGetStringUTFChars(env, jstr_ReceiverURI, cReceiverURI, sizeof(cReceiverURI) - 1, NULL);
	safeGetStringUTFChars(env, jstr_LocalMsgID, cLocalMsgID, sizeof(cLocalMsgID) - 1, NULL);

    str = env->GetStringUTFChars(jstr_Message, NULL);
    if (str == NULL)
    {
        return JNI_FALSE; /* OutOfMemoryError already thrown */
    }
    pMessage = (char *)ZX_malloc(strlen(str) + 1);
    if (NULL == pMessage)
    {
        return JNI_FALSE;
    }
    strncpy(pMessage, str, strlen(str));
    pMessage[strlen(str)] = '\0';
    env->ReleaseStringUTFChars(jstr_Message, str);

    bReturn = bIMSSendAttachMessage((long)iType, cFilePath, cFileType, pMessage, cReceiverURI, cLocalMsgID, iReceipt);
    ZX_free(pMessage);
    return bReturn;
}
static jstring     jni_cIMSSendFile(JNIEnv* env, jclass cls, jstring jstr_FilePathName, jstring jstr_ReceiverURI, jstring jstr_FileID)
{
    char cFilePathName[256 + 1] = {0};
    char cReceiverURI[256 + 1] = {0};
    char cFileID[32 + 1] = {0};

    safeGetStringUTFChars(env, jstr_FilePathName, cFilePathName, sizeof(cFilePathName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_ReceiverURI, cReceiverURI, sizeof(cReceiverURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_FileID, cFileID, sizeof(cFileID) - 1, NULL);

    return env->NewStringUTF(cIMSSendFile(cFilePathName, cReceiverURI, cFileID));
}
static jboolean  jni_bIMSDoFileTransReq(JNIEnv* env, jclass cls, jlong iAccept, jstring jstr_SenderURI, jstring jstr_FilePathName, jstring jstr_SessionID)
{
    char cSenderURI[256 + 1] = {0};
    char cFilePathName[256 + 1] = {0};
    char cSessionID[32 + 1] = {0};

    safeGetStringUTFChars(env, jstr_SenderURI, cSenderURI, sizeof(cSenderURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_FilePathName, cFilePathName, sizeof(cFilePathName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_SessionID, cSessionID, sizeof(cSessionID) - 1, NULL);

    return bIMSDoFileTransReq((long)iAccept, cSenderURI, cFilePathName, cSessionID);
}
static jboolean  jni_bIMSCancelSendingFile(JNIEnv* env, jclass cls, jstring jstr_SessionID)
{
    char cSessionID[32 + 1] = {0};

    safeGetStringUTFChars(env, jstr_SessionID, cSessionID, sizeof(cSessionID) - 1, NULL);

    return bIMSCancelSendingFile(cSessionID);
}
static jboolean  jni_bIMSCancelRecvingFile(JNIEnv* env, jclass cls, jstring jstr_SessionID)
{
    char cSessionID[32 + 1] = {0};

    safeGetStringUTFChars(env, jstr_SessionID, cSessionID, sizeof(cSessionID) - 1, NULL);

    return bIMSCancelRecvingFile(cSessionID);
}
static jboolean jni_bIMSGetDeptAddrList(JNIEnv* env, jclass cls, jlong iType, jstring pAddrList)
{
    char cURI[256 + 1] = {0};

    safeGetStringUTFChars(env, pAddrList, cURI, sizeof(cURI) - 1, NULL);

    return bIMSGetDeptAddrList(iType, cURI);
}
static jboolean jni_bIMSPsMethod(JNIEnv* env, jclass cls, jint iTpye, jobject jobj_IMSPsMethodParaobject)
{
	if(giScene == SCENE_ONLY_SS || giScene == SCENE_ONLY_IMS)
	{
	return FALSE;
	}
    PS_PRENSENCE_METHOD_PARA PrenceMethod = {0};

    /* get the class */
    jclass class_IMSPsMethod = env->GetObjectClass(jobj_IMSPsMethodParaobject);

    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "cNote", PrenceMethod.cNote,  sizeof(PrenceMethod.cNote) - 1);
    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "cLocation", PrenceMethod.cLocation,  sizeof(PrenceMethod.cLocation) - 1);
    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "cCamera", PrenceMethod.cCamera,  sizeof(PrenceMethod.cCamera) - 1);
    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "cMood", PrenceMethod.cMood,  sizeof(PrenceMethod.cMood) - 1);
    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "cJob", PrenceMethod.cJob,  sizeof(PrenceMethod.cJob) - 1);
    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "cPhoneStatus", PrenceMethod.cPhoneStatus,  sizeof(PrenceMethod.cPhoneStatus) - 1);
//   getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "cEnlarge2", PrenceMethod.cEnlarge2,  sizeof(PrenceMethod.cEnlarge2)-1);
    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "CTerminalType", PrenceMethod.CTerminalType,  sizeof(PrenceMethod.CTerminalType) - 1);
    getStringField(env, jobj_IMSPsMethodParaobject, class_IMSPsMethod, "COsType", PrenceMethod.COsType,  sizeof(PrenceMethod.COsType) - 1);
    PrenceMethod.cNote[MAX_NOTE_LEN] = '\0';
    PrenceMethod.cLocation[MAX_PRESENCE_LOCATION_LEN] = '\0';
    PrenceMethod.cCamera[MAX_BOOL_CAMERA_LEN] = '\0';
    PrenceMethod.cMood[512] = '\0';
    PrenceMethod.cJob[128] = '\0';
    PrenceMethod.cPhoneStatus[MAX_PHONESTATUS_LEN] = '\0';
    PrenceMethod.CTerminalType[16] = '\0';
    PrenceMethod.COsType[16] = '\0';

    return bIMSPsMethod((long)iTpye, (short far*)&PrenceMethod);
}
static jboolean jni_bIMSSearchRLS(JNIEnv* env, jclass cls)
{
    return bIMSSearchRLS();
}
static jboolean jni_bIMSGetPresenceRules(JNIEnv* env, jclass cls)
{
    return bIMSGetPresenceRules();
}
static jboolean jni_bIMSSubWatcher(JNIEnv* env, jclass cls)
{
    return bIMSSubWatcher();
}	
static jboolean jni_bIMSSubscribeOneRLS(JNIEnv* env, jclass cls, jstring pURI)
{
    char cURI[256 + 1] = {0};

    safeGetStringUTFChars(env, pURI, cURI, sizeof(cURI) - 1, NULL);

    return bIMSSubscribeOneRLS(cURI);
}
static jboolean jni_bIMSSendMessage(JNIEnv* env, jclass cls, jint iType, jint iReport, jstring jstr_Message, jstring jstr_DeliverTime,
                                    jstring jstr_ReceiverURI, jstring jstr_LocalMsgID, jstring jstr_TMSubject, jint iFileSeekSize, jint iReceipt)
{
    char *szMessage = NULL;//[1024 * 5 + 1] = {0};
    char szDeliverTime[32 + 1] = {0};
    char szReceiverURI[255 + 1] = {0};
    char szLocalMsgID[MAX_STRING_LEN_32 + 1] = {0};
    char szTMsubject [255 + 1] = {0};

    //safeGetStringUTFChars(env, jstr_Message, szMessage, sizeof(szMessage) - 1, NULL);
    szMessage = safeAllocStringUTFCharsFromJString(env, jstr_Message);
    safeGetStringUTFChars(env, jstr_DeliverTime, szDeliverTime, sizeof(szDeliverTime) - 1, NULL);
    safeGetStringUTFChars(env, jstr_ReceiverURI, szReceiverURI, sizeof(szReceiverURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_LocalMsgID, szLocalMsgID, sizeof(szLocalMsgID) - 1, NULL);
    safeGetStringUTFChars(env, jstr_TMSubject, szTMsubject, sizeof(szTMsubject) - 1, NULL);

    BOOL bRet = bIMSSendMessage(iType, iReport, szMessage, szDeliverTime, szReceiverURI, szLocalMsgID, szTMsubject, (double)iFileSeekSize, iReceipt);
    ZX_free(szMessage);
    return bRet;
}
static jboolean    jni_bIMSSendFileRequest(JNIEnv* env, jclass cls, jbyteArray jstr_FilePathName, jstring jstr_ReceiverURI, jstring jstr_FileID, jint iFileSize)
{
    char szFileName[1024 + 1] = {0};
    char szReceiverURI[255 + 1] = {0};
    char szFileID[32 + 1] = {0};
    int iArrayLen = 0;
    jbyte* pFileName = NULL;

    //safeGetStringUTFChars(env, jstr_FilePathName, szFileName, sizeof(szFileName) -1, NULL);
    if (jstr_FilePathName != NULL)
        iArrayLen = env->GetArrayLength(jstr_FilePathName);
    if (iArrayLen > 0)
    {
        pFileName = (jbyte *)ZX_malloc(iArrayLen + 1);
        memset(pFileName, 0, iArrayLen + 1);
        env->GetByteArrayRegion(jstr_FilePathName, 0, iArrayLen, pFileName);
    }
    ZX_strcpy(szFileName, pFileName);
    ZX_free(pFileName);
    safeGetStringUTFChars(env, jstr_ReceiverURI, szReceiverURI, sizeof(szReceiverURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_FileID, szFileID, sizeof(szFileID) - 1, NULL);

    return bIMSSendMessage(IM_TYPE_FILE_TRANS_REQ, iFileSize, szFileID, szFileName, szReceiverURI, "", "Request", 0);
}
static jboolean  jni_bIMSSendFileResponse(JNIEnv* env, jclass cls, jstring pFilePathName, jstring pReceiverURI, jstring pLocalMsgId, jstring pFileID, jstring pStatus, jint iFileSeekSize)
{
    char szMessage[1024 + 1] = {0};
    char szReceiverURI[255 + 1] = {0};
    char szFileId[32 + 1] = {0};
    char szStatus[32 + 1] = {0};
    char szLocalMsgId[32 + 1] = {0};

    safeGetStringUTFChars(env, pFilePathName, szMessage, sizeof(szMessage) - 1, NULL);
    safeGetStringUTFChars(env, pReceiverURI, szReceiverURI, sizeof(szReceiverURI) - 1, NULL);
    safeGetStringUTFChars(env, pFileID, szFileId, sizeof(szFileId) - 1, NULL);
    safeGetStringUTFChars(env, pStatus, szStatus, sizeof(szStatus) - 1, NULL);
    safeGetStringUTFChars(env, pLocalMsgId, szLocalMsgId, sizeof(szLocalMsgId) - 1, NULL);

    return bIMSSendMessage(IM_TYPE_FILE_TRANS_RESPONSE, 100, szMessage, szFileId, szReceiverURI, szLocalMsgId, szStatus, (DWORD)iFileSeekSize);
}
static jboolean jni_bIMSGetPublicGroup(JNIEnv* env, jclass cls, jstring jstr_Identifier, jint iOperType)
{//iOperType: 0--����etag������ѯ, 1--��������ȫ������
    char cIdentifierURI[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_Identifier, cIdentifierURI, sizeof(cIdentifierURI) - 1, NULL);

    return bIMSGetPublicGroup(cIdentifierURI, iOperType);
}

static jobjectArray jni_bSyncGetMyGroupList(JNIEnv* env, jclass cls, jstring jstr_phone)
{
    //��ѯȺ�黺������
    char filename[256] = {0};  
    char phone[64 + 1] = {0};
    
    FIRE_GROUP_INFO groupInfo; 
    memset(&groupInfo, 0, sizeof(FIRE_GROUP_INFO));
    //�˴���Ϊ�˱���δ��¼����²��ܻ�ȡ�����ļ�
    safeGetStringUTFChars(env, jstr_phone, phone, sizeof(phone) - 1, NULL);
    if(gUserInfo.phone[0] == '\0')
        ZX_strcpy(gUserInfo.phone, phone);

    if(gUserInfo.domain[0] == '\0')
    {
        strncpy(gUserInfo.domain, gConfig.IMSDomain, sizeof(gUserInfo.domain) / sizeof(char));
    }

    GetUserDataFile(MY_GROUP_INFO_FILE, filename);
    if(!ReadLocalGroupList(filename, &gMsgBuf, &groupInfo))
    {
        DEBUG_INFO("[jni_bSyncGetMyGroupList] Get group data from local file failed!");
        return NULL;
    }

    jfieldID id_fieldID;
    jclass class_FireAllGroup = env->FindClass("com/zte/softda/ocx/FireAllGroupPara");
    jmethodID id_FireAllGroup  = env->GetMethodID(class_FireAllGroup, "<init>", "()V");
    jobjectArray array_group = env->NewObjectArray(groupInfo.iNum, class_FireAllGroup, NULL);
    for(int i = 0; i < groupInfo.iNum; i++)
    {
        FIRE_ALL_GROUP* pFireGroupInfo = &groupInfo.pFireGroupInfo[i];

        jobject obj_FireGroupInfo = env->NewObject(class_FireAllGroup, id_FireAllGroup);

        jstring cGroupURI = env->NewStringUTF(pFireGroupInfo->cGroupURI);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cGroupURI", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cGroupURI);
        env->DeleteLocalRef(cGroupURI);

        jstring cGroupName = env->NewStringUTF(pFireGroupInfo->cGroupName);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cGroupName", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cGroupName);
        env->DeleteLocalRef(cGroupName);

        id_fieldID = env->GetFieldID(class_FireAllGroup, "iGroupType", "I");
        env->SetIntField(obj_FireGroupInfo, id_fieldID, pFireGroupInfo->iGroupType);

        id_fieldID = env->GetFieldID(class_FireAllGroup, "iGroupUsage", "I");
        env->SetIntField(obj_FireGroupInfo, id_fieldID, pFireGroupInfo->iGroupUsage);

        jstring cOwnerURI = env->NewStringUTF(pFireGroupInfo->cOwnerURI);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cOwnerURI", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cOwnerURI);
        env->DeleteLocalRef(cOwnerURI);

        jstring cType = env->NewStringUTF(pFireGroupInfo->cType);
        id_fieldID = env->GetFieldID(class_FireAllGroup, "cType", "Ljava/lang/String;");
        env->SetObjectField(obj_FireGroupInfo, id_fieldID, cType);
        env->DeleteLocalRef(cType);

        env->SetObjectArrayElement(array_group, i, obj_FireGroupInfo);
        env->DeleteLocalRef(obj_FireGroupInfo);
    }
              
    return array_group;
}

static jobject jni_bSyncGetGroupMembers(JNIEnv* env, jclass cls, jstring jstr_Identifier)
{
    char cGroupURI[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_Identifier, cGroupURI, sizeof(cGroupURI) - 1, NULL);

    //��ѯ���ػ�������
    char strTemp[256] = {0};
    char filename[256] = {0};
    char filenameShort[256] = {0};
    Group_T *pGroup = NULL;
    int grpIndex = FindGroupIndex(cGroupURI);
    if(grpIndex < 0)
    {
        DEBUG_INFO("jni_bSyncGetGroupMembers: cGroupURI[%s] not found in all Public GroupURI", cGroupURI);
    }
    else
    {
        pGroup = (Group_T*)list_get(gUserInfo.pGroups, grpIndex);
    }

    GetPubGroupLocalFile(cGroupURI, filenameShort);
    IMSGotOnePublicGroup_T groupData;
    GroupMemberList_T memberList;   
    memset(&groupData, 0, sizeof(IMSGotOnePublicGroup_T));
    memset(&memberList, 0, sizeof(GroupMemberList_T));
    GetUserDataFile(filenameShort, filename);
    if(!ReadLocalGroupDataFile(filename, &gMsgBuf, &groupData, &memberList))
    {
        DEBUG_INFO("[jni_bSyncGetGroupMembers(%s)] Get group data from local file failed!", cGroupURI);
        //֪ͨsoftagent����ȥ��ȡ
        if(pGroup)//pGroupû���ҵ����������ʹ���÷���Ҳ�ᱻ��
            bIMSGetPublicGroup(cGroupURI, 0);
        return NULL;
    }
    else
    {
        DEBUG_INFO("[jni_bSyncGetGroupMembers(%s)] local data etag:%s, cGroupName[%s] memberNum[%d]", 
            cGroupURI, groupData.cEtag, groupData.pGroupName, groupData.memberNum);

        if(pGroup && _stricmp(groupData.cEtag, pGroup->cGroupEtag))
        {//����etag�͵�½ʱ��ȡ������Ϣ���صĲ���ͬ����ô֪ͨsoftagent����ȥ��ȡ
            DEBUG_INFO("[jni_bSyncGetGroupMembers(%s)] local data etag:%s, pGroup->cGroupEtag[%s]", 
                cGroupURI, groupData.cEtag, pGroup->cGroupEtag);
             bIMSGetPublicGroup(cGroupURI, 0);
        }       
    }

    jfieldID id_fieldID;
    jclass class_Result = env->FindClass("com/zte/softda/ocx/FireGroupMemberList");
    jmethodID id_Result  = env->GetMethodID(class_Result, "<init>", "()V");
    jobject objResult = env->NewObject(class_Result, id_Result);

    jstring cOwner = env->NewStringUTF(groupData.attr.cOwner);
    id_fieldID = env->GetFieldID(class_Result, "cOwner", "Ljava/lang/String;");
    env->SetObjectField(objResult, id_fieldID, cOwner);
    env->DeleteLocalRef(cOwner);

    jstring cCreator = env->NewStringUTF(groupData.attr.cCreator);
    id_fieldID = env->GetFieldID(class_Result, "cCreator", "Ljava/lang/String;");
    env->SetObjectField(objResult, id_fieldID, cCreator);
    env->DeleteLocalRef(cCreator);

    jclass class_EntryType = env->FindClass("com/zte/softda/ocx/EntryTypeArray");
    jmethodID id_EntryType  = env->GetMethodID(class_EntryType, "<init>", "()V");
    jobjectArray array_EntryType = env->NewObjectArray(groupData.memberNum, class_EntryType, NULL);
    int iNum = 0;
    for(iNum = 0; iNum < groupData.memberNum; iNum++)
    {
        ENTRY_TYPE *pMember = (ENTRY_TYPE *)memberList.pMemberItem[iNum];
      
        jobject obj_EntryType = env->NewObject(class_EntryType, id_EntryType);

        jstring cDisplayName = env->NewStringUTF(pMember->cDisplayName);
        id_fieldID = env->GetFieldID(class_EntryType, "cDisplayName", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cDisplayName);
        env->DeleteLocalRef(cDisplayName);

        jstring cInfo = env->NewStringUTF(pMember->cInfo);
        id_fieldID = env->GetFieldID(class_EntryType, "cInfo", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cInfo);
        env->DeleteLocalRef(cInfo);

        jstring cURI = env->NewStringUTF(pMember->cURI);
        id_fieldID = env->GetFieldID(class_EntryType, "cURI", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cURI);
        env->DeleteLocalRef(cURI);

        jstring cPhone = env->NewStringUTF(pMember->cPhone);
        id_fieldID = env->GetFieldID(class_EntryType, "cPhone", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cPhone);
        env->DeleteLocalRef(cPhone);

        id_fieldID = env->GetFieldID(class_EntryType, "iMemberStatus", "I");
        env->SetIntField(obj_EntryType, id_fieldID, pMember->iMemberStatus);

        jstring cMemberType = env->NewStringUTF(pMember->cMemberType);
        id_fieldID = env->GetFieldID(class_EntryType, "cMemberType", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cMemberType);
        env->DeleteLocalRef(cMemberType);

        jstring cAllowModifyMyCaps = env->NewStringUTF(pMember->cAllowModifyMyCaps);
        id_fieldID = env->GetFieldID(class_EntryType, "cAllowModifyMyCaps", "Ljava/lang/String;");
        env->SetObjectField(obj_EntryType, id_fieldID, cAllowModifyMyCaps);
        env->DeleteLocalRef(cAllowModifyMyCaps);

        env->SetObjectArrayElement(array_EntryType, iNum, obj_EntryType);
        env->DeleteLocalRef(obj_EntryType);
    }

    id_fieldID = env->GetFieldID(class_Result, "memberList", "[Lcom/zte/softda/ocx/EntryTypeArray;");
    env->SetObjectField(objResult, id_fieldID, array_EntryType);
    env->DeleteLocalRef(array_EntryType);

    return objResult;
}

static jboolean  jni_bIMSSubscribeOneList(JNIEnv* env, jclass cls, jlong iType, jstring jstr_ListURI, jlong lURIType)
{
    char cListURI[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_ListURI, cListURI, sizeof(cListURI) - 1, NULL);

    return bIMSSubscribeOneList((long)iType, cListURI, (long)lURIType);
}
static jboolean  jni_bIMSDoPublicGroupInvite(JNIEnv* env, jclass cls, jstring jstr_OwnerName, jstring jstr_InviterURI, jstring jstr_GroupName, jlong iType, jstring jstr_GroupURI, jstring jstr_Return)
{
    char cOwnerName[256 + 1] = {0};
    char cInviterURI[256 + 1] = {0};
    char cGroupName[256 + 1] = {0};
    char cGroupURI[256 + 1] = {0};
    char cReturn[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_OwnerName, cOwnerName, sizeof(cOwnerName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_InviterURI, cInviterURI, sizeof(cInviterURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_GroupName, cGroupName, sizeof(cGroupName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_GroupURI, cGroupURI, sizeof(cGroupURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_Return, cReturn, sizeof(cReturn) - 1, NULL);

    return bIMSDoPublicGroupInvite(cOwnerName, cInviterURI, cGroupName, iType, cGroupURI, cReturn);
}
static jboolean jni_bIMSSetUserInfo(JNIEnv* env, jclass cls, jobject UserInfoParaobject)
{
    BOOL bReturn = JNI_FALSE;
    USER_INFO UserInfo = {0};
    const char *str = NULL;

    /* get the class */
    jclass class_UserInfo = env->GetObjectClass(UserInfoParaobject);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cName", UserInfo.cName,  sizeof(UserInfo.cName) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cSex", UserInfo.cSex,  sizeof(UserInfo.cSex) - 1);

    jfieldID  id_iOld = env->GetFieldID(class_UserInfo, "iOld", "J");
    UserInfo.iOld = env->GetLongField(UserInfoParaobject, id_iOld);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cBirthday", UserInfo.cBirthday,  sizeof(UserInfo.cBirthday) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cInterest", UserInfo.cInterest,  sizeof(UserInfo.cInterest) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cOccupation", UserInfo.cOccupation,  sizeof(UserInfo.cOccupation) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cCountry", UserInfo.cCountry,  sizeof(UserInfo.cCountry) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cZone", UserInfo.cZone,  sizeof(UserInfo.cZone) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cCity", UserInfo.cCity,  sizeof(UserInfo.cCity) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cUcNum", UserInfo.cUcNum,  sizeof(UserInfo.cUcNum) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cPocNum", UserInfo.cPocNum,  sizeof(UserInfo.cPocNum) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cMessageNum", UserInfo.cMessageNum,  sizeof(UserInfo.cMessageNum) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cFaxNum", UserInfo.cFaxNum,  sizeof(UserInfo.cFaxNum) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cHomeTel", UserInfo.cHomeTel,  sizeof(UserInfo.cHomeTel) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cOfficeTel", UserInfo.cOfficeTel,  sizeof(UserInfo.cOfficeTel) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cMobileTel", UserInfo.cMobileTel,  sizeof(UserInfo.cMobileTel) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cAutoTel", UserInfo.cAutoTel,  sizeof(UserInfo.cAutoTel) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cSHomeTel", UserInfo.cSHomeTel,  sizeof(UserInfo.cSHomeTel) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cSOfficeTel", UserInfo.cSOfficeTel,  sizeof(UserInfo.cSOfficeTel) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cSMobileTel", UserInfo.cSMobileTel,  sizeof(UserInfo.cSMobileTel) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cHomeAddr", UserInfo.cHomeAddr,  sizeof(UserInfo.cHomeAddr) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cOfficeAddr", UserInfo.cOfficeAddr,  sizeof(UserInfo.cOfficeAddr) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cOtherAddr", UserInfo.cOtherAddr,  sizeof(UserInfo.cOtherAddr) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cEmailAddr", UserInfo.cEmailAddr,  sizeof(UserInfo.cEmailAddr) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cDomain", UserInfo.cDomain,  sizeof(UserInfo.cDomain) - 1);

    jfieldID  id_iNotifyMode = env->GetFieldID(class_UserInfo, "iNotifyMode", "J");
    UserInfo.iNotifyMode = env->GetLongField(UserInfoParaobject, id_iNotifyMode);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cURI", UserInfo.cURI,  sizeof(UserInfo.cURI) - 1);

    jfieldID  id_cPhotoFile = env->GetFieldID(class_UserInfo, "cPhotoFile", "Ljava/lang/String;");
    jstring str_cPhotoFile = (jstring)env->GetObjectField(UserInfoParaobject, id_cPhotoFile);
    if(str_cPhotoFile)
    {
        str = env->GetStringUTFChars(str_cPhotoFile, NULL);
        if (str == NULL)
        {
            return JNI_FALSE; /* OutOfMemoryError already thrown */
        }
        UserInfo.cPhotoFile = (char *)ZX_malloc(strlen(str) + 1);
        if (NULL == UserInfo.cPhotoFile)
        {
            return JNI_FALSE;
        }
        strncpy(UserInfo.cPhotoFile, str, strlen(str));
        UserInfo.cPhotoFile[strlen(str)] = '\0';
        env->ReleaseStringUTFChars(str_cPhotoFile, str);
    }
    getStringField(env, UserInfoParaobject, class_UserInfo, "cPhotoIndex", UserInfo.cPhotoIndex,  sizeof(UserInfo.cPhotoIndex) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cSoftPhone", UserInfo.cSoftPhone,  sizeof(UserInfo.cSoftPhone) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cAffiliation", UserInfo.cAffiliation,  sizeof(UserInfo.cAffiliation) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cDepartment", UserInfo.cDepartment,  sizeof(UserInfo.cDepartment) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cDepartmentName", UserInfo.cDepartmentName,  sizeof(UserInfo.cDepartmentName) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cDepartmentURI", UserInfo.cDepartmentURI,  sizeof(UserInfo.cDepartmentURI) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cEmployeeNum", UserInfo.cEmployeeNum,  sizeof(UserInfo.cEmployeeNum) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cPersonEmailAddr", UserInfo.cPersonEmailAddr,  sizeof(UserInfo.cPersonEmailAddr) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cPosition", UserInfo.cPosition,  sizeof(UserInfo.cPosition) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cPhotoURI", UserInfo.cPhotoURI,  sizeof(UserInfo.cPhotoURI) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cURI2", UserInfo.cURI2,  sizeof(UserInfo.cURI2) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cBusinessPhone", UserInfo.cBusinessPhone,  sizeof(UserInfo.cBusinessPhone) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cSmsPhone", UserInfo.cSmsPhone,  sizeof(UserInfo.cSmsPhone) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cSmsBusiness", UserInfo.cSmsBusiness,  sizeof(UserInfo.cSmsBusiness) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cFaxBusiness", UserInfo.cFaxBusiness,  sizeof(UserInfo.cFaxBusiness) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cFaxExt", UserInfo.cFaxExt,  sizeof(UserInfo.cFaxExt) - 1);

    jfieldID  id_iVersionType = env->GetFieldID(class_UserInfo, "iVersionType", "J");
    UserInfo.iVersionType = env->GetLongField(UserInfoParaobject, id_iVersionType);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cRealName", UserInfo.cRealName,  sizeof(UserInfo.cRealName) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cOtherTel", UserInfo.cOtherTel,  sizeof(UserInfo.cOtherTel) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cFullSpelling", UserInfo.cFullSpelling,  sizeof(UserInfo.cFullSpelling) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cShortSpelling", UserInfo.cShortSpelling,  sizeof(UserInfo.cShortSpelling) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cFullSpelling1", UserInfo.cFullSpelling1,  sizeof(UserInfo.cFullSpelling1) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cShortSpelling1", UserInfo.cShortSpelling1,  sizeof(UserInfo.cShortSpelling1) - 1);

    jfieldID  id_iAreaCodeFlag = env->GetFieldID(class_UserInfo, "iAreaCodeFlag", "J");
    UserInfo.iAreaCodeFlag = env->GetLongField(UserInfoParaobject, id_iAreaCodeFlag);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cCompanyUri", UserInfo.cCompanyUri,  sizeof(UserInfo.cCompanyUri) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cUserCapacity", UserInfo.cUserCapacity,  sizeof(UserInfo.cUserCapacity) - 1);

    getStringField(env, UserInfoParaobject, class_UserInfo, "cAnswerMode", UserInfo.cAnswerMode,  sizeof(UserInfo.cAnswerMode) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cMsgDeliverReport", UserInfo.cMsgDeliverReport,  sizeof(UserInfo.cMsgDeliverReport) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cOnlyNotify", UserInfo.cOnlyNotify,  sizeof(UserInfo.cOnlyNotify) - 1);
    getStringField(env, UserInfoParaobject, class_UserInfo, "cAreaCode", UserInfo.cAreaCode,  sizeof(UserInfo.cAreaCode) - 1);

    bReturn = bIMSSetUserInfo((short far*)&UserInfo);
    if(UserInfo.cPhotoFile)
        ZX_free(UserInfo.cPhotoFile);
    return bReturn;
}

//add by zhenghe 211-11-07
static jboolean jni_bIMSGetPersonSubInfo(JNIEnv* env, jclass cls, jint iType, jobject  jobj_IMSGetPersonSubInfoParaobject)
{
    SUBINFO_PHOTO_PARA SubInfoPhoto = {0};

    jclass class_IMSGetPersonSubInfo = env->GetObjectClass(jobj_IMSGetPersonSubInfoParaobject);
    getStringField(env, jobj_IMSGetPersonSubInfoParaobject, class_IMSGetPersonSubInfo, "cURI", SubInfoPhoto.cURI, sizeof(SubInfoPhoto.cURI) - 1);
    getStringField(env, jobj_IMSGetPersonSubInfoParaobject, class_IMSGetPersonSubInfo, "cPhotoIndex", SubInfoPhoto.cPhotoIndex, sizeof(SubInfoPhoto.cPhotoIndex) - 1);

    return bIMSGetPersonSubInfo(iType, (short far*)&SubInfoPhoto);
}
static jboolean jni_bIMSSetMyPhoto(JNIEnv* env, jclass cls,  jstring jstr_Index, jbyteArray  jbytarr_FileContent, jlong iLen)
{
    char szIndex[256 + 1] = {0};
    jbyte *pFileContent = NULL;
    int iArrayLen = 0;

    safeGetStringUTFChars(env, jstr_Index, szIndex, sizeof(szIndex) - 1, NULL);
    if(jbytarr_FileContent != NULL)
        iArrayLen = env->GetArrayLength(jbytarr_FileContent);

    if(iArrayLen > 0)
    {
        pFileContent = (jbyte *)ZX_malloc(iArrayLen + 1);
        memset(pFileContent, 0, iArrayLen + 1);
        env->GetByteArrayRegion(jbytarr_FileContent, 0, iArrayLen, pFileContent);
    }
    return bIMSSetMyPhoto(szIndex, (short far*)pFileContent, (long)iArrayLen);
}
static jboolean jni_bDispUICmdForEcp2Method(JNIEnv* env, jclass cls, jint iType, jobject  UICmdForEcp2object)
{
    BOOL bRturn = FALSE;

    /* get the class */
    jclass class_UICmdForEcp2objectPara = env->GetObjectClass(UICmdForEcp2object);

    if(iType == 1)//�޸����빦��
    {
        Modify_MyPassword ModifyMyPasswordPara = {0};

        jfieldID id_ModifyMyPasswordInfo = env->GetFieldID(class_UICmdForEcp2objectPara, "ModifyMyPasswordPara", "Lcom/zte/softda/ocx/Modify_MyPassword;");
        jobject obj_ModifyMyPasswordInfo = env->GetObjectField(UICmdForEcp2object, id_ModifyMyPasswordInfo);
        jclass class_ModifyMyPasswordInfo = env->GetObjectClass(obj_ModifyMyPasswordInfo);

        getStringField(env, obj_ModifyMyPasswordInfo, class_ModifyMyPasswordInfo, "cOldPassword", ModifyMyPasswordPara.cOldPassword,  sizeof(ModifyMyPasswordPara.cOldPassword) - 1);
        getStringField(env, obj_ModifyMyPasswordInfo, class_ModifyMyPasswordInfo, "cNewPassword", ModifyMyPasswordPara.cNewPassword,  sizeof(ModifyMyPasswordPara.cNewPassword) - 1);

        bRturn = bDispUICmdForEcp2Method((long)iType, (short far*)&ModifyMyPasswordPara);
    }
    else if(iType == 8)//�����û���¼��Ϣ��������
    {
        USER_LOGON_INFO_METHOD UserlogonInfoPara = {0};

#if 0
        jfieldID id_UserlogonInfoPara = env->GetFieldID(class_UICmdForEcp2objectPara, "ModifyMyPasswordPara", "Lcom/zte/softda/ocx/Modify_MyPassword;");
        jobject obj_UserlogonInfoPara = env->GetObjectField(UICmdForEcp2object, id_ModifyMyPasswordInfo);
        jclass class_UserlogonInfoPara = env->GetObjectClass(obj_ModifyMyPasswordInfo);

        getStringField(env, obj_ModifyMyPasswordInfo, class_ModifyMyPasswordInfo, "cOldPassword", ModifyMyPasswordPara.cOldPassword,  sizeof(ModifyMyPasswordPara.cOldPassword) - 1);
        getStringField(env, obj_ModifyMyPasswordInfo, class_ModifyMyPasswordInfo, "cNewPassword", ModifyMyPasswordPara.cNewPassword,  sizeof(ModifyMyPasswordPara.cNewPassword) - 1);

        getStringField(env, UICmdForEcp2object, class_LoginPara, "cClientType", UserlogonInfoPara.cClientType,  sizeof(UserlogonInfoPara.cClientType) - 1);
        getStringField(env, UICmdForEcp2object, class_LoginPara, "cClientVersion", UserlogonInfoPara.cClientVersion,  sizeof(UserlogonInfoPara.cClientVersion) - 1);
        getStringField(env, UICmdForEcp2object, class_LoginPara, "cMacAddr", UserlogonInfoPara.cMacAddr,  sizeof(UserlogonInfoPara.cMacAddr) - 1);
        getStringField(env, UICmdForEcp2object, class_LoginPara, "cOSType", UserlogonInfoPara.cOSType,  sizeof(UserlogonInfoPara.cOSType) - 1);
#endif

        bRturn = bDispUICmdForEcp2Method((long)iType, (short far*)&UserlogonInfoPara);
    }
    return bRturn;
}
static jboolean jni_bCtdCallMethod(JNIEnv* env, jclass cls, jint iType, jobject  CtdCallobject)
{
    BOOL bReturn = FALSE;

    /* get the class */
    jclass class_CtdInfoPara = env->GetObjectClass(CtdCallobject);

    if(iType == CTD_TYPE_BIND)
    {
        CTD_BIND CtdBindInfoPara = {0};

        jfieldID id_CtdBindInfo = env->GetFieldID(class_CtdInfoPara, "CtdBindInfo", "Lcom/zte/softda/ocx/CTD_BIND;");
        jobject obj_CtdBindInfo = env->GetObjectField(CtdCallobject, id_CtdBindInfo);
        jclass class_CtdBindInfo = env->GetObjectClass(obj_CtdBindInfo);

        getStringField(env, obj_CtdBindInfo, class_CtdBindInfo, "chPhone", CtdBindInfoPara.chPhone,  sizeof(CtdBindInfoPara.chPhone) - 1);

        jfieldID  id_iResult = env->GetFieldID(class_CtdBindInfo, "iResult", "I");
        CtdBindInfoPara.iResult = env->GetIntField(obj_CtdBindInfo, id_iResult);

        getStringField(env, obj_CtdBindInfo, class_CtdBindInfo, "chSoftPhoneNum", CtdBindInfoPara.chSoftPhoneNum, sizeof(CtdBindInfoPara.chSoftPhoneNum) - 1);
        getStringField(env, obj_CtdBindInfo, class_CtdBindInfo, "chSoftPhonePwd", CtdBindInfoPara.chSoftPhonePwd, sizeof(CtdBindInfoPara.chSoftPhonePwd) - 1);
        getStringField(env, obj_CtdBindInfo, class_CtdBindInfo, "chSsIpAddr", CtdBindInfoPara.chSsIpAddr,  sizeof(CtdBindInfoPara.chSsIpAddr) - 1);

        jfieldID  id_iSsPort = env->GetFieldID(class_CtdBindInfo, "iSsPort", "I");
        CtdBindInfoPara.iSsPort = env->GetIntField(obj_CtdBindInfo, id_iSsPort);

        getStringField(env, obj_CtdBindInfo, class_CtdBindInfo, "chSsMeetingAddr", CtdBindInfoPara.chSsMeetingAddr,  sizeof(CtdBindInfoPara.chSsMeetingAddr) - 1);

        jfieldID  id_iSSMeetingPort = env->GetFieldID(class_CtdBindInfo, "iSSMeetingPort", "I");
        CtdBindInfoPara.iSSMeetingPort = env->GetIntField(obj_CtdBindInfo, id_iSSMeetingPort);
		
        jfieldID  id_iDoType = env->GetFieldID(class_CtdBindInfo, "iDoType", "I");
        CtdBindInfoPara.iDoType = env->GetIntField(obj_CtdBindInfo, id_iDoType);

//         if(g_WriteLogType == 2)
//             TraceMsgWindow1(1, "rcscontroller jni_bCtdCallMethod chPhone[%s], iResult[%d],chSoftPhoneNum[%s], chSoftPhonePwd[%s], chSsIpAddr[%s] iSsPort[%d]",
//                             CtdBindInfoPara.chPhone,
//                             CtdBindInfoPara.iResult,
//                             CtdBindInfoPara.chSoftPhoneNum,
//                             CtdBindInfoPara.chSoftPhonePwd,
//                             CtdBindInfoPara.chSsIpAddr,
//                             CtdBindInfoPara.iSsPort);
//         else if(g_WriteLogType == 1)
            LOGI(TAG, "jni_bCtdCallMethod chPhone[%s], iResult[%d],chSoftPhoneNum[%s], chSoftPhonePwd[%s], chSsIpAddr[%s] iSsPort[%d] chSsMeetingAddr[%s] iSSMeetingPort[%d] iDoType[%d]",
                 CtdBindInfoPara.chPhone,
                 CtdBindInfoPara.iResult,
                 CtdBindInfoPara.chSoftPhoneNum,
                 CtdBindInfoPara.chSoftPhonePwd,
                 CtdBindInfoPara.chSsIpAddr,
                 CtdBindInfoPara.iSsPort,
				 CtdBindInfoPara.chSsMeetingAddr,
				 CtdBindInfoPara.iSSMeetingPort,
				 CtdBindInfoPara.iDoType);

        bReturn = bCtdCallMethod((long)iType, (short far*)&CtdBindInfoPara);
    }
    else if(iType == CTD_TYPE_CALL)
    {
        CTD_CALL CtdCallInfoPara = {0};

        jfieldID id_CtdCallInfo = env->GetFieldID(class_CtdInfoPara, "CtdCallInfo", "Lcom/zte/softda/ocx/CTD_CALL;");
        jobject obj_CtdCallInfo = env->GetObjectField(CtdCallobject, id_CtdCallInfo);
        jclass class_CtdCallInfo = env->GetObjectClass(obj_CtdCallInfo);

        getStringField(env, obj_CtdCallInfo, class_CtdCallInfo, "chPhone", CtdCallInfoPara.chPhone,  sizeof(CtdCallInfoPara.chPhone) - 1);

        jfieldID  id_iCallId = env->GetFieldID(class_CtdCallInfo, "iCallId", "I");
        CtdCallInfoPara.iCallId = env->GetIntField(obj_CtdCallInfo, id_iCallId);

        jfieldID  id_iCallType = env->GetFieldID(class_CtdCallInfo, "iCallType", "I");
        CtdCallInfoPara.iCallType = env->GetIntField(obj_CtdCallInfo, id_iCallType);

        jfieldID  id_iResult = env->GetFieldID(class_CtdCallInfo, "iResult", "I");
        CtdCallInfoPara.iResult = env->GetIntField(obj_CtdCallInfo, id_iResult);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller jni_bCtdCallMethod chPhone[%s],iCallId[%d],iCallType[%d] iResult[%d]",
                            CtdCallInfoPara.chPhone,
                            CtdCallInfoPara.iCallId,
                            CtdCallInfoPara.iCallType,
                            CtdCallInfoPara.iResult);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "jni_bCtdCallMethod chPhone[%s],iCallId[%d],iCallType[%d] iResult[%d]",
                 CtdCallInfoPara.chPhone,
                 CtdCallInfoPara.iCallId,
                 CtdCallInfoPara.iCallType,
                 CtdCallInfoPara.iResult);

        bReturn = bCtdCallMethod((long)iType, (short far*)&CtdCallInfoPara);
    }
    else if(iType == CTD_TYPE_CTRL)
    {
        CTD_CALL_CTRL CtdCallCtrlInfoPara = {0};

        jfieldID id_CtdCallCtrlInfo = env->GetFieldID(class_CtdInfoPara, "CtdCallCtrlInfo", "Lcom/zte/softda/ocx/CTD_CALL_CTRL;");
        jobject obj_CtdCallCtrlInfo = env->GetObjectField(CtdCallobject, id_CtdCallCtrlInfo);
        jclass class_CtdCallCtrlInfo = env->GetObjectClass(obj_CtdCallCtrlInfo);

        jfieldID  id_lCtrlType = env->GetFieldID(class_CtdCallCtrlInfo, "lCtrlType", "J");
        CtdCallCtrlInfoPara.lCtrlType = env->GetLongField(obj_CtdCallCtrlInfo, id_lCtrlType);

        jfieldID  id_iCallId = env->GetFieldID(class_CtdCallCtrlInfo, "iCallId", "I");
        CtdCallCtrlInfoPara.iCallId = env->GetIntField(obj_CtdCallCtrlInfo, id_iCallId);

        getStringField(env, obj_CtdCallCtrlInfo, class_CtdCallCtrlInfo, "chPhone", CtdCallCtrlInfoPara.chPhone,  sizeof(CtdCallCtrlInfoPara.chPhone) - 1);

        jfieldID  id_iMemberId = env->GetFieldID(class_CtdCallCtrlInfo, "iMemberId", "I");
        CtdCallCtrlInfoPara.iMemberId = env->GetIntField(obj_CtdCallCtrlInfo, id_iMemberId);

        getStringField(env, obj_CtdCallCtrlInfo, class_CtdCallCtrlInfo, "chForwardPhone", CtdCallCtrlInfoPara.chForwardPhone,  sizeof(CtdCallCtrlInfoPara.chForwardPhone) - 1);

        jfieldID  id_iResult = env->GetFieldID(class_CtdCallCtrlInfo, "iResult", "I");
        CtdCallCtrlInfoPara.iResult = env->GetIntField(obj_CtdCallCtrlInfo, id_iResult);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller jni_bCtdCallMethod lCtrlType[%d], iCallId[%d], chPhone[%s], iMemberId[%d], chForwardPhone[%s] iResult[%d]",
                            CtdCallCtrlInfoPara.lCtrlType,
                            CtdCallCtrlInfoPara.iCallId,
                            CtdCallCtrlInfoPara.chPhone,
                            CtdCallCtrlInfoPara.iMemberId,
                            CtdCallCtrlInfoPara.chForwardPhone,
                            CtdCallCtrlInfoPara.iResult);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "jni_bCtdCallMethod lCtrlType[%d], iCallId[%d], chPhone[%s], iMemberId[%d], chForwardPhone[%s] iResult[%d]",
                 CtdCallCtrlInfoPara.lCtrlType,
                 CtdCallCtrlInfoPara.iCallId,
                 CtdCallCtrlInfoPara.chPhone,
                 CtdCallCtrlInfoPara.iMemberId,
                 CtdCallCtrlInfoPara.chForwardPhone,
                 CtdCallCtrlInfoPara.iResult);

        bReturn = bCtdCallMethod((long)iType, (short far*)&CtdCallCtrlInfoPara);
    }
    else if(iType == CTD_TYPE_PROC)
    {
        CTD_IN_CALL_PROC CtdInCallProcPara = {0};

        jfieldID id_CtdInCallProc = env->GetFieldID(class_CtdInfoPara, "CtdInCallProcInfo", "Lcom/zte/softda/ocx/CTD_IN_CALL_PROC;");
        jobject obj_CtdInCallProc = env->GetObjectField(CtdCallobject, id_CtdInCallProc);
        jclass class_CtdInCallProc = env->GetObjectClass(obj_CtdInCallProc);

        jfieldID  id_iCallId = env->GetFieldID(class_CtdInCallProc, "iCallId", "I");
        CtdInCallProcPara.iCallId = env->GetIntField(obj_CtdInCallProc, id_iCallId);

        getStringField(env, obj_CtdInCallProc, class_CtdInCallProc, "chPhone", CtdInCallProcPara.chPhone,  sizeof(CtdInCallProcPara.chPhone) - 1);

        jfieldID  id_iMemberId = env->GetFieldID(class_CtdInCallProc, "iMemberId", "I");
        CtdInCallProcPara.iMemberId = env->GetIntField(obj_CtdInCallProc, id_iMemberId);

        jfieldID  id_lProcType = env->GetFieldID(class_CtdInCallProc, "lProcType", "J");
        CtdInCallProcPara.lProcType = env->GetLongField(obj_CtdInCallProc, id_lProcType);

        getStringField(env, obj_CtdInCallProc, class_CtdInCallProc, "chForwardPhone", CtdInCallProcPara.chForwardPhone,  sizeof(CtdInCallProcPara.chForwardPhone) - 1);

        jfieldID  id_iAnswerModal = env->GetFieldID(class_CtdInCallProc, "iAnswerModal", "I");
        CtdInCallProcPara.iAnswerModal = env->GetIntField(obj_CtdInCallProc, id_iAnswerModal);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller jni_bCtdCallMethod iCallId[%d], chPhone[%s], iMemberId[%d], lProcType[%d], chForwardPhone[%s] iAnswerModal[%d]",
                            CtdInCallProcPara.iCallId,
                            CtdInCallProcPara.chPhone,
                            CtdInCallProcPara.iMemberId,
                            CtdInCallProcPara.lProcType,
                            CtdInCallProcPara.chForwardPhone,
                            CtdInCallProcPara.iAnswerModal);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "jni_bCtdCallMethod iCallId[%d], chPhone[%s], iMemberId[%d], lProcType[%d], chForwardPhone[%s] iAnswerModal[%d]",
                 CtdInCallProcPara.iCallId,
                 CtdInCallProcPara.chPhone,
                 CtdInCallProcPara.iMemberId,
                 CtdInCallProcPara.lProcType,
                 CtdInCallProcPara.chForwardPhone,
                 CtdInCallProcPara.iAnswerModal);

        bReturn = bCtdCallMethod((long)iType, (short far*)&CtdInCallProcPara);
    }
    else
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller jni_bCtdCallMethod unkown iType[%d]", iType);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "jni_bCtdCallMethod unkown iType[%d]", iType);
    }
    return bReturn;
}
static void jni_bIMSSearchOneListInfo(JNIEnv* env, jclass cls, jstring pListURI)
{
    char cListURI[256 + 1] = {0};

    safeGetStringUTFChars(env, pListURI, cListURI, sizeof(cListURI) - 1, NULL);

    bIMSSearchOneListInfo(cListURI);

    return;
}
static jboolean jni_bIMSCreateOnePrivateGroup(JNIEnv* env, jclass cls, jstring jstr_pGroupName)
{
    char szGroupName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pGroupName, szGroupName, sizeof(szGroupName) - 1, NULL);

    return bIMSCreateOnePrivateGroup(szGroupName);
}
static jboolean jni_bIMSModifyPrivateGroupName(JNIEnv* env, jclass cls, jstring jstr_pOldName, jstring jstr_pNewName)
{
    char szNewName[64 + 1] = {0};
    char szOldName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pNewName, szNewName, sizeof(szNewName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pOldName, szOldName, sizeof(szOldName) - 1, NULL);

    return bIMSModifyPrivateGroupName(szOldName, szNewName);
}
static jboolean jni_bIMSDeleteOnePrivateGroup(JNIEnv* env, jclass cls, jstring jstr_pGroupName)
{
    char szGroupName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pGroupName, szGroupName, sizeof(szGroupName) - 1, NULL);

    return bIMSDeleteOnePrivateGroup(szGroupName);
}
static jboolean jni_bIMSGetPrivateGroupMember(JNIEnv* env, jclass cls, jstring jstr_pGroupName)
{
    char szGroupName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pGroupName, szGroupName, sizeof(szGroupName) - 1, NULL);

    return bIMSGetPrivateGroupMember(szGroupName);
}
static jboolean jni_bIMSCopyListToPrivateGroup(JNIEnv* env, jclass cls, jstring jstr_pListURI,
                                               jstring jstr_pGroupName, jlong iGroupType, jstring jstr_pDisplayName, jlong iSeq)
{
    char szListUrl[64 + 1] = {0};
    char szGroupName[64 + 1] = {0};
    char szDisplayName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pListURI, szListUrl, sizeof(szListUrl) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pGroupName, szGroupName, sizeof(szGroupName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pDisplayName, szDisplayName, sizeof(szDisplayName) - 1, NULL);

    return bIMSCopyListToPrivateGroup(szListUrl, szGroupName, (long)iGroupType, szDisplayName, (long)iSeq);
}
static jboolean jni_bIMSDeleteListFromPrivateGroup(JNIEnv* env, jclass cls, jstring jstr_pListURI, jstring jstr_pGroupName)
{
    char szUri[64 + 1] = {0};
    char szGroupName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pListURI, szUri, sizeof(szUri) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pGroupName, szGroupName, sizeof(szGroupName) - 1, NULL);

    return bIMSDeleteListFromPrivateGroup(szUri, szGroupName);
}
static jboolean jni_bIMSRuleSetMethod(JNIEnv* env, jclass cls, jint iType, jobject  RuleSetobject)
{
    BOOL bRetrun = FALSE;
    /* get the class */
    jclass class_RuleSetPara = env->GetObjectClass(RuleSetobject);

    RULE_SET_METHOD RuleSetInfoPara = {0};

    jfieldID  id_lOpeType = env->GetFieldID(class_RuleSetPara, "lType", "I");
    RuleSetInfoPara.lOpeType = env->GetIntField(RuleSetobject, id_lOpeType);

    getStringField(env, RuleSetobject, class_RuleSetPara, "cID", RuleSetInfoPara.cID,  sizeof(RuleSetInfoPara.cID) - 1);

    getStringField(env, RuleSetobject, class_RuleSetPara, "cURI", RuleSetInfoPara.cURI,  sizeof(RuleSetInfoPara.cURI) - 1);

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bIMSRuleSetMethod lOpeType[%d],cID[%s],cURI[%s]",
                        RuleSetInfoPara.lOpeType,
                        RuleSetInfoPara.cID,
                        RuleSetInfoPara.cURI);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bIMSRuleSetMethod lOpeType[%d],cID[%s],cURI[%s]",
             RuleSetInfoPara.lOpeType,
             RuleSetInfoPara.cID,
             RuleSetInfoPara.cURI);

    bRetrun = bIMSRuleSetMethod((long)iType, (short far*)&RuleSetInfoPara);
    return bRetrun;
}
static jboolean jni_bIMSCreateChatRoom(JNIEnv* env, jclass cls, jstring jstr_pDlgID, jstring jstr_pSubject, jstring jstr_pMemberURI)
{
    char szDlgIo[64 + 1] = {0};
    char szSubject[64 + 1] = {0};
    char szMemberUrl[1024 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pDlgID, szDlgIo, sizeof(szDlgIo) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pMemberURI, szMemberUrl, sizeof(szMemberUrl) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pSubject, szSubject, sizeof(szSubject) - 1, NULL);

    return bIMSCreateChatRoom(szDlgIo, szSubject, szMemberUrl);
}
static jboolean jni_bIMSControlChatRoomMember(JNIEnv* env, jclass cls, jstring jstr_pListURI, jstring jstr_pChatRoomURI, jlong iMethod)
{
    char szListURI[128 + 1] = {0};
    char szChatRoomURI[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pListURI, szListURI, sizeof(szListURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pChatRoomURI, szChatRoomURI, sizeof(szChatRoomURI) - 1, NULL);

    return bIMSControlChatRoomMember(szListURI, szChatRoomURI, (long)iMethod);
}
static jboolean jni_bIMSLeaveChatRoom(JNIEnv* env, jclass cls, jstring jstr_pChatRoomURI, jlong iMethod)
{
    char szChatRoomURI[64 + 1] = {0};
    safeGetStringUTFChars(env, jstr_pChatRoomURI, szChatRoomURI, sizeof(szChatRoomURI) - 1, NULL);

    return bIMSLeaveChatRoom(szChatRoomURI, (long)iMethod);
}
static jboolean jni_bIMSDoChatRoomInvite(JNIEnv* env, jclass cls, jlong iDoType, jstring jstr_pCreaterURI, jstring jstr_pChatRoomURI)
{
    char szCreateUri[64] = {0};
    char szChatRoomUri[128] = {0};

    safeGetStringUTFChars(env, jstr_pCreaterURI, szCreateUri, sizeof(szCreateUri), NULL);
    safeGetStringUTFChars(env, jstr_pChatRoomURI, szChatRoomUri, sizeof(szChatRoomUri), NULL);

    return bIMSDoChatRoomInvite((long)iDoType, szCreateUri, szChatRoomUri);
}
static jboolean jni_bIMSSearchUserByURI(JNIEnv* env, jclass cls,  jstring jstr_URI,  jlong iPage)
{
    char szURI[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_URI, szURI, sizeof(szURI) - 1, NULL);

    return bIMSSearchUserByURI(szURI, (long) iPage);
}
static jboolean jni_bIMSSearchUserByDName(JNIEnv* env, jclass cls,  jstring jstr_DName,  jlong iPage)
{
    char szName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_DName, szName, sizeof(szName) - 1, NULL);
    return bIMSSearchUserByDName(szName,  (long)iPage);
}
static jboolean jni_bIMSSearchUserOnline(JNIEnv* env, jclass cls, jlong iPage)
{
    return bIMSSearchUserOnline((long)iPage);
}
static jboolean jni_bIMSSearchUserByPhone(JNIEnv* env, jclass cls, jstring jstr_Phone,  jlong iPage)
{
    char szPhone[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_Phone, szPhone, sizeof(szPhone) - 1, NULL);
    return bIMSSearchUserByPhone(szPhone, (long)iPage);
}
static jboolean jni_bIMSSearchUserByEmail(JNIEnv* env, jclass cls, jstring jstr_Email,  jlong iPage)
{
    char szEmail[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_Email, szEmail, sizeof(szEmail) - 1, NULL);
    return bIMSSearchUserByEmail(szEmail, (long)iPage);
}
static jboolean jni_bIMSAddOneAddressList(JNIEnv* env, jclass cls, jstring jstr_URI, jstring jstr_DisplayName, jstring jstr_Info)
{
    char szURI[256 + 1] = {0};
    char szDisplayName[64 + 1] = {0};
    char szInfo[127 + 1] = {0};

    safeGetStringUTFChars(env, jstr_URI, szURI, sizeof(szURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_DisplayName, szDisplayName, sizeof(szDisplayName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_Info, szInfo, sizeof(szInfo) - 1, NULL);
    return bIMSAddOneAddressList(szURI,  szDisplayName, szInfo);
}
static jboolean jni_bIMSDeleteOneAddressList(JNIEnv* env, jclass cls,  jstring jstr_URI)
{
    char szURI[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_URI, szURI, sizeof(szURI) - 1, NULL);
    return bIMSDeleteOneAddressList(szURI);
}
//add by zhanghe 2011-11-15 ���������ı�����Ϣ��Ӧ��
static jboolean jni_bIMSDoWatcherSubSeq(JNIEnv* env, jclass cls, jstring jstr_WatcherURI, jlong iAccept)
{
    char szWatcherURI[256 + 1] = {0};

    safeGetStringUTFChars(env, jstr_WatcherURI, szWatcherURI, sizeof(szWatcherURI) - 1, NULL);
    return bIMSDoWatcherSubSeq(szWatcherURI, iAccept);
}
static jboolean jni_bIMSAddRLSMember(JNIEnv* env, jclass cls, jstring pURI, jstring pListName, jstring pListMemberURI, jstring pInfo)
{
    char szURI[64 + 1] = {0};
    char szListName[64 + 1] = {0};
    char szListMemberURI[64 + 1] = {0};
    char szInfo[128 + 1] = {0};
    safeGetStringUTFChars(env, pURI, szURI, sizeof(szURI) - 1, NULL);
    safeGetStringUTFChars(env, pListName, szListName, sizeof(szListName) - 1, NULL);
    safeGetStringUTFChars(env, pListMemberURI, szListMemberURI, sizeof(szListMemberURI) - 1, NULL);
    safeGetStringUTFChars(env, pInfo, szInfo, sizeof(szInfo) - 1, NULL);

    return bIMSAddRLSMember(szURI, szListName, szListMemberURI, szInfo);
}
static jboolean jni_bIMSDeleteRLSMember(JNIEnv* env, jclass cls, jstring pURI,  jstring pListMemberURI)
{
    char szURI[64 + 1] = {0};
    char szListMemberURI[64 + 1] = {0};

    safeGetStringUTFChars(env, pURI, szURI, sizeof(szURI) - 1, NULL);

    safeGetStringUTFChars(env, pListMemberURI, szListMemberURI, sizeof(szListMemberURI) - 1, NULL);

    return bIMSDeleteRLSMember(szURI,  szListMemberURI);
}
static jboolean jni_bIMSSearchAddrList(JNIEnv* env, jclass cls, jlong iType, jobject pSearch, jlong iPage)
{
    jclass class_SerachDepartPara = env->GetObjectClass(pSearch);

    if(iType == 3)
    {
        ECP_DEPT_SEARCH_ADV_METHOD searcAdv = {0};

        jfieldID id_fireEcpDeptSearchAdv = env->GetFieldID(class_SerachDepartPara, "fireEcpDeptSearchAdvMethod", "Lcom/zte/softda/ocx/FireEcpDeptSearchAdvMethod;");
        jobject obj_fireEcpDeptSearchAdv = env->GetObjectField(pSearch, id_fireEcpDeptSearchAdv);
        jclass class_fireEcpDeptSearchAdv = env->GetObjectClass(obj_fireEcpDeptSearchAdv);

        getStringField(env, obj_fireEcpDeptSearchAdv, class_fireEcpDeptSearchAdv, "cName",	searcAdv.cName,	  sizeof(searcAdv.cName) - 1);
        getStringField(env, obj_fireEcpDeptSearchAdv, class_fireEcpDeptSearchAdv, "cURI",	searcAdv.cURI,	  sizeof(searcAdv.cURI) - 1);
        getStringField(env, obj_fireEcpDeptSearchAdv, class_fireEcpDeptSearchAdv, "cSpell", searcAdv.cSpell,  sizeof(searcAdv.cSpell) - 1);
        getStringField(env, obj_fireEcpDeptSearchAdv, class_fireEcpDeptSearchAdv, "cPhone", searcAdv.cPhone,  sizeof(searcAdv.cPhone) - 1);

        return bIMSSearchAddrList((long)iType, (short far*)&searcAdv, (long)iPage);
    }
    else
    {
        DEPT_SEARCH_ADV_METHOD searcAdv = {0};

        jfieldID id_fireDeptSearchAdv = env->GetFieldID(class_SerachDepartPara, "fireDeptSearchAdvMethod", "Lcom/zte/softda/ocx/FireDeptSearchAdvMethod;");
        jobject obj_fireDeptSearchAdv = env->GetObjectField(pSearch, id_fireDeptSearchAdv);
        jclass class_fireDeptSearchAdv = env->GetObjectClass(obj_fireDeptSearchAdv);

        getStringField(env, obj_fireDeptSearchAdv, class_fireDeptSearchAdv, "cDepartmentName", searcAdv.cDepartmentName, sizeof(searcAdv.cDepartmentName) - 1 );
        getStringField(env, obj_fireDeptSearchAdv, class_fireDeptSearchAdv, "cEmployeeNum",	   searcAdv.cEmployeeNum,	 sizeof(searcAdv.cEmployeeNum) - 1);

        getStringField(env, obj_fireDeptSearchAdv, class_fireDeptSearchAdv, "cName",	 searcAdv.cName,	  sizeof(searcAdv.cName) - 1);
        getStringField(env, obj_fireDeptSearchAdv, class_fireDeptSearchAdv, "cEmail",    searcAdv.cEmail,	  sizeof(searcAdv.cEmail) - 1);
        getStringField(env, obj_fireDeptSearchAdv, class_fireDeptSearchAdv, "cMobile",   searcAdv.cMobile,	  sizeof(searcAdv.cMobile) - 1);
        getStringField(env, obj_fireDeptSearchAdv, class_fireDeptSearchAdv, "cUniPhone", searcAdv.cUniPhone,  sizeof(searcAdv.cUniPhone) - 1);
	 getStringField(env, obj_fireDeptSearchAdv, class_fireDeptSearchAdv, "cCompanyURI", searcAdv.cCompanyURI,  sizeof(searcAdv.cCompanyURI) - 1);

        return bIMSSearchAddrList((long)iType, (short far*)&searcAdv, (long)iPage);
    }
}
static jboolean jni_bIMSLoadAddrList(JNIEnv* env, jclass cls, jint iType, jobject PersonListInfoList, jint iNum, jint iPage)
{
    BOOL bRetrun = FALSE;
    int i, size = 0;
    PERSON_LIST_INFO *pPersonListInfoPara = NULL;
    PERSON_LIST_INFO *pPersonInfoTemp = NULL;
    jmethodID getMethodID;
    jclass class_PersonInfoArrayList;
    jmethodID sizeMethodID;

    if(iType == 0)
    {
        class_PersonInfoArrayList = env->GetObjectClass(PersonListInfoList);
        getMethodID = (env)->GetMethodID(class_PersonInfoArrayList, "get", "(I)Ljava/lang/Object;");
        sizeMethodID = (env)->GetMethodID(class_PersonInfoArrayList, "size", "()I");
        size = (env)->CallIntMethod(PersonListInfoList, sizeMethodID);//��sizeӦ�ú�iNumһ��
    }
    if(size > 0)//���size��iNumֵ��һ������Ӧ����sizeΪ׼
    {
        pPersonListInfoPara = (PERSON_LIST_INFO *)ZX_malloc(sizeof(PERSON_LIST_INFO) * size);
        if(pPersonListInfoPara == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller jni_bIMSLoadAddrList ZX_malloc pPersonListInfoPara failed!");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "jni_bIMSLoadAddrList ZX_malloc pPersonListInfoPara failed!");

            return false;
        }
        memset(pPersonListInfoPara, 0, sizeof(PERSON_LIST_INFO)*size);
    }
    for(i = 0; i < size; i++)
    {
        pPersonInfoTemp = NULL;
        pPersonInfoTemp = &pPersonListInfoPara[i];
        if(pPersonInfoTemp == NULL)
        {
            if(g_WriteLogType == 2)
                TraceMsgWindow1(1, "rcscontroller jni_bIMSLoadAddrList pPersonInfoTemp == NULL");
            else if(g_WriteLogType == 1)
                LOGI(TAG, "jni_bIMSLoadAddrList pPersonInfoTemp == NULL");

            return false;
        }
        jobject jobj_PersonInfo = (jobject)(env)->CallObjectMethod(PersonListInfoList, getMethodID, i);
        jclass class_PersonInfo = env->GetObjectClass(jobj_PersonInfo);

        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cName", pPersonInfoTemp->cName,  sizeof(pPersonInfoTemp->cName) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cURI", pPersonInfoTemp->cURI,  sizeof(pPersonInfoTemp->cURI) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cURI2", pPersonInfoTemp->cURI2,  sizeof(pPersonInfoTemp->cURI2) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cOfficeTel", pPersonInfoTemp->cOfficeTel,  sizeof(pPersonInfoTemp->cOfficeTel) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cMobileTel", pPersonInfoTemp->cMobileTel,  sizeof(pPersonInfoTemp->cMobileTel) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cFectionNum", pPersonInfoTemp->cFectionNum,  sizeof(pPersonInfoTemp->cFectionNum) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cFaxNum", pPersonInfoTemp->cFaxNum,  sizeof(pPersonInfoTemp->cFaxNum) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cAutoTel", pPersonInfoTemp->cAutoTel,  sizeof(pPersonInfoTemp->cAutoTel) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cHomeTel", pPersonInfoTemp->cHomeTel,  sizeof(pPersonInfoTemp->cHomeTel) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cSecondOfficeTel", pPersonInfoTemp->cSecondOfficeTel,  sizeof(pPersonInfoTemp->cSecondOfficeTel) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cSecondMobileTel", pPersonInfoTemp->cSecondMobileTel,  sizeof(pPersonInfoTemp->cSecondMobileTel) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cOfficeAddress", pPersonInfoTemp->cOfficeAddress,  sizeof(pPersonInfoTemp->cOfficeAddress) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cOtherAddress", pPersonInfoTemp->cOtherAddress,  sizeof(pPersonInfoTemp->cOtherAddress) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cEmailAddress", pPersonInfoTemp->cEmailAddress,  sizeof(pPersonInfoTemp->cEmailAddress) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cAffiliation", pPersonInfoTemp->cAffiliation,  sizeof(pPersonInfoTemp->cAffiliation) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cSex", pPersonInfoTemp->cSex,  sizeof(pPersonInfoTemp->cSex) - 1);

        jfieldID  id_iOld = env->GetFieldID(class_PersonInfo, "iOld", "I");
        pPersonInfoTemp->iOld = env->GetIntField(jobj_PersonInfo, id_iOld);

        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cPhotoURL", pPersonInfoTemp->cPhotoURL,  sizeof(pPersonInfoTemp->cPhotoURL) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cPosition", pPersonInfoTemp->cPosition,  sizeof(pPersonInfoTemp->cPosition) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cEmployeeNum", pPersonInfoTemp->cEmployeeNum,  sizeof(pPersonInfoTemp->cEmployeeNum) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cPersonalDescription", pPersonInfoTemp->cPersonalDescription,  sizeof(pPersonInfoTemp->cPersonalDescription) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cGroupName", pPersonInfoTemp->cGroupName,  sizeof(pPersonInfoTemp->cGroupName) - 1);

        jfieldID  id_iCategory = env->GetFieldID(class_PersonInfo, "iCategory", "I");
        pPersonInfoTemp->iCategory = env->GetIntField(jobj_PersonInfo, id_iCategory);

        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cEtag0", pPersonInfoTemp->cEtag0,  sizeof(pPersonInfoTemp->cEtag0) - 1);
        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cEtag", pPersonInfoTemp->cEtag,  sizeof(pPersonInfoTemp->cEtag) - 1);

        jfieldID  id_bNeedSynchronize = env->GetFieldID(class_PersonInfo, "bNeedSynchronize", "I");
        pPersonInfoTemp->bNeedSynchronize = env->GetIntField(jobj_PersonInfo, id_bNeedSynchronize);

        getStringField(env, jobj_PersonInfo, class_PersonInfo, "cWorkAffiliation", pPersonInfoTemp->cWorkAffiliation,  sizeof(pPersonInfoTemp->cWorkAffiliation) - 1);
    }

    bRetrun = bIMSLoadAddrList((long)iType, (short far*)pPersonListInfoPara, (long)iNum, (long)iPage);

    if(pPersonListInfoPara)
        ZX_free((UINT8*)pPersonListInfoPara);
    return bRetrun;
}
static jboolean jni_bIMSDoPublicGroupJoinReq(JNIEnv* env, jclass cls,
                                             jstring jstr_pReqMemberName, jstring jstr_pReqURI, jlong iResult, jstring jstr_pReturn1, jstring jstr_pReturn2)
{
    char cReqMemberName[256 + 1] = {0}; //�����˵�URI����ʽΪ��sip:�ʺ�@�������64�ֽ�
    char cReqURI[256 + 1] = {0}; //�����˵�URI����ʽΪ��sip:�ʺ�@�������64�ֽ�
    char cReturn1[256 + 1] = {0}; //���¼�IMSReqJoinMyPublicGroup�еõ����ַ���, ʵ��Ϊ ����Ⱥ���URI
    char cReturn2[128 + 1] = {0}; //���¼�IMSReqJoinMyPublicGroup�еõ����ַ���, ʵ��Ϊ ����Ⱥ���ĵ���URL

    safeGetStringUTFChars(env, jstr_pReqMemberName, cReqMemberName, sizeof(cReqMemberName), NULL);
    safeGetStringUTFChars(env, jstr_pReqURI, cReqURI, sizeof(cReqURI), NULL);
    safeGetStringUTFChars(env, jstr_pReturn1, cReturn1, sizeof(cReturn1), NULL);
    safeGetStringUTFChars(env, jstr_pReturn2, cReturn2, sizeof(cReturn2), NULL);

    return bIMSDoPublicGroupJoinReq(cReqMemberName, cReqURI, iResult, cReturn1, cReturn2);
}
static jboolean jni_bIMSModifyOneListDpName(JNIEnv* env, jclass cls, jstring jstr_pURI, jstring jstr_pDisplayName)
{
    char cURI[64 + 1] = {0};
    char cDisplayName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pURI, cURI, sizeof(cURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pDisplayName, cDisplayName, sizeof(cDisplayName) - 1, NULL);

    return bIMSModifyOneListDpName(cURI, cDisplayName);
}
//����ӿ�
static jboolean jni_bIMSConfMethod(JNIEnv* env, jclass cls, jlong iType,  jobject  jobj_bIMSConfMethodObject)
{
    int i = 0;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bIMSConfMethod begin iType = %d", (int)iType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bIMSConfMethod begin iType = %d", (int)iType);

    jclass class_bIMSConfMethodPara = env->GetObjectClass(jobj_bIMSConfMethodObject);

    //��������ʹ���ԤԼ�����������ģ��
    if (GT_METHOD_CONF_CONTROL == iType)
    {

        CONF_CONTROL_METHOD ConfControlMethod = {0};
        jfieldID id_ConfControlMethod = env->GetFieldID(class_bIMSConfMethodPara, "fireConfControlPara", "Lcom/zte/softda/ocx/FireConfControlPara;");
        jobject obj_ConfControlMethod = env->GetObjectField(jobj_bIMSConfMethodObject, id_ConfControlMethod);
        jclass class_ConfControlMethod = env->GetObjectClass(obj_ConfControlMethod);

        jfieldID  id_iType = env->GetFieldID(class_ConfControlMethod, "iType", "I");
        ConfControlMethod.iType = env->GetIntField(obj_ConfControlMethod, id_iType);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cConfURI", ConfControlMethod.cConfURI,  sizeof(ConfControlMethod.cConfURI) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cSubject", ConfControlMethod.cSubject,  sizeof(ConfControlMethod.cSubject) - 1);

        jfieldID  id_iConfType = env->GetFieldID(class_ConfControlMethod, "iConfType", "I");
        ConfControlMethod.iConfType = env->GetIntField (obj_ConfControlMethod, id_iConfType);
        jfieldID  id_iInitMemberNum = env->GetFieldID(class_ConfControlMethod, "iInitMemberNum", "I");
        ConfControlMethod.iInitMemberNum = env->GetIntField(obj_ConfControlMethod, id_iInitMemberNum);

        jfieldID id_pInitMemberListArray = env->GetFieldID(class_ConfControlMethod, "pInitMemberList", "[Ljava/lang/String;");

        jobjectArray pInitMemberListArray = (jobjectArray)env->GetObjectField(obj_ConfControlMethod, id_pInitMemberListArray);
        if (pInitMemberListArray != NULL)
        {
            int i;

            for (i = 0; i < ConfControlMethod.iInitMemberNum; i++)
            {
                jstring jstr_pInitMemberListArray = (jstring)env->GetObjectArrayElement(pInitMemberListArray, i);
                if (jstr_pInitMemberListArray != NULL)
                {
                    int size = env->GetStringLength(jstr_pInitMemberListArray);

                    ConfControlMethod.pInitMemberList[i] = (char*)ZX_malloc(size + 1);
                    memset(ConfControlMethod.pInitMemberList[i], 0, size + 1);
                    safeGetStringUTFChars(env, jstr_pInitMemberListArray, ConfControlMethod.pInitMemberList[i], size, NULL);
                }
            }
        }

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMemberURI", ConfControlMethod.cMemberURI,  sizeof(ConfControlMethod.cMemberURI) - 1);

        jfieldID  id_iDoType = env->GetFieldID(class_ConfControlMethod, "iDoType", "I");
        ConfControlMethod.iDoType = env->GetIntField(obj_ConfControlMethod, id_iDoType);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cConfPassword", ConfControlMethod.cConfPassword,  sizeof(ConfControlMethod.cConfPassword) - 1);

        jfieldID  id_iSMSNum = env->GetFieldID(class_ConfControlMethod, "iSMSNum", "I");
        ConfControlMethod.iSMSNum = env->GetIntField(obj_ConfControlMethod, id_iSMSNum);

        jfieldID id_pSMSListArray = env->GetFieldID(class_ConfControlMethod, "pSMSList", "[Ljava/lang/String;");
        jobjectArray pSMSListArray = (jobjectArray)env->GetObjectField(obj_ConfControlMethod, id_pSMSListArray);
        if (pSMSListArray != NULL)
        {
            int i;

            for (i = 0; i < ConfControlMethod.iSMSNum; i++)
            {
                jstring jstr_pSMSListArray = (jstring)env->GetObjectArrayElement(pSMSListArray, i);
                if (jstr_pSMSListArray != NULL)
                {
                    int size = env->GetStringLength(jstr_pSMSListArray);

                    ConfControlMethod.pSMSList[i] = (char*)ZX_malloc(size + 1);
                    memset(ConfControlMethod.pSMSList[i], 0, size + 1);
                    safeGetStringUTFChars(env, jstr_pSMSListArray, ConfControlMethod.pSMSList[i], size, NULL);
                }
            }
        }

        jfieldID  id_pSMSMasterListArray = env->GetFieldID(class_ConfControlMethod, "pSMSMasterList", "[I");
        jintArray pSMSMasterListArray = (jintArray)env->GetObjectField(obj_ConfControlMethod, id_pSMSMasterListArray);

        if (pSMSMasterListArray != NULL)
        {
            jint *jpSMSMasterListArray = env->GetIntArrayElements(pSMSMasterListArray, 0);
            for (int i = 0; i < ConfControlMethod.iSMSNum; i++)
            {

                ConfControlMethod.pSMSMasterList[i] = jpSMSMasterListArray[i];

                //if(g_WriteLogType == 2)
                //	TraceMsgWindow1(1, "rcscontroller jni_bIMSConfMethod GT_METHOD_CONF_CONTROL pSMSMasterList = %d", ConfControlMethod.pSMSMasterList[i]);
                //else if(g_WriteLogType == 1)
                //LOGI(TAG, "GT_METHOD_CONF_CONTROL pSMSMasterList = %d", ConfControlMethod.pSMSMasterList[i]);
            }
        }

        jfieldID  id_iEmailNum = env->GetFieldID(class_ConfControlMethod, "iEmailNum", "I");
        ConfControlMethod.iEmailNum = env->GetIntField(obj_ConfControlMethod, id_iEmailNum);


        jfieldID id_pEmailListArray = env->GetFieldID(class_ConfControlMethod, "pEmailList", "[Ljava/lang/String;");
        jobjectArray pEmailListArray = (jobjectArray)env->GetObjectField(obj_ConfControlMethod, id_pEmailListArray);
        if (pEmailListArray != NULL)
        {
            int i;

            for (i = 0; i < ConfControlMethod.iEmailNum; i++)
            {
                jstring jobj_pEmailListArray = (jstring)env->GetObjectArrayElement(pEmailListArray, i);
                if (jobj_pEmailListArray != NULL)
                {
                    int size = env->GetStringLength(jobj_pEmailListArray);

                    ConfControlMethod.pEmailList[i] = (char*)ZX_malloc(size + 1);
                    memset(ConfControlMethod.pEmailList[i], 0, size + 1);
                    safeGetStringUTFChars(env, jobj_pEmailListArray, ConfControlMethod.pEmailList[i], size, NULL);
                }
            }
        }

        jfieldID  id_pEmailMasterListArray = env->GetFieldID(class_ConfControlMethod, "pEmailMasterList", "[I");
        jintArray pEmailMasterListArray = (jintArray)env->GetObjectField(obj_ConfControlMethod, id_pEmailMasterListArray);
        if (pSMSMasterListArray != NULL)
        {
            jint *jpEmailMasterListArray = env->GetIntArrayElements(pSMSMasterListArray, 0);

            for (int i = 0; i < ConfControlMethod.iEmailNum; i++)
            {
                ConfControlMethod.pSMSMasterList[i] = jpEmailMasterListArray[i];
            }
        }


        jfieldID  id_iMaxNum = env->GetFieldID(class_ConfControlMethod, "iMaxNum", "I");
        ConfControlMethod.iMaxNum = env->GetIntField(obj_ConfControlMethod, id_iMaxNum);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cStartTime", ConfControlMethod.cStartTime,  sizeof(ConfControlMethod.cStartTime) - 1);

        jfieldID  id_iPeriod = env->GetFieldID(class_ConfControlMethod, "iPeriod", "I");
        ConfControlMethod.iPeriod = env->GetIntField(obj_ConfControlMethod, id_iPeriod);
        jfieldID  id_iLimiteTime = env->GetFieldID(class_ConfControlMethod, "iLimiteTime", "I");
        ConfControlMethod.iLimiteTime = env->GetIntField(obj_ConfControlMethod, id_iLimiteTime);
        jfieldID  id_iVideoNum = env->GetFieldID(class_ConfControlMethod, "iVideoNum", "I");
        ConfControlMethod.iVideoNum = env->GetIntField(obj_ConfControlMethod, id_iVideoNum);
        jfieldID  id_iLayout = env->GetFieldID(class_ConfControlMethod, "iLayout", "I");
        ConfControlMethod.iLayout = env->GetIntField(obj_ConfControlMethod, id_iLayout);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMasterURI", ConfControlMethod.cMasterURI,  sizeof(ConfControlMethod.cMasterURI) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMasterPass", ConfControlMethod.cMasterPass,  sizeof(ConfControlMethod.cMasterPass) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMemberPass", ConfControlMethod.cMemberPass,  sizeof(ConfControlMethod.cMemberPass) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cVisitorPass", ConfControlMethod.cVisitorPass,  sizeof(ConfControlMethod.cVisitorPass) - 1);

        jfieldID  id_iOrigType = env->GetFieldID(class_ConfControlMethod, "iOrigType", "I");
        ConfControlMethod.iOrigType = env->GetIntField(obj_ConfControlMethod, id_iOrigType);
        jfieldID  id_iDestType = env->GetFieldID(class_ConfControlMethod, "iDestType", "I");
        ConfControlMethod.iDestType = env->GetIntField(obj_ConfControlMethod, id_iDestType);
        jfieldID  id_iUserID = env->GetFieldID(class_ConfControlMethod, "iUserID", "I");
        ConfControlMethod.iUserID = env->GetIntField(obj_ConfControlMethod, id_iUserID);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cDigest", ConfControlMethod.cDigest,  sizeof(ConfControlMethod.cDigest) - 1);

        jfieldID  id_iReleaseMode = env->GetFieldID(class_ConfControlMethod, "iReleaseMode", "I");
        ConfControlMethod.iReleaseMode = env->GetIntField(obj_ConfControlMethod, id_iReleaseMode);

        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller GT_METHOD_CONF_CONTROL	cSubject[%s]*iConfType[%d]*cMasterURI[%s]  [iInitMemberNum= %d] [iEmailNum = %d] [iSMSNum = %d], [cStartTime = %s]",
                            ConfControlMethod.cSubject, ConfControlMethod.iConfType, ConfControlMethod.cMasterURI, ConfControlMethod.iInitMemberNum, ConfControlMethod.iEmailNum, ConfControlMethod.iSMSNum, ConfControlMethod.cStartTime);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "GT_METHOD_CONF_CONTROL    cSubject[%s]*iConfType[%d]*cMasterURI[%s]  [iInitMemberNum= %d] [iEmailNum = %d] [iSMSNum = %d], [cStartTime = %s]",
                 ConfControlMethod.cSubject, ConfControlMethod.iConfType, ConfControlMethod.cMasterURI, ConfControlMethod.iInitMemberNum, ConfControlMethod.iEmailNum, ConfControlMethod.iSMSNum, ConfControlMethod.cStartTime);

        BOOL iResult = bIMSConfMethod(iType, (short far*)&ConfControlMethod);

        return iResult;
    }
    //��ȡ������ʱ��
    if (GT_METHOD_ASSISTTANT_CONTROL == iType)
    {
        CONF_ASSISTTANT_CONTROL_METHOD ConfAssisttantControlMethod = {0};
        jfieldID id_GetMethodAssisttantControl = env->GetFieldID(class_bIMSConfMethodPara, "fireConfAssistantCtr", "Lcom/zte/softda/ocx/FireConfAssistantCtr;");
        jobject obj_GetMethodAssisttantControl = env->GetObjectField(jobj_bIMSConfMethodObject, id_GetMethodAssisttantControl);
        jclass class_GetMethodAssisttantControl = env->GetObjectClass(obj_GetMethodAssisttantControl);

        jfieldID  id_iOperateType = env->GetFieldID(class_GetMethodAssisttantControl, "iOperateType", "I");
        ConfAssisttantControlMethod.iOperateType = env->GetIntField(obj_GetMethodAssisttantControl, id_iOperateType);

        return bIMSConfMethod(iType, (short far*)&ConfAssisttantControlMethod);
    }
    //�����ѯ�����¼
    if (GT_METHOD_QUERY_CONF == iType)
    {
        QUERY_CONF_STRUCT QueryConfStruct = {0};

        jfieldID id_GetMethodQueryConf = env->GetFieldID(class_bIMSConfMethodPara, "fireQueryConfStruct", "Lcom/zte/softda/ocx/FireQueryConfStruct;");
        jobject obj_GetMethodQueryConf = env->GetObjectField(jobj_bIMSConfMethodObject, id_GetMethodQueryConf);
        jclass class_GetMethodQueryConf = env->GetObjectClass(obj_GetMethodQueryConf);

        getStringField(env, obj_GetMethodQueryConf, class_GetMethodQueryConf, "cConfURI", QueryConfStruct.cConfURI,  sizeof(QueryConfStruct.cConfURI) - 1);

        jfieldID  id_iMediaType = env->GetFieldID(class_GetMethodQueryConf, "iMediaType", "I");
        QueryConfStruct.iMediaType = env->GetIntField(obj_GetMethodQueryConf, id_iMediaType);
        jfieldID  id_iConfType = env->GetFieldID(class_GetMethodQueryConf, "iConfType", "I");
        QueryConfStruct.iConfType = env->GetIntField(obj_GetMethodQueryConf, id_iConfType);

        getStringField(env, obj_GetMethodQueryConf, class_GetMethodQueryConf, "cStartTime", QueryConfStruct.cStartTime,  sizeof(QueryConfStruct.cStartTime) - 1);
        getStringField(env, obj_GetMethodQueryConf, class_GetMethodQueryConf, "cEndTime", QueryConfStruct.cEndTime,  sizeof(QueryConfStruct.cEndTime) - 1);

        jfieldID  id_iStatus = env->GetFieldID(class_GetMethodQueryConf, "iStatus", "I");
        QueryConfStruct.iStatus = env->GetIntField(obj_GetMethodQueryConf, id_iStatus);

        return bIMSConfMethod(iType, (short far*)&QueryConfStruct);
    }
    //ȡ��ԤԼ����
    if (ST_METHOD_CANCEL_CONF == iType)
    {
        char QueryCancel[MAX_IMS_MUL_CONf_URI_LEN + 1] = "";
        getStringField(env, jobj_bIMSConfMethodObject, class_bIMSConfMethodPara, "cConfURI", QueryCancel, sizeof(QueryCancel) - 1);

        return bIMSConfMethod(iType, (short far*)&QueryCancel);
    }

    if (ST_METHOD_DO_ORDER_CONF_NOTITY == iType)
    {
        DO_ORDER_CONF_NOTIFY_STRUCT DoOrderNotify = {0};
        jfieldID id_DoOrderConfNotity = env->GetFieldID(class_bIMSConfMethodPara, "fireDoOrderConfNotifyStruct", "Lcom/zte/softda/ocx/FireDoOrderConfNotifyStruct;");
        jobject obj_DoOrderConfNotity = env->GetObjectField(jobj_bIMSConfMethodObject, id_DoOrderConfNotity);
        jclass class_DoOrderConfNotity = env->GetObjectClass(obj_DoOrderConfNotity);

        jfieldID  id_iDoType = env->GetFieldID(class_DoOrderConfNotity, "iDoType", "I");
        DoOrderNotify.iDoType = env->GetIntField(obj_DoOrderConfNotity, id_iDoType);
        jfieldID  id_iCallID = env->GetFieldID(class_DoOrderConfNotity, "iCallID", "I");
        DoOrderNotify.iCallID = env->GetIntField(obj_DoOrderConfNotity, id_iCallID);

        getStringField(env, obj_DoOrderConfNotity, class_DoOrderConfNotity, "cConfURI", DoOrderNotify.cConfURI,  sizeof(DoOrderNotify.cConfURI) - 1);

        return bIMSConfMethod(iType, (short far*)&DoOrderNotify);
    }
    //��ȡ�����Աi��Ϣ
    if (GT_METHOD_ORDERCONF_MEMBER_INFO == iType)
    {
        char ConfUri[MAX_IMS_MUL_CONf_URI_LEN + 1] = "";
        getStringField(env, jobj_bIMSConfMethodObject, class_bIMSConfMethodPara, "cConfURI", ConfUri, sizeof(ConfUri) - 1);

        return bIMSConfMethod(iType, (short far*)&ConfUri);
    }
}
static jboolean jni_bIMSReqJoinPublicGroup(JNIEnv* env, jclass cls, jstring jstr_pGroupURI, jstring jstr_pGroupIdentifier, 
    jstring jstr_pDisplayName, jstring jstr_pShareUserURI, jstring jstr_pShareUserName)
{
    char GroupURI[64 + 1] = {0};
    char GroupIdentifier[128 + 1] = {0};
    char cDisplayName[64 + 1] = {0};
    char cShareUserURI[64+1] = {0};//��ά�������sip�˺�
	char cShareUserName[128+1] = {0};//��ά�����������

    safeGetStringUTFChars(env, jstr_pGroupURI, GroupURI, sizeof(GroupURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pGroupIdentifier, GroupIdentifier, sizeof(GroupIdentifier) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pDisplayName, cDisplayName, sizeof(cDisplayName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pShareUserURI, cShareUserURI, sizeof(cShareUserURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_pShareUserName, cShareUserName, sizeof(cShareUserName) - 1, NULL);

    return bIMSReqJoinPublicGroup(GroupURI, GroupIdentifier, cDisplayName, cShareUserURI, cShareUserName);
}
static jboolean jni_bIMSLeavePublicGroup(JNIEnv* env, jclass cls, jstring jstr_pGroupName, jstring jstr_GroupURI)
{
    char GroupURI[64 + 1] = {0};
    char GroupName[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_pGroupName, GroupName, sizeof(GroupName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_GroupURI, GroupURI, sizeof(GroupURI) - 1, NULL);

    return bIMSLeavePublicGroup(GroupName, GroupURI);
}
static jboolean jni_bIMSDeleteOnePublicGroup(JNIEnv* env, jclass cls, jstring jstr_GroupURI)
{
    char GroupURI[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_GroupURI, GroupURI, sizeof(GroupURI) - 1, NULL);

    return bIMSDeleteOnePublicGroup(GroupURI);
}
static jboolean jni_bIMSDeleteOneFromMyPublicGroup(JNIEnv* env, jclass cls, jstring jstr_SomeoneURI, jstring jstr_GroupURI)
{
    char GroupURI[64 + 1] = {0};
    char SomeoneURI[64 + 1] = {0};

    safeGetStringUTFChars(env, jstr_SomeoneURI, SomeoneURI, sizeof(SomeoneURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_GroupURI, GroupURI, sizeof(GroupURI) - 1, NULL);

    return bIMSDeleteOneFromMyPublicGroup(SomeoneURI, GroupURI);
}

//iType == 11Ⱥ����ʷ��Ϣ��δ����Ϣ��ѯ
static jboolean jni_bIMSDispUICmdMethod_11(JNIEnv* env, jclass cls, jint nMethodType, jstring cGroupURI, jstring timestamp, jint maxNum)
{
    GROUP_OFFLINE_MSG_METHOD_PARAM GroupOfflineMsgMethodPara = {0};
    GroupOfflineMsgMethodPara.nMethodType = nMethodType;   
    safeGetStringUTFChars(env, cGroupURI, GroupOfflineMsgMethodPara.cURI, sizeof(GroupOfflineMsgMethodPara.cURI) - 1, NULL);
    safeGetStringUTFChars(env, timestamp, GroupOfflineMsgMethodPara.timestamp, sizeof(GroupOfflineMsgMethodPara.timestamp) - 1, NULL);
    GroupOfflineMsgMethodPara.maxNum = maxNum; 
    return bIMSDispUICmdMethod(11, (short far*)&GroupOfflineMsgMethodPara);   
}

static jboolean jni_bIMSDispUICmdMethod(JNIEnv* env, jclass cls, jint iType, jobject CmdParaobject)
{
    BOOL bReturn = FALSE;
    jclass class_CmdParaobject = NULL;

    /* get the class */
    if(iType != 5)
        class_CmdParaobject = env->GetObjectClass(CmdParaobject);

    if(iType == 10)//my position
    {
        DEPT_FAVORITE_METHOD_PARAM DeptFavMethodPara = {0};

        jfieldID id_DeptFavMethodPara = env->GetFieldID(class_CmdParaobject, "DeptFavMethodPara", "Lcom/zte/softda/ocx/DEPT_FAVORITE_METHOD_PARAM;");
        jobject obj_DeptFavMethodPara = env->GetObjectField(CmdParaobject, id_DeptFavMethodPara);
        jclass class_DeptFavMethodPara = env->GetObjectClass(obj_DeptFavMethodPara);

        jfieldID  id_nMethodType = env->GetFieldID(class_DeptFavMethodPara, "nMethodType", "I");
        DeptFavMethodPara.nMethodType = env->GetIntField(obj_DeptFavMethodPara, id_nMethodType);

        getStringField(env, obj_DeptFavMethodPara, class_DeptFavMethodPara, "cName", DeptFavMethodPara.cName,  sizeof(DeptFavMethodPara.cName) - 1);
        getStringField(env, obj_DeptFavMethodPara, class_DeptFavMethodPara, "cURI", DeptFavMethodPara.cURI,  sizeof(DeptFavMethodPara.cURI) - 1);

        jfieldID  id_nType = env->GetFieldID(class_DeptFavMethodPara, "nType", "I");
        DeptFavMethodPara.nType = env->GetIntField(obj_DeptFavMethodPara, id_nType);

        jfieldID  id_nAutoExpand = env->GetFieldID(class_DeptFavMethodPara, "nAutoExpand", "I");
        DeptFavMethodPara.nAutoExpand = env->GetIntField(obj_DeptFavMethodPara, id_nAutoExpand);

        bReturn = bIMSDispUICmdMethod((long)iType, (short far*)&DeptFavMethodPara);
    }
    else if(iType == 18)
    {
        GROUP_QUERY_CLASS_METHOD_PARAM GroupQueryClassMethodPara = {0};

        jfieldID id_GroupQueryClassMethodPara = env->GetFieldID(class_CmdParaobject, "GroupQueryClassMethodPara", "Lcom/zte/softda/ocx/GROUP_QUERY_CLASS_METHOD_PARAM;");
        jobject obj_GroupQueryClassMethodPara = env->GetObjectField(CmdParaobject, id_GroupQueryClassMethodPara);
        jclass class_GroupQueryClassMethodPara = env->GetObjectClass(obj_GroupQueryClassMethodPara);

        getStringField(env, obj_GroupQueryClassMethodPara, class_GroupQueryClassMethodPara, "cGroupURI", GroupQueryClassMethodPara.cGroupURI,  sizeof(GroupQueryClassMethodPara.cGroupURI) - 1);
        getStringField(env, obj_GroupQueryClassMethodPara, class_GroupQueryClassMethodPara, "cIdentifier", GroupQueryClassMethodPara.cIdentifier,  sizeof(GroupQueryClassMethodPara.cIdentifier) - 1);
        getStringField(env, obj_GroupQueryClassMethodPara, class_GroupQueryClassMethodPara, "cGroupClassID", GroupQueryClassMethodPara.cGroupClassID,  sizeof(GroupQueryClassMethodPara.cGroupClassID) - 1);
        getStringField(env, obj_GroupQueryClassMethodPara, class_GroupQueryClassMethodPara, "cGroupClassName", GroupQueryClassMethodPara.cGroupClassName,  sizeof(GroupQueryClassMethodPara.cGroupClassName) - 1);

        jfieldID  id_nMethodType = env->GetFieldID(class_GroupQueryClassMethodPara, "nMethodType", "I");
        GroupQueryClassMethodPara.iResultCode = env->GetIntField(obj_GroupQueryClassMethodPara, id_nMethodType);

        bReturn = bIMSDispUICmdMethod((long)iType, (short far*)&GroupQueryClassMethodPara);
    }
    else if(iType == 5)//��ȡ����Ȩ��
    {
        bReturn = bIMSDispUICmdMethod((long)iType, NULL);
    }
    else if(iType == 20)
    {
        HTTP_HANGUP_CONF_EVENT_PARAM HttpHangUpConfMethodPara = {0};

        jfieldID id_HttpHangUpConfMethodPara = env->GetFieldID(class_CmdParaobject, "HttpHangUpConfParam", "Lcom/zte/softda/ocx/HTTP_HANGUP_CONF_EVENT_PARAM;");
        jobject obj_HttpHangUpConfMethodPara = env->GetObjectField(CmdParaobject, id_HttpHangUpConfMethodPara);
        jclass class_HttpHangUpConfMethodPara = env->GetObjectClass(obj_HttpHangUpConfMethodPara);

        getStringField(env, obj_HttpHangUpConfMethodPara, class_HttpHangUpConfMethodPara, "cConfURI", HttpHangUpConfMethodPara.cConfURI,  sizeof(HttpHangUpConfMethodPara.cConfURI) - 1);
        getStringField(env, obj_HttpHangUpConfMethodPara, class_HttpHangUpConfMethodPara, "cServer", HttpHangUpConfMethodPara.cServer,  sizeof(HttpHangUpConfMethodPara.cServer) - 1);

        bReturn = bIMSDispUICmdMethod((long)iType, (short far*)&HttpHangUpConfMethodPara);
    }
    return bReturn;
}

static jboolean jni_bUpdateMsgIndication(JNIEnv* env, jclass cls, jobject MsgIndicationParaobject)
{
    MsgIndication_T MsgIndicationPara = {0};
    jclass class_MsgIndicationPara = env->GetObjectClass(MsgIndicationParaobject);
    getStringField(env, MsgIndicationParaobject, class_MsgIndicationPara, "cMemberURI", MsgIndicationPara.cMemberURI,  sizeof(MsgIndicationPara.cMemberURI) - 1);
    getStringField(env, MsgIndicationParaobject, class_MsgIndicationPara, "tLastactive", MsgIndicationPara.tLastactive,  sizeof(MsgIndicationPara.tLastactive) - 1);

    jfieldID  id_mask = env->GetFieldID(class_MsgIndicationPara, "mask", "I");
    MsgIndicationPara.mask = env->GetIntField(MsgIndicationParaobject, id_mask);
    jfieldID  id_state = env->GetFieldID(class_MsgIndicationPara, "state", "I");
    MsgIndicationPara.state = env->GetIntField(MsgIndicationParaobject, id_state);
    jfieldID  id_refresh = env->GetFieldID(class_MsgIndicationPara, "refresh", "I");
    MsgIndicationPara.refresh = env->GetIntField(MsgIndicationParaobject, id_refresh);

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bUpdateMsgIndication cMemberURI[%s], tLastactive[%s],  mask[%d], state[%d],refresh[%d]",
                        MsgIndicationPara.cMemberURI,
                        MsgIndicationPara.tLastactive,
                        MsgIndicationPara.mask,
                        MsgIndicationPara.state,
                        MsgIndicationPara.refresh);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bUpdateMsgIndication cMemberURI[%s], tLastactive[%s],  mask[%d], state[%d],refresh[%d]",
             MsgIndicationPara.cMemberURI,
             MsgIndicationPara.tLastactive,
             MsgIndicationPara.mask,
             MsgIndicationPara.state,
             MsgIndicationPara.refresh);

    return bUpdateMsgIndication((short far*)&MsgIndicationPara);
}

static jboolean jni_bRequestLastLogInfo(JNIEnv* env, jclass cls, jobject LastLogInfoobject)
{
    LastLogInfo_T LastLogInfo = {0};
    jclass class_LastLogInfo = env->GetObjectClass(LastLogInfoobject);
    getStringField(env, LastLogInfoobject, class_LastLogInfo , "friendURI", LastLogInfo.friendURI,  sizeof(LastLogInfo.friendURI) - 1);
    getStringField(env, LastLogInfoobject, class_LastLogInfo , "lastlogintime", LastLogInfo.lastlogintime,  sizeof(LastLogInfo.lastlogintime) - 1);
    getStringField(env, LastLogInfoobject, class_LastLogInfo , "lastlogouttime", LastLogInfo.lastlogouttime,  sizeof(LastLogInfo.lastlogouttime) - 1);

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bRequestLastLogInfo friendURI[%s], lastlogouttime[%s]", LastLogInfo.friendURI, LastLogInfo.lastlogouttime);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bRequestLastLogInfo friendURI[%s], lastlogouttime[%s]", LastLogInfo.friendURI, LastLogInfo.lastlogouttime);

    return bRequestLastLogInfo((short far*)&LastLogInfo);
}
static jboolean jni_bIMSInviteSomeoneToMyPublicGroup(JNIEnv* env, jclass cls, jstring jstr_SomeoneURI, jstring jstr_DisplayName, jstring jstr_GroupURI)
{
    char cSomeoneURI[64 + 1] = {0};
    char cDisplayName[64 + 1] = {0};
    char cGroupURI[1024 + 1] = {0};

    safeGetStringUTFChars(env, jstr_SomeoneURI, cSomeoneURI, sizeof(cSomeoneURI) - 1, NULL);
    safeGetStringUTFChars(env, jstr_DisplayName, cDisplayName, sizeof(cDisplayName) - 1, NULL);
    safeGetStringUTFChars(env, jstr_GroupURI, cGroupURI, sizeof(cGroupURI) - 1, NULL);

    return bIMSInviteSomeoneToMyPublicGroup(cSomeoneURI, cDisplayName, cGroupURI);
}
static jboolean jni_bIMSCreateOnePublicGroup(JNIEnv* env, jclass cls, jobject jobj_bIMSCreateOnePublicGroup)
{
    int i = 0;
    PUBLIC_GROUP_INPUT_PARA_TEMP InputParaTemp = {0};

    jclass class_bIMSCreateOnePublicGroup = env->GetObjectClass(jobj_bIMSCreateOnePublicGroup);

    getStringField(env, jobj_bIMSCreateOnePublicGroup, class_bIMSCreateOnePublicGroup, "cGroupURI", InputParaTemp.cGroupURI,  sizeof(InputParaTemp.cGroupURI) - 1);
    getStringField(env, jobj_bIMSCreateOnePublicGroup, class_bIMSCreateOnePublicGroup, "cGroupName", InputParaTemp.cGroupName,  sizeof(InputParaTemp.cGroupName) - 1);
    getStringField(env, jobj_bIMSCreateOnePublicGroup, class_bIMSCreateOnePublicGroup, "cMyDisplayName", InputParaTemp.cMyDisplayName,  sizeof(InputParaTemp.cMyDisplayName) - 1);

    jfieldID  id_lDuration = env->GetFieldID(class_bIMSCreateOnePublicGroup, "lDuration", "I");
    InputParaTemp.lDuration = env->GetIntField(jobj_bIMSCreateOnePublicGroup, id_lDuration);

    jfieldID  id_lVisibitily = env->GetFieldID(class_bIMSCreateOnePublicGroup, "lVisibitily", "I");
    InputParaTemp.lVisibitily = env->GetIntField(jobj_bIMSCreateOnePublicGroup, id_lVisibitily);

    jfieldID  id_lNeedPermit = env->GetFieldID(class_bIMSCreateOnePublicGroup, "lNeedPermit", "I");
    InputParaTemp.lNeedPermit = env->GetIntField(jobj_bIMSCreateOnePublicGroup, id_lNeedPermit);

    jfieldID  id_lMax = env->GetFieldID(class_bIMSCreateOnePublicGroup, "lMax", "I");
    InputParaTemp.lMax = env->GetIntField(jobj_bIMSCreateOnePublicGroup, id_lMax);

    jfieldID  id_forbid_2D_join = env->GetFieldID(class_bIMSCreateOnePublicGroup, "forbid_2D_join", "I");
    InputParaTemp.forbid_2D_join = env->GetIntField(jobj_bIMSCreateOnePublicGroup, id_forbid_2D_join);

    jfieldID  id_member_invite_enable = env->GetFieldID(class_bIMSCreateOnePublicGroup, "member_invite_enable", "I");
    InputParaTemp.member_invite_enable = env->GetIntField(jobj_bIMSCreateOnePublicGroup, id_member_invite_enable);

    getStringField(env, jobj_bIMSCreateOnePublicGroup, class_bIMSCreateOnePublicGroup, "cSubject", InputParaTemp.cSubject,  sizeof(InputParaTemp.cSubject) - 1);
    getStringField(env, jobj_bIMSCreateOnePublicGroup, class_bIMSCreateOnePublicGroup, "cBulletin", InputParaTemp.cBulletin,  sizeof(InputParaTemp.cBulletin) - 1);
    getStringField(env, jobj_bIMSCreateOnePublicGroup, class_bIMSCreateOnePublicGroup, "cGroupClassID", InputParaTemp.cGroupClassID,  sizeof(InputParaTemp.cGroupClassID) - 1);
    getStringField(env, jobj_bIMSCreateOnePublicGroup, class_bIMSCreateOnePublicGroup, "cType", InputParaTemp.cType,  sizeof(InputParaTemp.cType) - 1);

    jfieldID  id_iListNum = env->GetFieldID(class_bIMSCreateOnePublicGroup, "iListNum", "I");
    InputParaTemp.iListNum = env->GetIntField(jobj_bIMSCreateOnePublicGroup, id_iListNum);

    jfieldID id_GroupMemberList = env->GetFieldID(class_bIMSCreateOnePublicGroup, "GroupMemberList", "[Ljava/lang/String;");
    jobjectArray pGroupMemberList = NULL;
    pGroupMemberList = (jobjectArray)env->GetObjectField(jobj_bIMSCreateOnePublicGroup, id_GroupMemberList);
    if(pGroupMemberList != NULL)
    {
        jstring jstr_pGroupMemberListOne = NULL;
        int size = 0;
        for(i = 0; i < InputParaTemp.iListNum; i++)
        {
            jstr_pGroupMemberListOne = (jstring)env->GetObjectArrayElement(pGroupMemberList, i);
            if(jstr_pGroupMemberListOne != NULL)
            {
                size = env->GetStringLength(jstr_pGroupMemberListOne);
                InputParaTemp.pGroupMemberList[i] = (char*)ZX_malloc(size + 1);
                memset(InputParaTemp.pGroupMemberList[i], 0, size + 1);
                safeGetStringUTFChars(env, jstr_pGroupMemberListOne, InputParaTemp.pGroupMemberList[i], size, NULL);
            }
        }
    }

    jfieldID id_GroupMemberDispList = env->GetFieldID(class_bIMSCreateOnePublicGroup, "GroupMemberDispList", "[Ljava/lang/String;");
    jobjectArray pGroupMemberDispList = (jobjectArray)env->GetObjectField(jobj_bIMSCreateOnePublicGroup, id_GroupMemberDispList);
    if(pGroupMemberDispList != NULL)
    {
        jstring jstr_ppGroupMemberDispListOne = NULL;
        int size1 = 0;
        for(i = 0; i < InputParaTemp.iListNum; i++)
        {
            jstr_ppGroupMemberDispListOne = (jstring)env->GetObjectArrayElement(pGroupMemberDispList, i);
            if(jstr_ppGroupMemberDispListOne != NULL)
            {
//				size1 = env->GetStringLength(jstr_ppGroupMemberDispListOne);
                size1 = env->GetStringUTFLength(jstr_ppGroupMemberDispListOne);
                InputParaTemp.pGroupMemberDispList[i] = (char*)ZX_malloc(size1 + 1);
                memset(InputParaTemp.pGroupMemberDispList[i], 0, size1 + 1);
                safeGetStringUTFChars(env, jstr_ppGroupMemberDispListOne, InputParaTemp.pGroupMemberDispList[i], size1, NULL);
            }
        }
    }

    return bIMSCreateOnePublicGroup((short far*)&InputParaTemp);
}
static jboolean jni_bServerLinkRecover(JNIEnv* env, jclass cls)
{
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bServerLinkRecover ");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bServerLinkRecover ");
    return bServerLinkRecover();
}
static jboolean jni_bCloseServerLink(JNIEnv* env, jclass cls)
{
    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bCloseServerLink ");
    else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bCloseServerLink ");
    return bCloseServerLink();
}

//lmd20141204
static jboolean jni_bDataConfFileUpload(JNIEnv* env, jclass cls, jlong iOperateType, jstring pUploadURI, jstring pFileName, jstring pFilePath,jlong iFileLen, jlong iBeginValue, jlong iFileType)
{
    char cUploadURI[512] = {0};
    char cFileName[256] = {0};
    char cFilePath[256] = {0};

    safeGetStringUTFChars(env, pUploadURI, cUploadURI, sizeof(cUploadURI) - 1, NULL);
    safeGetStringUTFChars(env, pFileName, cFileName, sizeof(cFileName) - 1, NULL);
    safeGetStringUTFChars(env, pFilePath, cFilePath, sizeof(cFilePath) - 1, NULL);

    return bDataConfFileUpload((long)iOperateType,cUploadURI, cFileName,cFilePath, (long)iFileLen, (long)iBeginValue, (long)iFileType);
}

static jboolean jni_bDataConfFileDownload(JNIEnv* env, jclass cls, jlong iOperateType, jstring pDownloadURI, jstring pFilePath, jstring pFileName)
{
    char cDownloadURI[512] = {0};
    char cFileName[256] = {0};
    char cFilePath[256] = {0};

    safeGetStringUTFChars(env, pDownloadURI, cDownloadURI, sizeof(cDownloadURI) - 1, NULL);
    safeGetStringUTFChars(env, pFileName, cFileName, sizeof(cFileName) - 1, NULL);
    safeGetStringUTFChars(env, pFilePath, cFilePath, sizeof(cFilePath) - 1, NULL);

    return bDataConfFileDownload((long)iOperateType,cDownloadURI,cFilePath, cFileName);
}
//lmd20141204, end


//zk
static void jni_nativeInit(JNIEnv* env, jclass cls)
{
    //LOGI(TAG, "jni_nativeRender");
	MediaEngin_VideoForAndroid_Init();
	return;	 
}
static void jni_nativeResize(JNIEnv* env, jclass cls, jint iWidth, jint iHeight)
{	 
	MediaEngin_VideoForAndroid_Resize(iWidth, iHeight);
	return;
}
static void jni_nativeRender(JNIEnv* env, jclass cls)
{
    //LOGI(TAG, "jni_nativeRender");
	MediaEngin_VideoForAndroid_Render();
	return;	 
}

//by smm 2013.04.02
static void jni_SensorAngle(JNIEnv* env, jclass cls, jint iAngle)
{
    //LOGI(TAG, "jni_nativeRender");
	MediaEngin_SensorAngle(iAngle);
	return;	 
}

//by smm 2014.11.03
static void jni_VideoReadBMPsrc(JNIEnv* env, jclass cls, jstring lpSrc_CIF, jstring lpSrc_QCIF)
{
	char cSrc_CIF[256 + 1] = {0};
	char cSrc_QCIF[256 + 1] = {0};
	
    safeGetStringUTFChars(env, lpSrc_CIF, cSrc_CIF, sizeof(cSrc_CIF) - 1, NULL);
	safeGetStringUTFChars(env, lpSrc_QCIF, cSrc_QCIF, sizeof(cSrc_QCIF) - 1, NULL);

	MediaEngin_VideoReadBMPsrc(cSrc_CIF, cSrc_QCIF);
	return;	 
}

void BridgeInfoToString(ConfBridgeControlParams_T& params, char* buf, size_t bufSize)
{
snprintf(buf, bufSize, "iConfType=%d,szConfURI=%s,szStartTime=%s,szEndTime=%s,nMaxMember=%d,"
	"nPeriod=%d,nLimit=%d,szSubject=%s,szChairPasswd=%s,szMemberPasswd=%s, szServer=%s,szDataConfServer=%s,szDataConfURI=%s,szConfNO=%s"
	, params.iConfType, params.szConfURI
	,params.szStartTime, params.szEndTime
	,params.nMaxMember,params.nPeriod
	,params.nLimit,params.szSubject,params.szChairPasswd
	,params.szMemberPasswd,params.szServer
	,params.szDataConfServer,params.szDataConfURI,params.szConfNO);
}

static jboolean jni_bConfBridgeControl(JNIEnv* env, jclass cls, jint iType, jobject mPara)
{
	// ConfBridgeControlParams mPara
	int i = 0;
	char buf[1024] = "";
	
	if(g_WriteLogType == 2)
	    TraceMsgWindow1(1, "rcscontroller jni_bConfBridgeControl begin iType = %d", (int)iType);
	else if(g_WriteLogType == 1)
	    LOGI(TAG, "jni_bConfBridgeControl begin iType = %d", (int)iType);

	JOjbectWrapper objParams(env, mPara);
	ConfBridgeControlParams_T params = {0};
	params.iConfType = objParams.GetIntValue("iConfType");
	objParams.GetStringValue("szConfURI", params.szConfURI, count_of_array(params.szConfURI));
	objParams.GetStringValue("szStartTime", params.szStartTime, count_of_array(params.szStartTime));
	objParams.GetStringValue("szEndTime", params.szEndTime, count_of_array(params.szEndTime));
	params.nMaxMember = objParams.GetIntValue("nMaxMember");
	params.nPeriod = objParams.GetIntValue("nPeriod");
	params.nLimit = objParams.GetIntValue("nLimit");
	objParams.GetStringValue("szSubject", params.szSubject, count_of_array(params.szSubject));
	objParams.GetStringValue("szChairPasswd", params.szChairPasswd, count_of_array(params.szChairPasswd));
	objParams.GetStringValue("szMemberPasswd", params.szMemberPasswd, count_of_array(params.szMemberPasswd));
	objParams.GetStringValue("szServer", params.szServer, count_of_array(params.szServer));
	objParams.GetStringValue("szDataConfServer", params.szDataConfServer, count_of_array(params.szDataConfServer));
	objParams.GetStringValue("szDataConfURI", params.szDataConfURI, count_of_array(params.szDataConfURI));
	objParams.GetStringValue("szConfNO", params.szConfNO, count_of_array(params.szConfNO));

	BridgeInfoToString(params, buf, 1023);
	if(g_WriteLogType == 2)
	    TraceMsgWindow1(1, "rcscontroller jni_bConfBridgeControl params{%s}", buf);
	else if(g_WriteLogType == 1)
	    LOGI(TAG, "rcscontroller jni_bConfBridgeControl params{%s}", buf);
	
	OCXReq_T sReq = {0};
	sReq.pConfBridgeControl = CreateConfBridgeControlHandler((ConfBridgeControlType)iType, (short*)&params);
	sReq.ChoiceType = cmdId_ConfBridgeControl;
	return SendToAgent(OCXCtrlReq_Event, (UINT8*)&sReq, sizeof(sReq));	
}

static jboolean jni_bSetXCAPAccount(JNIEnv* env, jclass cls, jstring jstrAccount, jstring jstrPassword)
{

	TraceMsgWindow1(1, "rcscontroller jni_bSetXCAPAccount jstrAccount{%s} jstrPassword{******}", jstrAccount);
	
    char cAccount[256 + 1] = {0};
    char cPassword[256 + 1] = {0};

    safeGetStringUTFChars(env, jstrAccount, cAccount, sizeof(cAccount) - 1, NULL);
    safeGetStringUTFChars(env, jstrPassword, cPassword, sizeof(cPassword) - 1, NULL);

    	OCXReq_T sReq = {0};
	strncpy(sReq.xcapAccountSet.ptszAccount, cAccount, 256);
	strncpy(sReq.xcapAccountSet.ptszPasswd, cPassword, 256);
	sReq.ChoiceType = cmdId_SetXcapAccount;
	return SendToAgent(OCXCtrlReq_Event, (UINT8*)&sReq, sizeof(sReq));
}

//by smm for AVTest 2012.11.08 begin
typedef struct AVTEST_METHOD {
	int		iOperateType;	//0-ֹͣ; 1-��ʼ
	int		iAudioType;	//0-����Ƶ; 1-G711a; 2-G711u; 3-G729AB; 4-SILK
	int		iVideoType;//0-����Ƶ; 1-H264; 2-H263
	char	cRemoteIPAddress[16+1];//Զ��IP��ַ
	char	cLocalIPAddress[16+1];//����IP��ַ
	long	lRemoteAudioPort;//Զ��IP����Ƶ�˿ں�
	long	lRemoteVideoPort;//Զ��IP����Ƶ�˿ں�
	long	lLocalAudioPort;//����IP����Ƶ�˿ں�
	long	lLocalVideoPort;//����IP����Ƶ�˿ں�
}AVTEST_METHOD;

#define AVTEST_OPERATE_START	1
#define AVTEST_OPERATE_STOP		0

#define AVTEST_AUDIOTYPE_NONE	0
#define AVTEST_AUDIOTYPE_G711a	1
#define AVTEST_AUDIOTYPE_G711u	2
#define AVTEST_AUDIOTYPE_G729AB	3
#define AVTEST_AUDIOTYPE_SILK	4

#define AVTEST_VIDEOTYPE_NONE	0
#define AVTEST_VIDEOTYPE_H264	1
#define AVTEST_VIDEOTYPE_H263	2

#define G711ALAW    0x08
#define G711ULAW    0x00
#define G729A       0x12
#define SILK        104

int g_iAudioTestChannelIndex = -1;
int g_iVideoTestChannelIndex = -1;

BOOL bAuidoInit = FALSE;

static void jni_AVTest(JNIEnv* env, jclass cls, jobject jobj_AVTestMethodObject)
{
	LOGI(TAG,"[jni_AVTest] begin!!");
	jclass class_AVTestMethodObject = env->GetObjectClass(jobj_AVTestMethodObject);

	AVTEST_METHOD AVTestMethod = {0};
	jfieldID  id_iOperateType = env->GetFieldID(class_AVTestMethodObject, "iOperateType", "I");
    AVTestMethod.iOperateType = env->GetIntField(jobj_AVTestMethodObject, id_iOperateType);

	LOGI(TAG,"[jni_AVTest]AVTestMethod.iOperateType=%d", AVTestMethod.iOperateType);
	if (AVTestMethod.iOperateType == AVTEST_OPERATE_STOP)//ֹͣ
	{
		LOGI(TAG,"[jni_AVTest]g_iAudioTestChannelIndex=%d, g_iVideoTestChannelIndex=%d!", g_iAudioTestChannelIndex, g_iVideoTestChannelIndex);
		if (g_iAudioTestChannelIndex >= 0)
		{
			MediaEngin_CloseAudioChannel(g_iAudioTestChannelIndex);
			MediaEngin_DestroyAudioChannel(g_iAudioTestChannelIndex);
			g_iAudioTestChannelIndex = -1;

			//for test temp
//			MediaEngin_AudioEnd();
		}

		if (g_iVideoTestChannelIndex >= 0)
		{
			MediaEngin_CloseVideoChannel(g_iVideoTestChannelIndex);
			MediaEngin_DestroyVideoChannel(g_iVideoTestChannelIndex);
			g_iVideoTestChannelIndex = -1;
			MediaEngin_VideoEnd(0);
		}
	} 
	else if(AVTestMethod.iOperateType == AVTEST_OPERATE_START)//��ʼ
	{
		jfieldID  id_iAudioType = env->GetFieldID(class_AVTestMethodObject, "iAudioType", "I");
		AVTestMethod.iAudioType = env->GetIntField(jobj_AVTestMethodObject, id_iAudioType);

		jfieldID  id_iVideoType = env->GetFieldID(class_AVTestMethodObject, "iVideoType", "I");
		AVTestMethod.iVideoType = env->GetIntField(jobj_AVTestMethodObject, id_iVideoType);

		LOGI(TAG,"[jni_AVTest]iAudioType=%d, iVideoType=%d", AVTestMethod.iAudioType, AVTestMethod.iVideoType);

		//Զ��IP��ַ
		getStringField(env, jobj_AVTestMethodObject, class_AVTestMethodObject, "cRemoteIPAddress", AVTestMethod.cRemoteIPAddress,  sizeof(AVTestMethod.cRemoteIPAddress)-1);
		
		//����IP��ַ
		getStringField(env, jobj_AVTestMethodObject, class_AVTestMethodObject, "cLocalIPAddress", AVTestMethod.cLocalIPAddress,  sizeof(AVTestMethod.cLocalIPAddress)-1);
		MediaEngin_SetValue(VB_g_dwLocalIP, inet_addr(AVTestMethod.cLocalIPAddress), AVTestMethod.cLocalIPAddress);

		LOGI(TAG,"[jni_AVTest]cRemoteIPAddress=%s, cLocalIPAddress=%s", AVTestMethod.cRemoteIPAddress, AVTestMethod.cLocalIPAddress);
		
		if (AVTestMethod.iAudioType == AVTEST_AUDIOTYPE_NONE)
		{
		} 
		else
		{
			//������Ƶ�˿ں�
			jfieldID  id_lLocalAudioPort = env->GetFieldID(class_AVTestMethodObject, "lLocalAudioPort", "J");
			AVTestMethod.lLocalAudioPort = env->GetLongField(jobj_AVTestMethodObject, id_lLocalAudioPort);
			
			//Զ����Ƶ�˿ں�
			jfieldID  id_lRemoteAuidoPort = env->GetFieldID(class_AVTestMethodObject, "lRemoteAudioPort", "J");
			AVTestMethod.lRemoteAudioPort = env->GetLongField(jobj_AVTestMethodObject, id_lRemoteAuidoPort);

			LOGI(TAG,"[jni_AVTest]lLocalAudioPort=%d, lRemoteAudioPort=%d", AVTestMethod.lLocalAudioPort, AVTestMethod.lRemoteAudioPort);

			WORD m_wAudioMillSecsPerFrame = 20;		//Ĭ�ϰ�G711a������д
			unsigned int m_AuidoRtpPT = G711ALAW;	//Ĭ�ϰ�G711a������д

			switch (AVTestMethod.iAudioType)
			{
			case AVTEST_AUDIOTYPE_G711a:		//G711a
				{
					m_wAudioMillSecsPerFrame = 20;
					m_AuidoRtpPT = G711ALAW;
				}
				break;
			case AVTEST_AUDIOTYPE_G711u:		//G711u
				{					
					m_wAudioMillSecsPerFrame = 20;
					m_AuidoRtpPT = G711ULAW;
				}
				break;
			case AVTEST_AUDIOTYPE_G729AB:		//G729AB
				{
					WORD m_wAudioMillSecsPerFrame = 20;
					m_AuidoRtpPT = G729A;
				}
				break;
				//by smm 2012.12.21 �������������֮ǰ
			case AVTEST_AUDIOTYPE_SILK:		//SILK
				{
					WORD m_wAudioMillSecsPerFrame = 20;
					m_AuidoRtpPT = SILK;
				}
				break;
				
			default:
				break;
			}

			if (!bAuidoInit)
			{
				MediaEngin_AudioInit();
				bAuidoInit = TRUE;
			}
			
			WORD iLocalAudioPort = AVTestMethod.lLocalAudioPort;
			MediaEngin_CreateAudioChannel(g_iAudioTestChannelIndex, iLocalAudioPort);
			if (iLocalAudioPort != AVTestMethod.lLocalAudioPort)
			{
				AVTestMethod.lLocalAudioPort = iLocalAudioPort;
				LOGI(TAG,"[jni_AVTest]iLocalAudioPort=%d", iLocalAudioPort);
			}
			
			MediaEngin_SetAudioChannel(g_iAudioTestChannelIndex, AVTestMethod.lLocalAudioPort, AVTestMethod.cRemoteIPAddress, AVTestMethod.lRemoteAudioPort
				, m_AuidoRtpPT, m_wAudioMillSecsPerFrame, 96, 97, 3, FALSE);
			
			if (g_iAudioTestChannelIndex >= 0)
			{
				MediaEngin_OpenAudioChannel(g_iAudioTestChannelIndex, FALSE, FALSE);
			}
		}

		switch (AVTestMethod.iVideoType)
		{
		case AVTEST_VIDEOTYPE_NONE:			//����Ƶ
			{
			}
			break;
		case AVTEST_VIDEOTYPE_H264:			//H264
			{
				//������Ƶ�˿ں�
				jfieldID  id_lLocalVideoPort = env->GetFieldID(class_AVTestMethodObject, "lLocalVideoPort", "J");
				AVTestMethod.lLocalVideoPort = env->GetLongField(jobj_AVTestMethodObject, id_lLocalVideoPort);

				//Զ����Ƶ�˿ں�
				jfieldID  id_lRemoteVideoPort = env->GetFieldID(class_AVTestMethodObject, "lRemoteVideoPort", "J");
				AVTestMethod.lRemoteVideoPort = env->GetLongField(jobj_AVTestMethodObject, id_lRemoteVideoPort);

				LOGI(TAG,"[jni_AVTest]H264!lLocalVideoPort=%d, lRemoteVideoPort=%d", AVTestMethod.lLocalVideoPort, AVTestMethod.lRemoteVideoPort);

				MediaEngin_VideoInit();
				WORD iLocalVideoPort = AVTestMethod.lLocalVideoPort;
				MediaEngin_CreateVideoChannel(g_iVideoTestChannelIndex, iLocalVideoPort);
				if (iLocalVideoPort != AVTestMethod.lLocalVideoPort)
				{
					AVTestMethod.lLocalVideoPort = iLocalVideoPort;
					LOGI(TAG,"[jni_AVTest]H264!iLocalVideoPort=%d", iLocalVideoPort);
				}

				MediaEngin_SetVideoChannel(g_iVideoTestChannelIndex, AVTestMethod.cRemoteIPAddress, AVTestMethod.lRemoteVideoPort, 99, "H264", 1);//"10.46.154.4" //96, "H264"  //34, "H263"

				if (g_iVideoTestChannelIndex >= 0)
				{
					char cFilename[16] = "";
					MediaEngin_OpenVideoChannel(g_iVideoTestChannelIndex, -1, cFilename);
				}
			}
			break;
		case AVTEST_VIDEOTYPE_H263:			//H263
			{
			}
			break;
		default:
			break;
		}
	}
	else
	{
		LOGI(TAG,"[jni_AVTest]Error!Unknown iOperateType!");
	}
	
	LOGI(TAG,"[jni_AVTest] end!!");
}
//by smm for AVTest 2012.11.08 end

extern T_Call g_Call_Temp;
static jboolean jni_bHasCreateSurfacet(JNIEnv* env, jclass cls)
{
	LOGI(TAG,"[jni_bHasCreateSurfacet]");
	return bHasCreateSurfacet();	 
}

static jboolean jni_bDataConfMethod(JNIEnv* env, jclass cls, jint iType, jobject jobj_bConfMethodPara)
{
	int i = 0;

    if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bIMSConfMethod begin iType = %d", (int)iType);
    else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bIMSConfMethod begin iType = %d", (int)iType);

    jclass class_bConfMethodPara = env->GetObjectClass(jobj_bConfMethodPara);

    //��������ʹ���ԤԼ�����������ģ��
    if (GT_METHOD_CONF_CONTROL == iType)
    {
        CONF_CONTROL_METHOD ConfControlMethod = {0};
        jfieldID id_ConfControlMethod = env->GetFieldID(class_bConfMethodPara, "confCtrlMethodPara", "Lcom/zte/softda/ocx/conference/ConfCtrlMethodPara;");
        jobject obj_ConfControlMethod = env->GetObjectField(jobj_bConfMethodPara, id_ConfControlMethod);
        jclass class_ConfControlMethod = env->GetObjectClass(obj_ConfControlMethod);

        jfieldID  id_iType = env->GetFieldID(class_ConfControlMethod, "iType", "I");
        ConfControlMethod.iType = env->GetIntField(obj_ConfControlMethod, id_iType);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cConfURI", ConfControlMethod.cConfURI,  sizeof(ConfControlMethod.cConfURI) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cSubject", ConfControlMethod.cSubject,  sizeof(ConfControlMethod.cSubject) - 1);

        jfieldID  id_iConfType = env->GetFieldID(class_ConfControlMethod, "iConfType", "I");
        ConfControlMethod.iConfType = env->GetIntField (obj_ConfControlMethod, id_iConfType);
        jfieldID  id_iInitMemberNum = env->GetFieldID(class_ConfControlMethod, "iInitMemberNum", "I");
        ConfControlMethod.iInitMemberNum = env->GetIntField(obj_ConfControlMethod, id_iInitMemberNum);

        jfieldID id_pInitMemberListArray = env->GetFieldID(class_ConfControlMethod, "cInitMemberList", "[Ljava/lang/String;");

        jobjectArray pInitMemberListArray = (jobjectArray)env->GetObjectField(obj_ConfControlMethod, id_pInitMemberListArray);
        if (pInitMemberListArray != NULL)
        {
            int i;

            for (i = 0; i < ConfControlMethod.iInitMemberNum; i++)
            {
                jstring jstr_pInitMemberListArray = (jstring)env->GetObjectArrayElement(pInitMemberListArray, i);
                if (jstr_pInitMemberListArray != NULL)
                {
                    int size = env->GetStringLength(jstr_pInitMemberListArray);
	
                    ConfControlMethod.pInitMemberList[i] = (char*)ZX_malloc(size + 1);
                    memset(ConfControlMethod.pInitMemberList[i], 0, size + 1);
                    safeGetStringUTFChars(env, jstr_pInitMemberListArray, ConfControlMethod.pInitMemberList[i], size, NULL);
                }
            }
        }

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMemberURI", ConfControlMethod.cMemberURI,  sizeof(ConfControlMethod.cMemberURI) - 1);

        jfieldID  id_iDoType = env->GetFieldID(class_ConfControlMethod, "iDoType", "I");
        ConfControlMethod.iDoType = env->GetIntField(obj_ConfControlMethod, id_iDoType);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cConfPassword", ConfControlMethod.cConfPassword,  sizeof(ConfControlMethod.cConfPassword) - 1);

        jfieldID  id_iSMSNum = env->GetFieldID(class_ConfControlMethod, "iSMSNum", "I");
        ConfControlMethod.iSMSNum = env->GetIntField(obj_ConfControlMethod, id_iSMSNum);
        jfieldID id_pSMSListArray = env->GetFieldID(class_ConfControlMethod, "cSMSList", "[Ljava/lang/String;");
        jobjectArray pSMSListArray = (jobjectArray)env->GetObjectField(obj_ConfControlMethod, id_pSMSListArray);
        if (pSMSListArray != NULL)
        {
            int i;

            for (i = 0; i < ConfControlMethod.iSMSNum; i++)
            {
                jstring jstr_pSMSListArray = (jstring)env->GetObjectArrayElement(pSMSListArray, i);
                if (jstr_pSMSListArray != NULL)
                {
                    int size = env->GetStringLength(jstr_pSMSListArray);

                    ConfControlMethod.pSMSList[i] = (char*)ZX_malloc(size + 1);
                    memset(ConfControlMethod.pSMSList[i], 0, size + 1);
                    safeGetStringUTFChars(env, jstr_pSMSListArray, ConfControlMethod.pSMSList[i], size, NULL);
                }
            }
        }

        jfieldID  id_pSMSMasterListArray = env->GetFieldID(class_ConfControlMethod, "nSMSMasterList", "[I");
        jintArray pSMSMasterListArray = (jintArray)env->GetObjectField(obj_ConfControlMethod, id_pSMSMasterListArray);

        if (pSMSMasterListArray != NULL)
        {
            jint *jpSMSMasterListArray = env->GetIntArrayElements(pSMSMasterListArray, 0);
            for (int i = 0; i < ConfControlMethod.iSMSNum; i++)
            {

                ConfControlMethod.pSMSMasterList[i] = jpSMSMasterListArray[i];

                //if(g_WriteLogType == 2)
                //	TraceMsgWindow1(1, "rcscontroller jni_bIMSConfMethod GT_METHOD_CONF_CONTROL pSMSMasterList = %d", ConfControlMethod.pSMSMasterList[i]);
                //else if(g_WriteLogType == 1)
                //LOGI(TAG, "GT_METHOD_CONF_CONTROL pSMSMasterList = %d", ConfControlMethod.pSMSMasterList[i]);
            }
        }

        jfieldID  id_iEmailNum = env->GetFieldID(class_ConfControlMethod, "iEmailNum", "I");
        ConfControlMethod.iEmailNum = env->GetIntField(obj_ConfControlMethod, id_iEmailNum);

        jfieldID id_pEmailListArray = env->GetFieldID(class_ConfControlMethod, "cEmailList", "[Ljava/lang/String;");
        jobjectArray pEmailListArray = (jobjectArray)env->GetObjectField(obj_ConfControlMethod, id_pEmailListArray);
        if (pEmailListArray != NULL)
        {
            int i;

            for (i = 0; i < ConfControlMethod.iEmailNum; i++)
            {
                jstring jobj_pEmailListArray = (jstring)env->GetObjectArrayElement(pEmailListArray, i);
                if (jobj_pEmailListArray != NULL)
                {
                    int size = env->GetStringLength(jobj_pEmailListArray);

                    ConfControlMethod.pEmailList[i] = (char*)ZX_malloc(size + 1);
                    memset(ConfControlMethod.pEmailList[i], 0, size + 1);
                    safeGetStringUTFChars(env, jobj_pEmailListArray, ConfControlMethod.pEmailList[i], size, NULL);
                }
            }
        }

        jfieldID  id_pEmailMasterListArray = env->GetFieldID(class_ConfControlMethod, "nEmailMasterList", "[I");
        jintArray pEmailMasterListArray = (jintArray)env->GetObjectField(obj_ConfControlMethod, id_pEmailMasterListArray);
        if (pSMSMasterListArray != NULL)
        {
            jint *jpEmailMasterListArray = env->GetIntArrayElements(pSMSMasterListArray, 0);

            for (int i = 0; i < ConfControlMethod.iEmailNum; i++)
            {
                ConfControlMethod.pSMSMasterList[i] = jpEmailMasterListArray[i];
            }
        }


        jfieldID  id_iMaxNum = env->GetFieldID(class_ConfControlMethod, "iMaxNum", "I");
        ConfControlMethod.iMaxNum = env->GetIntField(obj_ConfControlMethod, id_iMaxNum);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cStartTime", ConfControlMethod.cStartTime,  sizeof(ConfControlMethod.cStartTime) - 1);

        jfieldID  id_iPeriod = env->GetFieldID(class_ConfControlMethod, "iPeriod", "I");
        ConfControlMethod.iPeriod = env->GetIntField(obj_ConfControlMethod, id_iPeriod);
        jfieldID  id_iLimiteTime = env->GetFieldID(class_ConfControlMethod, "iLimiteTime", "I");
        ConfControlMethod.iLimiteTime = env->GetIntField(obj_ConfControlMethod, id_iLimiteTime);
        jfieldID  id_iVideoNum = env->GetFieldID(class_ConfControlMethod, "iVideoNum", "I");
        ConfControlMethod.iVideoNum = env->GetIntField(obj_ConfControlMethod, id_iVideoNum);
        jfieldID  id_iLayout = env->GetFieldID(class_ConfControlMethod, "iLayout", "I");
        ConfControlMethod.iLayout = env->GetIntField(obj_ConfControlMethod, id_iLayout);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMasterURI", ConfControlMethod.cMasterURI,  sizeof(ConfControlMethod.cMasterURI) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMasterPass", ConfControlMethod.cMasterPass,  sizeof(ConfControlMethod.cMasterPass) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cMemberPass", ConfControlMethod.cMemberPass,  sizeof(ConfControlMethod.cMemberPass) - 1);
        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cVisitorPass", ConfControlMethod.cVisitorPass,  sizeof(ConfControlMethod.cVisitorPass) - 1);

        jfieldID  id_iOrigType = env->GetFieldID(class_ConfControlMethod, "iOrigType", "I");
        ConfControlMethod.iOrigType = env->GetIntField(obj_ConfControlMethod, id_iOrigType);
        jfieldID  id_iDestType = env->GetFieldID(class_ConfControlMethod, "iDestType", "I");
        ConfControlMethod.iDestType = env->GetIntField(obj_ConfControlMethod, id_iDestType);
        jfieldID  id_iUserID = env->GetFieldID(class_ConfControlMethod, "iUserID", "I");
        ConfControlMethod.iUserID = env->GetIntField(obj_ConfControlMethod, id_iUserID);

        getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cDigest", ConfControlMethod.cDigest,  sizeof(ConfControlMethod.cDigest) - 1);

        jfieldID  id_iReleaseMode = env->GetFieldID(class_ConfControlMethod, "iReleaseMode", "I");
        ConfControlMethod.iReleaseMode = env->GetIntField(obj_ConfControlMethod, id_iReleaseMode);

	getStringField(env, obj_ConfControlMethod, class_ConfControlMethod, "cConfServerPath", ConfControlMethod.cConfServerPath,  sizeof(ConfControlMethod.cConfServerPath) - 1);
	
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller GT_METHOD_CONF_CONTROL	cSubject[%s]*iConfType[%d]*cMasterURI[%s]  [iInitMemberNum= %d] [iEmailNum = %d] [iSMSNum = %d], [cStartTime = %s]",
                            ConfControlMethod.cSubject, ConfControlMethod.iConfType, ConfControlMethod.cMasterURI, ConfControlMethod.iInitMemberNum, ConfControlMethod.iEmailNum, ConfControlMethod.iSMSNum, ConfControlMethod.cStartTime);
        else if(g_WriteLogType == 1)
            LOGI(TAG, "GT_METHOD_CONF_CONTROL    cSubject[%s]*iConfType[%d]*cMasterURI[%s]  [iInitMemberNum= %d] [iEmailNum = %d] [iSMSNum = %d], [cStartTime = %s]",
                 ConfControlMethod.cSubject, ConfControlMethod.iConfType, ConfControlMethod.cMasterURI, ConfControlMethod.iInitMemberNum, ConfControlMethod.iEmailNum, ConfControlMethod.iSMSNum, ConfControlMethod.cStartTime);

        BOOL iResult = bDataConfMethod(iType, (short far*)&ConfControlMethod);
	//�ͷ��ڴ�
	int nMemNum = ConfControlMethod.iInitMemberNum;
	for(int i1=0; i1<nMemNum; i1++)
	{
		if (ConfControlMethod.pInitMemberList[i1]) 
		{
			ZX_free(ConfControlMethod.pInitMemberList[i1]);
		}
		if (ConfControlMethod.pSMSList[i1]) 
		{
			ZX_free(ConfControlMethod.pSMSList[i1]);
		}
		if (ConfControlMethod.pEmailList[i1]) 
		{
			ZX_free(ConfControlMethod.pEmailList[i1]);
		}
	}

        return iResult;
    }
    //lmd20141204
    else if (GT_METHOD_CONF_PROPERTY == iType)
    {
        CONF_ATTR_CONTROL_METHOD ConfAttrControlMethod = {0};
	 jfieldID id_ConfAttrControlMethod = env->GetFieldID(class_bConfMethodPara, "confAttrControlMethodPara", "Lcom/zte/softda/ocx/conference/ConfAttrControlMethodPara;");
        jobject obj_ConfAttrControlMethod = env->GetObjectField(jobj_bConfMethodPara, id_ConfAttrControlMethod);
        jclass class_ConfAttrControlMethod = env->GetObjectClass(obj_ConfAttrControlMethod);

        jfieldID  id_iType = env->GetFieldID(class_ConfAttrControlMethod, "iType", "I");
        ConfAttrControlMethod.iType = env->GetIntField(obj_ConfAttrControlMethod, id_iType);
	 getStringField(env, obj_ConfAttrControlMethod, class_ConfAttrControlMethod, "cConfURI", ConfAttrControlMethod.cConfURI,  sizeof(ConfAttrControlMethod.cConfURI) - 1);
	 getStringField(env, obj_ConfAttrControlMethod, class_ConfAttrControlMethod, "cControl", ConfAttrControlMethod.cControl,  sizeof(ConfAttrControlMethod.cControl) - 1);
	 getStringField(env, obj_ConfAttrControlMethod, class_ConfAttrControlMethod, "cControl1", ConfAttrControlMethod.cControl1,  sizeof(ConfAttrControlMethod.cControl1) - 1);
	 getStringField(env, obj_ConfAttrControlMethod, class_ConfAttrControlMethod, "cControl2", ConfAttrControlMethod.cControl2,  sizeof(ConfAttrControlMethod.cControl2) - 1);
	 getStringField(env, obj_ConfAttrControlMethod, class_ConfAttrControlMethod, "cControl3", ConfAttrControlMethod.cControl3,  sizeof(ConfAttrControlMethod.cControl3) - 1);
	 jfieldID  id_iTime = env->GetFieldID(class_ConfAttrControlMethod, "iTime", "I");
        ConfAttrControlMethod.iTime = env->GetIntField(obj_ConfAttrControlMethod, id_iTime);
	 jfieldID  id_iMemberNum = env->GetFieldID(class_ConfAttrControlMethod, "iMemberNum", "I");
        ConfAttrControlMethod.iMemberNum = env->GetIntField(obj_ConfAttrControlMethod, id_iMemberNum);
	 jfieldID  id_iBgSoundNum = env->GetFieldID(class_ConfAttrControlMethod, "iBgSoundNum", "I");
        ConfAttrControlMethod.iBgSoundNum = env->GetIntField(obj_ConfAttrControlMethod, id_iBgSoundNum);
	 jfieldID  id_iUserID = env->GetFieldID(class_ConfAttrControlMethod, "iUserID", "I");
        ConfAttrControlMethod.iUserID = env->GetIntField(obj_ConfAttrControlMethod, id_iUserID);

	 BOOL iResult = bDataConfMethod(iType, (short far*)&ConfAttrControlMethod);
	 return iResult;
    }
    else if (GT_METHOD_STOP_FILE_TRANS == iType)
    {
        CONF_STOP_FILE_TRANS ConfStopFileTrans = {0};
	 jfieldID id_ConfStopFileTrans = env->GetFieldID(class_bConfMethodPara, "confStopFileTransPara", "Lcom/zte/softda/ocx/conference/ConfStopFileTransPara;");
        jobject obj_ConfStopFileTrans = env->GetObjectField(jobj_bConfMethodPara, id_ConfStopFileTrans);
        jclass class_ConfStopFileTrans = env->GetObjectClass(obj_ConfStopFileTrans);

        jfieldID  id_iOperateType = env->GetFieldID(class_ConfStopFileTrans, "iOperateType", "I");
        ConfStopFileTrans.iOperateType = env->GetIntField(obj_ConfStopFileTrans, id_iOperateType);

	 BOOL iResult = bDataConfMethod(iType, (short far*)&ConfStopFileTrans);
	 return iResult;
    }
    else
    {
       if(g_WriteLogType == 2)
        TraceMsgWindow1(1, "rcscontroller jni_bDataConfMethod %d is unknown iType", (int)iType);
       else if(g_WriteLogType == 1)
        LOGI(TAG, "jni_bDataConfMethod  %d is unknown iType", (int)iType);
    }
    //lmd20141204, end

	return TRUE;
}

//get stype info
WbStylePtr_t getWBStyle(JNIEnv* env, jclass class_shape, jobject obj_shape)
{
	WbStylePtr_t style = InitStyle();
	if (NULL == style)
	{
		return NULL;
	}

	
	//get style object
	jfieldID id_Style = env->GetFieldID(class_shape, "style", "Lcom/zte/softda/ocx/conference/wb/WBStyle;");
	jobject obj_Style = env->GetObjectField(obj_shape, id_Style);
	if (NULL == obj_Style)
	{
		free(style);
		return NULL;
	}
       jclass class_Style = env->GetObjectClass(obj_Style);

	getStringField(env, obj_Style, class_Style, "chFillColor", style->chFillColor,  sizeof(style->chFillColor) - 1);
	getStringField(env, obj_Style, class_Style, "chStrokeColor", style->chStrokeColor,  sizeof(style->chStrokeColor) - 1);
	jfieldID  id_iStrokeWidth = env->GetFieldID(class_Style, "iStrokeWidth", "I");
	style->iStrokeWidth= env->GetIntField(obj_Style, id_iStrokeWidth);
	jfieldID  id_fFillOpacity = env->GetFieldID(class_Style, "fFillOpacity", "F");
	style->fFillOpacity= env->GetFloatField(obj_Style, id_fFillOpacity);
	jfieldID  id_iFontSize = env->GetFieldID(class_Style, "iFontSize", "I");
	style->iFontSize= env->GetIntField(obj_Style, id_iFontSize);
	getStringField(env, obj_Style, class_Style, "chFontFamily", style->chFontFamily,  sizeof(style->chFontFamily) - 1);
	jfieldID  id_enumTextAnchor = env->GetFieldID(class_Style, "enumTextAnchor", "I");
	style->enumTextAnchor= (WB_ANCHOR_TYPE)env->GetIntField(obj_Style, id_enumTextAnchor);
	jfieldID  id_iBold = env->GetFieldID(class_Style, "iBold", "I");
	style->iBold= env->GetIntField(obj_Style, id_iBold);
	jfieldID  id_iItalic = env->GetFieldID(class_Style, "iItalic", "I");
	style->iItalic= env->GetIntField(obj_Style, id_iItalic);
	jfieldID  id_iTextDecorateType = env->GetFieldID(class_Style, "iTextDecorateType", "I");
	style->iTextDecorateType= env->GetIntField(obj_Style, id_iTextDecorateType);

	return style;
}

/*******************
iType=0:Ӧ�ù���(Զ��Э��)
iType=1:�װ�
iType=2:�ĵ�����
iType=3:��ҳ����
iType=4:����ռ�
iType=5:ͶƱ
iType=6:����
iType=7:���
iType=8:�ļ�����
********************/
static jboolean jni_bSend2MrsMethod(JNIEnv* env, jclass cls, jint iType, jobject jobj_bMrsMethodPara)
{
	int i = 0;
   	

	jclass class_bMrsMethodPara = env->GetObjectClass(jobj_bMrsMethodPara);

	switch (iType)
	{
                case 0:
		{
			APPShare_CONTROL_METHOD appshare = {0};

			jfieldID id_appControlMethod = env->GetFieldID(class_bMrsMethodPara, "appShareCtrlPara", "Lcom/zte/softda/ocx/conference/appshare/AppShareCtrlPara;");
        		jobject obj_appControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_appControlMethod);
        		jclass class_appControlMethod = env->GetObjectClass(obj_appControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_appControlMethod, "iType", "I");
			appshare.iType = env->GetIntField(obj_appControlMethod, id_Type);
			jfieldID  id_SubType = env->GetFieldID(class_appControlMethod, "iSubType", "I");
			appshare.iSubType = env->GetIntField(obj_appControlMethod, id_SubType);
			//jfieldID  id_AppWnd = env->GetFieldID(class_appControlMethod, "hAppWnd", "I");
			//appshare.hAppWnd = env->GetIntField(obj_appControlMethod, id_AppWnd);
			getStringField(env, obj_appControlMethod, class_appControlMethod, "chMemberURI", appshare.chMemberURI,  sizeof(appshare.chMemberURI) - 1);
			getStringField(env, obj_appControlMethod, class_appControlMethod, "chConfURI", appshare.chConfURI,  sizeof(appshare.chConfURI) - 1);
			getStringField(env, obj_appControlMethod, class_appControlMethod, "chChannelID", appshare.chChannelID,  sizeof(appshare.chChannelID) - 1);

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&appshare);
                }
		break;
		case 1:
		{
			WB_CONTROL_METHOD wb = {0};

			jfieldID id_WBControlMethod = env->GetFieldID(class_bMrsMethodPara, "wbCtrlMethod", "Lcom/zte/softda/ocx/conference/wb/WBControlMethod;");
        		jobject obj_WBControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_WBControlMethod);
        		jclass class_WBControlMethod = env->GetObjectClass(obj_WBControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_WBControlMethod, "iType", "I");
			wb.iType = env->GetIntField(obj_WBControlMethod, id_Type);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chConfURI", wb.chConfURI,  sizeof(wb.chConfURI) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chChannelID", wb.chChannelID,  sizeof(wb.chChannelID) - 1);
			jfieldID  id_XWBID = env->GetFieldID(class_WBControlMethod, "uXWBID", "I");
			wb.uXWBID = env->GetIntField(obj_WBControlMethod, id_XWBID);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chSizeX", wb.chSizeX,  sizeof(wb.chSizeX) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chSizeY", wb.chSizeY,  sizeof(wb.chSizeY) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chPosX", wb.chPosX,  sizeof(wb.chPosX) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chPosY", wb.chPosY,  sizeof(wb.chPosY) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chWidth", wb.chWidth,  sizeof(wb.chWidth) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chHeight", wb.chHeight,  sizeof(wb.chHeight) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chResolutionX", wb.chResolutionX,  sizeof(wb.chResolutionX) - 1);
			getStringField(env, obj_WBControlMethod, class_WBControlMethod, "chResolutionY", wb.chResolutionY,  sizeof(wb.chResolutionY) - 1);
			jfieldID  id_PageCount = env->GetFieldID(class_WBControlMethod, "iPageCount", "I");
			wb.iPageCount = env->GetIntField(obj_WBControlMethod, id_PageCount);
			//pPageList
			DEBUG_INFO("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			jfieldID id_PageList = env->GetFieldID(class_WBControlMethod, "pPageList", "[Lcom/zte/softda/ocx/conference/wb/WBPage;");
			DEBUG_INFO("id_PageList=%x", id_PageList);
			jobjectArray pPageList = (jobjectArray)env->GetObjectField(obj_WBControlMethod, id_PageList);
        		if (pPageList != NULL)
        		{
            			int i = 0;
				int iPageListSize = env->GetArrayLength(pPageList);
				WbPageListPtr_t pCurPage = NULL;
            			for (i = 0; i < iPageListSize; i++)
            			{
            				if (NULL == wb.pPageList)
            				{
            					// first page
						pCurPage = wb.pPageList = InitWbPageList();
            				}
					else
					{
						pCurPage = (pCurPage->pPageNext = InitWbPageList());
					}

					if (NULL == pCurPage)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitWbPageList failed]");
						return false;
					}

					jobject obj_page = (jobject)env->GetObjectArrayElement(pPageList, i);
					jclass  class_page = env->GetObjectClass(obj_page);

					jfieldID  id_PageNum = env->GetFieldID(class_page, "iPageNum", "I");
					pCurPage->iPageNum= env->GetIntField(obj_page, id_PageNum);
					jfieldID  id_PageOper = env->GetFieldID(class_page, "iPageOper", "I");
					pCurPage->iPageOper= env->GetIntField(obj_page, id_PageOper);					
					jfieldID  id_bActive = env->GetFieldID(class_page, "bActive", "Z");
					pCurPage->bActive= env->GetBooleanField(obj_page, id_bActive);	
					//pShapeList
					jfieldID id_ShapeList = env->GetFieldID(class_page, "shapelist", "[Lcom/zte/softda/ocx/conference/wb/WBShape;");
					jobjectArray pShapeList = (jobjectArray)env->GetObjectField(obj_page, id_ShapeList);
					
					if (pShapeList != NULL)
        				{
            					int j = 0;
						int iShapeListSize = env->GetArrayLength(pShapeList);
						DEBUG_INFO("iShapeListSize=%d", iShapeListSize);
						WbShapeListPtr_t pCurShape = NULL;
            					for (j = 0; j < iShapeListSize; j++)
            					{
            						if (NULL == pCurPage->pShapeList)
            						{
            							// first shape
								pCurShape = pCurPage->pShapeList = InitWbShapeList();
            						}
							else
							{
								pCurShape = (pCurShape->pShapeNext = InitWbShapeList());
							}

							if (NULL == pCurShape)
							{
								LOGI(TAG,"[jni_bSend2MrsMethod InitWbShapeList failed]");
								return false;
							}

							jobject obj_shape = (jobject)env->GetObjectArrayElement(pShapeList, j);
							DEBUG_INFO("obj_shape=%x, j=%d", obj_shape, j);
							jclass  class_shape = env->GetObjectClass(obj_shape);
							DEBUG_INFO("class_shape=%x", class_shape);
							if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/CircleShape")))
							{
								WbCirclePtr_t pCirclePtr = InitWbCircleInfo();
								if (NULL == pCirclePtr)
								{
									return false;
								}
								memset(pCirclePtr, 0, sizeof(WbCircle_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pCirclePtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pCirclePtr->chID,  sizeof(pCirclePtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pCirclePtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								pCirclePtr->style = getWBStyle(env, class_shape, obj_shape);
								//
								getStringField(env, obj_shape, class_shape, "chCx", pCirclePtr->chCx,  sizeof(pCirclePtr->chCx) - 1);
								getStringField(env, obj_shape, class_shape, "chCy", pCirclePtr->chCy,  sizeof(pCirclePtr->chCy) - 1);
								getStringField(env, obj_shape, class_shape, "chR", pCirclePtr->chR,  sizeof(pCirclePtr->chR) - 1);
															
								pCurShape->pShapedata = (void*)pCirclePtr;
								
							}
							else if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/DocPageShape")))
							{
								WbDocPagePtr_t pDocPatePtr = InitWbDocPageInfo();
								if (NULL == pDocPatePtr)
								{
									return false;
								}
								memset(pDocPatePtr, 0, sizeof(WbDocPage_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pDocPatePtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pDocPatePtr->chID,  sizeof(pDocPatePtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pDocPatePtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								//
								pDocPatePtr->pchHref = safeAllocStringUTFchars(env, obj_shape, class_shape, "pChHref");

								pCurShape->pShapedata = (void*)pDocPatePtr;
							}
							else if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/EllipseShape")))
							{
								WbEllipsePtr_t pllipsePtr= InitWbEllipseInfo();
								if (NULL == pllipsePtr)
								{
									return false;
								}
								memset(pllipsePtr, 0, sizeof(WbEllipse_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pllipsePtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pllipsePtr->chID,  sizeof(pllipsePtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pllipsePtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								pllipsePtr->style = getWBStyle(env, class_shape, obj_shape);
								//
								getStringField(env, obj_shape, class_shape, "chCx", pllipsePtr->chCx,  sizeof(pllipsePtr->chCx) - 1);
								getStringField(env, obj_shape, class_shape, "chCy", pllipsePtr->chCy,  sizeof(pllipsePtr->chCy) - 1);
								getStringField(env, obj_shape, class_shape, "chRx", pllipsePtr->chRx,  sizeof(pllipsePtr->chRx) - 1);
								getStringField(env, obj_shape, class_shape, "chRy", pllipsePtr->chRy,  sizeof(pllipsePtr->chRy) - 1);

								pCurShape->pShapedata = (void*)pllipsePtr;
							}
							else if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/ImageShape")))
							{
								WbImagePtr_t pImagePtr= InitWbImageInfo();
								if (NULL == pImagePtr)
								{
									return false;
								}
								memset(pImagePtr, 0, sizeof(WbImage_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pImagePtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pImagePtr->chID,  sizeof(pImagePtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pImagePtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								pImagePtr->pchHref = safeAllocStringUTFchars(env, obj_shape, class_shape, "pChHref");
								//
								getStringField(env, obj_shape, class_shape, "chX", pImagePtr->chX,  sizeof(pImagePtr->chX) - 1);
								getStringField(env, obj_shape, class_shape, "chY", pImagePtr->chY,  sizeof(pImagePtr->chY) - 1);
								getStringField(env, obj_shape, class_shape, "chWidth", pImagePtr->chWidth,  sizeof(pImagePtr->chWidth) - 1);
								getStringField(env, obj_shape, class_shape, "chHeight", pImagePtr->chHeight,  sizeof(pImagePtr->chHeight) - 1);

								pCurShape->pShapedata = (void*)pImagePtr;
							}
							else if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/LineShape")))
							{
								WbLinePtr_t pLinePtr= InitWbLineInfo();
								if (NULL == pLinePtr)
								{
									return false;
								}
								memset(pLinePtr, 0, sizeof(WbLine_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pLinePtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pLinePtr->chID,  sizeof(pLinePtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pLinePtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								pLinePtr->style = getWBStyle(env, class_shape, obj_shape);
								//	
								getStringField(env, obj_shape, class_shape, "chStartX", pLinePtr->chStartX,  sizeof(pLinePtr->chStartX) - 1);
								getStringField(env, obj_shape, class_shape, "chStartY", pLinePtr->chStartY,  sizeof(pLinePtr->chStartY) - 1);
								getStringField(env, obj_shape, class_shape, "chEndX", pLinePtr->chEndX,  sizeof(pLinePtr->chEndX) - 1);
								getStringField(env, obj_shape, class_shape, "chEndY", pLinePtr->chEndY,  sizeof(pLinePtr->chEndY) - 1);

								pCurShape->pShapedata = (void*)pLinePtr;
							}
							else if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/PathShape")))
							{
								WbPathPtr_t pPathPtr= InitWbPathInfo();
								if (NULL == pPathPtr)
								{
									return false;
								}
								memset(pPathPtr, 0, sizeof(WbPath_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pPathPtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pPathPtr->chID,  sizeof(pPathPtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pPathPtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								pPathPtr->style = getWBStyle(env, class_shape, obj_shape);
								//			
								pPathPtr->pchPoint = safeAllocStringUTFchars(env, obj_shape, class_shape, "pchPoint");
								DEBUG_INFO("pPathPtr->iObjectOper=%d, pPathPtr->pchPoint=%s", pPathPtr->iObjectOper, pPathPtr->pchPoint);
								pCurShape->pShapedata = (void*)pPathPtr;
							}
							else if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/RectShape")))
							{
								WbRectPtr_t pRectPtr = InitWbRectInfo();
								if (NULL == pRectPtr)
								{
									return false;
								}
								memset(pRectPtr, 0, sizeof(WbRect_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pRectPtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pRectPtr->chID,  sizeof(pRectPtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pRectPtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								pRectPtr->style = getWBStyle(env, class_shape, obj_shape);
								//
								getStringField(env, obj_shape, class_shape, "chX", pRectPtr->chX,  sizeof(pRectPtr->chX) - 1);
								getStringField(env, obj_shape, class_shape, "chY", pRectPtr->chY,  sizeof(pRectPtr->chY) - 1);
								getStringField(env, obj_shape, class_shape, "chWidth", pRectPtr->chWidth,  sizeof(pRectPtr->chWidth) - 1);
								getStringField(env, obj_shape, class_shape, "chHeight", pRectPtr->chHeight,  sizeof(pRectPtr->chHeight) - 1);
								getStringField(env, obj_shape, class_shape, "chRx", pRectPtr->chRx,  sizeof(pRectPtr->chRx) - 1);
								getStringField(env, obj_shape, class_shape, "chRy", pRectPtr->chRy,  sizeof(pRectPtr->chRy) - 1);

								pCurShape->pShapedata = (void*)pRectPtr;
							}
							else if (env->IsInstanceOf(obj_shape, env->FindClass("com/zte/softda/ocx/conference/wb/TextShape")))
							{
								WbTextPtr_t pTextPtr = InitWbTextInfo();
								if (NULL == pTextPtr)
								{
									return false;
								}							
								memset(pTextPtr, 0, sizeof(WbText_t));
								//base info
								jfieldID  id_enumShapeType = env->GetFieldID(class_shape, "enumShapeType", "I");
								pTextPtr->enumShapeType= (WB_SHAPE_TYPE)env->GetIntField(obj_shape, id_enumShapeType);
								getStringField(env, obj_shape, class_shape, "chID", pTextPtr->chID,  sizeof(pTextPtr->chID) - 1);
								jfieldID  id_iObjectOper = env->GetFieldID(class_shape, "iObjectOper", "I");
								pTextPtr->iObjectOper= env->GetIntField(obj_shape, id_iObjectOper);
								pTextPtr->style = getWBStyle(env, class_shape, obj_shape);
								//		
								getStringField(env, obj_shape, class_shape, "chX", pTextPtr->chX,  sizeof(pTextPtr->chX) - 1);
								getStringField(env, obj_shape, class_shape, "chY", pTextPtr->chY,  sizeof(pTextPtr->chY) - 1);
								getStringField(env, obj_shape, class_shape, "chWidth", pTextPtr->chWidth,  sizeof(pTextPtr->chWidth) - 1);
								getStringField(env, obj_shape, class_shape, "chHeight", pTextPtr->chHeight,  sizeof(pTextPtr->chHeight) - 1);
								pTextPtr->pchText = safeAllocStringUTFchars(env, obj_shape, class_shape, "pchText");

								pCurShape->pShapedata = (void*)pTextPtr;
            					}
					}
            			}
       		}
		}

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&wb);
			FreeWbPageListMem(wb.pPageList); 
        }
		break;
           case 2:
		{
			DOC_SHARE_METHOD docshare = {0};

			jfieldID id_docControlMethod = env->GetFieldID(class_bMrsMethodPara, "docshareCtrlMethod", "Lcom/zte/softda/ocx/conference/docshare/DocshareControlMethod;");
        		jobject obj_docControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_docControlMethod);
        		jclass class_docControlMethod = env->GetObjectClass(obj_docControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_docControlMethod, "iType", "I");
			docshare.iType = env->GetIntField(obj_docControlMethod, id_Type);
			getStringField(env, obj_docControlMethod, class_docControlMethod, "chConfURI", docshare.chConfURI,  sizeof(docshare.chConfURI) - 1);
			getStringField(env, obj_docControlMethod, class_docControlMethod, "chChannelID", docshare.chChannelID,  sizeof(docshare.chChannelID) - 1);
			jfieldID  id_DocShareID = env->GetFieldID(class_docControlMethod, "uDocShareID", "I");
			docshare.uDocShareID = env->GetIntField(obj_docControlMethod, id_DocShareID);
			jfieldID  id_DownID = env->GetFieldID(class_docControlMethod, "uDownID", "I");
			docshare.uDownID = env->GetIntField(obj_docControlMethod, id_DownID);
			jfieldID  id_Reply = env->GetFieldID(class_docControlMethod, "iReply", "I");
			docshare.iReply = env->GetIntField(obj_docControlMethod, id_Reply);
			jfieldID  id_DocType = env->GetFieldID(class_docControlMethod, "iDocType", "I");
			docshare.iDocType = env->GetIntField(obj_docControlMethod, id_DocType);
			getStringField(env, obj_docControlMethod, class_docControlMethod, "chDocName", docshare.chDocName,  sizeof(docshare.chDocName) - 1);
			getStringField(env, obj_docControlMethod, class_docControlMethod, "chReceiverURI", docshare.chReceiverURI,  sizeof(docshare.chReceiverURI) - 1);
			getStringField(env, obj_docControlMethod, class_docControlMethod, "chPageInfo", docshare.chPageInfo,  sizeof(docshare.chPageInfo) - 1);

			//pFileList
			DEBUG_INFO("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			jfieldID id_FileList = env->GetFieldID(class_docControlMethod, "pFileList", "[Lcom/zte/softda/ocx/conference/commonfile/CommonFileList;");
			DEBUG_INFO("id_FileList=%x", id_FileList);
			jobjectArray pFileList = (jobjectArray)env->GetObjectField(obj_docControlMethod, id_FileList);
        		if (pFileList != NULL)
        		{
            			int i = 0;
				int iFileListSize = env->GetArrayLength(pFileList);
				FileListPtr_t pCurFile = NULL;
            			for (i = 0; i < iFileListSize; i++)
            			{
            				if (NULL == docshare.pFileList)
            				{
            					// first page
						pCurFile = docshare.pFileList = InitFileList();
            				}
					else
					{
						pCurFile = (pCurFile->pFileNext = InitFileList());
					}

					if (NULL == pCurFile)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitFileList failed]");
						return false;
					}

					jobject obj_File = (jobject)env->GetObjectArrayElement(pFileList, i);
					jclass  class_File = env->GetObjectClass(obj_File);

					getStringField(env, obj_File, class_File, "chFileName", pCurFile->chFileName,  sizeof(pCurFile->chFileName) - 1);
					jfieldID  id_Size = env->GetFieldID(class_File, "iSize", "I");
					pCurFile->iSize= env->GetIntField(obj_File, id_Size);
					getStringField(env, obj_File, class_File, "chLastModiTime", pCurFile->chLastModiTime,  sizeof(pCurFile->chLastModiTime) - 1);
					getStringField(env, obj_File, class_File, "chURL", pCurFile->chURL,  sizeof(pCurFile->chURL) - 1);
					jfieldID  id_Cache = env->GetFieldID(class_File, "bCache", "Z");
					pCurFile->bCache= env->GetBooleanField(obj_File, id_Cache);			
       		                 }
		            }

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&docshare);
			FreeDocFileListMem(docshare.pFileList);
              }
		break;
              case 3:
		{
			WEB_SHARE_METHOD webshare = {0};

			jfieldID id_webControlMethod = env->GetFieldID(class_bMrsMethodPara, "webshareCtrlMethod", "Lcom/zte/softda/ocx/conference/webshare/WebshareControlMethod;");
        		jobject obj_webControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_webControlMethod);
        		jclass class_webControlMethod = env->GetObjectClass(obj_webControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_webControlMethod, "iType", "I");
			webshare.iType = env->GetIntField(obj_webControlMethod, id_Type);
			getStringField(env, obj_webControlMethod, class_webControlMethod, "chConfURI", webshare.chConfURI,  sizeof(webshare.chConfURI) - 1);
			getStringField(env, obj_webControlMethod, class_webControlMethod, "chChannelID", webshare.chChannelID,  sizeof(webshare.chChannelID) - 1);
			jfieldID  id_WebShareID = env->GetFieldID(class_webControlMethod, "uWebShareID", "I");
			webshare.uWebShareID = env->GetIntField(obj_webControlMethod, id_WebShareID);
			getStringField(env, obj_webControlMethod, class_webControlMethod, "chTabID", webshare.chTabID,  sizeof(webshare.chTabID) - 1);
			getStringField(env, obj_webControlMethod, class_webControlMethod, "chURL", webshare.chURL,  sizeof(webshare.chURL) - 1);
			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&webshare);
              }
		break;
            case 4:
		{
			FILE_SHARE_METHOD fileshare = {0};

			jfieldID id_fileshareControlMethod = env->GetFieldID(class_bMrsMethodPara, "fileshareCtrlMethod", "Lcom/zte/softda/ocx/conference/fileshare/FileshareControlMethod;");
        		jobject obj_fileshareControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_fileshareControlMethod);
        		jclass class_fileshareControlMethod = env->GetObjectClass(obj_fileshareControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_fileshareControlMethod, "iType", "I");
			fileshare.iType = env->GetIntField(obj_fileshareControlMethod, id_Type);
			getStringField(env, obj_fileshareControlMethod, class_fileshareControlMethod, "chConfURI", fileshare.chConfURI,  sizeof(fileshare.chConfURI) - 1);
			getStringField(env, obj_fileshareControlMethod, class_fileshareControlMethod, "chChannelID", fileshare.chChannelID,  sizeof(fileshare.chChannelID) - 1);
			jfieldID  id_FileShareID = env->GetFieldID(class_fileshareControlMethod, "uFileShareID", "I");
			fileshare.uFileShareID = env->GetIntField(obj_fileshareControlMethod, id_FileShareID);
			getStringField(env, obj_fileshareControlMethod, class_fileshareControlMethod, "chPath", fileshare.chPath,  sizeof(fileshare.chPath) - 1);

			//pFileList
			DEBUG_INFO("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			jfieldID id_FileList = env->GetFieldID(class_fileshareControlMethod, "pFileList", "[Lcom/zte/softda/ocx/conference/commonfile/CommonFileList;");
			DEBUG_INFO("id_FileList=%x", id_FileList);
			jobjectArray pFileList = (jobjectArray)env->GetObjectField(obj_fileshareControlMethod, id_FileList);
        		if (pFileList != NULL)
        		{
            			int i = 0;
				int iFileListSize = env->GetArrayLength(pFileList);
				FileListPtr_t pCurFile = NULL;
            			for (i = 0; i < iFileListSize; i++)
            			{
            				if (NULL == fileshare.pFileList)
            				{
            					// first page
						pCurFile = fileshare.pFileList = InitFileList();
            				}
					else
					{
						pCurFile = (pCurFile->pFileNext = InitFileList());
					}

					if (NULL == pCurFile)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitFileList failed]");
						return false;
					}

					jobject obj_File = (jobject)env->GetObjectArrayElement(pFileList, i);
					jclass  class_File = env->GetObjectClass(obj_File);

					getStringField(env, obj_File, class_File, "chFileName", pCurFile->chFileName,  sizeof(pCurFile->chFileName) - 1);
					jfieldID  id_Size = env->GetFieldID(class_File, "iSize", "I");
					pCurFile->iSize= env->GetIntField(obj_File, id_Size);
					getStringField(env, obj_File, class_File, "chLastModiTime", pCurFile->chLastModiTime,  sizeof(pCurFile->chLastModiTime) - 1);
					getStringField(env, obj_File, class_File, "chURL", pCurFile->chURL,  sizeof(pCurFile->chURL) - 1);
					jfieldID  id_Cache = env->GetFieldID(class_File, "bCache", "Z");
					pCurFile->bCache= env->GetBooleanField(obj_File, id_Cache);
        				
       		                 }
		            }

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&fileshare);
			FreeDocFileListMem(fileshare.pFileList);
              }
		break;
              case 5:
		{
			VOTE_METHOD vote = {0};

			jfieldID id_voteControlMethod = env->GetFieldID(class_bMrsMethodPara, "voteCtrlMethod", "Lcom/zte/softda/ocx/conference/vote/VoteControlMethod;");
        		jobject obj_voteControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_voteControlMethod);
        		jclass class_voteControlMethod = env->GetObjectClass(obj_voteControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_voteControlMethod, "iType", "I");
			vote.iType = env->GetIntField(obj_voteControlMethod, id_Type);
			getStringField(env, obj_voteControlMethod, class_voteControlMethod, "chConfURI", vote.chConfURI,  sizeof(vote.chConfURI) - 1);
			getStringField(env, obj_voteControlMethod, class_voteControlMethod, "chChannelID", vote.chChannelID,  sizeof(vote.chChannelID) - 1);
			jfieldID  id_VoteID = env->GetFieldID(class_voteControlMethod, "uVoteID", "I");
			vote.uVoteID = env->GetIntField(obj_voteControlMethod, id_VoteID);
			getStringField(env, obj_voteControlMethod, class_voteControlMethod, "chVoterURI", vote.chVoterURI,  sizeof(vote.chVoterURI) - 1);
			getStringField(env, obj_voteControlMethod, class_voteControlMethod, "chTitle", vote.chTitle,  sizeof(vote.chTitle) - 1);
			jfieldID  id_Duration = env->GetFieldID(class_voteControlMethod, "iDuration", "I");
			vote.iDuration = env->GetIntField(obj_voteControlMethod, id_Duration);
			jfieldID  id_DisplayMode = env->GetFieldID(class_voteControlMethod, "iDisplayMode", "I");
			vote.iDisplayMode = env->GetIntField(obj_voteControlMethod, id_DisplayMode);
			jfieldID  id_Secret = env->GetFieldID(class_voteControlMethod, "bSecret", "Z");
			vote.bSecret= env->GetBooleanField(obj_voteControlMethod, id_Secret);

			//pQuestionList
			DEBUG_INFO("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			jfieldID id_QuestionList = env->GetFieldID(class_voteControlMethod, "pQuestionList", "[Lcom/zte/softda/ocx/conference/vote/VoteQuestion;");
			DEBUG_INFO("id_QuestionList=%x", id_QuestionList);
			jobjectArray pQuestionList = (jobjectArray)env->GetObjectField(obj_voteControlMethod, id_QuestionList);
        		if (pQuestionList != NULL)
        		{
            			int i = 0;
				int iQuestionListSize = env->GetArrayLength(pQuestionList);
				VoteQuestionListPtr_t pCurQuestion = NULL;
            			for (i = 0; i < iQuestionListSize; i++)
            			{
            				if (NULL == vote.pQuestionList)
            				{
            					// first page
						pCurQuestion = vote.pQuestionList = InitVoteQuestionList();
            				}
					else
					{
						pCurQuestion = (pCurQuestion->pQuestionNext = InitVoteQuestionList());
					}

					if (NULL == pCurQuestion)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitVoteQuestionList failed]");
						return false;
					}

					jobject obj_Question = (jobject)env->GetObjectArrayElement(pQuestionList, i);
					jclass  class_Question = env->GetObjectClass(obj_Question);
					jfieldID  id_QuestionID = env->GetFieldID(class_Question, "iQuestionID", "I");
					pCurQuestion->iQuestionID= env->GetIntField(obj_Question, id_QuestionID);
					pCurQuestion->pchCaption = safeAllocStringUTFchars(env, obj_Question, class_Question, "pchCaption");
					jfieldID  id_bType = env->GetFieldID(class_Question, "bType", "Z");
			               pCurQuestion->bType= env->GetBooleanField(obj_Question, id_bType);
						   
					jfieldID id_pchChoiceArray = env->GetFieldID(class_Question, "pchChoice", "[Ljava/lang/String;");
                                    jobjectArray pchChoiceArray = (jobjectArray)env->GetObjectField(obj_Question, id_pchChoiceArray);
                                    if (pchChoiceArray != NULL)
                                   {
                                      int iArrayLen = 0;
					  iArrayLen = 	env->GetArrayLength(pchChoiceArray);		  
                                      int j;
                                      for (j = 0; j < iArrayLen; j++)
                                     {
                                        jstring jstr_pchChoiceArray = (jstring)env->GetObjectArrayElement(pchChoiceArray, j);

                                        if (jstr_pchChoiceArray != NULL)
                                       {
                                          int size = env->GetStringUTFLength(jstr_pchChoiceArray);

                                          pCurQuestion->pchChoice[j] = (char*)ZX_malloc(size + 1);
                                          memset(pCurQuestion->pchChoice[j], 0, size + 1);
                                          safeGetStringUTFChars(env, jstr_pchChoiceArray, pCurQuestion->pchChoice[j], size, NULL);
                                        }
                                   }
                                  }
       		                 }
		       }

			//pReplyList
			DEBUG_INFO("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			jfieldID id_ReplyList = env->GetFieldID(class_voteControlMethod, "pReplyList", "[Lcom/zte/softda/ocx/conference/vote/VoteReply;");
			DEBUG_INFO("id_ReplyList=%x", id_ReplyList);
			jobjectArray pReplyList = (jobjectArray)env->GetObjectField(obj_voteControlMethod, id_ReplyList);
        		if (pReplyList != NULL)
        		{
            			int i = 0;
				int iReplyListSize = env->GetArrayLength(pReplyList);
				VoteReplyListPtr_t pCurReply = NULL;
            			for (i = 0; i < iReplyListSize; i++)
            			{
            				if (NULL == vote.pReplyList)
            				{
            					// first page
						pCurReply = vote.pReplyList = InitVoteReplyList();
            				}
					else
					{
						pCurReply = (pCurReply->pReplyNext = InitVoteReplyList());
					}

					if (NULL == pCurReply)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitVoteReplyList failed]");
						return false;
					}

					jobject obj_Reply = (jobject)env->GetObjectArrayElement(pReplyList, i);
					jclass  class_Reply = env->GetObjectClass(obj_Reply);
					jfieldID  id_ReplyID = env->GetFieldID(class_Reply, "iReplyID", "I");
					pCurReply->iReplyID= env->GetIntField(obj_Reply, id_ReplyID);
					jfieldID  id_iChooseArray = env->GetFieldID(class_Reply, "iChoose", "[I");
                                    jintArray iChooseArray = (jintArray)env->GetObjectField(obj_Reply, id_iChooseArray);
                                    if (iChooseArray != NULL)
                                   {
                                      jint *jiChooseArray = env->GetIntArrayElements(iChooseArray, 0);
					  int iChooseArrayLen = 0;
					  iChooseArrayLen = env->GetArrayLength(iChooseArray);
                                      for (int k = 0;k < iChooseArrayLen;k++)
                                      {

                                       pCurReply->iChoose[k] = jiChooseArray[k];   
                                      }
                                    }
					jfieldID  id_bType = env->GetFieldID(class_Reply, "bType", "Z");
			               pCurReply->bType= env->GetBooleanField(obj_Reply, id_bType);	
       		                 }
		       }
			//pResultList
			DEBUG_INFO("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			jfieldID id_ResultList = env->GetFieldID(class_voteControlMethod, "pResultList", "[Lcom/zte/softda/ocx/conference/vote/VoteResult;");
			DEBUG_INFO("id_ResultList=%x", id_ResultList);
			jobjectArray pResultList = (jobjectArray)env->GetObjectField(obj_voteControlMethod, id_ResultList);
        		if (pResultList != NULL)
        		{
            			int i = 0;
				int iResultListSize = env->GetArrayLength(pResultList);
				VoteResultListPtr_t pCurResult = NULL;
            			for (i = 0; i < iResultListSize; i++)
            			{
            				if (NULL == vote.pResultList)
            				{
            					// first page
						pCurResult = vote.pResultList = InitVoteResultList();
            				}
					else
					{
						pCurResult = (pCurResult->pResultNext = InitVoteResultList());
					}

					if (NULL == pCurResult)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitVoteResultList failed]");
						return false;
					}

					jobject obj_Result = (jobject)env->GetObjectArrayElement(pQuestionList, i);
					jclass  class_Result = env->GetObjectClass(obj_Result);
					jfieldID  id_ResultID = env->GetFieldID(class_Result, "iResultID", "I");
					pCurResult->iResultID= env->GetIntField(obj_Result, id_ResultID);
					jfieldID  id_iSumArray = env->GetFieldID(class_Result, "iSum", "[I");
                                    jintArray iSumArray = (jintArray)env->GetObjectField(obj_Result, id_iSumArray);

                                    if (iSumArray != NULL)
                                   {
                                        jint *jiSumArray = env->GetIntArrayElements(iSumArray, 0);
					    int iSumArrayLen = 0;
					    iSumArrayLen = env->GetArrayLength(iSumArray);
                                        for (int m = 0; m < iSumArrayLen; m++)
                                        {

                                            pCurResult->iSum[m] = jiSumArray[m]; 
                                        }
                                    }
					jfieldID  id_Votesum = env->GetFieldID(class_Result, "iVotesum", "I");
					pCurResult->iVotesum= env->GetIntField(obj_Result, id_Votesum);
					jfieldID  id_Rejectsum = env->GetFieldID(class_Result, "iRejectsum", "I");
					pCurResult->iRejectsum= env->GetIntField(obj_Result, id_Rejectsum);
       		                 }
		            }

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&vote);
			FreeVoteQuestionListMem(vote.pQuestionList);
			FreeVoteReplyListMem(vote.pReplyList);
                      FreeVoteResultListMem(vote.pResultList);
              }
		break;
              case 6:
		{
			RAISE_HAND_METHOD hand = {0};

			jfieldID id_handControlMethod = env->GetFieldID(class_bMrsMethodPara, "raisehandCtrlMethod", "Lcom/zte/softda/ocx/conference/raisehand/RaisehandControlMethod;");
        		jobject obj_handControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_handControlMethod);
        		jclass class_handControlMethod = env->GetObjectClass(obj_handControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_handControlMethod, "iType", "I");
			hand.iType = env->GetIntField(obj_handControlMethod, id_Type);
			getStringField(env, obj_handControlMethod, class_handControlMethod, "chConfURI", hand.chConfURI,  sizeof(hand.chConfURI) - 1);
			getStringField(env, obj_handControlMethod, class_handControlMethod, "chChannelID", hand.chChannelID,  sizeof(hand.chChannelID) - 1);
			jfieldID  id_RaiseHandID = env->GetFieldID(class_handControlMethod, "uRaiseHandID", "I");
			hand.uRaiseHandID = env->GetIntField(obj_handControlMethod, id_RaiseHandID);
			getStringField(env, obj_handControlMethod, class_handControlMethod, "chTitle", hand.chTitle,  sizeof(hand.chTitle) - 1);
			jfieldID  id_Duration = env->GetFieldID(class_handControlMethod, "iDuration", "I");
			hand.iDuration = env->GetIntField(obj_handControlMethod, id_Duration);
			hand.pchResultNotify = safeAllocStringUTFchars(env, obj_handControlMethod, class_handControlMethod, "pchResultNotify");
			jfieldID  id_ReplyType = env->GetFieldID(class_handControlMethod, "iReplyType", "I");
			hand.iReplyType = env->GetIntField(obj_handControlMethod, id_ReplyType);
			jfieldID  id_ReplyYes = env->GetFieldID(class_handControlMethod, "iReplyYes", "I");
			hand.iReplyYes = env->GetIntField(obj_handControlMethod, id_ReplyYes);
			jfieldID  id_ReplyNo = env->GetFieldID(class_handControlMethod, "iReplyNo", "I");
			hand.iReplyNo = env->GetIntField(obj_handControlMethod, id_ReplyNo);
			jfieldID  id_ReplyCancel = env->GetFieldID(class_handControlMethod, "iReplyCancel", "I");
			hand.iReplyCancel = env->GetIntField(obj_handControlMethod, id_ReplyCancel);

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&hand);
			FreeRaiseHandMem(hand.pchResultNotify);
              }
		break;
              case 7:
		{
			AGENDA agenda = {0};

			jfieldID id_agendaControlMethod = env->GetFieldID(class_bMrsMethodPara, "agendaCtrlMethod", "Lcom/zte/softda/ocx/conference/agenda/AgendaControlMethod;");
        		jobject obj_agendaControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_agendaControlMethod);
        		jclass class_agendaControlMethod = env->GetObjectClass(obj_agendaControlMethod);
			
			getStringField(env, obj_agendaControlMethod, class_agendaControlMethod, "chConfURI", agenda.chConfURI,  sizeof(agenda.chConfURI) - 1);
			getStringField(env, obj_agendaControlMethod, class_agendaControlMethod, "chChannelID", agenda.chChannelID,  sizeof(agenda.chChannelID) - 1);
			jfieldID  id_AgendaID = env->GetFieldID(class_agendaControlMethod, "uAgendaID", "I");
			agenda.uAgendaID = env->GetIntField(obj_agendaControlMethod, id_AgendaID);
			getStringField(env, obj_agendaControlMethod, class_agendaControlMethod, "chTitle", agenda.chTitle,  sizeof(agenda.chTitle) - 1);
			getStringField(env, obj_agendaControlMethod, class_agendaControlMethod, "chStartTime", agenda.chStartTime,  sizeof(agenda.chStartTime) - 1);
			getStringField(env, obj_agendaControlMethod, class_agendaControlMethod, "chEndTime", agenda.chEndTime,  sizeof(agenda.chEndTime) - 1);

			//pAgendaItemList
			DEBUG_INFO("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			jfieldID id_AgendaItemList = env->GetFieldID(class_agendaControlMethod, "pAgendaItemList", "[Lcom/zte/softda/ocx/conference/agenda/AgendaItem;");
			DEBUG_INFO("id_AgendaItemList=%x", id_AgendaItemList);
			jobjectArray pAgendaItemList = (jobjectArray)env->GetObjectField(obj_agendaControlMethod, id_AgendaItemList);
        		if (pAgendaItemList != NULL)
        		{
            			int i = 0;
				int iAgendaItemListSize = env->GetArrayLength(pAgendaItemList);
				AgendaItemListPtr_t pCurAgendaItem = NULL;
            			for (i = 0; i < iAgendaItemListSize; i++)
            			{
            				if (NULL == agenda.pAgendaItemList)
            				{
            					// first page
						agenda.pAgendaItemList = InitAgendaItemList();
						pCurAgendaItem = agenda.pAgendaItemList;
            				}
					else
					{
						pCurAgendaItem->pItemNext = InitAgendaItemList();
						pCurAgendaItem = pCurAgendaItem->pItemNext;
					}

					if (NULL == pCurAgendaItem)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitAgendaItemList failed]");
						return false;
					}

					jobject obj_AgendaItem = (jobject)env->GetObjectArrayElement(pAgendaItemList, i);
					jclass  class_AgendaItem = env->GetObjectClass(obj_AgendaItem);

                                    jfieldID  id_ItemID = env->GetFieldID(class_AgendaItem, "iItemID", "I");
					pCurAgendaItem->iItemID= env->GetIntField(obj_AgendaItem, id_ItemID);
					jfieldID  id_Status = env->GetFieldID(class_AgendaItem, "iStatus", "I");
					pCurAgendaItem->iStatus= env->GetIntField(obj_AgendaItem, id_Status);
					getStringField(env, obj_AgendaItem, class_AgendaItem, "chStartTime", pCurAgendaItem->chStartTime,  sizeof(pCurAgendaItem->chStartTime) - 1);
					getStringField(env, obj_AgendaItem, class_AgendaItem, "chEndTime", pCurAgendaItem->chEndTime,  sizeof(pCurAgendaItem->chEndTime) - 1);
					pCurAgendaItem->pchCaption = safeAllocStringUTFchars(env, obj_AgendaItem, class_AgendaItem, "pchCaption");
       		                 }
		            }

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&agenda);
			FreeAgendaItemMethodMem(agenda.pAgendaItemList);
              }
		break;
              case 8:
		{
			FILE_TRANS_METHOD filetrans = {0};

			jfieldID id_filetransControlMethod = env->GetFieldID(class_bMrsMethodPara, "filetransCtrlMethod", "Lcom/zte/softda/ocx/conference/filetrans/FiletransControlMethod;");
        		jobject obj_filetransControlMethod = env->GetObjectField(jobj_bMrsMethodPara, id_filetransControlMethod);
        		jclass class_filetransControlMethod = env->GetObjectClass(obj_filetransControlMethod);
				
			jfieldID  id_Type = env->GetFieldID(class_filetransControlMethod, "iType", "I");
			filetrans.iType = env->GetIntField(obj_filetransControlMethod, id_Type);
			getStringField(env, obj_filetransControlMethod, class_filetransControlMethod, "chConfURI", filetrans.chConfURI,  sizeof(filetrans.chConfURI) - 1);
			getStringField(env, obj_filetransControlMethod, class_filetransControlMethod, "chChannelID", filetrans.chChannelID,  sizeof(filetrans.chChannelID) - 1);
			jfieldID  id_FileTransID = env->GetFieldID(class_filetransControlMethod, "uFileTransID", "I");
			filetrans.uFileTransID = env->GetIntField(obj_filetransControlMethod, id_FileTransID);
			jfieldID  id_Reply = env->GetFieldID(class_filetransControlMethod, "iReply", "I");
			filetrans.iReply = env->GetIntField(obj_filetransControlMethod, id_Reply);
			jfieldID  id_DownResult = env->GetFieldID(class_filetransControlMethod, "iDownResult", "I");
			filetrans.iDownResult = env->GetIntField(obj_filetransControlMethod, id_DownResult);
			jfieldID  id_DownID = env->GetFieldID(class_filetransControlMethod, "uDownID", "I");
			filetrans.uDownID = env->GetIntField(obj_filetransControlMethod, id_DownID);
			jfieldID id_pchReceiverURI= env->GetFieldID(class_filetransControlMethod, "pchReceiverURI", "[Ljava/lang/String;");
			int iArrayLen = 0;
                      jobjectArray pchReceiverURIArray  = (jobjectArray)env->GetObjectField(obj_filetransControlMethod, id_pchReceiverURI);
                      if (pchReceiverURIArray != NULL)
                      {
                           
                           iArrayLen = env->GetArrayLength(pchReceiverURIArray);
                           int j;
                           for (j = 0; j < iArrayLen;j++)
                           {
                                 jstring jstr_pchReceiverURIArray = (jstring)env->GetObjectArrayElement(pchReceiverURIArray, j);
                                 if (jstr_pchReceiverURIArray != NULL)
                                 {
                                      int size = env->GetStringUTFLength(jstr_pchReceiverURIArray);

                                      filetrans.pchReceiverURI[j] = (char*)ZX_malloc(size + 1);
                                      memset(filetrans.pchReceiverURI[j], 0, size + 1);
                                      safeGetStringUTFChars(env, jstr_pchReceiverURIArray, filetrans.pchReceiverURI[j], size, NULL);
					  DEBUG_INFO("jni_bIMSSend2MrsMethodfiletrans.pchReceiverURI[%s]",filetrans.pchReceiverURI[j]);
                                  }
                             }
                       }

			//pFileList
			jfieldID id_FileList = env->GetFieldID(class_filetransControlMethod, "pFileList", "[Lcom/zte/softda/ocx/conference/commonfile/CommonFileList;");
			DEBUG_INFO("id_FileList=%x", id_FileList);
			jobjectArray pFileList = (jobjectArray)env->GetObjectField(obj_filetransControlMethod, id_FileList);
        		if (pFileList != NULL)
        		{
            			int i = 0;
				int iFileListSize = env->GetArrayLength(pFileList);
				FileListPtr_t pCurFile = NULL;
            			for (i = 0; i < iFileListSize; i++)
            			{
            				if (NULL == filetrans.pFileList)
            				{
            					// first page
						pCurFile = filetrans.pFileList = InitFileList();
            				}
					else
					{
						pCurFile = (pCurFile->pFileNext = InitFileList());
					}

					if (NULL == pCurFile)
					{
						LOGI(TAG,"[jni_bSend2MrsMethod InitFileList failed]");
						return false;
					}

					jobject obj_File = (jobject)env->GetObjectArrayElement(pFileList, i);
					jclass  class_File = env->GetObjectClass(obj_File);

					getStringField(env, obj_File, class_File, "chFileName", pCurFile->chFileName,  sizeof(pCurFile->chFileName) - 1);
					jfieldID  id_Size = env->GetFieldID(class_File, "iSize", "I");
					pCurFile->iSize= env->GetIntField(obj_File, id_Size);
					getStringField(env, obj_File, class_File, "chLastModiTime", pCurFile->chLastModiTime,  sizeof(pCurFile->chLastModiTime) - 1);
					getStringField(env, obj_File, class_File, "chURL", pCurFile->chURL,  sizeof(pCurFile->chURL) - 1);
					jfieldID  id_Cache = env->GetFieldID(class_File, "bCache", "Z");
					pCurFile->bCache= env->GetBooleanField(obj_File, id_Cache);
        				
       		                 }
		            }

			//call ocx method
			bIMSSend2MrsMethod(iType, (short far*)&filetrans);
			for (int i = 0; i < iArrayLen; i++)
	              {
		           if(filetrans.pchReceiverURI[i])
		           {
			         ZX_free(filetrans.pchReceiverURI[i]);
				 filetrans.pchReceiverURI[i] = NULL;
		           }		
	              }
			
			FreeFileTransMethodMem(filetrans.pFileList);
              }
		break;
		default:
            {
			return FALSE;
        }
		break;
    }

    

	return TRUE;
}

static void jni_setFileSocktPath(JNIEnv* env, jclass cls, jstring path)
{
	char szPath[128] = "";
	safeGetStringUTFChars(env, path, szPath, 128, false);
	xcomm_set_tmp_path(szPath);
}


static void jni_bGetAllIMPrivateGroupList(JNIEnv* env, jclass cls)
{
	bGetAllIMPrivateGroupList();
}

static void jni_bGetIMPrivateGroupMember(JNIEnv* env, jclass cls, jstring sGroupIdentifier)
{
	TCHAR cGroupIdentifier[MAX_IDENTIFIER_LEN+1] = {0};

	safeGetStringUTFChars(env, sGroupIdentifier, cGroupIdentifier, sizeof(cGroupIdentifier) - 1, NULL);
	bGetIMPrivateGroupMember(cGroupIdentifier);
}

static void  jni_bSendSoapMessageByXcap (JNIEnv* env, jclass cls, jint iType, jstring jstrRequestLine, jstring jstrAction, jstring jstrBody)
{
	char cRequestLine[MAX_XCAP_HEAD_LEN+1] = {0};
	char cAction[MAX_XCAP_HEAD_LEN+1] = {0};

	char *pBody = safeAllocStringUTFCharsFromJString(env, jstrBody);
	safeGetStringUTFChars(env, jstrRequestLine, cRequestLine, sizeof(cRequestLine) - 1, NULL);
	safeGetStringUTFChars(env, jstrAction, cAction, sizeof(cAction) - 1, NULL);

	bSendSoapMessageByXcap(iType, cRequestLine, cAction, pBody);
}

static void   jni_bCheckURIIsMOA(JNIEnv* env, jclass cls, jstring jstrURI)
{
	char cURI[MAX_IMS_MESSAGE_LEN+1] = {0};
	safeGetStringUTFChars(env, jstrURI, cURI, sizeof(cURI) - 1, NULL);
	bCheckURIIsMOA(cURI);
}

static jboolean jni_bSendPubAccountReq(JNIEnv* env, jclass cls, jint iType, jstring jstrPubAccId)
{//iType: 1-��ѯ�Լ���ע�Ĺ��ں�, 2--��ע���ں� 3--ȡ����ע���ں�
    char cPubAccId[40] = {0};
    safeGetStringUTFChars(env, jstrPubAccId, cPubAccId, sizeof(cPubAccId) - 1, NULL);
    return bSendPubAccountReq(iType, cPubAccId);
}

static jboolean jni_bGetPubAccountMenu(JNIEnv* env, jclass cls, jstring jstrPubAccId, jstring jstrEtag)
{
    char cPubAccId[40] = {0};
    char cEtag[30] = {0};
    safeGetStringUTFChars(env, jstrPubAccId, cPubAccId, sizeof(cPubAccId) - 1, NULL);
    safeGetStringUTFChars(env, jstrEtag, cEtag, sizeof(cEtag) - 1, NULL);
    return bGetPubAccountMenu(cPubAccId, cEtag);
}

static jboolean jni_bGetHistoryPubMsg(JNIEnv* env, jclass cls, jint count, jstring jstrPubAccId, jint seq, jstring jstrFlag)
{
    char cPubAccId[40] = {0};
    char cFlag[30] = {0};
    
	safeGetStringUTFChars(env, jstrPubAccId, cPubAccId, sizeof(cPubAccId) - 1, NULL);
    safeGetStringUTFChars(env, jstrFlag, cFlag, sizeof(cFlag) - 1, NULL);    
    return bGetHistoryPubMsg(count, cPubAccId, seq, cFlag);
}

static jboolean jni_bSearchPubAccount(JNIEnv* env, jclass cls, jint iType, jstring jstrPubAccName, jint pageNo, jint pageSize)
{//iType: 1-ͨ�����ں�����ģ����ѯ��2-ͨ�����ں�ID��ѯ, 3-���չ�ע������ѯ��4-���մ���ʱ���ѯ
    char strPubAccName[256] = {0};
  
	safeGetStringUTFChars(env, jstrPubAccName, strPubAccName, sizeof(strPubAccName) - 1, NULL); 
    return bSearchPubAccount(iType, strPubAccName, pageNo, pageSize);
}

static jboolean jni_bGetRecommendContact(JNIEnv* env, jclass cls, jint pageNo, jint pageSize)
{
    return bGetRecommendContact(pageNo, pageSize);
}

static jboolean jni_bGet2DCodeInfo(JNIEnv* env, jclass cls, jint iType, jstring jstrKeyWord, jint validType, jint clientExist, jint codeType, jstring jstrMsgId)
{//iType: 1����ȡȺ��ά�벢���¶�ά����Ч�ڣ�2-����key��ȡjson��
    char strKeyWord[1024+1] = {0};
    char msgId[64+1] = {0};
  
	safeGetStringUTFChars(env, jstrKeyWord, strKeyWord, sizeof(strKeyWord) - 1, NULL); 
    safeGetStringUTFChars(env, jstrMsgId, msgId, sizeof(msgId) - 1, NULL); 
    return bGet2DCodeInfo(iType, strKeyWord, validType, clientExist, codeType, msgId);
}

static jstring  jni_bVarEncryptPassword(JNIEnv* env, jclass cls, jstring jstrOri)
{
    char str_ori[256+1] = {0};
	char str_dest[256+1] = {0};
	if(NULL == safeGetStringUTFChars(env, jstrOri, str_ori, sizeof(str_ori) - 1, NULL))
    {
        return NULL;
    }

    if(strlen(str_ori)<1)
    {
        return NULL;
    }

    //����
    if(!bVarEncryptPassword(str_ori, str_dest, 0))
    {
        return NULL;
    }

    return env->NewStringUTF(str_dest);
}

static jstring  jni_bVarDecryptPassword(JNIEnv* env, jclass cls, jstring jstrOri)
{
    char str_ori[256+1] = {0};
	char str_dest[256+1] = {0};

	if(NULL == safeGetStringUTFChars(env, jstrOri, str_ori, sizeof(str_ori) - 1, NULL))
    {
        return NULL;
    }

    if(strlen(str_ori)<24)
    {//�Լ��ܴ�����������ֹjni����ʱ�򱨴�ֱ�ӹҵ�
        return NULL;
    }

    //����
    if(!bVarEncryptPassword(str_ori, str_dest, 1))
    {   
        return NULL;
    }
    
    return env->NewStringUTF(str_dest);
}

static void  jni_bCheckOsTimer(JNIEnv* env, jclass cls)
{ 
    bCheckOsTimer(SIPAgentPno); 
}

static jboolean jni_bClientLoginNotify(JNIEnv* env, jclass cls)
{
    return bClientLoginNotify();
}

static jboolean jni_bLoginInfoReport(JNIEnv* env, jclass cls, jstring jstrRecord)
{
    char loginRecord[MAX_STRING_LEN_1024+1] = {0};

    safeGetStringUTFChars(env, jstrRecord, loginRecord, sizeof(loginRecord) - 1, NULL);
    return bLoginInfoReport(loginRecord);
}

static jboolean jni_bSyncUserCodeMoaInfo(JNIEnv* env, jclass cls, jint iType, jstring jstrMsgId, jstring jstrDeviceId, jstring jstrXmlBody)
{
    char strMsgId[MAX_STRING_LEN_32+1] = {0};
    char strDeviceId[MAX_STRING_LEN_256+1] = {0};
    char *pXmlBody = NULL;
    safeGetStringUTFChars(env, jstrMsgId, strMsgId, sizeof(strMsgId) - 1, NULL);
    safeGetStringUTFChars(env, jstrDeviceId, strDeviceId, sizeof(strDeviceId) - 1, NULL);
    pXmlBody = safeAllocStringUTFCharsFromJString(env, jstrXmlBody);
    BOOL bRet = bSyncUserCodeMoaInfo(iType, strMsgId, strDeviceId, pXmlBody);
//    ZX_free(pXmlBody);//����softagent������ͷ�
    return bRet;
}

static jboolean jni_bMOAImmediatelyReReg(JNIEnv* env, jclass cls)
{
    return bMOAImmediatelyReReg();
}

static jboolean jni_bCheckConnection(JNIEnv* env, jclass cls)
{
    return bCheckConnection();
}

static void jni_bSetJGPLibLogpath(JNIEnv* env, jclass cls, jstring jstrlogpath)
{
	char str_logpath[256+1] = {0};

	safeGetStringUTFChars(env, jstrlogpath, str_logpath, sizeof(str_logpath) - 1, NULL);

	DEBUG_INFO("[jni_bSetJGPLibLogpath] %s", str_logpath);
	SetJGPLibLogpath(str_logpath);
}

//static jint jni_bMakeGroupThumbnailJpg(JNIEnv* env, jclass cls, jstring jstrInPath1, jstring jstrInPath2, jstring jstrInPath3, jstring jstrInPath4, jstring jstrOutPath)
static jint jni_bMakeGroupThumbnailJpg(JNIEnv* env, jclass cls, jobjectArray jstrArray, jint mode, jint composemode, jstring jstrOutfile)
{
	/*
    char str_JpgPath1[256+1] = {0};
	char str_JpgPath2[256+1] = {0};
    char str_JpgPath3[256+1] = {0};
    char str_JpgPath4[256+1] = {0};
    char str_OutJpgPath[256+1] = {0};

	safeGetStringUTFChars(env, jstrInPath1, str_JpgPath1, sizeof(str_JpgPath1) - 1, NULL);
    safeGetStringUTFChars(env, jstrInPath2, str_JpgPath2, sizeof(str_JpgPath2) - 1, NULL);
    safeGetStringUTFChars(env, jstrInPath3, str_JpgPath3, sizeof(str_JpgPath3) - 1, NULL);
    safeGetStringUTFChars(env, jstrInPath4, str_JpgPath4, sizeof(str_JpgPath4) - 1, NULL);
    safeGetStringUTFChars(env, jstrOutPath, str_OutJpgPath, sizeof(str_OutJpgPath) - 1, NULL);

    return MakeGroupThumbnailJpg(str_JpgPath1, str_JpgPath2, str_JpgPath3, str_JpgPath4, str_OutJpgPath, 1);
	*/

	int i;
	char *pinfile[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	char infile[9][256] = {0};
	char strOutfile[256+1] = {0};

	int size = env->GetArrayLength(jstrArray);
	if (size > 9)
	{
		size = 9;
	}

	//DEBUG_INFO("[jni_bMakeGroupThumbnailJpg] =============");
	for(i = 0; i < size; i++)
	{
		jstring js_string = (jstring)env->GetObjectArrayElement(jstrArray, i);
		safeGetStringUTFChars(env, js_string, infile[i], sizeof(infile[i])-1, NULL);
		if ('\0' == infile[i][0])
		{
			break;
		}
		pinfile[i] = infile[i];
		//DEBUG_INFO("[jni_bMakeGroupThumbnailJpg] %s", pinfile[i]);
	}

	safeGetStringUTFChars(env, jstrOutfile, strOutfile, sizeof(strOutfile)-1, NULL);
	//DEBUG_INFO("[jni_bMakeGroupThumbnailJpg] %s", strOutfile);

	return MakeGroupThumbnailJpg(pinfile, mode, composemode, strOutfile);
}

static jint jni_bImageZoom(JNIEnv* env, jclass cls, jint jintype, jstring jImgSrc, jint jSrclen, jint jlongwidth, 
						   jint jquality, jstring joutfile)
{
    int nRet = 0;
	char str_ImgSrc[256+1] = {0};
    char str_outfile[256+1] = {0};

    safeGetStringUTFChars(env, jImgSrc, str_ImgSrc, sizeof(str_ImgSrc) - 1, NULL);
    safeGetStringUTFChars(env, joutfile, str_outfile, sizeof(str_outfile) - 1, NULL);

    nRet = ImageZoom(jintype, str_ImgSrc, jSrclen, jlongwidth, jquality, str_outfile);

	if (nRet != 0)
	{
		DEBUG_INFO("[jni_bImageZoom] fail(%d), jintype=%d,str_ImgSrc=%s, str_outfile=%s", 
			nRet, jintype,str_ImgSrc, str_outfile);
	}

	return nRet;
}

static jint jni_bScaleToRect(JNIEnv* env, jclass cls, jstring jinfile, jint jwidth, jint jheight, jstring joutfile)
{
    int nRet = 0;
	char str_infile[256+1] = {0};
    char str_outfile[256+1] = {0};

    safeGetStringUTFChars(env, jinfile, str_infile, sizeof(str_infile) - 1, NULL);
    safeGetStringUTFChars(env, joutfile, str_outfile, sizeof(str_outfile) - 1, NULL);

	nRet = ScaleToRect(str_infile, jwidth, jheight, str_outfile);

	if (nRet != 0)
	{
		DEBUG_INFO("[jni_bScaleToRect] fail(%d), jwidth=%d, jheight=%d, str_infile=%s, str_outfile=%s", 
			nRet, jwidth, jheight, str_infile, str_outfile);
	}

	return nRet;
}

static jint  jni_bGetUserStatus(JNIEnv* env, jclass cls)
{   
    return (jint)gUserInfo.logStatus;
}

static jint  jni_bCheckHttpServer(JNIEnv* env, jclass cls, jstring jstrServerIP, jint port, jint timeout)
{
    char ServerIP[256+1] = {0};
    safeGetStringUTFChars(env, jstrServerIP, ServerIP, sizeof(ServerIP) - 1, NULL);
    return (jint)checkHttpServer(ServerIP, port, timeout);
}

static jint  jni_bCheckSipServer(JNIEnv* env, jclass cls, jstring jstrServerIP, jint port, jint timeout)
{  
    char ServerIP[256+1] = {0};
    safeGetStringUTFChars(env, jstrServerIP, ServerIP, sizeof(ServerIP) - 1, NULL);
    return (jint)checkSipServer(ServerIP, port, timeout);
}

static void  jni_bSetOcxLogLevel(JNIEnv* env, jclass cls, jint level)
{   
    //if(1 == level)
        g_WriteLogType = 2;
    //else
    //    g_WriteLogType = 0;     
}

static jobjectArray jni_bSyncGetSensWordList(JNIEnv* env, jclass cls)
{
    char filename[256] = {0}; 
    GetSensWordListResult_T SensWordListResult;
    memset(&SensWordListResult, 0, sizeof(GetSensWordListResult_T));
    GetUserDataFile(SENSITIVE_WORD_FILE, filename);
 
    if(!ReadSensitiveWordList(filename, &gMsgBuf, &SensWordListResult, TRUE))
    {
        DEBUG_INFO("[jni_bSyncGetSensWordList] Get Sens Word from local file failed!");
        return NULL;
    }

    jclass class_string = env->FindClass("java/lang/String");
    jobjectArray array_words = env->NewObjectArray(SensWordListResult.nSensWordNum, class_string, NULL);
    OneSensWord_T *pSensWordData = (OneSensWord_T *)SensWordListResult.pSensWordData;
	for (int i = 0; i < SensWordListResult.nSensWordNum; i++)
    {
       	jstring sensWord = env->NewStringUTF(pSensWordData[i].sensWord);          
        env->SetObjectArrayElement(array_words, i, sensWord);
        env->DeleteLocalRef(sensWord);
    }   

    return array_words;
}

static jboolean jni_bGetCheckCode(JNIEnv* env, jclass cls, jstring jstrUserCode)
{
    char strUserCode[MAX_STRING_LEN_32+1] = {0};

    safeGetStringUTFChars(env, jstrUserCode, strUserCode, sizeof(strUserCode) - 1, NULL);
    return bGetCheckCode(strUserCode);
}

static jint  jni_bUserRegistReq(JNIEnv* env, jclass cls, jint iType, jstring jstrUserName, jstring jstrUserURI, jstring jstrPasswd, jstring jstrCompany, jstring jstrCheckCode)
{
    char str_UserName[256+1] = {0};
	char str_UserURI[256+1] = {0};
    char str_Passwd[256+1] = {0};
    char str_Company[256+1] = {0};
    char str_CheckCode[16+1] = {0};
    
	safeGetStringUTFChars(env, jstrUserName, str_UserName, sizeof(str_UserName) - 1, NULL);
    safeGetStringUTFChars(env, jstrUserURI, str_UserURI, sizeof(str_UserURI) - 1, NULL);
    safeGetStringUTFChars(env, jstrPasswd, str_Passwd, sizeof(str_Passwd) - 1, NULL);
    safeGetStringUTFChars(env, jstrCompany, str_Company, sizeof(str_Company) - 1, NULL);
    safeGetStringUTFChars(env, jstrCheckCode, str_CheckCode, sizeof(str_CheckCode) - 1, NULL);

    return bUserRegistReq(iType, str_UserName, str_UserURI, str_Passwd, str_Company, str_CheckCode);
}

static jint  jni_bUserModifyPasswdReq(JNIEnv* env, jclass cls, jint iType, jstring jstrUserURI, jstring jstrOldPasswd, jstring jstrNewPasswd)
{
	char str_UserURI[256+1] = {0};
    char str_OldPasswd[256+1] = {0};
    char str_NewPasswd[256+1] = {0};

    safeGetStringUTFChars(env, jstrUserURI, str_UserURI, sizeof(str_UserURI) - 1, NULL);
    safeGetStringUTFChars(env, jstrOldPasswd, str_OldPasswd, sizeof(str_OldPasswd) - 1, NULL);
    safeGetStringUTFChars(env, jstrNewPasswd, str_NewPasswd, sizeof(str_NewPasswd) - 1, NULL);

    return bUserModifyPasswdReq(iType, str_UserURI, str_OldPasswd, str_NewPasswd);
}

static jint  jni_bSendPubAccMsg(JNIEnv* env, jclass cls, jstring jstrPubAccId, jstring jstrSender, jint iMsgType, jstring jstrPutMsgId, jstring jstrMenuId, jstring jstrContent)
{
    char str_PubAccId[32+1] = {0};
	char str_strSender[64+1] = {0};
    char str_PutMsgId[64+1] = {0};
    char str_MenuId[32+1] = {0};
    char str_Content[256+1] = {0};
  
	safeGetStringUTFChars(env, jstrPubAccId, str_PubAccId, sizeof(str_PubAccId) - 1, NULL);
    safeGetStringUTFChars(env, jstrSender, str_strSender, sizeof(str_strSender) - 1, NULL);
    safeGetStringUTFChars(env, jstrPutMsgId, str_PutMsgId, sizeof(str_PutMsgId) - 1, NULL);
    safeGetStringUTFChars(env, jstrMenuId, str_MenuId, sizeof(str_MenuId) - 1, NULL);
    safeGetStringUTFChars(env, jstrContent, str_Content, sizeof(str_Content) - 1, NULL);

    return bSendPubAccMsg(str_PubAccId, str_strSender, iMsgType, str_PutMsgId, str_MenuId, str_Content);
}

static jint  jni_bUserBindOperateReq(JNIEnv* env, jclass cls, jint iOperType, jstring jstrBindNumber, jint iNumberType, jstring jstrPasswd, jstring jstrCompanyID)
{//iOperType:��������,1-bind 2-unbind 3-��ѯbind��ϵ
    char str_BindNumber[256+1] = {0};
    char str_Passwd[256+1] = {0};
    char str_CompanyID[256+1] = {0};
  
	safeGetStringUTFChars(env, jstrBindNumber, str_BindNumber, sizeof(str_BindNumber) - 1, NULL);
    safeGetStringUTFChars(env, jstrPasswd, str_Passwd, sizeof(str_Passwd) - 1, NULL);
    safeGetStringUTFChars(env, jstrCompanyID, str_CompanyID, sizeof(str_CompanyID) - 1, NULL);

    return bUserBindOperateReq(iOperType, str_BindNumber, iNumberType, str_Passwd, str_CompanyID);
}

static jint  jni_bModifyGroupAttribute(JNIEnv* env, jclass cls, jstring jstrGroupURI, jstring jModType, jstring jstrNewValue)
{
    char str_GroupURI[256+1] = {0};
	char str_ModType[64+1] = {0};
    char str_NewValue[MAX_GROUP_ATTR_LEN+1] = {0};
  
	safeGetStringUTFChars(env, jstrGroupURI, str_GroupURI, sizeof(str_GroupURI) - 1, NULL);
    safeGetStringUTFChars(env, jModType, str_ModType, sizeof(str_ModType) - 1, NULL);
    safeGetStringUTFChars(env, jstrNewValue, str_NewValue, sizeof(str_NewValue) - 1, NULL);

    return bModifyGroupAttributeReq(str_GroupURI, str_ModType, str_NewValue);
}

static jboolean jni_bGetGrayVersion(JNIEnv* env, jclass cls, jstring jstrVersion)
{
    char strVersion[MAX_STRING_LEN_128+1] = {0};

    safeGetStringUTFChars(env, jstrVersion, strVersion, sizeof(strVersion) - 1, NULL);
    return bGetGrayVersion(strVersion);
}

static jboolean jni_bMessageMuteNotifyOperateReq(JNIEnv* env, jclass cls, jint iType, jstring jstrURI)
{/*iType: 1-��ѯ��������Ϣ���ŵ�Ⱥ����ϵ�ˡ����ں��б�  2-����Ⱥ���� 3-ȡ������Ⱥ����  4-������ϵ������ 5-ȡ��������ϵ������
6-���ù��ں����� 7-ȡ�����ù��ں�����*/
    char cURI[256+1] = {0};
    safeGetStringUTFChars(env, jstrURI, cURI, sizeof(cURI) - 1, NULL);
    return bMessageMuteNotifyOperateReq(iType, cURI);
}

static jboolean jni_bMessageReceiptOperateReq(JNIEnv* env, jclass cls, jint iOperType, jstring jstrSessionID, jint isGroup, jstring jstrURI, jstring jstrMsgIDs)
{//iOperType: 1-�Ķ���ִ�ϱ�  2-�����Ķ���ִ��ѯ����  3-Ⱥ���Ķ���ִ��ѯ����
    char cSessionID[64+1] = {0};
    char cURI[256+1] = {0};
    char *pMsgIDs = NULL;
    safeGetStringUTFChars(env, jstrSessionID, cSessionID, sizeof(cSessionID) - 1, NULL);
    safeGetStringUTFChars(env, jstrURI, cURI, sizeof(cURI) - 1, NULL);

    pMsgIDs = safeAllocStringUTFCharsFromJString(env, jstrMsgIDs); 
    if(pMsgIDs == NULL)
        return FALSE;
    BOOL bRet = bMessageReceiptOperateReq(iOperType, cSessionID, isGroup, cURI, pMsgIDs);
    ZX_free(pMsgIDs);
    return bRet;
}

static jboolean jni_bGetPublicMsg(JNIEnv* env, jclass cls, jstring jstrLocalMsgID, jstring jstrURL)
{
    char tmpMsgID[MAX_STRING_LEN_64+1] = {0};
    char tmpURL[MAX_STRING_LEN_512+1] = {0};
    safeGetStringUTFChars(env, jstrLocalMsgID, tmpMsgID, sizeof(tmpMsgID) - 1, NULL);
    safeGetStringUTFChars(env, jstrURL, tmpURL, sizeof(tmpURL) - 1, NULL);
    return bGetPublicMsg(tmpMsgID, tmpURL);
}

/*
pSessionID����ʶ�����ϴ��ļ���ID��,����Ϊ32�ֽ�
pFilePathName��������·�����ļ���,�255�ֽ�
iOperType:  0-�����ϴ���1-�ϵ�������2-ȡ���ϴ���pSessionID������pSessionID����һ�£�
pFileID���ϵ�������iOperType=1����Ҫ��д���������ɵ��ļ�ID�����������""
*/
static jboolean  jni_bUploadFileReq(JNIEnv* env, jclass cls, jstring jstrSessionID, jstring jstrFilePathName, jint iOperType, jstring jstrFileID)
{
	char pSessionID[32+1]= {0};
	char pFilePathName[MAX_FILE_ID_LEN+1] = {0}; 
    char pFileID[MAX_FILE_ID_LEN+1] = {0};

	safeGetStringUTFChars(env, jstrSessionID, pSessionID, sizeof(pSessionID) - 1, NULL);
	safeGetStringUTFChars(env, jstrFilePathName, pFilePathName, sizeof(pFilePathName) - 1, NULL);
    safeGetStringUTFChars(env, jstrFileID, pFileID, sizeof(pFileID) - 1, NULL);

	return bUploadFileReq(pSessionID, pFilePathName, iOperType, pFileID);
}

/*
pSessionID����ʶ��������ĻỰID��,����Ϊ32�ֽ�
pFilePathName�����ڱ����ļ��Ĵ�����·�����ļ������255�ֽ�
iOperType:  0-�������أ�1-�ϵ�������2-ȡ�����أ�pSessionID������pSessionID����һ�£�
pFileID����д���������ɵĴ������ļ�ID
*/
static jboolean  jni_bDownloadFileReq(JNIEnv* env, jclass cls, jstring jstrSessionID, jstring jstrFilePathName, jint iOperType, jstring jstrFileID)
{
	char pSessionID[32+1]= {0};
	char pFilePathName[MAX_FILE_ID_LEN+1] = {0}; 
    char pFileID[MAX_FILE_ID_LEN+1] = {0};

	safeGetStringUTFChars(env, jstrSessionID, pSessionID, sizeof(pSessionID) - 1, NULL);
	safeGetStringUTFChars(env, jstrFilePathName, pFilePathName, sizeof(pFilePathName) - 1, NULL);
    safeGetStringUTFChars(env, jstrFileID, pFileID, sizeof(pFileID) - 1, NULL);

	return bDownloadFileReq(pSessionID, pFilePathName, iOperType, pFileID);
}

/*
jstrSessionID:�ỰID,��Ӧʱ�ؼ�͸����UI,��󳤶�32�ֽ�
jstrRequestPath: ����url��·������: http://IP:PORT/xxxxx�е�"xxxxx"��������
jstrMethod: HTTP��������GET,POST��
jstrBody:HTTP��body���ݣ�û��bodyʱ��д""
*/
static jboolean  jni_bSendRequestMsgToHttpAP (JNIEnv* env, jclass cls, jstring jstrSessionID, jstring jstrRequestLine, jstring jstrMethod, jstring jstrBody)
{
    char cSessionID[32+1]= {0};
	char cRequestLine[MAX_XCAP_HEAD_LEN+1] = {0};
	char cMethod[20+1] = {0};    
	char *pBody = safeAllocStringUTFCharsFromJString(env, jstrBody);
    
    safeGetStringUTFChars(env, jstrSessionID, cSessionID, sizeof(cSessionID) - 1, NULL);
	safeGetStringUTFChars(env, jstrRequestLine, cRequestLine, sizeof(cRequestLine) - 1, NULL);
	safeGetStringUTFChars(env, jstrMethod, cMethod, sizeof(cMethod) - 1, NULL);
    
	return bSendRequestMsgToHttpAP(cSessionID, cRequestLine, cMethod, pBody);
}

static jboolean jni_bLogonSS(JNIEnv* env, jclass cls)
{
	LOGI(TAG,"[jni_bLogonSS] \n");
	return bLogonSS();
}

static jboolean jni_bLogoutSS(JNIEnv* env, jclass cls)
{
    LOGI(TAG,"[jni_bLogoutSS] \n");
	return bLogoutSS();
}

static jboolean  jni_bGetOfflineMsgReq(JNIEnv* env, jclass cls, jstring jstrSeq, jint iMaxCount)
{
	char pSeq[MAX_SEQ_LEN+1]= {0};
	safeGetStringUTFChars(env, jstrSeq, pSeq, sizeof(pSeq) - 1, NULL);
	return bGetOfflineMsgReq(pSeq, iMaxCount);
}

static jboolean jni_MOAEncryptFile(JNIEnv* env, jclass cls, jstring jOriFileName, jstring jDstFileName)
{
    int nRet = 0;
	char oriFileName[256+1] = {0};
    char mcfFileName[256+1] = {0};

    safeGetStringUTFChars(env, jOriFileName, oriFileName, sizeof(oriFileName) - 1, NULL);
    safeGetStringUTFChars(env, jDstFileName, mcfFileName, sizeof(mcfFileName) - 1, NULL);

    return Mcf_EncryptFile(oriFileName, mcfFileName);
}

static jboolean jni_MOADecryptFile(JNIEnv* env, jclass cls, jstring jOriFileName, jstring jDstFileName)
{
    int nRet = 0;	
    char mcfFileName[256+1] = {0};
    char decFileName[256+1] = {0};
   
    safeGetStringUTFChars(env, jOriFileName, mcfFileName, sizeof(mcfFileName) - 1, NULL);
    safeGetStringUTFChars(env, jDstFileName, decFileName, sizeof(decFileName) - 1, NULL);

    return Mcf_DecryptFile(mcfFileName, decFileName);
}

static jboolean  jni_bSendBroadcastMsgReq(JNIEnv* env, jclass cls, jstring jstrMsgID, jstring jstrPURIs, jstring jstrGURIs, jstring jstrFilePath)
{
    char cMsgID[MAX_STRING_LEN_36+1];	
    char *pURIs = NULL;  	
    char *gURIs = NULL; 	
    char filePath[256+1];

    pURIs = safeAllocStringUTFCharsFromJString(env, jstrPURIs);
    if(pURIs == NULL)
        return FALSE;
    
    gURIs = safeAllocStringUTFCharsFromJString(env, jstrGURIs);
    if(gURIs == NULL)
    {
        ZX_free(pURIs);
        return FALSE;
    }
    
	safeGetStringUTFChars(env, jstrMsgID, cMsgID, sizeof(cMsgID) - 1, NULL);
    safeGetStringUTFChars(env, jstrFilePath, filePath, sizeof(filePath) - 1, NULL);
	return bSendBroadcastMsgReq(cMsgID, pURIs, gURIs, filePath);
}

static jboolean jni_bGetUserLogonStatus(JNIEnv* env, jclass cls, jstring jstrURI)
{
    char tmpURI[MAX_STRING_LEN_512+1] = {0};
    safeGetStringUTFChars(env, jstrURI, tmpURI, sizeof(tmpURI) - 1, NULL);
    return bGetUserLogonStatus(tmpURI);
}

static jboolean  jni_bConcernGroupOperateReq(JNIEnv* env, jclass cls, jint iType, jstring jstrEtag, jstring jstrGURI)
{
	char eTag[64+1] = {0};
    char gURI[MAX_IMS_URI_LEN+1] = {0};
	safeGetStringUTFChars(env, jstrEtag, eTag, sizeof(eTag) - 1, NULL);
    safeGetStringUTFChars(env, jstrGURI, gURI, sizeof(gURI) - 1, NULL);
	return bConcernGroupOperateReq(iType, eTag, gURI);
}

static jboolean  jni_bUploadFileByXcapReq(JNIEnv* env, jclass cls, jint iType, jstring jstrMsgID, jstring jstrFilePath)
{
	char cMsgID[MAX_STRING_LEN_36+1] = {0};
    char filePath[256+1] = {0};
	safeGetStringUTFChars(env, jstrMsgID, cMsgID, sizeof(cMsgID) - 1, NULL);
    safeGetStringUTFChars(env, jstrFilePath, filePath, sizeof(filePath) - 1, NULL);
	return bUploadFileByXcapReq(iType, cMsgID, filePath);
}

/*===============================================
* end jni method
*===============================================*/

/*
* native methods arrays
*/
static JNINativeMethod methods[] =
{
    {"setRcsOCXEnv", "(I)I", (void*)setRcsOCXEnv},
    {"jni_launcherThreads", "(Ljava/lang/String;)Z", (void*)jni_launcherThreads},
    {"jni_bLogon", "(ILcom/zte/softda/ocx/UCSLogonPara;)Z", (void*)jni_bLogon},
    {"jni_bLogonSS2", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bLogonSS},
    {"jni_bCreateMultiCall", "(Ljava/lang/String;Ljava/lang/String;J)Z", (void*)jni_bCreateMultiCall},
    {"jni_bAcceptCall", "(Ljava/lang/String;)Z", (void*)jni_bAcceptCall},
    {"jni_bRejectCall", "(Ljava/lang/String;I)Z", (void*)jni_bRejectCall},
    {"jni_bLeaveCall", "(I)Z", (void*)jni_bLeaveCall},
    {"jni_bLogoutServer", "()Z", (void*)jni_bLogoutServer},
    {"jni_bForceLogonServer", "()Z", (void*)jni_bForceLogonServer},
    {"jni_bGoOnLogonServer", "()Z", (void*)jni_bGoOnLogonServer},
    {"jni_SecondDialNum", "(Ljava/lang/String;)V", (void*)jni_SecondDialNum},
    {"jni_bCallHold", "(Ljava/lang/String;J)Z", (void*)jni_bCallHold},
    {"jni_bDoNewCallWhenBusy2", "(JLjava/lang/String;)Z", (void*)jni_bDoNewCallWhenBusy2},
    {"jni_bCreateConfCall", "(JJ)Z", (void*)jni_bCreateConfCall},
    {"jni_GetAudioProps", "()Ljava/lang/String;", (void*)jni_GetAudioProps},
    {"jni_GetVideoProps", "()Ljava/lang/String;", (void*)jni_GetVideoProps},
    {"jni_SetAudioProps", "(Ljava/lang/String;J)V", (void*)jni_SetAudioProps},
    {"jni_SetVideoProps", "(Ljava/lang/String;J)V", (void*)jni_SetVideoProps},
    {"jni_bIMSCallMethod", "(JLcom/zte/softda/ocx/UCSNormalCallMethodPara;)Z", (void*)jni_bIMSCallMethod},
    {"jni_bWhetherEncrypt", "(J)Z", (void*)jni_bWhetherEncrypt},

    {"jni_bStartRecord", "(Ljava/lang/String;)Z", (void*)jni_bStartRecord},
    {"jni_bStopRecord", "()Z", (void*)jni_bStopRecord},
    {"jni_bReleaseTalker", "(Ljava/lang/String;)Z", (void*)jni_bReleaseTalker},
    {"jni_bSelectTalker", "(Ljava/lang/String;)Z", (void*)jni_bSelectTalker},
    {"jni_bInviteToMultiCall", "(Ljava/lang/String;Ljava/lang/String;J)Z", (void*)jni_bInviteToMultiCall},

    {"jni_bIMSSendAttachMessage", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)Z", (void*)jni_bIMSSendAttachMessage},
    {"jni_cIMSSendFile", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", (void*)jni_cIMSSendFile},
    {"jni_bIMSDoFileTransReq", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSDoFileTransReq},
    {"jni_bIMSCancelSendingFile", "(Ljava/lang/String;)Z", (void*)jni_bIMSCancelSendingFile},
    {"jni_bIMSCancelRecvingFile", "(Ljava/lang/String;)Z", (void*)jni_bIMSCancelRecvingFile},
    {"jni_bIMSGetDeptAddrList", "(JLjava/lang/String;)Z", (void*)jni_bIMSGetDeptAddrList},
    {"jni_bIMSPsMethod", "(ILcom/zte/softda/ocx/UCSIMSPsMethodPara;)Z", (void*)jni_bIMSPsMethod},
    {"jni_bIMSSearchRLS", "()Z", (void*)jni_bIMSSearchRLS},
    {"jni_bIMSGetPresenceRules", "()Z", (void*)jni_bIMSGetPresenceRules},
    {"jni_bIMSSubWatcher", "()Z", (void*)jni_bIMSSubWatcher},
    {"jni_bIMSSubscribeOneRLS", "(Ljava/lang/String;)Z", (void*)jni_bIMSSubscribeOneRLS},
    {"jni_bIMSSendMessage", "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;II)Z", (void*)jni_bIMSSendMessage},

    {"jni_bIMSGetPublicGroup", "(Ljava/lang/String;I)Z", (void*)jni_bIMSGetPublicGroup},
    {"jni_bSyncGetMyGroupList", "(Ljava/lang/String;)[Lcom/zte/softda/ocx/FireAllGroupPara;", (void*)jni_bSyncGetMyGroupList},
    {"jni_bSyncGetGroupMembers", "(Ljava/lang/String;)Lcom/zte/softda/ocx/FireGroupMemberList;", (void*)jni_bSyncGetGroupMembers},
    {"jni_bIMSSubscribeOneList", "(JLjava/lang/String;J)Z", (void*)jni_bIMSSubscribeOneList},
    {"jni_bIMSDoPublicGroupInvite", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSDoPublicGroupInvite},
    {"jni_bIMSSetUserInfo", "(Lcom/zte/softda/ocx/FireIMSGotUserInfoPara;)Z", (void*)jni_bIMSSetUserInfo},
    {"jni_bIMSSendFileRequest", "([BLjava/lang/String;Ljava/lang/String;I)Z", (void*)jni_bIMSSendFileRequest},
    {"jni_bIMSSendFileResponse", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)Z", (void*)jni_bIMSSendFileResponse},
    {"jni_bIMSSetMyPhoto", "(Ljava/lang/String;[BJ)Z", (void*)jni_bIMSSetMyPhoto},
    {"jni_bIMSGetPersonSubInfo", "(ILcom/zte/softda/ocx/FireIMSGetPersonSubInfoPara;)Z", (void*)jni_bIMSGetPersonSubInfo},
    {"jni_bDispUICmdForEcp2Method", "(ILcom/zte/softda/ocx/UserLogonInfoPara;)Z", (void*)jni_bDispUICmdForEcp2Method},
    {"jni_bCtdCallMethod", "(ILcom/zte/softda/ocx/CTDInfoPara;)Z", (void*)jni_bCtdCallMethod},
    {"jni_bIMSSearchOneListInfo", "(Ljava/lang/String;)Z", (void*)jni_bIMSSearchOneListInfo},

    {"jni_bIMSCreateOnePrivateGroup", "(Ljava/lang/String;)Z", (void*)jni_bIMSCreateOnePrivateGroup},
    {"jni_bIMSModifyPrivateGroupName", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSModifyPrivateGroupName},
    {"jni_bIMSDeleteOnePrivateGroup", "(Ljava/lang/String;)Z", (void*)jni_bIMSDeleteOnePrivateGroup},
    {"jni_bIMSGetPrivateGroupMember", "(Ljava/lang/String;)Z", (void*)jni_bIMSGetPrivateGroupMember},
    {"jni_bIMSCopyListToPrivateGroup", "(Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;J)Z", (void*)jni_bIMSCopyListToPrivateGroup},
    {"jni_bIMSDeleteListFromPrivateGroup", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSDeleteListFromPrivateGroup},

    {"jni_bIMSRuleSetMethod", "(ILcom/zte/softda/ocx/RuleSetPara;)Z", (void*)jni_bIMSRuleSetMethod},

    {"jni_bIMSCreateChatRoom", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSCreateChatRoom},
    {"jni_bIMSControlChatRoomMember", "(Ljava/lang/String;Ljava/lang/String;J)Z", (void*)jni_bIMSControlChatRoomMember},
    {"jni_bIMSLeaveChatRoom", "(Ljava/lang/String;J)Z", (void*)jni_bIMSLeaveChatRoom},
    {"jni_bIMSDoChatRoomInvite", "(JLjava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSDoChatRoomInvite},

    {"jni_bIMSSearchUserByURI", "(Ljava/lang/String;J)Z", (void*)jni_bIMSSearchUserByURI},
    {"jni_bIMSSearchUserByDName", "(Ljava/lang/String;J)Z", (void*)jni_bIMSSearchUserByDName},
    {"jni_bIMSSearchUserOnline", "(J)Z", (void*)jni_bIMSSearchUserOnline},
    {"jni_bIMSSearchUserByPhone", "(Ljava/lang/String;J)Z", (void*)jni_bIMSSearchUserByPhone},
    {"jni_bIMSSearchUserByEmail", "(Ljava/lang/String;J)Z", (void*)jni_bIMSSearchUserByEmail},

    {"jni_bIMSDeleteOneAddressList", "(Ljava/lang/String;)Z", (void*)jni_bIMSDeleteOneAddressList},
    {"jni_bIMSDoWatcherSubSeq", "(Ljava/lang/String;J)Z", (void*)jni_bIMSDoWatcherSubSeq},
    {"jni_bIMSAddOneAddressList", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSAddOneAddressList},
    {"jni_bIMSSearchAddrList", "(JLcom/zte/softda/ocx/FireSerachDepartPara;J)Z", (void*)jni_bIMSSearchAddrList},
    {"jni_bIMSLoadAddrList", "(ILjava/util/ArrayList;II)Z", (void*)jni_bIMSLoadAddrList},
    {"jni_bIMSDoPublicGroupJoinReq", "(Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSDoPublicGroupJoinReq},
    {"jni_bIMSAddRLSMember", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSAddRLSMember},
    {"jni_bIMSDeleteRLSMember", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSDeleteRLSMember},
    {"jni_bIMSModifyOneListDpName", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSModifyOneListDpName},
    {"jni_bIMSConfMethod", "(JLcom/zte/softda/ocx/FireIMSConfMethodPara;)Z", (void*)jni_bIMSConfMethod},
    {"jni_bIMSReqJoinPublicGroup", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSReqJoinPublicGroup},
    {"jni_bIMSLeavePublicGroup", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSLeavePublicGroup},
    {"jni_bIMSDeleteOnePublicGroup", "(Ljava/lang/String;)Z", (void*)jni_bIMSDeleteOnePublicGroup},
    {"jni_bIMSDeleteOneFromMyPublicGroup", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSDeleteOneFromMyPublicGroup},
    {"jni_bIMSDispUICmdMethod", "(ILcom/zte/softda/ocx/IMSDispUICmdMethodPara;)Z", (void*)jni_bIMSDispUICmdMethod},
    {"jni_bUpdateMsgIndication", "(Lcom/zte/softda/ocx/FireMsgIndicationPara;)Z", (void*)jni_bUpdateMsgIndication},
    {"jni_bRequestLastLogInfo", "(Lcom/zte/softda/ocx/FireLastLogInfo;)Z", (void*)jni_bRequestLastLogInfo},
    {"jni_bIMSInviteSomeoneToMyPublicGroup", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bIMSInviteSomeoneToMyPublicGroup},
    {"jni_bIMSCreateOnePublicGroup", "(Lcom/zte/softda/ocx/PublicGroupInputParaTemp;)Z", (void*)jni_bIMSCreateOnePublicGroup},
    {"jni_bServerLinkRecover", "()Z", (void*)jni_bServerLinkRecover},
    {"jni_bCloseServerLink", "()Z", (void*)jni_bCloseServerLink},

	{"jni_nativeInit", "()V", (void*)jni_nativeInit},		//zk
    {"jni_nativeResize", "(II)V", (void*)jni_nativeResize},  //zk
    {"jni_nativeRender", "()V", (void*)jni_nativeRender},	//zk
	{"jni_SensorAngle", "(I)V", (void*)jni_SensorAngle},		//by smm 2013.04.02
	{"jni_AVTest", "(Lcom/zte/softda/ocx/AVTest_MethodPara;)V",(void*)jni_AVTest},//by smm 2012.11.08
	{"jni_bHasCreateSurfacet", "()Z", (void*)jni_bHasCreateSurfacet},//by gongsuling 20.13.7.26
    //����ӿ�
    {"jni_bDataConfMethod", "(ILcom/zte/softda/ocx/conference/ConfMethodPara;)Z", (void*)jni_bDataConfMethod},
    {"jni_bSend2MrsMethod", "(ILcom/zte/softda/ocx/conference/ConfMrsMethodPara;)Z", (void*)jni_bSend2MrsMethod}, 
    {"jni_setFileSocktPath", "(Ljava/lang/String;)V", (void*)jni_setFileSocktPath},//by yanglei 2013.8.29
    {"jni_bConfBridgeControl", "(ILcom/zte/softda/ocx/confbridge/ConfBridgeControlParams;)Z", (void*)jni_bConfBridgeControl},//yanglei 2014.2.6
    {"jni_bSetXCAPAccount", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bSetXCAPAccount},
    {"jni_bDataConfFileUpload", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;JJJ)Z", (void*)jni_bDataConfFileUpload},
    {"jni_bDataConfFileDownload", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bDataConfFileDownload},
    //MOA�ӿ�
    //��ȡ����IM������Ϣ
    {"jni_bGetAllIMPrivateGroupList", "()Z", (void*)jni_bGetAllIMPrivateGroupList},
    //��ȡIM�����µĳ�Ա�б�
    {"jni_bGetIMPrivateGroupMember", "(Ljava/lang/String;)Z", (void*)jni_bGetIMPrivateGroupMember},
    //ͨ��xcapЭ��ջ����http�����������webservices
    {"jni_bSendSoapMessageByXcap", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bSendSoapMessageByXcap},
    {"jni_bCheckURIIsMOA", "(Ljava/lang/String;)Z", (void*)jni_bCheckURIIsMOA}, 
    //�����ʺ���ع���
    {"jni_bSendPubAccountReq", "(ILjava/lang/String;)Z", (void*)jni_bSendPubAccountReq}, 
    {"jni_bGetHistoryPubMsg", "(ILjava/lang/String;ILjava/lang/String;)Z", (void*)jni_bGetHistoryPubMsg},
    {"jni_bSearchPubAccount", "(ILjava/lang/String;II)Z", (void*)jni_bSearchPubAccount},
    {"jni_bGetPubAccountMenu", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bGetPubAccountMenu},
    {"jni_bSendPubAccMsg", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bSendPubAccMsg},
    //��ѯ�����Ƽ�
    {"jni_bGetRecommendContact", "(II)Z", (void*)jni_bGetRecommendContact},
    
    //Ⱥ��������Ϣ��ȡ
    {"jni_bIMSDispUICmdMethod_11", "(ILjava/lang/String;Ljava/lang/String;I)Z", (void*)jni_bIMSDispUICmdMethod_11},
    //AES�ӽ����㷨,�ṩ��UI����
    {"jni_bVarEncryptPassword", "(Ljava/lang/String;)Ljava/lang/String;", (void*)jni_bVarEncryptPassword},
    {"jni_bVarDecryptPassword", "(Ljava/lang/String;)Ljava/lang/String;", (void*)jni_bVarDecryptPassword},
    //CheckOsTimer
    {"jni_bCheckOsTimer", "()V", (void*)jni_bCheckOsTimer},
    {"jni_bClientLoginNotify", "()Z", (void*)jni_bClientLoginNotify}, 
    {"jni_bMOAImmediatelyReReg", "()Z", (void*)jni_bMOAImmediatelyReReg},
    //����������������
    {"jni_bCheckConnection", "()Z", (void*)jni_bCheckConnection},
    //jpgdeal���
    {"jni_bSetJGPLibLogpath", "(Ljava/lang/String;)V", (void*)jni_bSetJGPLibLogpath},
    //{"jni_bMakeGroupThumbnailJpg", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I", (void*)jni_bMakeGroupThumbnailJpg},
    {"jni_bMakeGroupThumbnailJpg", "([Ljava/lang/String;IILjava/lang/String;)I", (void*)jni_bMakeGroupThumbnailJpg},
    {"jni_bImageZoom", "(ILjava/lang/String;IIILjava/lang/String;)I", (void*)jni_bImageZoom},
    //
    {"jni_bLoginInfoReport", "(Ljava/lang/String;)Z", (void*)jni_bLoginInfoReport},
    {"jni_bSyncUserCodeMoaInfo", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bSyncUserCodeMoaInfo},
    {"jni_bGetUserStatus", "()I", (void*)jni_bGetUserStatus},
    {"jni_bCheckHttpServer", "(Ljava/lang/String;II)I", (void*)jni_bCheckHttpServer},
    {"jni_bCheckSipServer", "(Ljava/lang/String;II)I", (void*)jni_bCheckSipServer},
    {"jni_bSetOcxLogLevel", "(I)V", (void*)jni_bSetOcxLogLevel},
    {"jni_bSyncGetSensWordList", "()[Ljava/lang/String;", (void*)jni_bSyncGetSensWordList},
    {"jni_bGetCheckCode", "(Ljava/lang/String;)Z", (void*)jni_bGetCheckCode},
    {"jni_bUserRegistReq", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bUserRegistReq},
    {"jni_bUserModifyPasswdReq", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bUserModifyPasswdReq},
    {"jni_bGetGrayVersion", "(Ljava/lang/String;)Z", (void*)jni_bGetGrayVersion},
    //����bind��ز���
    {"jni_bUserBindOperateReq", "(ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)Z", (void*)jni_bUserBindOperateReq}, 
    {"jni_bModifyGroupAttribute", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bModifyGroupAttribute}, 
    {"jni_bGet2DCodeInfo", "(ILjava/lang/String;IIILjava/lang/String;)Z", (void*)jni_bGet2DCodeInfo},
    {"jni_bMessageMuteNotifyOperateReq", "(ILjava/lang/String;)Z", (void*)jni_bMessageMuteNotifyOperateReq}, 
    {"jni_bMessageReceiptOperateReq", "(ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)Z", (void*)jni_bMessageReceiptOperateReq},     
    {"jni_bGetPublicMsg", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bGetPublicMsg},
    {"jni_bUploadFileReq", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)Z", (void*)jni_bUploadFileReq},
    {"jni_bDownloadFileReq", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)Z", (void*)jni_bDownloadFileReq},   
    {"jni_bSendRequestMsgToHttpAP", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bSendRequestMsgToHttpAP},     
	{"jni_bLogonSS", "()Z", (void*)jni_bLogonSS},
	{"jni_bLogoutSS", "()Z", (void*)jni_bLogoutSS},
	{"jni_bGetOfflineMsgReq", "(Ljava/lang/String;I)Z", (void*)jni_bGetOfflineMsgReq},
	{"jni_MOAEncryptFile", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_MOAEncryptFile},
	{"jni_MOADecryptFile", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_MOADecryptFile},
	{"jni_bSendBroadcastMsgReq", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)jni_bSendBroadcastMsgReq},
	{"jni_bGetUserLogonStatus", "(Ljava/lang/String;)Z", (void*)jni_bGetUserLogonStatus},
	{"jni_bConcernGroupOperateReq", "(ILjava/lang/String;Ljava/lang/String;)Z", (void*)jni_bConcernGroupOperateReq},
	{"jni_bUploadFileByXcapReq", "(ILjava/lang/String;Ljava/lang/String;)Z", (void*)jni_bUploadFileByXcapReq},
	{"jni_bScaleToRect", "(Ljava/lang/String;IILjava/lang/String;)I", (void*)jni_bScaleToRect},
};

//getStringField�����жԿյ�ָ��Ӧ����������(����������󲻿գ�ֻ�Ƕ����ĳ�������ݿ�
//�ؼ����ѱ��������Խ��洫���ؼ��Ķ�����Ϊ�գ������ĳ�����������û�У���Ҫ��ʼ��""
//����ؼ����Ҳ���\0��������)
//�ؼ�Ҳ�������Ŷ�ĳ���������Ϊ��Ҳ��������
//����ǽ���������
char *getStringField(JNIEnv* env, jobject jobj, jclass jcs,  const char * fieldName, char *result, int ilen)
{
    const char *str = NULL;

    /* get the field ID */
    jfieldID  id_softPhone = env->GetFieldID(jcs, fieldName, "Ljava/lang/String;");

    /* get the field value */
    jstring  softPhone = (jstring)env->GetObjectField(jobj, id_softPhone);
    if (softPhone == NULL)
    {
        return NULL;
    }
    str = env->GetStringUTFChars(softPhone, NULL);
    if (str == NULL)
    {
        return NULL; /* OutOfMemoryError already thrown */
    }

    strncpy(result, str, ilen);
    env->ReleaseStringUTFChars(softPhone, str);

    return result;
}
char *safeGetStringUTFChars(JNIEnv* env, jstring string, char* dest, int size, jboolean *iscopy)
{
    const char *str = NULL;

    if (NULL == string)
    {
        if(g_WriteLogType == 2)
            TraceMsgWindow1(1, "rcscontroller the para string is null!");
        else if(g_WriteLogType == 1)
            LOGE(TAG, "the para string is null!");

        return JNI_FALSE;
    }
    str = env->GetStringUTFChars(string, iscopy);
    if (str == NULL)
    {
        return JNI_FALSE; /* OutOfMemoryError already thrown */
    }

    strncpy(dest, str, size);
    env->ReleaseStringUTFChars(string, str);

    return dest;
}


//�ڴ���Ҫ�����Լ��ͷţ�ZX_free�ͷ�
char *safeAllocStringUTFCharsFromJString(JNIEnv* env, jstring string)
{
   if(string != NULL)
   {
		int nStrLen = env->GetStringUTFLength(string);
		char *pString = (char*)ZX_malloc(nStrLen + 1);
		if (NULL == pString)
		{
			return NULL;
		}
        memset(pString, 0, nStrLen + 1);
        safeGetStringUTFChars(env, string, pString, nStrLen, NULL);

		return pString;
	}

	return NULL;
}

//�ڴ���Ҫ�����Լ��ͷ�(ZX_free)
char* safeAllocStringUTFchars(JNIEnv* env, jobject jobj, jclass jcs,  const char * fieldName)
{
	//
	jfieldID id_string = env->GetFieldID(jcs, fieldName, "Ljava/lang/String;");
	/* get the field value */
    	jstring  string = (jstring)env->GetObjectField(jobj, id_string);
      
	return safeAllocStringUTFCharsFromJString(env, string);
}

/*
* register native methods for some class's methods  we know about.
*
* returns JNI_TRUE on success.
*/
static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    LOGI(TAG, "7-before registerNativeMethods");
    clazz = env->FindClass(className);

    if (clazz == NULL)
    {
        LOGE("Native registration unable to find class '%s'", className);
        LOGI(TAG, "8-after registerNativeMethods");
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0)
    {
        LOGE("RegisterNatives failed for '%s'", className);
        LOGI(TAG, "8-after registerNativeMethods");
        return JNI_FALSE;
    }

    LOGI(TAG, "8-after registerNativeMethods ok");
    return JNI_TRUE;
}

/*
* Register native methods for all classes we know about.
*
* returns JNI_TRUE on success.
*/
static int registerNatives(JNIEnv* env)
{
    LOGI(TAG, "6- before registerNatives");

    if (!registerNativeMethods(env, classPathName,
                               methods, sizeof(methods) / sizeof(methods[0])))
    {
        LOGI(TAG, "9-after registerNatives");
        return JNI_FALSE;
    }

    LOGI(TAG, "9-after registerNatives");
    return JNI_TRUE;
}

/*
* This is called by the VM when the shared library is first loaded.
*/
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI(TAG, "5-before JNI_OnLoad");

    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    LOGI(TAG, "JNI_OnLoad");
    gs_jvm = vm;
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE(TAG, "ERROR: GetEnv failed");
        goto fail;
    }

    env = uenv.env;
    if (registerNatives(env) != JNI_TRUE)
    {
        LOGE(TAG, "ERROR: registerNatives failed");
        goto fail;
    }

    result = JNI_VERSION_1_4;

fail:
    LOGI(TAG, "10-after JNI_OnLoad");
    return result;
}
void JNI_OnUnload(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    JNIEnv* env = NULL;
    uenv.venv = NULL;

    LOGI(TAG, "JNI_OnUnload");
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE(TAG, "JNI_OnUnload: ERROR: GetEnv failed");
        goto fail;
    }

    env = uenv.env;
    if (NULL != gpostClass)
    {
        env->DeleteGlobalRef((jobject)gpostClass);
        gpostClass = NULL;
    }
fail:
    LOGI(TAG, "after JNI_OnUnload");
    return;
}


