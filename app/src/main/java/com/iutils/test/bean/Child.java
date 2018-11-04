package com.iutils.test.bean;

import com.iutils.utils.ILog;

public class Child extends Parent {
    static{
        ILog.c("Child static");
    }

    public Child(){
        super();
        ILog.c("Child()");
    }
}
