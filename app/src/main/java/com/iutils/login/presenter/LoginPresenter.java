package com.iutils.login.presenter;

import com.iutils.common.IPresenter;
import com.iutils.login.view.ILoginView;
import com.iutils.pattern.observer.IObserver;
import com.iutils.pattern.observer.LoginManager;
import com.iutils.pattern.observer.Result;
import com.iutils.utils.ThreadUtil;

/**
 * Created by 10110878 on 2016/12/22.
 */
public class LoginPresenter implements IPresenter {
    private static final String TAG = "LoginPresenter";
    private ILoginView iView;
    private LoginManager loginManager;

    public LoginPresenter(ILoginView iView)
    {
        this.iView = iView;
        this.loginManager = LoginManager.getInstance();
    }

    public void login()
    {
        loginManager.login("10110878", "111111", new IObserver() {
            @Override
            public void onCallback(final Result result) {
                if(result != null){
                    if(result.resultCode == 202){
                        iView.runOnUiTask(new Runnable() {
                            @Override
                            public void run() {
                                iView.toast("login success");
                                iView.hideSoftInputWindow();

                                iView.gotoMainActivity();

                            }
                        });
                    }else{
                        iView.runOnUiTask(new Runnable() {
                            @Override
                            public void run() {
                                iView.toast("login failed result["+result+"]");
                                iView.hideSoftInputWindow();
                            }
                        });
                    }
                }
            }
        });
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {


            }
        });
    }

    @Override
    public void release() {
        iView = null;
    }
}
