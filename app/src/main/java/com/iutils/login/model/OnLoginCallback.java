package com.iutils.login.model;

/**
 * Created by kevin on 2017/11/4.
 */
public interface OnLoginCallback {
    void onSuccess();
    void onFailed(int retCode, String desc);
}
