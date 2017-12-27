package com.iutils.concurent;

import com.iutils.utils.SystemUtil;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Semaphore;

/**
 * Created by kevin on 2017/10/29.
 */
public class SemaphoreTest {

    class Task implements Runnable{
        private Semaphore semaphore = null;
        private int userId;
        public Task(Semaphore semaphore, int userId)
        {
            this.semaphore = semaphore;
            this.userId = userId;
        }
        @Override
        public void run() {
            try
            {
                semaphore.acquire();
                long start = System.currentTimeMillis();
                System.out.println("用户【"+userId+"】准备买票...");
                Thread.sleep((long)(Math.random()*10000));
                long end = System.currentTimeMillis();
                System.out.println("用户【"+userId+"】买票结束离开,用时【"+(end-start)+"ms】");
                semaphore.release();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void execute()
    {
        Semaphore semaphore = new Semaphore(2);
        ExecutorService es = Executors.newCachedThreadPool();
        for(int i=0 ;i<20;i++){
            es.execute(new Task(semaphore,i+1));
        }
        es.shutdown();
    }

    public  static void main(String[] agrs)
    {
        SemaphoreTest semaphoreTest = new SemaphoreTest();
        semaphoreTest.execute();
    }


}
