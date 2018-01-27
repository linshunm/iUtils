package com.iutils.monitor;

import android.os.Looper;
import android.util.Printer;

import com.iutils.utils.ILog;

/**
 * Created by kevin on 2018/1/26.
 */
public class BlockDetectByPrinter {
    private final static String TAG = "BlockDetectByPrinter";
    public static void start() {
        Looper.getMainLooper().setMessageLogging(new Printer() {
            private static final String START = ">>>>> Dispatching";
            private static final String END = "<<<<< Finished";
            @Override
            public void println(String x) {
                ILog.i(TAG, x);
                if (x.startsWith(START)) {
                    LogMonitor.getInstance().startMonitor();
                }
                if (x.startsWith(END)) {
                    LogMonitor.getInstance().removeMonitor();
                }
            }
        });
    }
}
