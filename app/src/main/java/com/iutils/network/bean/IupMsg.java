package com.iutils.network.bean;

import junit.framework.Assert;

import java.util.Arrays;

/**
 * Created by 10110878 on 2017/8/20.
 */
public class IupMsg {
    public static final int IM_MSG = 10001;
    public boolean isHeadFinished = false;
    public boolean isBodyFinished = false;
    public int currentContentLength = 0;


    public int msgType;
    public int contentLength =0;
    public byte[] bytesContent;

    public IupMsg()
    {}

    public IupMsg(String content) {
        this.msgType = IM_MSG;
        this.contentLength = content.getBytes().length;
        this.bytesContent = content.getBytes();
    }

    @Override
    public String toString() {
        return "IupMsg{" +
                "isHeadFinished=" + isHeadFinished +
                ", isBodyFinished=" + isBodyFinished +
                ", currentContentLength=" + currentContentLength +
                ", msgType=" + msgType +
                ", contentLength=" + contentLength +
                ", bytesContent=" + Arrays.toString(bytesContent) +
                '}';
    }
}
