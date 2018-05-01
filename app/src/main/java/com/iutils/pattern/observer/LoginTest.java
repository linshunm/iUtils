package com.iutils.pattern.observer;

public class LoginTest {
    public static void main(String[] args){
        CheckServerJob checkServerJob = new CheckServerJob();
        LoginJob loginJob = new LoginJob.Builder()
                .userId("sip:10110878@zte.com.cn")
                .pwd("123456")
                .build();

        checkServerJob.setNextAction(loginJob);

        checkServerJob.doJob();
    }
}
