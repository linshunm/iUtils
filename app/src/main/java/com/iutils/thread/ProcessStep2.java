package com.iutils.thread;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by linshunming on 2017/11/14.
 */
public class ProcessStep2 implements Runnable{
    private static final Lock lock = new ReentrantLock();
    private static final Condition condition = lock.newCondition();
    private static final int count = 1000;
    private static int time = count;
    private final int step;

    public ProcessStep2(int step)
    {
        this.step = step;
    }

    @Override
    public void run() {
        System.out.println("Process step[" + step + "] start ...");
        lock.lock();

        try {
            while(time != step)
            {
                condition.await();
            }


            time --;

            System.out.println("Process step["+step+"] end!");

            condition.signalAll();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }finally {

            lock.unlock();
        }


    }

    public static void main(String[] args)
    {
        ExecutorService es = Executors.newCachedThreadPool();
        for(int i = count; i>0 ;i--)
        {
            es.execute(new ProcessStep2(i));
        }
//        for(int i = 0; i<10 ;i++)
//        {
//            es.execute(new ProcessStep(i));
//        }
    }
}
