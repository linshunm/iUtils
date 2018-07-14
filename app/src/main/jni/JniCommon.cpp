#include "JniCommon.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "JObjectWrapper.hpp"
#include "JniNative.h"
//#include "CrashHunter.h"

static const char *jniClsPathName = "com/iutils/jni/JniNative";
static const char *clsPathName_JniCallback = "com/iutils/jni/JniCallback";

static jclass gpostClass = NULL;
static jclass gJniCallbackCls = NULL;

static JNIClass_T gpjniClass[] =
{
    {NULL, "com/iutils/jni/bean/Result"},
    {NULL, "java/lang/String"},
    {NULL, "java/util/ArrayList"}

};

static int gpjniClassNum = sizeof(gpjniClass) / sizeof(gpjniClass[0]);

/*
 * native methods arrays
 */
static JNINativeMethod jniMethods[] =
{
    //
    {"jni_bHelloWorld", "()Ljava/lang/String;", (void*)jni_bHelloWorld},

    //other
    {"jni_bTestCrash", "()V", (void*)jni_bTestCrash}
};



int JNIGetEnv(void **o_pJNIEnv, bool &o_b8DoDetach)
{
    int iresult1 = -1;
    int iresult2 = -1;
    
    o_b8DoDetach = FALSE;

    iresult1 = gs_jvm->GetEnv(o_pJNIEnv, JNI_VERSION_1_4);
    if ( iresult1 != JNI_OK)
    {
        //LOGE(TAG, "ERROR: GetEnv failed: %d", iresult);
        iresult2 = gs_jvm->AttachCurrentThread((JNIEnv **)o_pJNIEnv, NULL);
        if (iresult2 < 0)
        {
            LOGE(TAG, "ERROR: GetEnv failed: %d & AttachCurrentThread failed: %d", iresult1, iresult2);
            return JNI_ERR;
        }
        o_b8DoDetach = TRUE;
    }
    
    return JNI_OK;
}


void JNIDetachEnv(bool &i_bDoDetach)
{
    if (i_bDoDetach)
    {
        gs_jvm->DetachCurrentThread();
        i_bDoDetach = FALSE;
    }
}

jint RegisterJNIClass(JNIEnv* env)
{
    int i;
    LOGI(TAG, "RegisterJNIClass start...");
    
    for (i=0; i<gpjniClassNum; i++)
    {
        jclass  localClass = env->FindClass(gpjniClass[i].clsName);
        if (localClass == NULL)
        {
            LOGI(TAG, "localClass[%s] no found", gpjniClass[i].clsName);
            continue;
        }
        
        gpjniClass[i].cls = (jclass)env->NewGlobalRef(localClass);
        env->DeleteLocalRef((jobject)localClass);
        
        if (gpjniClass[i].cls == NULL)
        {
            LOGI(TAG, "gpjniClass[%s].cls is NULL", gpjniClass[i].clsName);
            continue;
        }
    }
    
    LOGI(TAG, "RegisterJNIClass finish.");
    return JNI_OK;
}

jclass JNIFindClass(char *classname)
{
    int i = 0;
    
    for (i=0; i<gpjniClassNum; i ++)
    {
        if (0 == strcmp(classname, gpjniClass[i].clsName))
        {
            return gpjniClass[i].cls;
        }
    }
    
    LOGI(TAG, "JNIFindClass no found classname[%s]", classname);
    return NULL;
}


////////////////////////////////////////////////
int getStaticJniCallbackMethod(jclass *clsJniCallback, char *pFuncName, char *paraList, JNIEnv **pEnv, jmethodID *pJinMethod, bool &o_b8DoDetach)
{
    jmethodID gJinMethod;
    JNIEnv *env;
    int iresult = -1;
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    
    o_b8DoDetach = FALSE;
    LOGI(TAG, "getStaticJniCallbackMethod funcname[%s], paraList[%s]", pFuncName, paraList);
    
    if (NULL == gs_jvm)
    {
        LOGE(TAG, "gs_jvm is NULL");
        return JNI_ERR;
    }
    
    if (NULL == gJniCallbackCls)
    {
        LOGE(TAG, "gJniCallbackCls is NULL");
        
        return JNI_ERR;
    }
	else
	{
        *clsJniCallback = gJniCallbackCls;
        //LOGI(TAG, "*clsJniCallback = gJniCallbackCls");
	}
    /*
     if (gs_jvm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
     {
     LOGE(TAG, "ERROR: GetEnv failed");
     
     return JNI_ERR;
     }*/
    
    //LOGI(TAG, "JNIGetEnv");
    iresult  = JNIGetEnv(&uenv.venv, o_b8DoDetach);
    if ( iresult != JNI_OK)
    {
        return JNI_ERR;
    }
    
    env = uenv.env;
    
    /* notes: if this , when subthread invoking, will not find this clsss , so get the jclass's globalref    */
    /*
     jclass  JniClass = env->FindClass(classPathName_postagent);
     if (JniClass == NULL)
     {
     LOGI(TAG, "JniClass is NULL");
     
     return 1;
     }
     */
    
    LOGI(TAG, "get gJinMethod");
    gJinMethod = env->GetStaticMethodID(gJniCallbackCls, pFuncName, paraList);
    if(gJinMethod == 0 || gJinMethod == NULL)
    {
        LOGE(TAG, "getStaticJniCallbackMethod Failed, funcname[%s], paraList[%s]", pFuncName, paraList);
        JNIDetachEnv(o_b8DoDetach);
        return JNI_ERR;
    }
    
    *pJinMethod = gJinMethod;
    *pEnv = env;
    
    return JNI_OK;
}


