package com.iutils.common;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;

import com.iutils.utils.ILog;
import com.iutils.utils.ToastUtil;

/**
 * Created by 10110878 on 2016/12/14.
 */
public class BaseActivity extends AppCompatActivity implements IView{
    private final static String TAG = "BaseActivity";
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ILog.d(TAG, "onCreate["+this.getClass().getSimpleName()+"]");
    }

    @Override
    protected void onStop() {
        super.onStop();
        ILog.d(TAG, "onStop["+this.getClass().getSimpleName()+"]");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ILog.d(TAG, "onDestroy["+this.getClass().getSimpleName()+"]");
    }

    @Override
    protected void onPause() {
        super.onPause();
        ILog.d(TAG, "onPause["+this.getClass().getSimpleName()+"]");
    }

    @Override
    protected void onResume() {
        super.onResume();
        ILog.d(TAG, "onResume["+this.getClass().getSimpleName()+"]");
    }

    @Override
    public void toast(int resInt) {
        ToastUtil.showToast(getString(resInt));
    }

    @Override
    public void toast(String tips) {
        ToastUtil.showToast(tips);
    }

    @Override
    public void runOnUiTask(Runnable r) {
        runOnUiThread(r);
    }
}
