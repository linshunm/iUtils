package com.iutils.thread;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by linshunming on 2017/11/14.
 */
public class ProcessStep3 implements Runnable{
    private static final Lock lock = new ReentrantLock();
    private static final int MAX_STEP = 1000;
    private static final Condition[] conditions = new Condition[MAX_STEP];
    private static int time = 0;
    private final int step;

    public ProcessStep3(int step)
    {
        this.step = step;
        if(0<=step && step<= MAX_STEP)
        {
            conditions[step] = lock.newCondition();
        }
    }

    @Override
    public void run() {
        System.out.println("Process step[" + step + "] start ...");
        lock.lock();
        System.out.println("Process step[" + step + "] get lock");

        try {
            while(time != step)
            {
                conditions[step].await();
            }


            time ++;
            if(step +1<  conditions.length)
            {
                conditions[step+1].signal();
            }

            System.out.println("Process step["+step+"] end!");

            conditions[step].signal();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }finally {

            lock.unlock();
        }


    }

    public static void main(String[] args)
    {
        ExecutorService es = Executors.newCachedThreadPool();
        for(int i = MAX_STEP-1; i>=0 ;i--)
        {
            es.execute(new ProcessStep3(i));
        }
//        for(int i = 0; i<10 ;i++)
//        {
//            es.execute(new ProcessStep(i));
//        }
    }
}
