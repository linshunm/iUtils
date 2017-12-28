package com.iutils.test;

import java.lang.ref.PhantomReference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.SoftReference;
import java.lang.ref.WeakReference;
import java.util.concurrent.TimeUnit;

/**
 * Created by linshunming on 2017/12/4.
 */
public class ReferenceTest {

    public static void main(String[] args)
    {

        Object object1 = new Object();
        SoftReference<Object> obj1 = new SoftReference<Object>(object1);
        object1 = null;

        System.gc();
        System.out.println("after system.gc  soft reference="+obj1.get());

        Object object2 = new Object();
        WeakReference<Object> obj2 = new WeakReference<Object>(object2);
        object2 = null;

        System.out.println("before system.gc  weak reference="+obj2.get());

        System.gc();
        System.out.println("after system.gc  weak reference="+obj2.get());

        Object object3 = new Object();
        ReferenceQueue<Object> referenceQueue = new ReferenceQueue<>();
        PhantomReference<Object> phantomReference = new PhantomReference<>(object3, referenceQueue);


        System.gc();
        System.out.println("after system.gc  phantom reference="+phantomReference.get());
    }
}
