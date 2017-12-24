package com.iutils;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.os.Environment;

import com.iutils.service.CoreService;
import com.squareup.leakcanary.LeakCanary;
import com.squareup.leakcanary.RefWatcher;

import cn.jesse.nativelogger.NLogger;
import cn.jesse.nativelogger.formatter.SimpleFormatter;
import cn.jesse.nativelogger.logger.LoggerLevel;
import cn.jesse.nativelogger.util.CrashWatcher;

/**
 * Created by linshunming on 2016/12/22.
 */
public class IUtilsApplication extends Application {
    private final static String TAG = "IUtilsApplication";
    private static Context mContext;
    private static RefWatcher refWatcher;

    public static Context getIUtilsApplicationContext() {
        return mContext;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        mContext = this.getApplicationContext();


        NLogger.init(this);


        NLogger.getInstance()
                .builder()
                .tag("iUtils")
                .loggerLevel(LoggerLevel.DEBUG)
                .fileLogger(true)
                .fileDirectory(Environment.getExternalStorageDirectory().getPath() + "/iUtils/.log")
//                .fileDirectory(getApplicationContext().getFilesDir().getPath() + "/logs")
                .fileFormatter(new SimpleFormatter())
                .expiredPeriod(3)
                .catchException(true, new CrashWatcher.UncaughtExceptionListener() {
                    @Override
                    public void uncaughtException(Thread thread, Throwable ex) {
                        NLogger.e("uncaughtException", ex);
                        android.os.Process.killProcess(android.os.Process.myPid());
                    }
                })
                .build();

        refWatcher = LeakCanary.install(this);

//        Intent mIntent = new Intent(this, CoreService.class);
//        startService(mIntent);


    }

    public static RefWatcher getRefWatcher(Context context) {
        IUtilsApplication application = (IUtilsApplication) context.getApplicationContext();
        return application.refWatcher;
    }
}
