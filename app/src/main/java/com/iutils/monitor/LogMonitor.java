package com.iutils.monitor;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.util.Log;

import com.iutils.utils.ILog;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by kevin on 2018/1/26.
 */
public class LogMonitor {
    private final static String TAG = "LogMonitor";
    private static LogMonitor sInstance = new LogMonitor();
    private HandlerThread mLogThread = new HandlerThread("log");
    private Handler mIoHandler;
    private static boolean isMonitor;
    private static final long TIME_BLOCK = 1000L;
    private static final long TIME_COLLECT = 52L;
    private static final Map<Integer, Integer> hitMap = new HashMap<>();
    private static String mStackInfo = null;
    private static int mHits = 0;

    private LogMonitor() {
        mLogThread.start();
        mIoHandler = new Handler(mLogThread.getLooper());
    }

    private static Runnable mLogRunnable = new Runnable() {
        @Override
        public void run() {
            ILog.e(TAG, "mHits["+mHits+"]  mStackInfo:"+mStackInfo);
            isMonitor = false;
            clearStack();
        }
    };

    private  Runnable mCollectRunnable = new Runnable() {
        @Override
        public void run() {
            //每次采集都记录到内存中，直到采集完毕后，如果卡顿时间超过阈值则打印采集到相同堆栈信息次数最多的那个堆栈信息
            String stackInfo = getStackInfo();
            int hashCode = stackInfo.hashCode();
            int  curHit = 0;
            synchronized (hitMap)
            {
                if(hitMap.get(hashCode)==null)
                {
                    curHit = 1;
                    hitMap.put(hashCode,curHit);
                }
                else
                {
                    curHit = hitMap.get(hashCode)+1;
                    hitMap.put(hashCode,curHit);
                }

                if(curHit>mHits)
                {
                    mHits = curHit;
                    mStackInfo = stackInfo;
                }

                if(isMonitor)
                {
                    mIoHandler.postDelayed(mCollectRunnable, TIME_COLLECT);
                }
            }

        }
    };

    private Runnable mClearStack = new Runnable() {
        @Override
        public void run() {
            clearStack();
        }
    };

    public static LogMonitor getInstance() {
        return sInstance;
    }
    public boolean isMonitor() {
//        return mIoHandler.hasCallbacks(mLogRunnable);
        return isMonitor;
    }
    public void startMonitor() {
        mIoHandler.postDelayed(mLogRunnable, TIME_BLOCK);
        mIoHandler.post(mClearStack);
        mIoHandler.postDelayed(mCollectRunnable, TIME_COLLECT);
        isMonitor = true;
    }
    public void removeMonitor() {
        mIoHandler.removeCallbacks(mLogRunnable);
        isMonitor = false;
    }

    private static String getStackInfo()
    {
        StringBuilder sb = new StringBuilder();
        StackTraceElement[] stackTrace = Looper.getMainLooper().getThread().getStackTrace();
        for (StackTraceElement s : stackTrace) {
            sb.append(s.toString() + "\n");
        }
        return sb.toString();
    }

    private static void clearStack()
    {
        synchronized (hitMap)
        {
            hitMap.clear();
            mHits = 0;
            mStackInfo = null;
        }
    }


}
