//
// Created by 林舜铭 on 2018/7/7.
//

#include "JniCallback.h"
#include "JniCommon.hpp"
#include <jni.h>
#include "JObjectWrapper.hpp"
#include <unistd.h>

/**
 *sdk消息发送和新消息通知
 */
void* onJniCallback(void *args)
{
    int index = *(int *)args;
    LOGI(TAG, "onJniCallback index[%d]", index);

    jclass clsJniCallback;
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    CTMPDoDetach tmpDoDetach;


    usleep(30000);//单位为微秒 1秒 = 1000毫秒 = 1000000微秒

    iresult = getStaticJniCallbackMethod(&clsJniCallback, (char *)"onJniCallback", (char *)"(Lcom/iutils/jni/bean/Result;)V", &env, &gJinMethod, tmpDoDetach.m_bDetach);

    if(JNI_ERR == iresult || gJinMethod == 0 || gJinMethod == NULL)
    {
        LOGE(TAG, "onJniCallback method not found");
        return NULL;
    }


    JObjectWrapper jwResult(env, JNIFindClass("com/iutils/jni/bean/Result"));
    jwResult.NewObject();
    jwResult.AutoDeleteLocalRef(false);

    /**
     * public int notifyType;
    public boolean success;
    public int resultCode;
    public String body;
     */
    jwResult.SetStringValue("body", "this is body: hello kevin !");
    jwResult.SetIntValue("notifyType", 1);
    jwResult.SetIntValue("resultCode", index);
    jwResult.SetBoolValue("success", true);

    env->CallStaticVoidMethod(clsJniCallback, gJinMethod, jwResult.GetObject());
    LOGI(TAG, "onJniCallback finish index[%d]", index);
    return 0;
}