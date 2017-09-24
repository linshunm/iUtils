package com.iutils.network.view;

/**
 * Created by 10110878 on 2016/12/14.
 */
public interface INetworkView {
    int getPort();
    String getIp();
    void setIp(String ip);
    void setPort(int port);
    void showRecieveMsg(String msg);
    String getSendMsg();
    void clearSendMsg();
}
