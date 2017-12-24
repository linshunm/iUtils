package com.iutils.leak;

/**
 * Created by kevin on 2017/12/24.
 */
public class OutterClass {
    private String name;
    class InnerClass{
        public void list(){
            System.out.println("outter name is " + name);
        }
    }
}
