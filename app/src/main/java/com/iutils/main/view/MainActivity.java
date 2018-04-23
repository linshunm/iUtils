package com.iutils.main.view;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.alibaba.android.arouter.facade.annotation.Route;
import com.alibaba.android.arouter.launcher.ARouter;
import com.iutils.R;
import com.iutils.common.BaseActivity;
import com.iutils.framework.JniTest;
import com.iutils.main.presenter.MainPresenter;
import com.iutils.okhttp.OkHttpTestActivity;
import com.iutils.utils.ILog;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

@Route(path = "/main/MainActivity")
public class MainActivity extends BaseActivity implements IMainView {

    private static final String TAG = "MainActivity";
    @BindView(R.id.btn_server)
    Button btnServer;
    @BindView(R.id.btn_client)
    Button btnClient;
    @BindView(R.id.rl_btn)
    LinearLayout rlBtn;
    @BindView(R.id.et_ip)
    EditText etIp;
    @BindView(R.id.et_port)
    EditText etPort;
    @BindView(R.id.et_msg)
    EditText etMsg;
    @BindView(R.id.tv_info)
    TextView tvInfo;
    private MainPresenter mainPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ILog.i(TAG, "onCreate");
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        mainPresenter = new MainPresenter(this);


        initData();
    }


    private void initData() {
        tvInfo.setText("packageName: " + JniTest.getPackageName(MainActivity.this));
    }


    @OnClick(R.id.btn_test)
    void gotoTestActivity(){
        ARouter.getInstance().build("/okhttp/OkHttpTestActivity").navigation();
    }

    void start() {
        mainPresenter.start();
        showInfo("start ...");
    }

    void stop() {
        mainPresenter.stop();
        btnClient.setVisibility(View.VISIBLE);
        etIp.setVisibility(View.VISIBLE);

        btnServer.setVisibility(View.VISIBLE);
    }

    void server() {
        mainPresenter.setMode(1);
        btnServer.setVisibility(View.VISIBLE);
        btnClient.setVisibility(View.GONE);
        etIp.setVisibility(View.GONE);
    }

    void client() {
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
