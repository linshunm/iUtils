package com.iutils.login.model;

/**
 * Created by 10110878 on 2016/12/22.
 */
public interface ILoginModel {
    void login(String userid, String pwd, OnLoginCallback onLoginCallback);

}
