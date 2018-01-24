package com.iutils.main.model;


import com.iutils.main.presenter.MainPresenter;
import com.iutils.network.stack.IHandler;
import com.iutils.network.stack.IupClient;
import com.iutils.network.stack.OnRsp;
import com.iutils.network.stack.IupServer;
import com.iutils.utils.FileUtil;
import com.iutils.utils.ILog;
import com.iutils.utils.ThreadUtil;

import java.io.File;
import java.io.IOException;
import java.util.Random;

import rx.Observable;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.schedulers.Schedulers;

/**
 * Created by 10110878 on 2016/12/10.
 */
public class MainModelImpl implements IMainModel {
    private final static String TAG = "MainModelImpl";
    private IupClient client;
    private IupServer server;
    private MainPresenter presenter;
    private boolean isStarted = false;

    public MainModelImpl(MainPresenter presenter)
    {
        this.presenter = presenter;
    }
    @Override
    public void rxCreateTest() {
        Observable.create(new Observable.OnSubscribe<Integer>() {

            @Override
            public void call(Subscriber<? super Integer> subscriber) {
                if (!subscriber.isUnsubscribed()) {
                    for (int i = 0; i < 5; i++) {
                        int temp = new Random().nextInt(10);
                        subscriber.onNext(temp);
                        /*
                        if (temp > 8) {
                            subscriber.onError(new Throwable("value >8"));
                            //break;
                        } else {
                            subscriber.onNext(temp);
                        }
                        */

                        // on error,complete the job

                        if (i == 4) {
                            subscriber.onCompleted();
                        }
                    }
                }
            }
        }).subscribeOn(Schedulers.io()).observeOn(AndroidSchedulers.mainThread()).subscribe(new Subscriber<Integer>() {
            @Override
            public void onCompleted() {
                ILog.i(TAG, "onCompleted");

            }

            @Override
            public void onError(Throwable throwable) {
                ILog.i(TAG, "onError["+throwable.getMessage()+"]");
            }

            @Override
            public void onNext(Integer integer) {
                ILog.d(TAG, "onNext i["+integer+"]");
            }
        });
    }

    @Override
    public void startClient(final String host, final int port) {
        ILog.i(TAG, "startClient");
        if(isStarted)return;
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                client = new IupClient(host, port);
                try {
                    client.setOnRsp(presenter);
                    client.connect();
                    isStarted = true;
                    presenter.onCallback("client started");
                } catch (IOException e) {
                    e.printStackTrace();
                    presenter.onCallback("client start exception["+e.getMessage()+"]");
                }
            }
        });

    }

    @Override
    public void stopClient() {
        ILog.i(TAG, "stopClient");
        if(!isStarted)return;
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                if(client != null)
                {
                    try {
                        client.disconnect();
                    } catch (IOException e) {
                        e.printStackTrace();
                        presenter.onCallback("client stopped exception["+e.getMessage()+"]");
                    }
                }
                isStarted = false;
                presenter.onCallback("client stopped");
            }
        });

    }

    @Override
    public void startServer(final int port) {
        ILog.i(TAG, "startServer");
        if(isStarted)return;


        try {
            server = new IupServer(port);
        } catch (IOException e) {
            e.printStackTrace();
        }
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    server.setOnRsp(presenter);
                    server.accept();
                    isStarted = true;
                    presenter.onCallback("server started");
                } catch (IOException e) {
                    e.printStackTrace();
                    presenter.onCallback("server start exception["+e.getMessage()+"]");
                }
            }
        });
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    server.service();
                } catch (IOException e) {
                    e.printStackTrace();
                    presenter.onCallback("server start exception["+e.getMessage()+"]");
                }
            }
        });

    }

    @Override
    public void stopServer() {
        ILog.i(TAG, "stopServer");
        if(!isStarted)return;
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                if(server != null)
                {
                    try {
                        server.stop();
                    } catch (IOException e) {
                        e.printStackTrace();
                        presenter.onCallback("server stopped exception["+e.getMessage()+"]");
                    }
                }
                isStarted = false;
                presenter.onCallback("server stopped");
            }
        });

    }


    @Override
    public void sendMsgToClient(String msg) {
        if(client != null)
        {
            client.sendMsg(msg);
        }
    }

    @Override
    public void sendMsgToServer(String msg) {
        if(server != null)
        {
            server.send(msg);
        }
    }

    @Override
    public void sendFileMsg(File file) {
        IHandler handler = null;
        if(client != null)
        {
            handler = client.getHandler();
        }
        else if(server != null)
        {
            handler = server.getHandler();
        }

        if(handler != null)
        {
            handler.sendFileMsg(file);
        }
    }


}
