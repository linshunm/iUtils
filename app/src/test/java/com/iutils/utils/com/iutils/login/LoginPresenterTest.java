package com.iutils.utils.com.iutils.login;

import com.iutils.login.model.ILoginModel;
import com.iutils.login.model.LoginModelImpl;
import com.iutils.login.presenter.LoginPresenter;

import org.junit.Rule;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

/**
 * Created by kevin on 2017/11/26.
 */
public class LoginPresenterTest {


    @Mock
    LoginModelImpl loginModel;

    @Rule
    public MockitoRule mockitoRule = MockitoJUnit.rule();

    @org.junit.Before
    public void setUp() throws Exception {
        System.out.println("setUp run");
    }

    @Test
    public void loginTest()throws Exception
    {
        ILoginModel loginModel = Mockito.mock(LoginModelImpl.class);
        LoginPresenter loginPresenter = new LoginPresenter(loginModel);
        loginPresenter.login("linshunm","123456");
        Mockito.verify(loginModel).login("linshunm","123456");
    }

    @Test
    public void loginTest1()throws Exception
    {
        LoginPresenter loginPresenter = new LoginPresenter(loginModel);
        loginPresenter.login("linshunm","123456");
        Mockito.verify(loginModel).login("linshunm","123456");
    }


}
