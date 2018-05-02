package com.iutils.okhttp;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.alibaba.android.arouter.facade.annotation.Route;
import com.iutils.R;
import com.iutils.dagger2.DaggerOkHttpTestActivityComponent;
import com.iutils.utils.ILog;
import com.iutils.utils.ThreadUtil;

import java.io.IOException;

import javax.inject.Inject;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import rx.Observable;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.schedulers.Schedulers;

@Route(path = "/okhttp/OkHttpTestActivity")
public class OkHttpTestActivity extends AppCompatActivity{

    private static final String TAG = "OkHttpTestActivity";

    @BindView(R.id.btn_request)
    Button btnReq;
    @BindView(R.id.tv_req_result)
    TextView tvResult;
    @Inject
    OkHttpTest okHttpTest;

    private String result;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ok_http_test);
        DaggerOkHttpTestActivityComponent.create().inject(this);
        ButterKnife.bind(this);

    }


    @OnClick(R.id.btn_request)
    void request()
    {
        Observable.create(new Observable.OnSubscribe<String>() {
            @Override
            public void call(Subscriber<? super String> subscriber) {
                String result = null;
                try {
                    result = okHttpTest.run("http://www.wooyun.org/");
                } catch (IOException e) {
                    e.printStackTrace();
                    subscriber.onError(e.getCause());
                }
                subscriber.onNext(result);
                subscriber.onCompleted();
            }
        })
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<String>() {
            @Override
            public void call(String s) {
                tvResult.setText(s);
            }
        });
    }

    private void download()
    {
        String url = "http://moatest.zte.com.cn/m/zte/MOA-zte-3.10.1-beta-20171225-1.apk";
        DownloadUtil.get().download(url, "download", new DownloadUtil.OnDownloadListener() {
            @Override
            public void onDownloadSuccess() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvResult.setText("下载完成");
                    }
                });

            }
            @Override
            public void onDownloading(final int progress) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvResult.setText("下载进展："+progress+"%");
                    }
                });
            }
            @Override
            public void onDownloadFailed() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvResult.setText("下载失败");
                    }
                });

            }
        });

    }

    private void async()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    okHttpTest.async();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private void block1()
    {
        try {
            ILog.i(TAG, "block1 sleep");
            Thread.sleep(200);
            ILog.i(TAG, "I'm wakeup");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void block2()
    {
        try {
            ILog.i(TAG, "sleep block2");
            Thread.sleep(500);
            ILog.i(TAG, "I'm wakeup");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    private void block3()
    {
        try {
            ILog.i(TAG, "block3 sleep");
            Thread.sleep(350);
            ILog.i(TAG, "I'm wakeup");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
