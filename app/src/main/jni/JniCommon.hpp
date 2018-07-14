//
//
//  Created by 林舜铭 on 2018/5/25.
//  Copyright © 2018年 林舜铭. All rights reserved.
//

#ifndef JniCommon_hpp
#define JniCommon_hpp

#define TRUE	1
#define FALSE	0

#include <jni.h>
#include "androidLog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "JniString.hpp"
#define JNI_STR(env, str) JNIString(env, str)._jstr
#ifdef __cplusplus
extern "C" {
#endif

static char TAG[64] = "iUtils-Jni";
    
//typedef uint8_t  jboolean;       /* unsigned 8 bits */

typedef uint8_t u1;  

typedef union
{
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

typedef unsigned char byte;
static JavaVM* gs_jvm = NULL;



////////////////////
typedef struct tagJNIClass
{
    jclass cls;
    char clsName[256];
}JNIClass_T;




///////////////////////////////////////////////////////////////////
bool getbooleanField(JNIEnv* env, jobject jobj, jclass jcs, const char * fieldName);
int getIntField(JNIEnv* env, jobject jobj, jclass jcs, const char * fieldName);
char* getStringField(JNIEnv* env, jobject jobj, jclass jcs, const char * fieldName, char *result, int ilen);
std::string getCppStringFromJobj(JNIEnv* env, jobject jobj, jclass jcs, const char *fieldName);
char* safeGetStringUTFChars(JNIEnv* env, jstring string, char* dest, int size, jboolean *iscopy);
char* safeAllocStringUTFCharsFromJString(JNIEnv* env, jstring string);
char* safeAllocStringUTFchars(JNIEnv* env, jobject jobj, jclass jcs,  const char * fieldName);

int JNIGetEnv(void **o_pJNIEnv, bool &o_b8DoDetach);
void JNIDetachEnv(bool &i_bDoDetach);
jint RegisterJNIClass(JNIEnv* env);
jclass JNIFindClass(char *classname);

int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods);
int registerNatives(JNIEnv* env);

int getStaticJniCallbackMethod(jclass *clsJniCallback, char *pFuncName, char *paraList, JNIEnv **pEnv, jmethodID *pJinMethod, bool &o_b8DoDetach);


jboolean ToJbool(bool value);
bool ToCppbool(jboolean value);
std::string ToCppString(JNIEnv *env, jstring value);
jstring ToJString(JNIEnv *env, std::string &value);
void String2Int(int &iTmp, std::string &sTmp);
void Int2String(int &iTmp, std::string &sTmp);
u1 checkUtfBytes(const char* bytes, const char** errorKind);
jstring charTojstring(JNIEnv* env, const char* pat);

//////////////////////////////////////////////////////////////////////////////////////////////





class CTMPDoDetach
{
public:
    CTMPDoDetach();
	
    ~CTMPDoDetach();
public:
    bool m_bDetach;
};

/*
class CTMPDoDetach
{
public:
    CTMPDoDetach()
    {
        m_bDetach = FALSE;
    }
	
    ~CTMPDoDetach()
    {
        JNIDetachEnv(m_bDetach);
    }
public:
    bool m_bDetach;
};
*/

#ifdef __cplusplus
}
#endif

#endif /* JniCommon_hpp */
