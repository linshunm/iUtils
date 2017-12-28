package com.iutils.webview;

import android.graphics.Bitmap;
import android.webkit.WebView;
import android.webkit.WebViewClient;

/**
 * Created by linshunming on 2017/12/25.
 */
//内部类
public class MyWebViewClient extends WebViewClient {
    // 如果页面中链接，如果希望点击链接继续在当前browser中响应，
    // 而不是新开Android的系统browser中响应该链接，必须覆盖 webview的WebViewClient对象。
    public boolean shouldOverviewUrlLoading(WebView view, String url) {
        view.loadUrl(url);
        return true;
    }

    public void onPageStarted(WebView view, String url, Bitmap favicon) {
    }

    public void onPageFinished(WebView view, String url) {
    }

    public void onReceivedError(WebView view, int errorCode,
                                String description, String failingUrl) {
    }
}


