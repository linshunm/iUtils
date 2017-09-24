package com.iutils.network.model;

import java.net.DatagramSocket;
import java.net.SocketException;

/**
 * Created by 10110878 on 2016/12/17.
 */
class UDPSocket {
    private static DatagramSocket socket = null;


    public synchronized static DatagramSocket getUdpSocket()
    {
        if(socket == null)
        {
            try {
                socket = new DatagramSocket(6000);
            }
            catch (SocketException e)
            {
                e.printStackTrace();
            }
        }
        return socket;
    }
}
