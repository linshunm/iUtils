package com.iutils.network.stack;

import com.iutils.network.bean.IupMsg;
import com.iutils.network.utils.IupUtil;
import com.iutils.utils.AES128;
import com.iutils.utils.ILog;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
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
        SocketChannel socketChannel = (SocketChannel) key.channel();

        synchronized (sendBuf)
        {
            //ILog.i(TAG,"write sendBuf["+sendBuf+"]");
            if(sendBuf.position() == 0)
            {
                //buffer没有数据不需要发送
                sendBuf.limit(sendBuf.capacity());
                sendBuf.notifyAll();
                return;
            }
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
            sendBuf.notifyAll();
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

    public void sendTxtMsg(String str)
    {
        ILog.i(TAG, "sendTxtMsg str["+str.getBytes().length+"] sendBuf["+sendBuf+"]");
        synchronized (sendBuf)
        {
            //sendBuf.limit(sendBuf.capacity());
            IupMsg iupMsg = new IupMsg(str);
            String head = IupUtil.head(iupMsg);
            byte[] headBytes = head.getBytes();
            int msgLength = headBytes.length +(int)iupMsg.contentLength;
            byte[] msgBytes = new byte[msgLength];
            System.arraycopy(headBytes,0, msgBytes, 0, headBytes.length);
            System.arraycopy(iupMsg.bytesContent, 0, msgBytes, headBytes.length, (int)iupMsg.contentLength);

            if(sendBuf.remaining()>msgLength)
            {
                sendBuf.put(msgBytes);
            }
            else
            {
                sendBuf.put(msgBytes,0,sendBuf.remaining());
            }


        }
    }

    @Override
    public void sendFileMsg(File file) {
        ILog.i(TAG, "sendFileMsg file["+file.getAbsolutePath()+"] filesize["+file.length()+"]bytes md5["+ AES128.getFileMd5Str(file)+"]");
        synchronized (sendBuf)
        {
            //sendBuf.limit(sendBuf.capacity());
            IupMsg iupMsg = new IupMsg(file);
            String head = IupUtil.head(iupMsg);
            ILog.i(TAG,"iupMsg["+iupMsg+"]");
            byte[] headBytes = head.getBytes();

            sendBuf.put(headBytes);
            FileInputStream fileInputStream = null;
            FileChannel fileChannel = null;

            try {
                fileInputStream = new FileInputStream(file);
                fileChannel = fileInputStream.getChannel();
                int len = 0;
                while((len=fileChannel.read(sendBuf))!=-1)
                {
                    ILog.i(TAG,"len["+len+"] sendBuf["+sendBuf+"]");
                    if(!sendBuf.hasRemaining())
                    {
                        sendBuf.wait();
                    }
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {

                if(fileChannel!= null)
                {
                    try {
                        fileChannel.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                if(fileInputStream!= null)
                {
                    try {
                        fileInputStream.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }


        }
    }


}
