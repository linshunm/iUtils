package com.iutils.pattern.decorate;

import com.iutils.utils.ILog;

public class CheckAction extends Action {
    final static String tag = "CheckAction";
    @Override
    public void doAction() {
        ILog.c(tag, "only do check action");
        super.doAction();
    }
}
