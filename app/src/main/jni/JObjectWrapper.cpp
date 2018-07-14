//
//  JObjectWrapper.cpp
//  USSDK_AD
//
//  Created by 林舜铭 on 2018/6/29.
//  Copyright © 2018年 林舜铭. All rights reserved.
//

#include "JObjectWrapper.hpp"
#include <jni.h>
#include "androidLog.h"
#include "JniCommon.hpp"
    
void JObjectWrapper::set(JNIEnv* env, jclass cls, jobject obj)
{
    _env = env;
    _cls = cls;
    _obj = obj;
}
    /*
     JObjectWrapper()
     {
     set(NULL, NULL, NULL);
     _bNewcls = false;
     _bNewobj = false;
     }
     */
JObjectWrapper::JObjectWrapper(JNIEnv* env, jobject obj)
{
    set(env, env->GetObjectClass(obj), obj);
    _bNewcls = true;
    _bNewobj = false;
}

JObjectWrapper::JObjectWrapper(JNIEnv* env, jclass cls, jobject obj)
{
    set(env, cls, obj);
    _bNewcls = false;
    _bNewobj = false;
}
JObjectWrapper::JObjectWrapper(JNIEnv* env, char const*  clsName)
{
    set(env, env->FindClass(clsName), NULL);
    _bNewcls = true;
    _bNewobj = false;
}
JObjectWrapper::JObjectWrapper(JNIEnv* env, jclass jcls)
{
    set(env, jcls, NULL);
    _bNewcls = false;
    _bNewobj = false;
}
JObjectWrapper::JObjectWrapper(JNIEnv* env, char const*  clsName, jobject obj)
{
    set(env, env->FindClass(clsName), obj);
    _bNewcls = true;
    _bNewobj = false;
}
JObjectWrapper::JObjectWrapper(JObjectWrapper const& rv)
{
    set(rv._env, rv._cls, rv._obj);
    _bNewcls = rv._bNewcls;
    _bNewobj = rv._bNewobj;
}

JObjectWrapper& JObjectWrapper::operator  = (JObjectWrapper &rv)
{
    LOGI(TAG_L, "operator=(JObjectWrapper const& rv");
    set(rv._env, rv._cls, rv._obj);
    _bNewcls = rv._bNewcls;
    _bNewobj = rv._bNewobj;
    return *this;
}

JObjectWrapper::~JObjectWrapper()
{
    LOGI(TAG_L, "<<<~JObjectWrapper()");
    if (NULL != _cls && _bNewcls)
    {
        _env->DeleteLocalRef(_cls);
    }
    if (NULL != _obj && _bNewobj)
    {
        _env->DeleteLocalRef(_obj);
    }
}
    /*
     JObjectWrapper operator=(JObjectWrapper const& rv)
     {
     set(rv._env, rv._cls, rv._obj);
     _bNewcls = rv._bNewcls;
     _bNewobj = rv._bNewobj;
     
     return *this;
     }
     */
jobject JObjectWrapper::GetObject()
{
    return _obj;
}

jclass JObjectWrapper::GetClass()
{
    return _cls;
}
bool JObjectWrapper::NewObject()
{
    if (_cls)
    {
        jmethodID id  = _env->GetMethodID(_cls, "<init>", "()V");
        _obj = _env->NewObject(_cls, id);
        _bNewobj = true;
        return _obj != NULL;
    }
    return false;
}

void JObjectWrapper::AutoDeleteLocalRef(bool bAuto)
{
    _bNewobj = bAuto;
}

int JObjectWrapper::GetIntValue(char const* filedName)
{
    jfieldID  fieldID = _env->GetFieldID(_cls, filedName, "I");
    return _env->GetIntField(_obj, fieldID);
}
void JObjectWrapper::SetIntValue(char const* fieldName, int value)
{
    jfieldID id = _env->GetFieldID(_cls, fieldName, "I");
    _env->SetIntField(_obj, id, value);
}

int JObjectWrapper::GetLongValue(char const* filedName)
{
    jfieldID  fieldID = _env->GetFieldID(_cls, filedName, "J");
    return _env->GetLongField(_obj, fieldID);
}
void JObjectWrapper::SetLongValue(char const* fieldName, long value)
{
    jfieldID id = _env->GetFieldID(_cls, fieldName, "J");
    _env->SetLongField(_obj, id, (jlong)value);
}

jboolean JObjectWrapper::GetBoolValue(char const* filedName)
{
    jfieldID  id = _env->GetFieldID(_cls, filedName, "Z");
    return _env->GetBooleanField(_obj, id);
}

void JObjectWrapper::SetBoolValue(char const* fieldName, bool value)
{
    jfieldID id = _env->GetFieldID(_cls, fieldName, "Z");
    jboolean jbool = value ? JNI_TRUE : JNI_FALSE;
    _env->SetBooleanField(_obj, id, jbool);
}

void JObjectWrapper::GetStringValue(char const* fieldName, char* outBuf, unsigned int bufSize)
{
    jfieldID  id = _env->GetFieldID(_cls, fieldName, "Ljava/lang/String;");
    
    jstring  softPhone = (jstring)_env->GetObjectField(_obj, id);
    if (softPhone != NULL)
    {
        const char* str = _env->GetStringUTFChars(softPhone, NULL);
        if (str != NULL)
        {
            strncpy(outBuf, str, bufSize);
            _env->ReleaseStringUTFChars(softPhone, str);
        }
    }
}

void JObjectWrapper::SetStringValue(char const* fieldName, std::string value)
{
    SetStringValue(fieldName, value.c_str());
}

void JObjectWrapper::SetStringValue(char const* fieldName, char const* value)
{
    jfieldID id = _env->GetFieldID(_cls, fieldName, "Ljava/lang/String;");
    if(NULL == id)
    {
        LOGE(TAG_L, "SetStringValue no such field[%s]", fieldName);
        return;
    }
    
    jstring jstr;
    const char* errorKind = NULL;
    u1 utf8 = checkUtfBytes(value, &errorKind);
    if (errorKind != NULL) {
        LOGW(TAG_L, "JNI WARNING: input is not valid Modified UTF-8: illegal[%s] byte[0x%x] str[%s]", errorKind, utf8, value);
        jstr = charTojstring(_env, value);
    }
    else
    {
        jstr = _env->NewStringUTF(NULL==value?"":value);
    }
    _env->SetObjectField(_obj, id, jstr);
    _env->DeleteLocalRef(jstr);
}

JObjectWrapper JObjectWrapper::GetContainsObject(char const* fieldName, char const* clsName)
{
    jfieldID id = _env->GetFieldID(_cls, fieldName, clsName);
    jobject containsObj = _env->GetObjectField(_obj, id);
    return JObjectWrapper(_env, containsObj);
}

void JObjectWrapper::SetContainsObject(char const* fieldName, char const* clsName, JObjectWrapper jobj)
{
    SetContainsObject(fieldName, clsName, jobj.GetObject());
}

void JObjectWrapper::SetContainsObject(char const* fieldName, char const* clsName, jobject obj)
{
    jfieldID id = _env->GetFieldID(_cls, fieldName, clsName);
    
    _env->SetObjectField(_obj, id, obj);
}

float JObjectWrapper::GetFloatValue(char const* fieldName)
{
    jfieldID  id = _env->GetFieldID(_cls, fieldName, "F");
    return  _env->GetFloatField(_obj, id);
}


