package com.iutils.pattern.decorate;

import com.iutils.utils.ILog;

public class LoginAction extends Action {
    final static String tag = "LoginAction";

    @Override
    public void doAction() {
        ILog.c(tag, "only do login action");
        super.doAction();
    }
}
