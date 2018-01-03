package com.iutils.utils;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by linshunming on 2016/12/24.
 */
public class ThreadUtil {
    private final static ExecutorService es = Executors.newCachedThreadPool();

    public static void execute(Runnable r)
    {
        es.execute(r);
    }

}
