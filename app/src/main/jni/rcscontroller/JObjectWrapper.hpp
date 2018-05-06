
#include <jni.h>


static char TAG_L[64] = "JObjectWrapper";


class JNIString
{

public:
	JNIString(JNIEnv *env, const char* str)
	{
		_env = env;
		_jstr = env->NewStringUTF(str);
	}
	
	
	~JNIString()
	{
		Clear();
	}
	
	JNIString operator=(JNIString &rv)
    {		 
    	  if (this != &rv)
    	  {
    	       Clear();
    	   
     	_env = rv._env;
     	_jstr = rv._jstr;      
    	  }

        return *this;
    }

    void Clear()
    {
        if (NULL != _env)
        {
        _env->DeleteLocalRef(_jstr);
        }		
    }
	
	JNIEnv* _env;
	jstring _jstr;
};


#define JNI_STR(env, str) JNIString(env, str)._jstr


///////////////////////////////////////////////////////////////////////////////////////////
class JObjectWrapper
{
    JNIEnv* _env;
    jclass _cls;
    jobject _obj;
    bool _bNewcls;
    bool _bNewobj;

    void set(JNIEnv* env, jclass cls, jobject obj)
    {
        _env = env;
        _cls = cls;
        _obj = obj;
    }
public:
    JObjectWrapper(JNIEnv* env, jobject obj)
    {
        set(env, env->GetObjectClass(obj), obj);
        _bNewcls = true;
        _bNewobj = false;
    }
    JObjectWrapper(JNIEnv* env, jclass cls, jobject obj)
    {
        set(env, cls, obj);
        _bNewcls = false;
        _bNewobj = false;	
    }
    JObjectWrapper(JNIEnv* env, char const*  clsName)
    {
        set(env, env->FindClass(clsName), NULL);
        _bNewcls = true;
        _bNewobj = false;		
    }
    JObjectWrapper(JNIEnv* env, jclass jcls)
    {
    	set(env, jcls, NULL);
        _bNewcls = false;
        _bNewobj = false;		
    }
    JObjectWrapper(JNIEnv* env, char const*  clsName, jobject obj)
    {
        set(env, env->FindClass(clsName), obj);
        _bNewcls = true;
        _bNewobj = false;		
    }
    JObjectWrapper(JObjectWrapper const& rv)
    {
        set(rv._env, rv._cls, rv._obj);
        _bNewcls = rv._bNewcls;
        _bNewobj = rv._bNewobj;
    }
    ~JObjectWrapper()
    {
    	if (NULL != _cls && _bNewcls)
    	{
    		_env->DeleteLocalRef(_cls);
    	}
	    if (NULL != _obj && _bNewobj)
    	{
    		_env->DeleteLocalRef(_obj);
    	}
    }
    JObjectWrapper operator=(JObjectWrapper const& rv)
    {
        set(rv._env, rv._cls, rv._obj);
        _bNewcls = rv._bNewcls;
        _bNewobj = rv._bNewobj;

         return *this;
    }
    jobject GetObject()
    {
        return _obj;
    }
    jclass GetClass()
    {
        return _cls;
    }
    bool NewObject()
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
    int GetIntValue(char const* filedName)
    {
        jfieldID  fieldID = _env->GetFieldID(_cls, filedName, "I");
        return _env->GetIntField(_obj, fieldID);
    }
    void SetIntValue(char const* fieldName, int value)
    {
        jfieldID id = _env->GetFieldID(_cls, fieldName, "I");
        _env->SetIntField(_obj, id, value);
    }

    int GetLongValue(char const* filedName)
    {
        jfieldID  fieldID = _env->GetFieldID(_cls, filedName, "J");
        return _env->GetLongField(_obj, fieldID);
    }
    void SetLongValue(char const* fieldName, long value)
    {
        jfieldID id = _env->GetFieldID(_cls, fieldName, "J");
        _env->SetLongField(_obj, id, (jlong)value);
    }

    jboolean GetBoolValue(char const* filedName)
    {
        jfieldID  id = _env->GetFieldID(_cls, filedName, "Z");
        return _env->GetBooleanField(_obj, id);
    }

    void SetBoolValue(char const* fieldName, bool value)
    {
        jfieldID id = _env->GetFieldID(_cls, fieldName, "Z");
        _env->SetBooleanField(_obj, id, value);    	
    }

    void GetStringValue(char const* fieldName, char* outBuf, unsigned int bufSize)
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
    void SetStringValue(char const* fieldName, char const* value)
    {
        jfieldID id = _env->GetFieldID(_cls, fieldName, "Ljava/lang/String;");
        if(NULL == id)
        {
            LOGE(TAG_L, "SetStringValue no such field[%s]", fieldName);
            return;
        }
	 
        jstring jstr = _env->NewStringUTF(NULL==value?"":value);
        _env->SetObjectField(_obj, id, jstr);
        _env->DeleteLocalRef(jstr);
    }

    JObjectWrapper GetContainsObject(char const* fieldName, char const* clsName)
    {
        jfieldID id = _env->GetFieldID(_cls, fieldName, clsName);
        jobject containsObj = _env->GetObjectField(_obj, id);
        return JObjectWrapper(_env, containsObj);
    }

    void SetContainsObject(char const* fieldName, char const* clsName, JObjectWrapper jobj)
    {
        SetContainsObject(fieldName, clsName, jobj.GetObject());
    }

    void SetContainsObject(char const* fieldName, char const* clsName, jobject obj)
    {
        jfieldID id = _env->GetFieldID(_cls, fieldName, clsName);

        _env->SetObjectField(_obj, id, obj);
    }

    float GetFloatValue(char const* fieldName)
    {
        jfieldID  id = _env->GetFieldID(_cls, fieldName, "F");
        return  _env->GetFloatField(_obj, id);
    }
};