bool getbooleanField(JNIEnv* env, jobject jobj, jclass jcs, const char * fieldName)
{
    jfieldID fieldID = env->GetFieldID(jcs, fieldName, "Z");
    jboolean jbResult = env->GetBooleanField(jobj, fieldID);
    return ToCppbool(jbResult);
}

int getIntField(JNIEnv* env, jobject jobj, jclass jcs, const char * fieldName)
{
    int result = -1;
    jfieldID fieldID = env->GetFieldID(jcs, fieldName, "I");
    result = env->GetIntField(jobj, fieldID);
    return result;
}

char *getStringField(JNIEnv* env, jobject jobj, jclass jcs,  const char * fieldName, char *result, int ilen)
{
    const char *str = NULL;
    
    /* get the field ID */
    jfieldID  id_softPhone = env->GetFieldID(jcs, fieldName, "Ljava/lang/String;");
    
    /* get the field value */
    jstring  softPhone = (jstring)env->GetObjectField(jobj, id_softPhone);
    if (softPhone == NULL)
    {
        return NULL;
    }
    str = env->GetStringUTFChars(softPhone, NULL);
    if (str == NULL)
    {
        return NULL; /* OutOfMemoryError already thrown */
    }
    
    strncpy(result, str, ilen);
    env->ReleaseStringUTFChars(softPhone, str);
    
    return result;
}

std::string getCppStringFromJobj(JNIEnv* env, jobject jobj, jclass jcs, const char *fieldName)
{
    const char *cValue = NULL;
    jboolean isCopy;
    
    /* get the field ID */
    jfieldID  fieldId = env->GetFieldID(jcs, fieldName, "Ljava/lang/String;");
    
    /* get the field value */
    jstring  jstr = (jstring)env->GetObjectField(jobj, fieldId);
    if (jstr == NULL)
    {
        return "";
    }
    cValue = env->GetStringUTFChars(jstr, &isCopy);
    if (cValue == NULL)
    {
        return ""; /* OutOfMemoryError already thrown */
    }
    
    std::string result(cValue);
    
    if(isCopy == JNI_TRUE)
    {
        env->ReleaseStringUTFChars(jstr, cValue);
    }
    
    return result;
}

char *safeGetStringUTFChars(JNIEnv* env, jstring string, char* dest, int size, jboolean *iscopy)
{
    const char *str = NULL;
    
    if (NULL == string)
    {
        LOGE(TAG, "the para string is null!");
        
        return JNI_FALSE;
    }
    str = env->GetStringUTFChars(string, iscopy);
    if (str == NULL)
    {
        return JNI_FALSE; /* OutOfMemoryError already thrown */
    }
    
    strncpy(dest, str, size);
    env->ReleaseStringUTFChars(string, str);
    
    return dest;
}



char *safeAllocStringUTFCharsFromJString(JNIEnv* env, jstring string)
{
    if(string != NULL)
    {
        int nStrLen = env->GetStringUTFLength(string);
        char *pString = (char*)malloc(nStrLen + 1);
        if (NULL == pString)
        {
            return NULL;
        }
        memset(pString, 0, nStrLen + 1);
        safeGetStringUTFChars(env, string, pString, nStrLen, NULL);
        
        return pString;
    }
    
    return NULL;
}


char* safeAllocStringUTFchars(JNIEnv* env, jobject jobj, jclass jcs,  const char * fieldName)
{
    //
    jfieldID id_string = env->GetFieldID(jcs, fieldName, "Ljava/lang/String;");
    /* get the field value */
    jstring  string = (jstring)env->GetObjectField(jobj, id_string);
    
    return safeAllocStringUTFCharsFromJString(env, string);
}

/*
 * register native methods for some class's methods  we know about.
 *
 * returns JNI_TRUE on success.
 */
int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    
    LOGI(TAG, "before registerNativeMethods");
    clazz = env->FindClass(className);
    
    if (clazz == NULL)
    {
        LOGE(TAG, "Native registration unable to find class '%s'", className);
        LOGE(TAG, " registerNativeMethods");
        return JNI_FALSE;
    }
    
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0)
    {
        LOGE(TAG, "RegisterNatives failed for '%s'", className);
        LOGE(TAG, "registerNativeMethods");
        return JNI_FALSE;
    }
    
    LOGI(TAG, "registerNativeMethods ok");
    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
int registerNatives(JNIEnv* env)
{
    LOGI(TAG, "registerNatives start...");
    
    if (!registerNativeMethods(env, jniClsPathName,
                               jniMethods, sizeof(jniMethods) / sizeof(jniMethods[0])))
    {
        LOGI(TAG, "registerNewNatives failed");
        return JNI_FALSE;
    }
    
    LOGI(TAG, "registerNatives finish.");
    return JNI_TRUE;
}

