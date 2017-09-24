package com.iutils.network.model;

import com.iutils.network.presenter.NetworkPresenter;
import com.iutils.utils.ILog;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by 10110878 on 2016/12/14.
 */
public class NetworkModelImpl implements INetworkModel
{
    private final static String TAG = "NetworkModelImpl";
    private final NetworkPresenter presenter;
    private ExecutorService exec = null;
    private UDPServer server = null;
    //private UDPClient client = null;
    public NetworkModelImpl(NetworkPresenter presenter)
    {
        this.presenter = presenter;
        exec = Executors.newCachedThreadPool();
        server = new UDPServer(presenter.getPort());
        server.setPresenter(presenter);
        //client = new UDPClient(presenter.getIp(), presenter.getPort());
    }
    @Override
    public void startUdpServer() {
        server.setLife(true);
        exec.execute(server);
    }

    @Override
    public void stopUdpServer() {
        server.setLife(false);
        //exec.shutdown();
    }

    @Override
    public void sendMsg(final String msg) {
        exec.execute(new Runnable() {
            @Override
            public void run() {
                InetAddress local = null;
                DatagramSocket dSocket;
                String ip = presenter.getIp();
                int port = presenter.getPort();
                if (ip == null) {
                    ILog.d(TAG, "ip cann't be null return");
                    return;
                }
                if (msg == null) {
                    ILog.d(TAG, "msg is null return");
                    return;
                }

                try {
                    local = InetAddress.getByName(ip); // 本机测试
                    ILog.d(TAG, "已找到服务器,连接中...");
                } catch (UnknownHostException e) {
                    ILog.d(TAG, "未找到服务器.");
                    e.printStackTrace();
                }
                dSocket = UDPSocket.getUdpSocket(); // 注意此处要先在配置文件里设置权限,否则会抛权限不足的异常

                if(dSocket == null)
                {
                    ILog.d(TAG, "create socket exception");
                    return;
                }
                int msg_len = msg.length();
                DatagramPacket dPacket = new DatagramPacket(msg.getBytes(), msg_len,
                        local, port);
                try {
                    dSocket.send(dPacket);
                    ILog.d(TAG, "消息发送成功!");
                } catch (IOException e) {
                    e.printStackTrace();
                    ILog.d(TAG, "消息发送失败.");
                }
            }
        });
    }
}
