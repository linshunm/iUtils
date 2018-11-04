package com.iutils.test.bean;

import com.iutils.utils.ILog;

public class Parent {
    static{
        ILog.c("Parent static");
    }

    public Parent(){
        ILog.c("Parent()");
    }
}
