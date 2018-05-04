package com.iutils.pattern.observer;

public class ObserverManager {
    private static ObserverManager instance;

    private IObserver observer= null;
    private ObserverManager(){}

    public static ObserverManager getInstance(){
        if(instance == null){
            synchronized (ObserverManager.class){
                if(instance == null){
                    instance = new ObserverManager();
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
