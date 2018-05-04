package com.iutils.pattern.observer;

import com.iutils.utils.StringUtil;

public abstract class Job implements IAction, IObserver {
    private IAction action;
    protected String sessionId;

    private IObserver observer;

    public void setListener(IObserver observer){
        this.observer = observer;
    }

    public Job(){
        sessionId = StringUtil.getUniqueStrId();
    }

    public void setNextAction(IAction action) {
        this.action = action;
    }

    @Override
    public void doJob(){
        if(action != null){
            action.doJob();
        }
    }

    @Override
    public void onCallback(Result result){
        if(observer != null){
            observer.onCallback(result);
        }
    }

}
