package com.iutils.network.utils;

import android.support.annotation.NonNull;

import com.iutils.pattern.observer.AP;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;

/**
 * Created by 10110878 on 2017/8/19.
 */
public class SocketUtil {

    private static  Charset charset = Charset.forName("utf-8");
    @NonNull
    public static String decode(ByteBuffer buffer)
    {
        CharBuffer charBuffer = charset.decode(buffer);
        return charBuffer.toString();
    }

    @NonNull
    public static ByteBuffer encode(String str)
    {
        return charset.encode(str);
    }

    public static byte[] getData(ByteBuffer buffer)
    {
        byte[] b = new byte[buffer.limit()];
        buffer.get(b, 0, buffer.limit());
        return b;
    }

    public static BufferedReader getReader(InputStream is)
    {
        return new BufferedReader(new InputStreamReader(is));
    }

    public static BufferedWriter getWriter(OutputStream os)
    {
        return new BufferedWriter(new OutputStreamWriter(os));
    }

    public static String getIpByDomain(String domain){
        String ip = null;
        InetAddress addr = null;
        try {
            addr = InetAddress.getByName(domain);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        if(addr != null){
            ip = addr.getHostAddress();
        }

        return ip;
    }


    public static void checkServerConn(AP ap){
        if(ap == null){
            return;
        }
        long startTime = System.currentTimeMillis();

        Socket socket = null;
        String localIp = null;

        try {
            socket = new Socket();
            socket.connect(new InetSocketAddress(ap.ip, ap.xcapPort), AP.TIME_OUT);

            localIp = socket.getLocalAddress() != null ?socket.getLocalAddress().getHostAddress():null;
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            if(socket != null){
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        long takeTime = System.currentTimeMillis()-startTime;

        ap.checkTime = (int)takeTime;

        if(localIp == null){
            ap.isTimeout = true;
        }else{
            ap.isTimeout = false;
        }
    }
}
