package com.iutils.main.view;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.iutils.R;
import com.iutils.common.BaseActivity;
import com.iutils.main.presenter.MainPresenter;

public class MainActivity extends BaseActivity implements IMainView,View.OnClickListener{


    private MainPresenter mainPresenter;
    private Button btnServer;
    private Button btnClient;
    private Button btnZipLog;
    private Button btnStart;
    private Button btnStop;
    private Button btnSend;
    private TextView tvInfo;
    private EditText etIp;
    private EditText etPort;
    private EditText etMsg;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mainPresenter = new MainPresenter(this);
        initWidget();
    }

    private void initWidget()
    {
        btnZipLog = (Button)findViewById(R.id.btn_ziplog);
        btnZipLog.setOnClickListener(this);
        btnZipLog.setVisibility(View.GONE);

        btnServer = (Button)findViewById(R.id.btn_server);
        btnServer.setOnClickListener(this);
        btnClient = (Button)findViewById(R.id.btn_client);
        btnClient.setOnClickListener(this);
        btnStart = (Button)findViewById(R.id.btn_start);
        btnStart.setOnClickListener(this);
        btnStop = (Button)findViewById(R.id.btn_stop);
        btnStop.setOnClickListener(this);
        btnSend = (Button)findViewById(R.id.btn_send);
        btnSend.setOnClickListener(this);

        tvInfo = (TextView)findViewById(R.id.tv_info);
        etIp = (EditText) findViewById(R.id.et_ip);
        etPort = (EditText)findViewById(R.id.et_port);
        etMsg = (EditText)findViewById(R.id.et_msg);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_ziplog:
            {
                mainPresenter.zipLog();
                break;
            }
            case R.id.btn_start:
            {
                start();
                break;
            }
            case R.id.btn_stop:
            {
                stop();
                break;
            }
            case R.id.btn_send:
            {
                mainPresenter.send();
                break;
            }
            case R.id.btn_server:
            {
                server();
                break;
            }
            case R.id.btn_client:
            {
                client();
                break;
            }
        }
    }

    private void start()
    {
        mainPresenter.start();
        showInfo("start ...");
    }
    private void stop()
    {
        mainPresenter.stop();
        btnClient.setVisibility(View.VISIBLE);
        etIp.setVisibility(View.VISIBLE);

        btnServer.setVisibility(View.VISIBLE);
    }

    private void server()
    {
        mainPresenter.setMode(1);
        btnServer.setVisibility(View.VISIBLE);
        btnClient.setVisibility(View.GONE);
        etIp.setVisibility(View.GONE);
    }

    private void client()
    {
        mainPresenter.setMode(2);
        btnServer.setVisibility(View.GONE);
        btnClient.setVisibility(View.VISIBLE);
        etIp.setVisibility(View.VISIBLE);
    }

    @Override
    public void showInfo(String info) {
        tvInfo.setText(info);
    }

    @Override
    public String getIp() {
        return etIp.getText().toString();
    }

    @Override
    public int getPort() {
        return (int) Integer.valueOf(etPort.getText().toString());
    }

    @Override
    public String getMsg() {
        return etMsg.getText().toString();
    }
}
