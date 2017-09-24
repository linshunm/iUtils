package com.iutils.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.SystemClock;

import com.iutils.service.CoreService;

import cn.jesse.nativelogger.NLogger;

public class HeartbeatReceiver extends BroadcastReceiver {
    private static final String TAG = "HeartbeatReceiver";
    private static long lastTime = 0L;
    public HeartbeatReceiver() {
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        if(lastTime == 0)
        {
            NLogger.i(TAG, "onReceive first time");
        }
        else
        {
            double interval = SystemClock.elapsedRealtime() - lastTime;
            double interMins = interval/(1000*60);
            NLogger.i(TAG, "onReceive interval time["+interMins+"]");
        }
        lastTime = SystemClock.elapsedRealtime();

        Intent mIntent = new Intent(context, CoreService.class);
        context.startService(mIntent);
    }
}
