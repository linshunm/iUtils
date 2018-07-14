//
//  JNIString.hpp
//  USSDK_AD
//
//  Created by 林舜铭 on 2018/6/29.
//  Copyright © 2018年 林舜铭. All rights reserved.
//

#ifndef JNIString_hpp
#define JNIString_hpp

#include <stdio.h>
#include <jni.h>

class JNIString
{
    
public:
    JNIString(JNIEnv *env, const char* str);
    
    ~JNIString();
    
    JNIString operator=(JNIString &rv);
    
    void Clear();
    
    JNIEnv* _env;
    jstring _jstr;
};

#endif /* JNIString_hpp */
