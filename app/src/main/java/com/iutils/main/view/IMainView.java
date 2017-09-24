package com.iutils.main.view;

import com.iutils.common.IView;

/**
 * Created by 10110878 on 2016/12/10.
 */
public interface IMainView extends IView {
    void showInfo(String info);

    String getIp();
    int getPort();

    String getMsg();

}
