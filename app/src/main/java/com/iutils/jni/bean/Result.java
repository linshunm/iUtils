package com.iutils.jni.bean;

public class Result {
    public int notifyType;
    public boolean success;
    public int resultCode;
    public String body;

    @Override
    public String toString() {
        return "Result{" +
                "notifyType=" + notifyType +
                ", success=" + success +
                ", resultCode=" + resultCode +
                ", body=" + body +
                '}';
    }
}
