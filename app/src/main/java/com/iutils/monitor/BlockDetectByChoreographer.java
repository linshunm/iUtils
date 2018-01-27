package com.iutils.monitor;

import android.view.Choreographer;

import com.iutils.utils.ILog;

import java.util.concurrent.TimeUnit;

/**
 * Created by kevin on 2018/1/26.
 */
public class BlockDetectByChoreographer {
    private final static String TAG = "BlockDetectByChoreographer";
    public static void start() {
        Choreographer.getInstance().postFrameCallback(new Choreographer.FrameCallback() {
            long lastFrameTimeNanos = 0;
            long currentFrameTimeNanos = 0;

            @Override
            public void doFrame(long frameTimeNanos) {
                if(lastFrameTimeNanos == 0){
                    lastFrameTimeNanos = frameTimeNanos;
                }
                currentFrameTimeNanos = frameTimeNanos;
                long diffMs = TimeUnit.MILLISECONDS.convert(currentFrameTimeNanos-lastFrameTimeNanos, TimeUnit.NANOSECONDS);
                if (diffMs > 16.6f) {
//                    long droppedCount = (long)(diffMs / 16.6);
//                    ILog.i(TAG, "UI not fluency diffMs["+diffMs+"] droppedCount["+droppedCount+"]");
                }
                if (LogMonitor.getInstance().isMonitor()) {
                    LogMonitor.getInstance().removeMonitor();
                }
                LogMonitor.getInstance().startMonitor();
                Choreographer.getInstance().postFrameCallback(this);
            }
        });
    }
}
