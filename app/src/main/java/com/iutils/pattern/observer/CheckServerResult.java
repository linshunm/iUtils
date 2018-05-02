package com.iutils.pattern.observer;

public class CheckServerResult extends Result {
    public String ip;
    public int checkTime;

    @Override
    public String toString() {
        return "CheckServerResult{" +
                "ip='" + ip + '\'' +
                ", checkTime=" + checkTime +
                ", resultCode=" + resultCode +
                ", sessionId='" + sessionId + '\'' +
                '}';
    }
}
