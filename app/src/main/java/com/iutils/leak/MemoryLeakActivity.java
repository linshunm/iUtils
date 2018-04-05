package com.iutils.leak;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.webkit.DownloadListener;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.widget.Button;

import com.iutils.IUtilsApplication;
import com.iutils.R;
import com.iutils.common.BaseActivity;
import com.iutils.utils.FileUtil;
import com.iutils.utils.ILog;
import com.iutils.utils.ToastUtil;
import com.iutils.webview.MyWebViewClient;
import com.squareup.leakcanary.RefWatcher;

import java.io.File;

public class MemoryLeakActivity extends BaseActivity implements View.OnClickListener{

    private final static String TAG = "MemoryLeakActivity";
    private static OutterClass.InnerClass innerClass;

    private Button btnTest;
    private WebView webview;

    private byte[] buffers = new byte[5*1024*1024];
    
    private Handler mHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what)
            {
                case 101:
                {
                    ILog.i(TAG, "101 msg !");
                    break;
                }
                default:
                {
                    ILog.i(TAG, "default msg !");
                    break;
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_static_inner_class);
        //初始化界面
        initView();
        new AudioProcessorService(this);

        mHandler.sendEmptyMessageDelayed(101, 5*60*1000);

        OutterClass outterClass = new OutterClass();
        innerClass = outterClass.new InnerClass();
        ToastUtil.showToast("MemoryLeakActivity onCreate");

        RefWatcher refWatcher = IUtilsApplication.getRefWatcher(this);
        if(refWatcher != null)
        {
            refWatcher.watch(outterClass);
            outterClass = null;
        }
    }

    private void initView()
    {
        btnTest = (Button) this.findViewById(R.id.btn_test);
        btnTest.setOnClickListener(this);

        webview = (WebView) findViewById(R.id.wv_bs);
        webview.getSettings().setJavaScriptEnabled(true);
        webview.setWebChromeClient(new WebChromeClient(){});
        webview.requestFocus();
//              webview.loadUrl("file:///android_asset/risktest.html");
        // 设置web视图客户端
        webview.setWebViewClient(new MyWebViewClient());
        webview.setDownloadListener(new DownloadListener() {
            @Override
            public void onDownloadStart(String url, String userAgent, String contentDisposition, String mimetype, long contentLength) {

            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ToastUtil.showToast("MemoryLeakActivity onDestroy");
    }

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_test:
            {
                openFile();
                break;
            }
        }
    }

    private void openFile()
    {
        File file = new File(FileUtil.getSDCardPath()+File.separator+"v.txt");
        if(file.exists())
        {
            Intent intent = new Intent();
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setAction(Intent.ACTION_VIEW);
            String type = FileUtil.getMIMEType(file);
            intent.setDataAndType(Uri.fromFile(file), type);
            startActivity(intent);
        }
    }

}
