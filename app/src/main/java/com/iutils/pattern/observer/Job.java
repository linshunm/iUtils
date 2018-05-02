package com.iutils.pattern.observer;

import com.iutils.utils.StringUtil;

public abstract class Job implements IAction, IObserver {
    private IAction action;
    protected String sessionId;

    public Job(){
        sessionId = StringUtil.getUniqueStrId();
    }

    public void setNextAction(IAction action) {
        this.action = action;
    }

    public void doJob(){
        if(action != null){
            action.doJob();
        }
    }

}
