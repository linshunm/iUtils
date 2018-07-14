package com.iutils.jni;

import com.iutils.jni.bean.Result;
import com.iutils.utils.ILog;

public class JniCallback {
    private static final String TAG = "JniCallback";

    public static void onJniCallback(Result result){
        ILog.i(TAG, "onJniCallback result["+result+"]");
    }
}
