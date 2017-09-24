package com.iutils.login.presenter;

import com.iutils.common.IPresenter;
import com.iutils.login.model.ILoginModel;
import com.iutils.login.model.LoginModelImpl;
import com.iutils.login.view.ILoginView;
import com.iutils.utils.ThreadUtil;

/**
 * Created by 10110878 on 2016/12/22.
 */
public class LoginPresenter implements IPresenter {
    private static final String TAG = "LoginPresenter";
    private ILoginView iView;
    private ILoginModel iModel;

    public LoginPresenter(ILoginView iView)
    {
        this.iView = iView;
        this.iModel = new LoginModelImpl();
    }

    public void login()
    {
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {

                iView.runOnUiTask(new Runnable() {
                    @Override
                    public void run() {
                        iView.toast("login success");
                        iView.hideSoftInputWindow();

                        iView.gotoMainActivity();

                    }
                });
            }
        });
    }

    @Override
    public void release() {
        iView = null;
        iModel = null;
    }
}
