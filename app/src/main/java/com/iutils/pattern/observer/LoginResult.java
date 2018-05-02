package com.iutils.pattern.observer;

public class LoginResult extends Result {
    @Override
    public String toString() {
        return "LoginResult{" +
                "resultCode=" + resultCode +
                ", sessionId='" + sessionId + '\'' +
                '}';
    }
}
