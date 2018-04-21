package com.iutils.framework;

import android.content.Context;
import android.media.AudioManager;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;

import com.iutils.R;
import com.iutils.common.BaseActivity;
import com.iutils.dagger2.DaggerJniTestActivityComponent;
import com.iutils.test.Task;
import com.iutils.utils.ILog;

import java.util.List;

import javax.inject.Inject;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

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

    }


    @OnClick(R.id.btn_test)
    void get2Array() {
        ILog.i(TAG, "get2Array");
        //JniTest jniTest = new JniTest();
        int dimon = 10;
        int[][] int2Arr = jniTest.get2Array(dimon);
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < dimon; i++) {
            for (int j = 0; j < dimon; j++) {
                sb.append(int2Arr[i][j] + " ");
            }
            sb.append("\r\n");
        }
        tvInfo.setText(sb.toString());

    }

    void getTaskByNative() {
        try {
            //JniTest jniTest = new JniTest();
            Task task = jniTest.getTask();
            if (task != null) {
                tvInfo.setText(task.toString());
            } else {
                tvInfo.setText("task is null");
            }

        } catch (Exception e) {

        }
    }

    void getTaskListByNative() {
        try {
            //JniTest jniTest = new JniTest();
            List<Task> list = jniTest.getTaskList();
            ILog.i(TAG, "get list finished");

            StringBuffer sb = new StringBuffer();
            for (Task task : list) {
                sb.append(task.toString() + "\r\n");
            }
            tvInfo.setText(sb.toString());
        } catch (Exception e) {
            tvInfo.setText(e.toString());
        }
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
