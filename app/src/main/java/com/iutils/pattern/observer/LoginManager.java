package com.iutils.pattern.observer;

public class LoginManager {
    private static LoginManager instance;

    private IObserver observer= null;
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

    public void setObserver(IObserver iObserver){
        observer = iObserver;
    }


    public void notify(Result result){
        if(result == null) return;

        if(observer != null){
            observer.onCallback(result);
        }
    }
}
