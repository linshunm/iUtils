package com.iutils.main.presenter;

import com.iutils.main.model.IMainModel;
import com.iutils.main.model.MainModelImpl;
import com.iutils.main.view.IMainView;
import com.iutils.network.bean.IupMsg;
import com.iutils.network.stack.OnRsp;
import com.iutils.utils.FileUtil;
import com.iutils.utils.ThreadUtil;
import com.iutils.utils.ToastUtil;

import java.io.File;

import cn.jesse.nativelogger.NLogger;
import cn.jesse.nativelogger.logger.base.IFileLogger;
import rx.Observable;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Func1;
import rx.schedulers.Schedulers;


/**
 * Created by 10110878 on 2016/12/10.
 */
public class MainPresenter implements OnRsp{
    private final IMainView iView;
    private final IMainModel iModel;

    private int mode = 0;//0-未选择 1-服务端 2-客户端


    @Override
    public void onCallback(final IupMsg msg) {
        iView.runOnUiTask(new Runnable() {
            @Override
            public void run() {
                iView.showInfo(new String(msg.bytesContent));
            }
        });
    }

    @Override
    public void onCallback(final String msg) {
        iView.runOnUiTask(new Runnable() {
            @Override
            public void run() {
                iView.showInfo(msg);
            }
        });
    }

    public void showInfo(final String str)
    {
        iView.runOnUiTask(new Runnable() {
            @Override
            public void run() {
                iView.showInfo(str);
            }
        });
    }

    public MainPresenter(IMainView iView)
    {
        this.iView = iView;
        iModel = new MainModelImpl(this);
    }

    public void rxCreateTest()
    {
        iModel.rxCreateTest();
    }

    public void rxMap()
    {
        final StringBuilder sb = new StringBuilder();
        Observable
                .create(new Observable.OnSubscribe<String>(){
            @Override
            public void call(Subscriber<? super String> subscriber) {
                sb.append("step 1 \n");
                subscriber.onNext("1");
                subscriber.onCompleted();
            }
        })
                .map(new Func1<String, Integer>() {
            @Override
            public Integer call(String s) {
                sb.append("step 2 \n");
                return Integer.parseInt(s);
            }
        })
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Subscriber<Integer>() {
                    @Override
                    public void onCompleted() {
                        sb.append("step 4 \n");
                        iView.showInfo(sb.toString());
                        iView.toast("onCompleted");

                    }

                    @Override
                    public void onError(Throwable throwable) {

                    }

                    @Override
                    public void onNext(Integer integer) {
                        sb.append("step 3 \n");
                    }
                });
    }

    public void zipLog()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                NLogger.zipLogs(new IFileLogger.OnZipListener() {
                    @Override
                    public void onZip(boolean succeed, String target) {
                        final String tips = "succeed : " + target;
                        if (succeed)
                            NLogger.i("zip", tips);
                        iView.runOnUiTask(new Runnable() {
                            @Override
                            public void run() {
                                ToastUtil.showToast(tips);
                            }
                        });
                    }
                });

            }
        });
    }

    public void start()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                if(mode == 0)
                {
                    showInfo("mode selected first!");
                }
                else if(mode == 1)
                {
                    iModel.startServer(iView.getPort());
                }
                else if(mode == 2)
                {
                    iModel.startClient(iView.getIp(), iView.getPort());
                }
            }
        });
    }

    public void stop()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                if(mode == 0)
                {
                    showInfo("mode selected first!");
                }
                else if(mode == 1)
                {
                    iModel.stopServer();
                }
                else if(mode == 2)
                {
                    iModel.stopClient();
                }
                setMode(0);
                showInfo("stopped");
            }
        });
    }
    public void send()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                if(mode == 0)
                {
                    showInfo("mode selected first!");
                }
                else if(mode == 1)
                {
                    iModel.sendMsgToServer(iView.getMsg());
                }
                else if(mode == 2)
                {
                    iModel.sendMsgToClient(iView.getMsg());
                }
            }
        });
    }

    public void sendFile()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                if(mode == 0)
                {
                    showInfo("mode selected first!");
                }
                else if(mode == 1 || mode == 2)
                {
                    iModel.sendFileMsg(new File(FileUtil.getSDCardPath()+File.separator+"init.lua"));
                }
            }
        });
    }

    public void setMode(int mode) {
        this.mode = mode;
    }
}
