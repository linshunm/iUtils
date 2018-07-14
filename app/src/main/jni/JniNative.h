//
// Created by 林舜铭 on 2018/7/7.
//

#ifndef IUTILS_JNINATIVE_H
#define IUTILS_JNINATIVE_H

#include <jni.h>

jstring jni_bHelloWorld(JNIEnv *env, jclass cls);

/**
 * jni crash test
 */
void jni_bTestCrash(JNIEnv *env, jclass cls);

void onCallback();

#endif //IUTILS_JNINATIVE_H
