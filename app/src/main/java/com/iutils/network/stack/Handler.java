package com.iutils.network.stack;

import com.iutils.network.bean.IupMsg;
import com.iutils.network.utils.IupUtil;
import com.iutils.network.utils.SocketUtil;
import com.iutils.utils.ILog;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

/**
 * Created by 10110878 on 2017/8/25.
 */
public class Handler implements IHandler {

    private static final String TAG = "Handler";
    private IupMsg iupMsg = new IupMsg();
    private OnRsp onRsp = null;
    private ByteBuffer sendBuf = ByteBuffer.allocate(1024);

    @Override
    public void connect(SelectionKey key) {
        SocketChannel socketChannel = (SocketChannel) key.channel();
        if(!socketChannel.isConnected())
        {
            if(onRsp != null)
            {
                onRsp.onCallback("socket is not connect");
            }
        }
    }

    @Override
    public void write(SelectionKey key) {
        //ILog.i(TAG,"write");
        SocketChannel socketChannel = (SocketChannel) key.channel();

        synchronized (sendBuf)
        {
            sendBuf.flip();
            while (sendBuf.hasRemaining())
            {
                ILog.i(TAG,"send");
                try {
                    socketChannel.write(sendBuf);
                } catch (IOException e) {
                    e.printStackTrace();
                    try {
                        if (key != null )
                        {
                            key.cancel();
                            key.channel().close();
                        }
                        if(onRsp != null)
                        {
                            onRsp.onCallback("when write socket is not connect");
                        }
                    }catch (IOException e1)
                    {
                        e1.printStackTrace();
                    }
                }
            }
            sendBuf.compact();
        }

    }

    @Override
    public void read(SelectionKey key) {
        //ILog.i(TAG,"read");
        ByteBuffer buffer = (ByteBuffer) key.attachment();
        SocketChannel socketChannel = (SocketChannel) key.channel();
        try {
            socketChannel.read(buffer);
            IupUtil.decode(buffer,iupMsg);
            if(iupMsg.isBodyFinished)
            {
                //listenner has new msg
                if(onRsp != null)
                {
                    onRsp.onCallback(iupMsg);
                }

                iupMsg = new IupMsg();
            }
        } catch (IOException e) {
            e.printStackTrace();
            try {
                if (key != null )
                {
                    key.cancel();
                    key.channel().close();
                }
                if(onRsp != null)
                {
                    onRsp.onCallback("when read socket is not connect");
                }
            }catch (IOException e1)
            {
                e1.printStackTrace();
            }
        }
    }

    public void setOnRsp(OnRsp onRsp) {
        this.onRsp = onRsp;
    }

    public void setData(String str)
    {
        ILog.i(TAG, "str["+str.getBytes().length+"] sendBuf["+sendBuf+"]");
        synchronized (sendBuf)
        {
            sendBuf.limit(sendBuf.capacity());
            IupMsg iupMsg = new IupMsg(str);
            IupUtil.encode(sendBuf, iupMsg);
        }
    }
}
