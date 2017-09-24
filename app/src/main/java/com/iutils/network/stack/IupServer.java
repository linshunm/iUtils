package com.iutils.network.stack;

import com.iutils.utils.ILog;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

/**
 * Created by 10110878 on 2017/8/25.
 */
public class IupServer {
    private static final String TAG = "IupServer";
    private Selector selector = null;
    private ServerSocketChannel serverSocketChannel = null;
    private InetSocketAddress isa;
    private Object lock = new Object();
    private boolean isRunning = true;

    private IHandler handler = null;
    private OnRsp onRsp;

    public IupServer(int port) throws IOException {
        isa = new InetSocketAddress(port);
        selector = Selector.open();
        serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.socket().setReuseAddress(true);
        serverSocketChannel.socket().bind(isa);
        handler = new Handler();
        ILog.i(TAG, "IUP Server start");
    }

    public void accept() throws IOException {
        while(isRunning){
            try
            {
                ILog.i(TAG, "wait for connection");
                SocketChannel socketChannel = serverSocketChannel.accept();
                ILog.i(TAG, "receive new connection socket["+socketChannel.socket()+"]");
                socketChannel.configureBlocking(false);
                ByteBuffer buffer = ByteBuffer.allocate(1024);
                synchronized (lock)
                {
                    selector.wakeup();
                    socketChannel.register(selector,SelectionKey.OP_READ|SelectionKey.OP_WRITE|SelectionKey.OP_CONNECT, buffer);

                }
                if(onRsp != null)
                {
                    onRsp.onCallback("server accept socket["+socketChannel.socket()+"]");
                }
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }
    }

    public void service() throws IOException {
        while (isRunning)
        {
            synchronized (lock){};
            int n = selector.select();
            if(n == 0)continue;

            Set<SelectionKey> keySet = selector.selectedKeys();
            Iterator<SelectionKey> it = keySet.iterator();
            while (it.hasNext())
            {
                SelectionKey key = it.next();
                if(key.isWritable())
                {
                    handler.write(key);
                }
                if(key.isReadable())
                {
                    handler.read(key);
                }
                it.remove();
            }
        }
    }

    public void stop() throws IOException {
        if(serverSocketChannel!= null)
        {
            serverSocketChannel.close();
            isRunning = false;
        }
    }

    public void setOnRsp(OnRsp onRsp)
    {
        if(handler != null)
        {
            handler.setOnRsp(onRsp);
        }
        this.onRsp = onRsp;
    }

    public void send(String str)
    {
        if(handler != null)
        {
            handler.setData(str);
        }
    }



}
