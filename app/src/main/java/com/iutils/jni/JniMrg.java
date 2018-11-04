package com.iutils.jni;

public class JniMrg {
    private static final String TAG = "JniMrg";

    private static JniMrg ins;

    public static JniMrg getIns(){
        if(ins == null){
            synchronized (JniMrg.class){
                if(ins == null){
                    ins = new JniMrg();
                }
            }
        }
        return ins;
    }

    public void initLib(){
        System.loadLibrary("native-lib");
    }

    public String helloworld(){
        return JniNative.jni_bHelloWorld();
    }

    public void crash(){
        JniNative.jni_bTestCrash();
    }
}
