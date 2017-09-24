package com.iutils.network.model;

/**
 * Created by 10110878 on 2016/12/14.
 */
public interface INetworkModel {

    void startUdpServer();
    void stopUdpServer();
    void sendMsg(String msg);
}
