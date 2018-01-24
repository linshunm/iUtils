package com.iutils.network.stack;

import com.iutils.utils.ILog;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

/**
 * Created by 10110878 on 2017/8/17.
 */
public class IupClient {

    private final static String TAG = "IupClient";

    private SocketChannel socketChannel = null;

    private ByteBuffer sendBuf = ByteBuffer.allocate(1024);
    private ByteBuffer receiveBuf = ByteBuffer.allocate(1024);


    private Selector selector = null;

    private InetSocketAddress isa;

    private IHandler handler = null;
    private OnRsp onRsp;

    private boolean isRunning = true;

    public IupClient(String host, int port){
        isa = new InetSocketAddress(host, port);
        handler = new Handler();
    }

    public void connect() throws IOException
    {
        ILog.i(TAG, "connect to server ...");
        socketChannel = SocketChannel.open();
        socketChannel.socket().setReuseAddress(true);
        socketChannel.connect(isa);
        socketChannel.configureBlocking(false);
        ILog.i(TAG, "connect to server success");
        selector = Selector.open();
        if(onRsp != null)
        {
            onRsp.onCallback("client socket connection");
        }

        service();
    }

    public void disconnect() throws IOException {
        ILog.i(TAG, "disconnect");
        if(socketChannel != null && socketChannel.isConnected())
        {
            isRunning = false;
            socketChannel.close();
            socketChannel = null;
            selector.close();
            if(onRsp != null)
            {
                onRsp.onCallback("client socket close");
            }
        }
        ILog.i(TAG, "disconnect success");
    }

    public void sendMsg(String str) {
        ILog.i(TAG, "sendMsg str["+str+"]");
        if(handler != null)
        {
            handler.sendTxtMsg(str);
        }
        ILog.i(TAG, "sendMsg success");
    }

    private void service() throws IOException {
        if(socketChannel != null)
        {
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            socketChannel.register(selector, SelectionKey.OP_READ|SelectionKey.OP_WRITE|SelectionKey.OP_CONNECT, buffer);
        }
        else
        {
            return;
        }
        while(isRunning)
        {
            int n = selector.select();
            if(n == 0)continue;
            try
            {
                Set<SelectionKey> readyKeys = selector.selectedKeys();
                Iterator<SelectionKey> it = readyKeys.iterator();
                while (it.hasNext())
                {
                    SelectionKey key = it.next();

                    if(key.isConnectable())
                    {
                        handler.connect(key);
                    }

                    if(key.isReadable())
                    {
                        handler.read(key);
                    }

                    if(key.isWritable())
                    {
                        handler.write(key);
                    }
                    it.remove();
                }
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }

        }
    }


    public void setOnRsp(OnRsp onRsp) {
        if(handler != null)
        {
            handler.setOnRsp(onRsp);
        }
        this.onRsp = onRsp;
    }

    public IHandler getHandler()
    {
        return handler;
    }
}
