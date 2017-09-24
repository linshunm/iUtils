package com.iutils.utils;

import android.content.Context;
import android.util.DisplayMetrics;
import android.view.WindowManager;

import com.iutils.IUtilsApplication;

/**
 * Created by linshunming on 2016/12/22.
 */
public class SystemUtil {

    public static int getScreenHeight()
    {
        DisplayMetrics metrics = new DisplayMetrics();
        WindowManager wm = (WindowManager) IUtilsApplication.getIUtilsApplicationContext() .getSystemService(Context.WINDOW_SERVICE);
        wm.getDefaultDisplay().getMetrics(metrics);
        return metrics.heightPixels;
    }

    public static int getScreenWidth()
    {
        DisplayMetrics metrics = new DisplayMetrics();
        WindowManager wm = (WindowManager) IUtilsApplication.getIUtilsApplicationContext() .getSystemService(Context.WINDOW_SERVICE);
        wm.getDefaultDisplay().getMetrics(metrics);
        return metrics.widthPixels;
    }
}
