
#ifndef JObjectWrapper_hpp
#define JObjectWrapper_hpp
#include <jni.h>
#include "androidLog.h"
#include <stdio.h>
#include <string>

static char TAG_L[64] = "JObjectWrapper";
using namespace std;

class JObjectWrapper
{
    JNIEnv* _env;
    jclass _cls;
    jobject _obj;
    bool _bNewcls;
    bool _bNewobj;

    void set(JNIEnv* env, jclass cls, jobject obj);
public:
    /*
    JObjectWrapper()
    {
        set(NULL, NULL, NULL);
        _bNewcls = false;
        _bNewobj = false;
    }
     */
    JObjectWrapper(JNIEnv* env, jobject obj);
    
    JObjectWrapper(JNIEnv* env, jclass cls, jobject obj);
    
    JObjectWrapper(JNIEnv* env, char const*  clsName);
    
    JObjectWrapper(JNIEnv* env, jclass jcls);
    
    JObjectWrapper(JNIEnv* env, char const*  clsName, jobject obj);
    
    JObjectWrapper(JObjectWrapper const& rv);
    
    JObjectWrapper& operator  = (JObjectWrapper &rv);
    
    ~JObjectWrapper();
    
    /*
    JObjectWrapper operator=(JObjectWrapper const& rv)
    {
        set(rv._env, rv._cls, rv._obj);
        _bNewcls = rv._bNewcls;
        _bNewobj = rv._bNewobj;

         return *this;
    }
     */
    
    jobject GetObject();
    
    jclass GetClass();
    
    bool NewObject();
    
    void AutoDeleteLocalRef(bool bAuto);
    
    int GetIntValue(char const* filedName);
    
    void SetIntValue(char const* fieldName, int value);

    int GetLongValue(char const* filedName);
    
    void SetLongValue(char const* fieldName, long value);

    jboolean GetBoolValue(char const* filedName);

    void SetBoolValue(char const* fieldName, bool value);

    void GetStringValue(char const* fieldName, char* outBuf, unsigned int bufSize);
    
    void SetStringValue(char const* fieldName, std::string value);
    
    void SetStringValue(char const* fieldName, char const* value);

    JObjectWrapper GetContainsObject(char const* fieldName, char const* clsName);

    void SetContainsObject(char const* fieldName, char const* clsName, JObjectWrapper jobj);

    void SetContainsObject(char const* fieldName, char const* clsName, jobject obj);

    float GetFloatValue(char const* fieldName);    
};

#endif /* JObjectWrapper_hpp */
