//
// Created by kevin on 2018/5/2.
//

#include <cwchar>
#include "LoginService.h"
#include "LogUtils.h"

/*
 * Class:     com_iutils_pattern_observer_LoginService
 * Method:    initSDK
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_iutils_pattern_observer_LoginService_initSDK
        (JNIEnv *env, jobject obj, jstring priDataPath, jstring pubDataPath){
    env->GetJavaVM(&gJavaVM);
    gJavaObj = obj;

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

}
