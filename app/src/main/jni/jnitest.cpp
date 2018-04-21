//
// Created by kevin on 2018/2/1.
//
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <stdlib.h>
#include "jnitest.h"
#include <android/log.h>
#include <iostream>
#include <string>
#include "util/StringUtils.h"
#include "tq/Task.h"
#include <pthread.h>
#include <unistd.h>
#include <vector>

#define TAG "jnitest"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)


using namespace std;

#define THREAD_NUM 3
void* doTask(void* args){
    LOGI("DO JOB");
    Task *pTask = reinterpret_cast<Task*>(args);
    if(pTask == NULL)
    {
        LOGE("get task failed");
        return NULL;
    }

    std::string temp;
    std::string log;
    for(int i =0; i<10000; i++){
        StringUtils::int2str(i,temp);
        log.append(temp);
        log.append(",");
    }


    //int *pid = (int *) args;
    usleep(1000);//等价于sleep(1)

    LOGI("job-%s", pTask->getTaskId().c_str());
    return 0;
}
void createTaskList(vector<Task> &_taskList){
    LOGI("createTaskList");
    pthread_t tids[THREAD_NUM];
    vector<Task>::iterator it = _taskList.begin();
    int i;
    for(i=0; i<THREAD_NUM; i++){
        Task task = *(it+i);
        LOGI("task id=%s, priority=%d",task.getTaskId().c_str(), task.getPriority());
        //int ret = pthread_create(&tids[i], NULL, doTask, (void*)&(*(it+i)));
        int ret = pthread_create(&tids[i], NULL, Task::run, (void*)&(*(it+i)));
        if(ret != 0){
            cout<<"pthread_create error["<<ret<<"]"<<endl;
        }else{
            LOGI("tid[%d]=%ld",i,(long)tids[i]);
        }

    }

    LOGI("EXIT");
    pthread_exit(NULL);
}

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
 * Class: com_iutils_framework_JniTest
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


/**
 * Class: com_iutils_framework_JniTest
 * Method: getTask
 * Signature:()Lcom/iutils/test/Task
 */
JNIEXPORT jobject JNICALL Java_com_iutils_framework_JniTest_getTask
        (JNIEnv *env, jobject obj)
{
    //获取类的jclass
    jclass classTask = env->FindClass("com/iutils/test/Task");
    if(classTask == NULL)
    {
        LOGE("classTask not found");
        return NULL;
    }

    jmethodID constructMethodId = env->GetMethodID(classTask, "<init>", "()V");
    if(constructMethodId == NULL)
    {
        LOGE("Task construct method not found !");
        return NULL;
    }

    jfieldID taskFieldId = env->GetFieldID(classTask, "taskId", "Ljava/lang/String;");
    if(taskFieldId == NULL)
    {
        LOGE("taskFieldId is null");
        return NULL;
    }

    jfieldID priorityFieldId = env->GetFieldID(classTask, "priority", "I");
    if(priorityFieldId == NULL)
    {
        LOGE("priorityFieldId is null");
        return NULL;
    }
    jfieldID isDoingFieldId = env->GetFieldID(classTask, "isDoing", "Z");
    if(isDoingFieldId == NULL)
    {
        LOGE("isDoingFieldId is null");
        return NULL;
    }


    jstring taskId = env->NewStringUTF("134567777");
    jint priority = 3;
    jboolean isDoing = true;

    jobject task = env->NewObject(classTask, constructMethodId);

    //设置属性值.
    env->SetObjectField(task, taskFieldId, taskId);
    env->SetIntField(task, priorityFieldId, priority);
    env->SetBooleanField(task, isDoingFieldId, isDoing);

    env->ReleaseStringUTFChars(taskId, NULL);

    return task;

}

/**
 * Class: com_iutils_framework_JniTest
 * public native ArrayList<Task> getTaskList();
 */
JNIEXPORT jobject JNICALL Java_com_iutils_framework_JniTest_getTaskList
        (JNIEnv *env, jobject obj)
{
    jclass arrayListCls = env->FindClass("java/util/ArrayList");
    if(arrayListCls == NULL)
    {
        LOGE("arrayListCls not found");
        return NULL;
    }
    jmethodID arrayListConstructMethodID = env->GetMethodID(arrayListCls, "<init>", "()V");
    if(arrayListConstructMethodID == NULL){
        LOGE("arrayListConstructMethodID not found");
        return NULL;
    }

    jobject arrayList = env->NewObject(arrayListCls, arrayListConstructMethodID);
    jmethodID listAddMethodID = env->GetMethodID(arrayListCls, "add", "(Ljava/lang/Object;)Z");
    if(listAddMethodID == NULL)
    {
        LOGE("listAddMethodID not found");
        return NULL;
    }

    //获取类的jclass
    jclass classTask = env->FindClass("com/iutils/test/Task");
    if(classTask == NULL)
    {
        LOGE("classTask not found");
        return NULL;
    }

    jmethodID constructMethodId = env->GetMethodID(classTask, "<init>", "()V");
    if(constructMethodId == NULL)
    {
        LOGE("Task construct method not found !");
        return NULL;
    }

    jfieldID taskFieldId = env->GetFieldID(classTask, "taskId", "Ljava/lang/String;");
    if(taskFieldId == NULL)
    {
        LOGE("taskFieldId is null");
        return NULL;
    }

    jfieldID priorityFieldId = env->GetFieldID(classTask, "priority", "I");
    if(priorityFieldId == NULL)
    {
        LOGE("priorityFieldId is null");
        return NULL;
    }
    jfieldID isDoingFieldId = env->GetFieldID(classTask, "isDoing", "Z");
    if(isDoingFieldId == NULL)
    {
        LOGE("isDoingFieldId is null");
        return NULL;
    }

    string temp;
    string &id =temp;
    vector<Task> taskList;

    for(int i=0; i<3; i++){
        StringUtils::int2str(i,id);

        jstring taskId = env->NewStringUTF(temp.c_str());
        jint priority = i;
        jboolean isDoing = true;

        Task task(temp);
        task.setPriority(i);
        task.setProcessing(false);
        taskList.push_back(task);

        jobject jtask = env->NewObject(classTask, constructMethodId);

        //设置属性值.
        env->SetObjectField(jtask, taskFieldId, taskId);
        env->SetIntField(jtask, priorityFieldId, priority);
        env->SetBooleanField(jtask, isDoingFieldId, isDoing);

        env->CallBooleanMethod(arrayList, listAddMethodID, jtask);
    }


    createTaskList(taskList);

    return arrayList;
}






