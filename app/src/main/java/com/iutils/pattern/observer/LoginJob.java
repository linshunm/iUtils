package com.iutils.pattern.observer;

import com.iutils.utils.ILog;
import com.iutils.utils.ThreadUtil;

public class LoginJob extends Job {
    private final static String tag = "LoginJob";
    private String userId;
    private String pwd;

    private LoginJob(Builder builder){
        this.userId = builder.userId;
        this.pwd = builder.pwd;
    }

    @Override
    public void doJob(){
        ILog.c(tag, "do login sessionId["+sessionId+"] userId["+userId+"] pwd["+pwd+"]");
        LoginManager.getInstance().setObserver(this);
        //TODO FOR REQUEST

        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                try{
                    Thread.sleep(1000);
                    LoginResult result = new LoginResult();
                    result.resultCode= 202;
                    result.sessionId = sessionId;
                    LoginManager.getInstance().notify(result);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });

    }

    @Override
    public void onCallback(Result result) {
        if(result instanceof LoginResult){
            ILog.c(tag, result.toString());
            super.doJob();
        }
    }

    public static class Builder{
        private String userId;
        private String pwd;

        public Builder userId(String userId){
            this.userId = userId;
            return this;
        }

        public Builder pwd(String pwd){
            this.pwd = pwd;
            return this;
        }

        LoginJob build(){
            return new LoginJob(this);
        }
    }
}
