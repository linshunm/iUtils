package com.iutils.main.model;


import java.io.File;

/**
 * Created by 10110878 on 2016/12/10.
 */
public interface IMainModel {

    void rxCreateTest();
    void startClient(String host, int port);
    void stopClient();
    void startServer(int port);
    void stopServer();
    void sendMsgToClient(String msg);
    void sendMsgToServer(String msg);
    void sendFileMsg(File file);
}
