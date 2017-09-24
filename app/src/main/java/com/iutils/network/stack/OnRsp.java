package com.iutils.network.stack;

import com.iutils.network.bean.IupMsg;

/**
 * Created by 10110878 on 2017/8/20.
 */
public interface OnRsp {
    void onCallback(IupMsg msg);
    void onCallback(String msg);
}
