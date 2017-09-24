package com.iutils.service;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.os.SystemClock;

import com.iutils.receiver.HeartbeatReceiver;

import cn.jesse.nativelogger.NLogger;

public class CoreService extends Service {
    private static final String TAG = "CoreService";
    public CoreService() {
        NLogger.i(TAG, "CoreService");
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        startAlarm();
        return super.onStartCommand(intent, flags, startId);
    }

    private void startAlarm()
    {
        NLogger.i(TAG, "send broadcast");
        AlarmManager am =  (AlarmManager) getApplicationContext().getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(this, HeartbeatReceiver.class);
        PendingIntent pi = PendingIntent.getBroadcast(this, 1, intent, PendingIntent.FLAG_UPDATE_CURRENT);
        long offset= (long) (1 * 60 * 1000);//间隔时间4.5min
        long triggerAtTime = SystemClock.elapsedRealtime() + offset;

        // long interval = (long) (4.5 * 60 * 1000);
        //am.setRepeating(AlarmManager.RTC_WAKEUP, System.currentTimeMillis(), interval, pi);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            am.setExactAndAllowWhileIdle(AlarmManager.ELAPSED_REALTIME_WAKEUP, triggerAtTime,pi);
        }
    }
}
