package com.iutils.rxjava;

import com.iutils.utils.ILog;

import rx.Observable;
import rx.Subscriber;
import rx.functions.Action0;
import rx.functions.Action1;

/**
 * Created by linshunming on 2018/1/10.
 */
public class RxJavaTest {

    public static void main(String[] args)
    {
        RxJavaTest test = new RxJavaTest();
        test.just();
    }

    private void just()
    {
        Observable observable = Observable.just("hello", "lsm",null);
        observable.subscribe(new Action1<String>() {
            @Override
            public void call(String o) {
                ILog.c("next:"+o.length());
            }
        }, new Action1<Throwable>() {
            @Override
            public void call(Throwable throwable) {
                ILog.c("exception:");
                throwable.printStackTrace();
            }
        }, new Action0() {
            @Override
            public void call() {
                ILog.c("onCompleted");
            }
        });

        Observable.create(new Observable.OnSubscribe<String>() {
            @Override
            public void call(Subscriber<? super String> subscriber) {

            }
        });
    }
}
