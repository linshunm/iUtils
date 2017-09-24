package com.iutils.common;

/**
 * Created by linshunming on 2016/12/24.
 */
public interface IView {
    public void toast(int resInt);
    public void toast(String tips);
    public void runOnUiTask(Runnable r);
}
