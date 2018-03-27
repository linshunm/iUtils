//
// Created by kevin on 2018/2/1.
//
#include <stdio.h>
#include <string.h>
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


//jstring to char*
char* jstringToChar(JNIEnv* env, jstring jstr)
{
       char* rtn = NULL;
       jclass clsstring = env->FindClass("java/lang/String");
       jstring strencode = env->NewStringUTF("utf-8");
       jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
       jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
       jsize alen = env->GetArrayLength(barr);
       jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
       if (alen > 0)
       {
                 rtn = (char*)malloc(alen + 1);
                 memcpy(rtn, ba, alen);
                 rtn[alen] = 0;
       }
       env->ReleaseByteArrayElements(barr, ba, 0);
       return rtn;
}
//char* to jstring
jstring charToJstring(JNIEnv* env, const char* pat)
{
       jclass strClass = env->FindClass("Ljava/lang/String;");
       jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
       jbyteArray bytes = env->NewByteArray(strlen(pat));
       env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
       jstring encoding = env->NewStringUTF("utf-8");
       return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

JNIEXPORT jstring JNICALL Java_com_iutils_framework_JniTest_getPackageName(JNIEnv *env, jclass clazz, jobject obj){
    jclass native_class = env->GetObjectClass(obj);
    jmethodID mId = env->GetMethodID(native_class, "getPackageName", "()Ljava/lang/String;");
    jstring packName = static_cast<jstring>(env->CallObjectMethod(obj, mId));
    jclass proxyClass = env->FindClass("com/iutils/framework/FrameworkProxy");
    //无参数构造方法
    jmethodID constructMethodId = env->GetMethodID(proxyClass, "<init>", "()V");
    jmethodID getDeviceIdMethodId = env->GetMethodID(proxyClass, "getDeviceId", "()Ljava/lang/String;");
    jobject proxyObj = NULL;
    if(constructMethodId == NULL)
    {
        LOGE("constructMethodId is null");
        return NULL;
    }
    else
    {
        proxyObj = env->NewObject(proxyClass, constructMethodId);
    }

    if(getDeviceIdMethodId == NULL)
    {
        LOGE("getDeviceIdMethodId is null");
        return NULL;
    }
    else
    {
        jstring deviceId = (jstring)env->CallObjectMethod(proxyObj, getDeviceIdMethodId);
        if(deviceId == NULL)
        {
            LOGE("deviceId is null");
            return NULL;
        }
        //char* cid = jstringToChar(env, deviceId);
        const char *cid = env->GetStringUTFChars(deviceId, NULL);
        LOGI("get new getDeviceIdMethodId %s", cid);
    }
    //(*env)->CallVoidMethod(env,obj,mid); //执行java方法

    env->DeleteLocalRef(proxyObj);
    LOGI("iUtils hello world !");
    return packName;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {

    LOGI("JNI_OnLoad");

    return JNI_VERSION_1_4;
}

/*
 * Class: com_iutils_framework
 * Method: get2Array
 * Signature:(I)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_com_iutils_framework_JniTest_get2Array
        (JNIEnv *env, jobject obj, jint dimon)
{
    //获得一维数组的类型.
    jclass intArrayClass = env->FindClass("[I");

    //构造一个指向intArray类型的数组
    jobjectArray objIntArray = env->NewObjectArray(dimon, intArrayClass, NULL);

    //创建dimon个jintArray数组对象，并放入到对象数组objIntArray里面
    for(int i = 0; i<dimon; i++)
    {
        jintArray intArray = env->NewIntArray(dimon);

        //初始化一个容器，假设dimon不会超过10
        jint temp[10];
        for(int j = 0; j<dimon; j++)
        {
            temp[j] = i+j;
        }

        //对intArray数组进行赋值
        env->SetIntArrayRegion(intArray, 0, dimon, temp);

        //对对象数组进行赋值
        env->SetObjectArrayElement(objIntArray, i, intArray);

        //释放局部变量
        env->DeleteLocalRef(intArray);
    }

    return objIntArray;
}


