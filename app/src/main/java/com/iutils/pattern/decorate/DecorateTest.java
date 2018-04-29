package com.iutils.pattern.decorate;

public class DecorateTest {

    public static void main(String[] args){
        Action checkAction = new CheckAction();
        Action loginAction = new LoginAction();

        checkAction.setNextAction(loginAction);

        checkAction.doAction();
    }
}
