package com.iutils.pattern.observer;

public abstract class Job implements IAction, IObserver {
    private IAction action;

    public void setNextAction(IAction action) {
        this.action = action;
    }

    public void doJob(){
        if(action != null){
            action.doJob();
        }
    }

}
