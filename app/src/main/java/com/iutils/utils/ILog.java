package com.iutils.utils;

import android.util.Log;

import cn.jesse.nativelogger.NLogger;

/**
 * Created by 10110878 on 2016/12/10.
 */
public class ILog {
    public static void d(String tag, String msg)
    {
        NLogger.d(tag, msg);
    }

    public static void i(String tag, String msg)
    {
        //Log.i(tag, msg);
        NLogger.i(tag, msg);

    }

    public static void e(String tag, String msg)
    {
        //Log.i(tag, msg);
        NLogger.e(tag, msg);

    }


}
