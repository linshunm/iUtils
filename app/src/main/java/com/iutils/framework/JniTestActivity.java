package com.iutils.framework;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.iutils.R;
import com.iutils.test.Task;
import com.iutils.utils.ILog;

import java.util.List;

public class JniTestActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "JniTestActivity";
    Button btnTest;
    TextView tvInfo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jni_test);
        initView();
    }

    void initView()
    {
        btnTest = (Button) findViewById(R.id.btn_test);
        btnTest.setOnClickListener(this);
        tvInfo = (TextView) findViewById(R.id.tv_info);
    }

    void get2Array()
    {
        JniTest jniTest = new JniTest();
        int dimon = 10;
        int[][] int2Arr = jniTest.get2Array(dimon);
        StringBuffer sb = new StringBuffer();
        for(int i=0; i<dimon; i++)
        {
            for(int j=0; j<dimon; j++)
            {
                sb.append(int2Arr[i][j] +" ");
            }
            sb.append("\r\n");
        }
        tvInfo.setText(sb.toString());

    }

    void getTaskByNative()
    {
        try{
            JniTest jniTest = new JniTest();
            Task task = jniTest.getTask();
            if(task != null){
                tvInfo.setText(task.toString());
            }else{
                tvInfo.setText("task is null");
            }

        }catch (Exception e){

        }
    }

    void getTaskListByNative()
    {
        try{
            JniTest jniTest = new JniTest();
            List<Task> list = jniTest.getTaskList();
            ILog.i(TAG, "get list finished");

            StringBuffer sb = new StringBuffer();
            for(Task task :list){
                sb.append(task.toString()+"\r\n");
            }
            tvInfo.setText(sb.toString());
        }catch (Exception e){
            tvInfo.setText(e.toString());
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_test:
            {
                getTaskListByNative();
                //getTaskByNative();
                //get2Array();
                break;
            }
        }
    }
}
