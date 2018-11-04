package com.iutils.thread;

import com.iutils.utils.ILog;
import com.iutils.utils.ThreadUtil;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class CountDownLatchTest {
    public static void main(String[] args){
        int count = 3;
        CountDownLatch  latch = new CountDownLatch(count);
        ThreadUtil.execute(new CheckJob(latch));

        for(int i = 0; i< count; i++){
            ThreadUtil.execute(new WorkJob(latch, i));
        }
    }
}

class WorkJob implements Runnable{

    private CountDownLatch latch;
    private final String name;
    public WorkJob(CountDownLatch latch, int index){
        this.latch = latch;
        this.name = "job-"+index;
    }

    @Override
    public void run() {
        try {
            ILog.c("name["+name+"] do work");
            Thread.sleep(100000);
            ILog.c("name["+name+"] done");
            latch.countDown();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

class CheckJob implements Runnable{

    private CountDownLatch latch;
    private final String name;
    public CheckJob(CountDownLatch latch){
        this.latch = latch;
        this.name = "check-job";
    }

    @Override
    public void run() {
        try {
            ILog.c("name["+name+"] waiting for all works done");
            latch.await(1, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        ILog.c("name["+name+"] check done");
    }
}
