package com.iutils.network.utils;

import android.support.annotation.NonNull;

import com.iutils.network.bean.IupMsg;
import com.iutils.utils.AES128;
import com.iutils.utils.DateFormatUtil;
import com.iutils.utils.FileUtil;
import com.iutils.utils.ILog;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.HashMap;
import java.util.Map;

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
        if(IupMsg.IM_TXT_MSG.equals(msg.msgType))
        {
            sb.append("IupMsg-Type:"+IupMsg.IM_TXT_MSG+"\r\n");
        }
        else if(IupMsg.FILE_MSG.equals(msg.msgType))
        {
            sb.append("IupMsg-Type:"+IupMsg.FILE_MSG+"\r\n");
        }
        sb.append("Msg-Id:" + msg.msgId + "\r\n");
        sb.append("Content-Length:" + msg.contentLength + "\r\n");
        sb.append("\r\n");

        return  sb.toString();
    }

    private static void resolvedHead(ByteBuffer buffer, IupMsg iupMsg)
    {
        //需要先解释头部信息
        String tempStr = SocketUtil.decode(buffer);

        int endIndex= -1;
        long contentLength = 0L ;
        if((endIndex=tempStr.indexOf("\r\n\r\n")) == -1) {
            //头部信息还没有接收完毕,继续等待消息读取
            buffer.compact();
            return ;
        }

        ILog.i(TAG, "tempStr[" + tempStr + "] length[" + tempStr.length() + "]");
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
                contentLength = Long.valueOf(headValue);
                iupMsg.contentLength = contentLength;
                iupMsg.bytesContent = new byte[(int)contentLength];
                continue;
            }

            if("IupMsg-Type".equals(headName))
            {
                iupMsg.msgType = headValue;
                continue;
            }

            if("Msg-Id".equals(headName))
            {
                iupMsg.msgId = headValue;
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

    private static void resolvedBody(ByteBuffer buffer, IupMsg iupMsg)
    {
        //读取body内容字节流
        buffer.flip();

        if(IupMsg.IM_TXT_MSG.equals(iupMsg.msgType))
        {
            int limit = buffer.limit();

            long left = iupMsg.contentLength - iupMsg.currentContentLength;
            long bodyLength = left<=limit?left:limit;

            buffer.get(iupMsg.bytesContent, iupMsg.currentContentLength, (int)bodyLength);

            iupMsg.currentContentLength += bodyLength;

            if (iupMsg.currentContentLength == iupMsg.contentLength)
            {
                iupMsg.isBodyFinished = true;
            }
        }
        else if(IupMsg.FILE_MSG.equals(iupMsg.msgType))
        {
            FileChannel fileChannel = getFileChannel(iupMsg);
            try {
                int limit = buffer.limit();

                long left = iupMsg.contentLength - iupMsg.currentContentLength;
                long bodyLength = left<=limit?left:limit;

                fileChannel.write(buffer);

                iupMsg.currentContentLength += bodyLength;

                if (iupMsg.currentContentLength == iupMsg.contentLength)
                {
                    iupMsg.isBodyFinished = true;
                    closeFileChannel(iupMsg);
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        buffer.compact();
    }
    public static void decode(ByteBuffer buffer, IupMsg iupMsg)
    {
        buffer.flip();
        if(!iupMsg.isHeadFinished)
        {
            resolvedHead(buffer, iupMsg);
        }

        if(!iupMsg.isBodyFinished)
        {
            resolvedBody(buffer, iupMsg);
        }

        ILog.i(TAG, "iupMsg[" + iupMsg + "]");
    }

    private static Integer idSeq = 0;

    private static Map<String, FileOutputStream> fosm = new HashMap<>();
    private static FileChannel getFileChannel(IupMsg iupMsg)
    {
        FileOutputStream fos = fosm.get(iupMsg.msgId);
        if(fos == null)
        {
            String filepath = FileUtil.getWorkDir()+iupMsg.msgId;
            iupMsg.bytesContent = filepath.getBytes();
            try {
                fos = new FileOutputStream(new File(filepath));
                fosm.put(iupMsg.msgId, fos);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }
        return fos.getChannel();
    }

    private static void closeFileChannel(IupMsg iupMsg)
    {
        FileOutputStream fos = fosm.get(iupMsg.msgId);
        if(fos != null)
        {
            String filepath = FileUtil.getWorkDir()+iupMsg.msgId;
            File file = new File(filepath);
            try {
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            fosm.remove(iupMsg.msgId);
            ILog.i(TAG, "iupMsg[" + iupMsg + "] md5[" + AES128.getFileMd5Str(file) + "]");
        }
    }

    /**
     * 获取17位MsgId
     */
    public static String getNewMsgId()
    {
        // 设置消息随机数
        String szMsgId = null;

        String timeStamp = DateFormatUtil.getCompleteTimeStr1();

        synchronized(idSeq)
        {
            szMsgId = String.format("%s%03x", timeStamp, idSeq++%4096);
        }

        return szMsgId;
    }


}
