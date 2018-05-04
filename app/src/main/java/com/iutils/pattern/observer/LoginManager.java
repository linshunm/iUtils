package com.iutils.pattern.observer;

public class LoginManager {
    private static LoginManager instance;

    private LoginManager(){}

    public static LoginManager getInstance(){
        if(instance == null){
            synchronized (LoginManager.class){
                if(instance == null){
                    instance = new LoginManager();
                }
            }
        }
        return instance;
    }

    public void login(String userId, String password, IObserver observer){
        CheckServerJob checkServerJob = new CheckServerJob();
        LoginJob loginJob = new LoginJob.Builder()
                .userId("sip:"+userId+"@zte.com.cn")
                .pwd(password)
                .build();
        loginJob.setListener(observer);

        checkServerJob.setNextAction(loginJob);

        checkServerJob.doJob();
    }
}
