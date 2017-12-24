package com.iutils.leak;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.iutils.IUtilsApplication;
import com.iutils.R;
import com.iutils.utils.ToastUtil;
import com.squareup.leakcanary.RefWatcher;

public class StaticInnerClassActivity extends AppCompatActivity {

    private static OutterClass.InnerClass innerClass;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_static_inner_class);
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

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ToastUtil.showToast("StaticInnerClassActivity onDestroy");
    }
}
