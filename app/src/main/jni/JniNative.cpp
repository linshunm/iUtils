//
// Created by 林舜铭 on 2018/7/7.
//

#include "JniNative.h"
#include "androidLog.h"
#include "JniCommon.hpp"
#include "JniCallback.h"
#include <pthread.h>
#include <unistd.h>
#define THREAD_NUM 1000

jstring jni_bHelloWorld(JNIEnv *env, jclass cls){
    std::string hello = "Hello from C++";
    onCallback();
    return env->NewStringUTF(hello.c_str());
}

/**
 * jni crash test
 */
void jni_bTestCrash(JNIEnv *env, jclass cls){
    LOGI(TAG, "jni_bTestCrash");
    int a, b, c;
    a = 0;
    b = 10;
    c =  b/a;
}


void onCallback(){
    pthread_t tids[THREAD_NUM];
    int i;
    for(i=0; i<THREAD_NUM; i++){
        LOGI(TAG, "onCallback index=%d",i);
        //int ret = pthread_create(&tids[i], NULL, doTask, (void*)&(*(it+i)));
        int parm = i;
        int ret = pthread_create(&tids[i], NULL, onJniCallback, &parm);
        if(ret != 0){
            LOGE(TAG, "pthread_create error[%d]", ret);
        }else{
            LOGI(TAG, "tid[%d]=%ld",i,(long)tids[i]);
        }
        //pthread_join(tids[i], NULL);
    }
}
