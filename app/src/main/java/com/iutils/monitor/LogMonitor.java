package com.iutils.monitor;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;

import com.iutils.utils.ILog;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/**
 * Created by kevin on 2018/1/26.
 */
public class LogMonitor {
    private final static String TAG = "LogMonitor";
    private static LogMonitor sInstance = new LogMonitor();
    private HandlerThread mLogThread = new HandlerThread("log");
    private Handler mIoHandler;
    //private static boolean isMonitor;
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
//            synchronized (hitMap)
//            {
//                Set<Map.Entry<Integer, Integer>> hitSet = hitMap.entrySet();
//                Iterator<Map.Entry<Integer,Integer>> iterator =hitSet.iterator();
//                while(iterator.hasNext())
//                {
//                    Map.Entry<Integer, Integer> i =  iterator.next();
//                    ILog.e(TAG, "key["+i.getKey()+"] value["+i.getValue()+"]");
//                }
//            }
//            isMonitor = false;
            clearStack();
        }
    };

    private  Runnable mCollectRunnable = new Runnable() {
        @Override
        public void run() {
            //每次采集都记录到内存中，直到采集完毕后，如果卡顿时间超过阈值则打印采集到相同堆栈信息次数最多的那个堆栈信息
            String stackInfo = getStackInfo();
            int hashCode = stackInfo.hashCode();
            int  curHit;
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

                mIoHandler.postDelayed(mCollectRunnable, TIME_COLLECT);

            }//synchronized end

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
        //return mIoHandler.hasCallbacks(mLogRunnable);
        //因为hasCallbacks方法为hide类型，无法直接引用，故只能通过反射的方式调用
        boolean isMonitor = false;
        Class handlerClass = mIoHandler.getClass();
        try {
            Method hasCallbacksMethod = handlerClass.getDeclaredMethod("hasCallbacks", Runnable.class);
            hasCallbacksMethod.setAccessible(true);
            isMonitor =  (boolean) hasCallbacksMethod.invoke(mIoHandler, mLogRunnable);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        return isMonitor;
    }

    public void startMonitor() {
        mIoHandler.postDelayed(mLogRunnable, TIME_BLOCK);
        mIoHandler.post(mClearStack);
        mIoHandler.postDelayed(mCollectRunnable, TIME_COLLECT);
//        isMonitor = true;
    }
    public void removeMonitor() {
        mIoHandler.removeCallbacks(mLogRunnable);
        mIoHandler.removeCallbacks(mCollectRunnable);
//        isMonitor = false;
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