jboolean ToJbool(bool value)
{
    return value ? JNI_TRUE : JNI_FALSE;
}

bool ToCppbool(jboolean value)
{
    return value == JNI_TRUE;
}

std::string ToCppString(JNIEnv *env, jstring value)
{
    jboolean isCopy;
    const char *cValue = env->GetStringUTFChars(value, &isCopy);
    std::string result(cValue);
    if(isCopy == JNI_TRUE)
    {
        env->ReleaseStringUTFChars(value, cValue);
    }
    return result;
}

jstring ToJString(JNIEnv *env, std::string &value)
{
    return env->NewStringUTF(value.c_str());
}

void String2Int(int &iTmp, std::string &sTmp)
{
    std::stringstream stream(sTmp);
    stream>>iTmp;
}

void Int2String(int &iTmp, std::string &sTmp)
{
    std::stringstream stream;
    stream<<iTmp;
    sTmp = stream.str();
}

u1 checkUtfBytes(const char* bytes, const char** errorKind)
{
    while (*bytes != '\0') {
        u1 utf8 = *(bytes++);
        // Switch on the high four bits.
        switch (utf8 >> 4) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                // Bit pattern 0xxx. No need for any extra bytes.
                break;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0f:
                /*
                 * Bit pattern 10xx or 1111, which are illegal start bytes.
                 * Note: 1111 is valid for normal UTF-8, but not the
                 * modified UTF-8 used here.
                 */
                *errorKind = "start";
                return utf8;
            case 0x0e:
                // Bit pattern 1110, so there are two additional bytes.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    *errorKind = "continuation";
                    return utf8;
                }
                // Fall through to take care of the final byte.
            case 0x0c:
            case 0x0d:
                // Bit pattern 110x, so there is one additional byte.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    *errorKind = "continuation";
                    return utf8;
                }
                break;
        }
    }
    return 0;
}

jstring charTojstring(JNIEnv* env, const char* pat)
{
    //定义java String类 strClass
    jclass strClass = JNIFindClass("java/lang/String");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (env)->NewStringUTF("UTF-8");
    //将byte数组转换为java String,并输出
    jstring result = (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
    
    env->DeleteLocalRef(encoding);
    env->DeleteLocalRef(bytes);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////

//
CTMPDoDetach::CTMPDoDetach()
{
    m_bDetach = FALSE;
}
	
CTMPDoDetach::~CTMPDoDetach()
{
    JNIDetachEnv(m_bDetach);
}

/*
get the virtual machine context
*/
jint setRcsOCXEnv(JNIEnv* env)
{
    LOGI(TAG, "rcsOcx jvm Env start...");

    //int retGvm=env->GetJavaVM(&gs_jvm);

    
    jclass	jniCallbackCls = env->FindClass(clsPathName_JniCallback);
    if (jniCallbackCls == NULL)
    {
        LOGE(TAG, "jniCallbackCls is NULL");
        
        return JNI_ERR;
    }
    gJniCallbackCls = (jclass)env->NewGlobalRef(jniCallbackCls);
    env->DeleteLocalRef((jobject)jniCallbackCls);
    
    if(gJniCallbackCls == NULL)
    {
        LOGE(TAG, "gJniCallbackCls is NULL");
        
        return JNI_ERR;
    }


	RegisterJNIClass(env);

    LOGI(TAG, "rcsOcx jvm Env finish.");

    return JNI_OK;
}


/*
* This is called by the VM when the shared library is first loaded.
*/
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI(TAG, "JNI_OnLoad start...");

    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    gs_jvm = vm;
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE(TAG, "ERROR: GetEnv failed");
        goto fail;
    }

    env = uenv.env;
    if (registerNatives(env) != JNI_TRUE)
    {
        LOGE(TAG, "ERROR: registerNatives failed");
        goto fail;
    }
	
	setRcsOCXEnv(env);

    result = JNI_VERSION_1_4;
    
    LOGI(TAG, "JNI_OnLoad finish.");
    //_crashHunterSetup("/storage/sdcard/MOATEST/.Log/");
    //LOGI(TAG, "JNI_OnLoad _crashHunterSetup finish");

fail:
    LOGI(TAG, "after JNI_OnLoad");
    return result;
}

void JNI_OnUnload(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    JNIEnv* env = NULL;
    uenv.venv = NULL;

    LOGI(TAG, "JNI_OnUnload start...");
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE(TAG, "JNI_OnUnload: ERROR: GetEnv failed");
        goto fail;
    }

    env = uenv.env;
    if (NULL != gpostClass)
    {
        env->DeleteGlobalRef((jobject)gpostClass);
        gpostClass = NULL;
    }
    
    if (NULL != gJniCallbackCls)
    {
        env->DeleteGlobalRef((jobject)gJniCallbackCls);
        gJniCallbackCls = NULL;
    }

    LOGI(TAG, "JNI_OnUnload finish.");
fail:
    LOGI(TAG, "after JNI_OnUnload");
    return;
}
