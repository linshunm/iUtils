package com.iutils.test;

/**
 * Created by linshunming on 2017/10/30.
 */
public class Cycle
{
    private final int balance;

    private static final Cycle c = new Cycle();
    private static final int deposit = (int) (Math.random()*100);

    public Cycle()
    {
        balance = deposit - 10;
    }


    public static void main(String[] args)
    {
        System.out.println("The account balance is:"+c.balance);
    }
}
