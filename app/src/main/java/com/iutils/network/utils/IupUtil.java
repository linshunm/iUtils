package com.iutils.network.utils;

import android.support.annotation.NonNull;

import com.iutils.network.bean.IupMsg;
import com.iutils.utils.ILog;

import java.nio.ByteBuffer;

/**
 * Created by 10110878 on 2017/8/20.
 */
public class IupUtil {

private static final String TAG = "IupUtil";

    @NonNull
    public static String head(IupMsg msg)
    {
        StringBuilder sb = new StringBuilder();
        sb.append("IUP/1.0\r\n");
        if(msg.msgType == IupMsg.IM_MSG)
        {
            sb.append("IupMsg-Type:IM\r\n");
        }
        sb.append("Content-Length:"+msg.contentLength+"\r\n");
        sb.append("\r\n");

        return  sb.toString();
    }
    public static void decode(ByteBuffer buffer, IupMsg iupMsg)
    {
        buffer.flip();
        if(!iupMsg.isHeadFinished)
        {
            //需要先解释头部信息
            String tempStr = SocketUtil.decode(buffer);

            int endIndex= -1;
            int contentLength = 0 ;
            if((endIndex=tempStr.indexOf("\r\n\r\n")) == -1) {
                //头部信息还没有接收完毕,继续等待消息读取
                buffer.compact();
                return ;
            }

            ILog.i(TAG, "tempStr["+tempStr+"] length["+tempStr.length()+"]");
            String headStr = tempStr.substring(0, endIndex + 4);
            int headLength = headStr.getBytes().length;
            ILog.i(TAG, "headStr[" + headStr + "] length[" + headStr.getBytes().length + "]");
            String heads[] = headStr.split("\r\n");
            for (String s : heads) {
                String[] head = s.split(":");
                if(head.length != 2)continue;
                String headName = head[0];
                String headValue = head[1];
                if ("Content-Length".equals(headName)) {
                    contentLength = Integer.valueOf(headValue);
                    iupMsg.contentLength = contentLength;
                    iupMsg.bytesContent = new byte[contentLength];
                    continue;
                }

                if("IupMsg-Type".equals(headName))
                {
                    String strMsgType = headValue;
                    if("IM".equals(strMsgType))
                    {
                        iupMsg.msgType = IupMsg.IM_MSG;
                    }
                    continue;
                }
            }
            iupMsg.isHeadFinished = true;
            if(iupMsg.contentLength == 0)
            {
                iupMsg.isBodyFinished = true;
            }

            buffer.position(headLength);
            buffer.compact();
        }

        if(!iupMsg.isBodyFinished)
        {
            //读取body内容字节流
            buffer.flip();

            int limit = buffer.limit();

            int left = iupMsg.contentLength - iupMsg.currentContentLength;
            int bodyLength = left<=limit?left:limit;

            buffer.get(iupMsg.bytesContent, iupMsg.currentContentLength, bodyLength);

            iupMsg.currentContentLength += bodyLength;

            if (iupMsg.currentContentLength == iupMsg.contentLength)
            {
                iupMsg.isBodyFinished = true;
            }

            buffer.compact();
        }
        ILog.i(TAG, "iupMsg[" + iupMsg + "]");
    }

    public static void encode(ByteBuffer buffer, IupMsg msg)
    {
        String head = head(msg);
        ILog.i(TAG, "buffer["+buffer+"] remaining="+buffer.remaining() +"head="+head+" bytesContent="+msg.bytesContent.length);

        buffer.put(head.getBytes());
        buffer.put(msg.bytesContent);
        ILog.i(TAG, "buffer["+buffer+"]");
    }
}
