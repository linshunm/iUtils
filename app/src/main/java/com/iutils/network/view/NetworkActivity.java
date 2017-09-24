package com.iutils.network.view;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.iutils.common.BaseActivity;
import com.iutils.R;
import com.iutils.network.presenter.NetworkPresenter;

public class NetworkActivity extends BaseActivity implements View.OnClickListener, INetworkView {

    private final static String TAG = "NetworkActivity";
    private EditText etIP;
    private EditText etPort;
    private EditText etInfo;
    private TextView tvInfo;


    private NetworkPresenter presenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_network);
        initWidget();
        presenter = new NetworkPresenter(this);

    }

    @Override
    protected void onResume() {
        super.onResume();
        presenter.startUdpLisenter();
    }

    @Override
    protected void onPause() {
        super.onPause();
        presenter.stopUdpLisenter();
    }

    private void initWidget() {
        etIP = (EditText) findViewById(R.id.et_ip);
        etPort = (EditText) findViewById(R.id.et_port);
        etInfo = (EditText) findViewById(R.id.et_send_info);
        Button btnSend = (Button) findViewById(R.id.btn_send);
        assert btnSend != null;
        btnSend.setOnClickListener(this);
        tvInfo = (TextView) findViewById(R.id.tv_info);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_send: {
                presenter.sendMsg();
                break;
            }

        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public int getPort() {
        int port = 6000;
        try {
            port = Integer.valueOf(etPort.getText().toString());
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }
        return port;
    }

    @Override
    public String getIp() {
        return etIP.getText().toString();
    }

    @Override
    public void setIp(String ip) {
        etIP.setText(ip);
    }

    @Override
    public void setPort(int port) {
        etPort.setText(String.valueOf(port));
    }
    @Override
    public void showRecieveMsg(String msg) {
        tvInfo.setText(msg);
    }

    @Override
    public String getSendMsg() {
        return etInfo.getText().toString();
    }

    @Override
    public void clearSendMsg() {
        etInfo.setText("");

    }
}
