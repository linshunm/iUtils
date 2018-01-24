package com.iutils.network.bean;

import com.iutils.network.utils.IupUtil;
import com.iutils.utils.StringUtil;

import junit.framework.Assert;

import java.io.File;
import java.util.Arrays;

/**
 * Created by 10110878 on 2017/8/20.
 */
public class IupMsg {
    public static final String IM_TXT_MSG = "IM_TXT";
    public static final String FILE_MSG = "FILE";
    public boolean isHeadFinished = false;
    public boolean isBodyFinished = false;
    public int currentContentLength = 0;


    public String msgId;
    public String msgType;
    public long contentLength =0L;
    public byte[] bytesContent;

    public IupMsg()
    {
        this.msgId = IupUtil.getNewMsgId();
    }

    public IupMsg(String content) {
        this();
        this.msgType = IM_TXT_MSG;
        this.contentLength = content.getBytes().length;
        this.bytesContent = content.getBytes();
    }

    public IupMsg(File file)
    {
        this();
        this.msgType = FILE_MSG;
        this.contentLength = file.length();
    }

    @Override
    public String toString() {
        return "IupMsg{" +
                "msgId=" + msgId +
                ", isHeadFinished=" + isHeadFinished +
                ", isBodyFinished=" + isBodyFinished +
                ", currentContentLength=" + currentContentLength +
                ", msgType=" + msgType +
                ", contentLength=" + contentLength +
                ", bytesContent=" + StringUtil.byteArrayToString(bytesContent) +
                '}';
    }
}
