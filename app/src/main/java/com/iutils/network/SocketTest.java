package com.iutils.network;

import com.iutils.network.bean.CheckIpConnTask;
import com.iutils.network.utils.SocketUtil;
import com.iutils.utils.AES128;
import com.iutils.utils.ILog;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;

import rx.Observable;
import rx.Subscriber;
import rx.functions.Action1;
import rx.schedulers.Schedulers;

/**
 * Created by linshunming on 2018/1/23.
 */
public class SocketTest {
    final static String tag = "SocketTest";

    public static void main(String[] args)
    {
        SocketTest test = new SocketTest();
        test.checkConnectTest();
        //test.fileCopy();
        //test.bufferOp();
    }

    void checkConnectTest()
    {
        String dominListStr = "moaportal.zte.com.cn|hk.moaportal.zte.com.cn|de.moaportal.zte.com.cn|sa.moaportal.zte.com.cn|br.moaportal.zte.com.cn|us.moaportal.zte.com.cn";
        int xcapPort = 8088;
        int sipTcpPort = 15065;
        String[] domainArray = dominListStr.split("\\|");

        for(String domain:domainArray){
            long startTime = System.currentTimeMillis();
            ILog.c(tag, "get ip by domain["+domain+"]");
            String ip = SocketUtil.getIpByDomain(domain);
            long endTime = System.currentTimeMillis();
            ILog.c(tag, "parse domain end, get ip["+ip+"] take["+(endTime-startTime)+"]ms");

            connServer(ip, xcapPort);
        }

    }

    void connServer(final String ip, final int port){

        new Thread(){
            public void run(){
                CheckIpConnTask task = new CheckIpConnTask.Builder(ip, port).build();
                //SocketUtil.checkServerConn(task);
                ILog.c(tag, task.toString());
            }
        }.start();
    }

    void bufferOp()
    {
        Charset charset = Charset.forName("utf-8");
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        ILog.c("buffer[" + buffer + "]");
        buffer.put("hello world!".getBytes());
        ILog.c("buffer[" + buffer + "]");

        CharBuffer charBuffer = charset.decode(buffer);
        ILog.c("buffer[" + buffer + "] content["+charBuffer.toString()+"]");
        buffer.flip();
        ILog.c("buffer[" + buffer + "]");
        buffer.clear();
        ILog.c("buffer[" + buffer + "]");
        buffer.put("1234567890".getBytes());
        ILog.c("buffer[" + buffer + "]");
        buffer.compact();
        ILog.c("buffer[" + buffer + "]");
        buffer.rewind();
        ILog.c("buffer[" + buffer + "]");
        FileOutputStream fileOutputStream = null;
        try {
            fileOutputStream = new FileOutputStream(new File("/Users/linshunming/socket_test.txt"));
            FileChannel fileChannel = fileOutputStream.getChannel();
            buffer.put("fuck you".getBytes());
            ILog.c("buffer[" + buffer + "]");
            buffer.flip();
            ILog.c("buffer[" + buffer + "]");
            while (buffer.hasRemaining())
            {
                int len = fileChannel.write(buffer);
                ILog.c("buffer[" + buffer + "] len["+len+"]");
            }
            buffer.compact();
            ILog.c("buffer[" + buffer + "]");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            if (fileOutputStream != null)
            {
                try {
                    fileOutputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    void fileCopy()
    {
        FileChannel fin = null;
        FileChannel fout = null;
        try {
            File inFile = new File("/Users/linshunming/log.txt");
            ILog.c("IN FILE MD5["+ AES128.getFileMd5Str(inFile)+"]");
            FileInputStream fileInputStream = new FileInputStream(inFile);
            fin = fileInputStream.getChannel();
            File outFile = new File("/Users/linshunming/log_bak.txt");
            if(outFile.exists()&& outFile.isFile())
            {
                outFile.delete();
            }
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            fout = fileOutputStream.getChannel();
            ByteBuffer byteBuffer = ByteBuffer.allocate(1024);

            int count = 0;
            while (fin.read(byteBuffer)>0)
            {
                ILog.c("count[" + count + "] byteBuffer[" + byteBuffer + "]");
                byteBuffer.flip();
                ILog.c("count[" + count + "] byteBuffer[" + byteBuffer + "]");
                fout.write(byteBuffer);
                ILog.c("count[" + count + "] byteBuffer[" + byteBuffer + "]");
                byteBuffer.compact();
                ILog.c("count["+count+"] byteBuffer[" + byteBuffer + "]");
                count++;
            }
            ILog.c("OUT FILE MD5["+ AES128.getFileMd5Str(outFile)+"]");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
