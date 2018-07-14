package com.iutils.pattern.observer;

import com.iutils.utils.ILog;
import com.iutils.utils.SystemUtil;

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
        ILog.i(tag, "do login sessionId["+sessionId+"] userId["+userId+"] pwd["+pwd+"]");
        ObserverManager.getInstance().setObserver(this);
        //TODO FOR REQUEST

        LoginParam param = new LoginParam();
        param.account = userId;
        param.password = pwd;
        param.deviceId = SystemUtil.getDeviceId();
        param.terminalType = "Android@Phone";
        param.versionCode = SystemUtil.getVersionCode();


    }

    @Override
    public void onCallback(Result result) {
        if(result instanceof LoginResult){
            ILog.i(tag, result.toString());
            super.onCallback(result);
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
