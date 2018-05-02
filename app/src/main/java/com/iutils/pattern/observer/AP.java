package com.iutils.pattern.observer;

import com.iutils.network.utils.SocketUtil;
import com.iutils.utils.ThreadUtil;

import java.util.Timer;
import java.util.TimerTask;

public class AP {
    public final static int TIME_OUT = 10 * 1000;//ap检测10秒超时
    public String domain;
    public String ip;
    public int sipPort;
    public int xcapPort;
    public int checkTime;//单位毫秒
    public boolean isTimeout;

    public void check(final AP.ICheckApListener listener){
        checkTime = (int)(Math.random() * 15 * 1000);
        final Timer timer = new Timer();
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                try{
                    //Thread.sleep(checkTime);
                    ip = SocketUtil.getIpByDomain(domain);
                    SocketUtil.checkServerConn(AP.this);
                    if(!isTimeout){
                        timer.cancel();
                        isTimeout = false;
                        listener.onResult(AP.this);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                isTimeout = true;
                listener.onResult(AP.this);
            }
        }, TIME_OUT);
    }

    @Override
    public String toString() {
        return "AP{" +
                "domain='" + domain + '\'' +
                ", ip='" + ip + '\'' +
                ", sipPort=" + sipPort +
                ", xcapPort=" + xcapPort +
                ", checkTime=" + checkTime +
                ", isTimeout=" + isTimeout +
                '}';
    }

    public interface ICheckApListener{
        void onResult(AP ap);
    }
}

