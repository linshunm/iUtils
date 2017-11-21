package com.iutils.thread;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by linshunming on 2017/11/14.
 */
public class ProcessStep implements Runnable{
    //private static final Object lock = new Object();
    private static final Lock lock = new ReentrantLock();
    //private static final Condition condition = lock.newCondition();
    private static int time = 10;
    private final int step;

    public ProcessStep(int step)
    {
        this.step = step;
    }

    @Override
    public void run() {
        System.out.println("Process step["+step+"] start ...");
        synchronized (lock)
        {
            while(time != step)
            {
                try {
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            time --;

            System.out.println("Process step["+step+"] end!");

            lock.notifyAll();
        }

    }

    public static void main(String[] args)
    {
        ExecutorService es = Executors.newCachedThreadPool();
        for(int i = 10; i>0 ;i--)
        {
            es.execute(new ProcessStep(i));
        }
//        for(int i = 0; i<10 ;i++)
//        {
//            es.execute(new ProcessStep(i));
//        }
    }
}
