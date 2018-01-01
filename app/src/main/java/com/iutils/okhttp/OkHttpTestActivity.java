package com.iutils.okhttp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.iutils.R;
import com.iutils.utils.ThreadUtil;

import java.io.IOException;

public class OkHttpTestActivity extends AppCompatActivity implements View.OnClickListener{

    private static final String TAG = "OkHttpTestActivity";

    private Button btnReq;
    private TextView tvResult;
    private OkHttpTest okHttpTest;

    private String result;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ok_http_test);

        initView();

        okHttpTest = new OkHttpTest();
    }

    private void initView()
    {
        btnReq = (Button)findViewById(R.id.btn_request);
        btnReq.setOnClickListener(this);
        tvResult = (TextView) findViewById(R.id.tv_req_result);
    }

    private void request()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    result = okHttpTest.run("http://www.wooyun.org/");
                } catch (IOException e) {
                    e.printStackTrace();
                }

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvResult.setText(result);
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

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_request:
            {
                //request();
                async();
                break;
            }
        }
    }
}
