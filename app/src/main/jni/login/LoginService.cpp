//
// Created by kevin on 2018/5/2.
//

#include <cwchar>
#include "LoginService.h"
#include "LogUtils.h"
#include <pthread.h>
#include <unistd.h>
#include "LoginParam.h"


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    LOGI("JNI_OnLoad");

    gJavaVM = jvm;
    //https://blog.csdn.net/fu_shuwu/article/details/41121741
    //gJavaVM->AttachCurrentThread(&env, NULL);

    //gJavaVM->DetachCurrentThread();

    return JNI_VERSION_1_4;
}

/*
 * Class:     com_iutils_pattern_observer_LoginService
 * Method:    initSDK
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_iutils_pattern_observer_LoginService_initSDK
        (JNIEnv *env, jobject obj, jstring priDataPath, jstring pubDataPath){
    env->GetJavaVM(&gJavaVM);
    gJavaObj = env->NewGlobalRef(obj);

    const char *cPriDataPath = env->GetStringUTFChars(priDataPath, NULL);
    const char *cPubDataPath = env->GetStringUTFChars(pubDataPath, NULL);

    LOGI("init sdk priDataPath=%s pubDataPath=%s", cPriDataPath, cPubDataPath);
}

/*
 * Class:     com_iutils_pattern_observer_LoginService
 * Method:    login
 * Signature: (Lcom/iutils/pattern/observer/LoginParam;)V
 */
JNIEXPORT void JNICALL Java_com_iutils_pattern_observer_LoginService_login
        (JNIEnv *env, jobject obj, jobject loginParam){
    LOGI("login...");
    LoginParam *pLoginParam = new LoginParam();
    jclass loginParamClass = env->GetObjectClass(loginParam);
    jfieldID accountFieldId = env->GetFieldID(loginParamClass, "account", "Ljava/lang/String;");
    if(accountFieldId == NULL){
        LOGE("accountFieldId is null return");
        return;
    }
    jstring jstrAccount = (jstring)env->GetObjectField(loginParam, accountFieldId);
    pLoginParam->account = env->GetStringUTFChars(jstrAccount, JNI_FALSE);
    env->ReleaseStringUTFChars(jstrAccount, NULL);

    jfieldID passwordFieldId = env->GetFieldID(loginParamClass, "password", "Ljava/lang/String;");
    if(passwordFieldId == NULL){
        LOGE("passwordFieldId is null return");
        return;
    }
    jstring jstrPassword = (jstring)env->GetObjectField(loginParam, passwordFieldId);
    pLoginParam->password = env->GetStringUTFChars(jstrPassword, JNI_FALSE);
    env->ReleaseStringUTFChars(jstrPassword, NULL);

    jfieldID deviceIdFieldId = env->GetFieldID(loginParamClass, "deviceId", "Ljava/lang/String;");
    if(deviceIdFieldId == NULL){
        LOGE("deviceIdFieldId is null return");
        return;
    }
    jstring jstrDeviceId = (jstring)env->GetObjectField(loginParam, deviceIdFieldId);
    pLoginParam->deviceId = env->GetStringUTFChars(jstrDeviceId, JNI_FALSE);
    env->ReleaseStringUTFChars(jstrDeviceId, NULL);

    jfieldID terminalTypeFieldId = env->GetFieldID(loginParamClass, "terminalType", "Ljava/lang/String;");
    if(terminalTypeFieldId == NULL){
        LOGE("terminalTypeFieldId is null return");
        return;
    }
    jstring jstrTerminalType = (jstring)env->GetObjectField(loginParam, terminalTypeFieldId);
    pLoginParam->terminalType = env->GetStringUTFChars(jstrTerminalType, JNI_FALSE);
    env->ReleaseStringUTFChars(jstrTerminalType, NULL);

    jfieldID versionCodeFieldId = env->GetFieldID(loginParamClass, "versionCode", "I");
    if(versionCodeFieldId == NULL){
        LOGE("versionCodeFieldId is null return");
        return;
    }
    jint jintTerminalType = (jint)env->GetObjectField(loginParam, versionCodeFieldId);
    pLoginParam->versionCode = jintTerminalType;

    /*
    LOGI("do login account=%s,password=%s,deviceId=%s,terminalType=%s,versionCode=%d",
         pLoginParam->account.c_str(),
         pLoginParam->password.c_str(),
         pLoginParam->deviceId.c_str(),
         pLoginParam->terminalType.c_str(),
         pLoginParam->versionCode
    );

    sleep(1);

    //获取回调对象
    jclass loginServiceClass = env->GetObjectClass(obj);
    jmethodID onCallbackMethodId = env->GetMethodID(loginServiceClass, "onLoginResult", "(Lcom/iutils/pattern/observer/LoginResult;)V");

    jclass loginResultClass = env->FindClass("com/iutils/pattern/observer/LoginResult");
    jmethodID ctorID = env->GetMethodID(loginResultClass, "<init>", "()V");
    jobject loginResult = env->NewObject(loginResultClass, ctorID);

    jfieldID resultCodeFieldId = env->GetFieldID(loginResultClass, "resultCode", "I");
    if(resultCodeFieldId == NULL){
        LOGE("resultCodeFieldId is null return");
        return;
    }



    env->SetIntField(loginResult, resultCodeFieldId, 202);

    env->CallVoidMethod(gJavaObj, onCallbackMethodId, loginResult);
    */


    pthread_t threadId;

    if(pthread_create(&threadId, NULL, ocxCallback, (void *)loginParam) != 0){
        LOGE("login ocx thread create failed.");
        return;
    }
    LOGI("login thread created success");

}

static void* ocxCallback(void *arg){
    LOGI("ocxCallback");
    if(gJavaVM == NULL){
        LOGE("gJavaVM is null");
        return NULL;
    }

    /*
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    if (gJavaVM->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE( "JNI_OnUnload: ERROR: GetEnv failed");
        return NULL;
    }
     */

    JNIEnv *env = NULL;

    //env = uenv.env;

    gJavaVM->AttachCurrentThread(&env, NULL);
    LoginParam *pLoginParam = (LoginParam *) arg ;

    LOGI("do login account=%s,password=%s,deviceId=%s,terminalType=%s,versionCode=%d",
         pLoginParam->account.c_str(),
         pLoginParam->password.c_str(),
         pLoginParam->deviceId.c_str(),
         pLoginParam->terminalType.c_str(),
         pLoginParam->versionCode
    );

    sleep(1);

    //获取回调对象
    jclass loginServiceClass = env->GetObjectClass(gJavaObj);
    jmethodID onCallbackMethodId = env->GetMethodID(loginServiceClass, "onLoginResult", "Lcom.iutils.pattern.observer.LoginResult;");

    jclass loginResultClass = env->FindClass("com.iutils.pattern.observer.LoginResult");
    jmethodID ctorID = env->GetMethodID(loginResultClass, "<init>", "()V");
    jobject loginResult = env->NewObject(loginResultClass, ctorID);

    jfieldID resultCodeFieldId = env->GetFieldID(loginResultClass, "resultCode", "I");
    if(resultCodeFieldId == NULL){
        LOGE("resultCodeFieldId is null return");
        return NULL;
    }

    env->SetIntField(loginResult, resultCodeFieldId, 202);

    env->CallVoidMethod(gJavaObj, onCallbackMethodId, loginResult);


    gJavaVM->DetachCurrentThread();

    return NULL;
}
