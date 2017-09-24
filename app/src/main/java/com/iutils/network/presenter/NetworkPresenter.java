package com.iutils.network.presenter;

import com.iutils.network.model.INetworkModel;
import com.iutils.network.model.NetworkModelImpl;
import com.iutils.network.view.INetworkView;
import com.iutils.utils.ILog;

/**
 * Created by 10110878 on 2016/12/14.
 */
public class NetworkPresenter {
    private final static String TAG = "NetworkPresenter";
    private final INetworkModel iModel;
    private final INetworkView iView;

    public NetworkPresenter(INetworkView iView)
    {
        this.iView = iView;
        iModel = new NetworkModelImpl(this);
    }

    public void startUdpLisenter()
    {
        iModel.startUdpServer();
    }

    public void stopUdpLisenter()
    {
        iModel.stopUdpServer();
    }

    public int getPort()
    {
        return iView.getPort();
    }

    public String getIp()
    {
        return iView.getIp();
    }


    public void setIp(String ip) {
        iView.setIp(ip);
    }

    public void setPort(int port) {
        iView.setPort(port);
    }

    public void sendMsg()
    {
        iModel.sendMsg(iView.getSendMsg());
        iView.clearSendMsg();
    }

    public void showMsg(String msg)
    {
        ILog.d(TAG, "show msg["+msg+"]");
        iView.showRecieveMsg(msg);
    }
}
