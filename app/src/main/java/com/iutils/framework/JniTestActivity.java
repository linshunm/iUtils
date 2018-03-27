package com.iutils.framework;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.iutils.R;

public class JniTestActivity extends AppCompatActivity implements View.OnClickListener {

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

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_test:
            {
                get2Array();
                break;
            }
        }
    }
}
