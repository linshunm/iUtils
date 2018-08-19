package com.iutils.thread;

import com.iutils.utils.ILog;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by linshunming on 2017/11/14.
 */
public class ProcessStep implements Runnable{
    //private static final Object lock = new Object();
    private static final Lock lock = new ReentrantLock();
    //private static final Condition condition = lock.newCondition();
    private static volatile int time = 9;
    private final int step;

    public ProcessStep(int step)
    {
        this.step = step;
    }

    @Override
    public void run() {
        ILog.c("Process step["+step+"] start ...");
        synchronized (lock)
        {
            while(time != step)
            {
                ILog.c("Process step["+step+"] wait");
                try {
                    Thread.sleep(100);
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            time --;

            ILog.c("Process step["+step+"] end!");

            lock.notifyAll();

            /**
             * 此处不能用notify()，因为其只会随机唤醒所有等待状态的其中一个线程，
             * 有可能导致应该被唤醒的线程没有被唤醒而使程序处于"死锁"状态；
             */
            //lock.notify();
        }

    }

    public static void main(String[] args)
    {
        ExecutorService es = Executors.newCachedThreadPool();
        /*
        for(int i = 10; i>0 ;i--)
        {
            es.execute(new ProcessStep(i));
        }
        */

        for(int i = 0; i<10 ;i++)
        {
            es.execute(new ProcessStep(i));
        }
    }
}
