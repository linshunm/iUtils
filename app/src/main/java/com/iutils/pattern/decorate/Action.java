package com.iutils.pattern.decorate;

public abstract class Action implements IAction{

    private IAction mAction;
    public void setNextAction(IAction action){
        mAction = action;
    }

    @Override
    public void doAction() {
        if(mAction != null){
            mAction.doAction();
        }
    }
}
