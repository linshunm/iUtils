package com.iutils.pattern.observer;

import com.iutils.utils.SystemUtil;

public class UCSDK {

    /**
     * 加载so库或jni库，在使用到该库之前加载就行，不一定非要写在这个类内
     * 系统自己会判断扩展名是dll还是so,这里加载libJNI_ANDROID_TEST.so
     */
    static {
        System.loadLibrary("JNI_ANDROID_TEST");
    }

    public static void init(){
        String priDataPath = SystemUtil.getAppPath();
        String pubDataPath = SystemUtil.getWorkPath();
        LoginService.getInstance().initSDK(priDataPath, pubDataPath);

    }
}
