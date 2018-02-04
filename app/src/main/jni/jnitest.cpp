//
// Created by kevin on 2018/2/1.
//
#include <stdio.h>
#include <jni.h>
#include <stdlib.h>
#include "jnitest.h"
#include <android/log.h>
#define TAG "jnitest"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

JNIEXPORT jstring JNICALL Java_com_iutils_framework_JniTest_getPackageName(JNIEnv *env, jclass clazz, jobject obj){
    jclass native_class = env->GetObjectClass(obj);
    jmethodID mId = env->GetMethodID(native_class, "getPackageName", "()Ljava/lang/String;");
    jstring packName = static_cast<jstring>(env->CallObjectMethod(obj, mId));
    jclass iutilClass = env->FindClass("com/iutils/framework/FrameworkProxy");
    jmethodID getDeviceIdMethodId = env->GetMethodID(iutilClass, "getDeviceId", "()Ljava/lang/String;");
    if(getDeviceIdMethodId == NULL)
    {
        LOGE("getDeviceIdMethodId is null");
    }
    else
    {
        LOGI("get getDeviceIdMethodId");
    }
    //(*env)->CallVoidMethod(env,obj,mid); //执行java方法
    LOGI("iUtils hello world !");
    return packName;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {

    LOGI("JNI_OnLoad");

    return JNI_VERSION_1_4;
}