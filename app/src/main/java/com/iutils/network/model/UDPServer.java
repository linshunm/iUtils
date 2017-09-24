package com.iutils.network.model;

import com.iutils.network.presenter.NetworkPresenter;
import com.iutils.utils.ILog;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import rx.Observable;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;

/**
 * Created by 10110878 on 2016/12/5.
 */
class UDPServer implements Runnable {
    private final static String TAG = "UDPServer";
    private byte[] msg = new byte[1024];
    private boolean life = true;
    private int port;
    private NetworkPresenter presenter;
    private DatagramSocket dSocket = null;

    public UDPServer(int port) {
        this.port = 6000;
        /*
        if (port == 0) {
            this.port = 6000;
        } else {
            this.port = port;
        }
        */
    }

    public boolean isLife() {
        return life;
    }

    public void setLife(boolean life) {
        this.life = life;
    }

    public void setPresenter(NetworkPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void run() {
        DatagramPacket dPacket = new DatagramPacket(msg, msg.length);
        try {
            if (dSocket == null) {
                dSocket = UDPSocket.getUdpSocket();
                dSocket.setReuseAddress(true);
                dSocket.setBroadcast(true);
                //dSocket.bind(new InetSocketAddress(port));
            }

            //dSocket = new DatagramSocket(port);
            while (life) {
                try {
                    dSocket.receive(dPacket);
                    String data = new String(dPacket.getData(), dPacket.getOffset(), dPacket.getLength());
                    final String ip = dPacket.getAddress().getHostAddress();
                    final int port = dPacket.getPort();
                    ILog.d(TAG, "address[" + ip + "] " + "port[" + port + "] data[" + data + "]");
                    Observable
                            .just(data)
                            .observeOn(AndroidSchedulers.mainThread())
                            .subscribe(new Subscriber<String>() {
                                @Override
                                public void onCompleted() {
                                }
                                @Override
                                public void onError(Throwable throwable) {
                                }
                                @Override
                                public void onNext(String s) {
                                    ILog.d(TAG, "show msg["+s+"]");
                                    if(presenter != null)
                                    {
                                        presenter.showMsg(s);
                                        presenter.setIp(ip);
                                        presenter.setPort(port);
                                    }
                                }
                            });

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(dSocket != null)
            {
                dSocket.close();
            }
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }

}
