package com.iutils.framework;

import com.iutils.test.Task;

import java.util.ArrayList;

import javax.inject.Inject;

/**
 * Created by kevin on 2018/2/1.
 */
public class JniTest {

    @Inject
    public JniTest(){}
    /**
     * 将用C++代码实现，在android代码中调用的方法：获取当前app的包名
     * @param o
     * @return
     */
    public static native String getPackageName(Object o);

    public native int[][] get2Array(int dimon);

    public native Task getTask();

    public native ArrayList<Task> getTaskList();

    /**
     * 加载so库或jni库，在使用到该库之前加载就行，不一定非要写在这个类内
     * 系统自己会判断扩展名是dll还是so,这里加载libJNI_ANDROID_TEST.so
     */
    static {
        System.loadLibrary("JNI_ANDROID_TEST");
    }
}
