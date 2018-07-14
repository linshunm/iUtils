//
//  JNIString.cpp
//  USSDK_AD
//
//  Created by 林舜铭 on 2018/6/29.
//  Copyright © 2018年 林舜铭. All rights reserved.
//

#include "JNIString.hpp"

JNIString::JNIString(JNIEnv *env, const char* str)
{
    _env = env;
    _jstr = env->NewStringUTF(str);
}


JNIString::~JNIString()
{
    Clear();
}

JNIString JNIString::operator=(JNIString &rv)
{
    if (this != &rv)
    {
        Clear();
        
        _env = rv._env;
        _jstr = rv._jstr;
    }
    
    return *this;
}

void JNIString::Clear()
{
    if (NULL != _env)
    {
        _env->DeleteLocalRef(_jstr);
    }
}

