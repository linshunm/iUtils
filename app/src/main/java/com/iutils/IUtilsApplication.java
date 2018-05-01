package com.iutils;

import android.app.Application;
import android.content.Context;
import android.os.Environment;

import com.alibaba.android.arouter.launcher.ARouter;
import com.iutils.hook.BinderHookHelper;
import com.iutils.hook.HookHelper;
import com.iutils.monitor.BlockDetectByChoreographer;
import com.iutils.monitor.BlockDetectByPrinter;
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

        //BlockDetectByPrinter.start();
        BlockDetectByChoreographer.start();

        ARouter.openLog();     // 打印日志
        //ARouter.openDebug();   // 开启调试模式(如果在InstantRun模式下运行，必须开启调试模式！线上版本需要关闭,否则有安全风险)

        ARouter.init(this); // 尽可能早，推荐在Application中初始化


//        Intent mIntent = new Intent(this, CoreService.class);
//        startService(mIntent);


        // 这两行必须写在init之前，否则这些配置在init过程中将无效
        ARouter.openLog();     // 打印日志
        ARouter.openDebug();   // 开启调试模式(如果在InstantRun模式下运行，必须开启调试模式！线上版本需要关闭,否则有安全风险)
        ARouter.init(this); // 尽可能早，推荐在Application中初始化


    }

    @Override
    protected void attachBaseContext(Context newBase) {
        super.attachBaseContext(newBase);
        try {
            // 在这里进行Hook
            HookHelper.attachContext();

            BinderHookHelper.hookClipboardService();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static RefWatcher getRefWatcher(Context context) {
        IUtilsApplication application = (IUtilsApplication) context.getApplicationContext();
        return application.refWatcher;
    }
}
