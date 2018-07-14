package com.iutils.framework;

import android.content.Context;
import android.media.AudioManager;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;

import com.alibaba.android.arouter.facade.annotation.Route;
import com.iutils.R;
import com.iutils.common.BaseActivity;
import com.iutils.dagger2.DaggerJniTestActivityComponent;
import com.iutils.jni.JniMrg;
import com.iutils.utils.ILog;

import javax.inject.Inject;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

@Route(path = "/framework/JniTestActivity")
public class JniTestActivity extends BaseActivity  {

    private static final String TAG = "JniTestActivity";
    @BindView(R.id.btn_test)
    Button btnTest;
    @BindView(R.id.tv_info)
    TextView tvInfo;
    private Context mContext;

    @Inject
    JniTest jniTest;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jni_test);
        ButterKnife.bind(this);
        mContext = this;

        DaggerJniTestActivityComponent.create().inject(this);

        JniMrg.getIns().initLib();

    }


    @OnClick(R.id.btn_test)
    void get2Array() {
        ILog.i(TAG, "helloworld");
        String s = JniMrg.getIns().helloworld();
        tvInfo.setText(s);

    }

    void showVolume() {
        AudioManager audioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
        int currentVolume = audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
        tvInfo.setText("当前媒体音量：" + currentVolume);
        getMainLooper().setMessageLogging(null);

    }

    void uiBlockTest() {
        block1();
        block2();
        block3();
    }

    void block1() {
        try {
            Thread.sleep(300);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    void block2() {
        try {
            Thread.sleep(600);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    void block3() {
        try {
            Thread.sleep(400);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }



}
