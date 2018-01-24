package com.iutils.network.stack;

import java.io.File;
import java.nio.channels.SelectionKey;

/**
 * Created by 10110878 on 2017/8/25.
 */
public interface IHandler {

    void connect(SelectionKey key);
    void write(SelectionKey key);
    void read(SelectionKey key);
    void setOnRsp(OnRsp onRsp);
    void sendTxtMsg(String str);
    void sendFileMsg(File file);
}
