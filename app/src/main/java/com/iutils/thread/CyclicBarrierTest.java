package com.iutils.thread;

import com.iutils.utils.ILog;
import com.iutils.utils.ThreadUtil;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class CyclicBarrierTest {
    public static void main(String[] args){
        int count = 3;
        CyclicBarrier cyclicBrarrier = new CyclicBarrier(count);

        for(int i = 0; i<count;i++){
            ThreadUtil.execute(new BarrierWorkJob(cyclicBrarrier, i));
        }
    }

}

class BarrierWorkJob implements Runnable{

    private CyclicBarrier barrier;
    private final String name;
    private int index;
    public BarrierWorkJob(CyclicBarrier barrier, int index){
        this.barrier = barrier;
        this.name = "job-"+index;
        this.index = index;
    }

    @Override
    public void run() {
        try {
            if(index/2 ==0){
                Thread.sleep(4000);
            }
            ILog.c("name["+name+"] do work");
            try {
                barrier.await(1, TimeUnit.SECONDS);
            } catch (BrokenBarrierException e) {
                ILog.c("name["+name+"] BrokenBarrierException");
                e.printStackTrace();
            } catch (TimeoutException e) {
                ILog.c("name["+name+"] TimeoutException");
                e.printStackTrace();
            }
            ILog.c("name["+name+"] done");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
