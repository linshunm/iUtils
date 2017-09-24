package com.iutils.network.model;

import com.iutils.utils.ILog;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 * Created by 10110878 on 2016/12/5.
 */
class UDPClient implements Runnable {
    private DatagramSocket dSocket = null;
    private String msg;
    private boolean isStop = false;
    private final static String TAG = "UDPClient";
    private String ip;
    private int port;


    public UDPClient(String ip, int port) {
        this.ip = ip;
        this.port = port;
    }

    public void setStop(boolean stop) {
        isStop = stop;
    }

    public void send(String msg) {
        synchronized (this) {
            this.msg = msg;
            notify();
        }
    }


    @Override
    public void run() {
        InetAddress local = null;
        if (ip == null) {
            ILog.d(TAG, "ip cann't be null");
            return;
        }
        try {
            local = InetAddress.getByName(ip); // 本机测试
            ILog.d(TAG, "已找到服务器,连接中...");
        } catch (UnknownHostException e) {
            ILog.d(TAG, "未找到服务器.");
            e.printStackTrace();
        }
        try {
            dSocket = new DatagramSocket(6000); // 注意此处要先在配置文件里设置权限,否则会抛权限不足的异常
            ILog.d(TAG, "正在连接服务器...");
        } catch (SocketException e) {
            e.printStackTrace();
            ILog.d(TAG, "服务器连接失败.");
        }
        while (!isStop) {
            synchronized (this) {
                if (msg == null) {
                    try {
                        wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                int msg_len = msg == null ? 0 : msg.length();
                DatagramPacket dPacket = new DatagramPacket(msg.getBytes(), msg_len,
                        local, port);
                try {
                    dSocket.send(dPacket);
                    ILog.d(TAG, "消息发送成功!");
                    msg = null;
                    //sb.append("消息发送成功!").append("/n");
                } catch (IOException e) {
                    e.printStackTrace();
                    //sb.append("消息发送失败.").append("/n");
                }
            }
        }

        dSocket.close();
    }
}
