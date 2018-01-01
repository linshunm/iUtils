package com.iutils.leak;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.DownloadListener;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.widget.Button;

import com.iutils.IUtilsApplication;
import com.iutils.R;
import com.iutils.utils.FileUtil;
import com.iutils.utils.ToastUtil;
import com.iutils.webview.MyWebViewClient;
import com.squareup.leakcanary.RefWatcher;

import java.io.File;

public class StaticInnerClassActivity extends AppCompatActivity implements View.OnClickListener{

    private static OutterClass.InnerClass innerClass;

    private Button btnTest;
    private WebView webview;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_static_inner_class);
        //初始化界面
        initView();

        OutterClass outterClass = new OutterClass();
        innerClass = outterClass.new InnerClass();
        ToastUtil.showToast("StaticInnerClassActivity onCreate");

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
        ToastUtil.showToast("StaticInnerClassActivity onDestroy");
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

    // 如果不做任何处理，浏览网页，点击系统“Back”键，整个Browser会调用finish()而结束自身，
    // 如果希望浏览的网 页回退而不是推出浏览器，需要在当前Activity中处理并消费掉该Back事件。
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // if((keyCode==KeyEvent.KEYCODE_BACK)&&webview.canGoBack()){
        // webview.goBack();
        // return true;
        // }
        return false;
    }

}
