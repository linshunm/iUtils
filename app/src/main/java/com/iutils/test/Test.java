package com.iutils.test;

import com.iutils.utils.AES128;
import com.iutils.utils.ILog;
import com.iutils.utils.StringUtil;
import com.iutils.utils.SystemUtil;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.security.NoSuchAlgorithmException;

/**
 * Created by linshunming on 2017/9/20.
 */
public class Test {

    static {
        temp = 2;
        //System.out.println("temp:"+temp);
    }
    public static int temp = 1;

    public static void main(String[] agrs)
    {
        System.out.println("hello world !");
        temp = 3;
        System.out.println("temp:" + temp);

        try {
            Class stClass = Class.forName("com.iutils.utils.StringUtil");

            Method method = stClass.getMethod("replaceBlank", String.class);
            String ret = (String)method.invoke(null, "lin shunming");
            System.out.println("ret="+ret);

        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }

        try {
            byte[] bytes = AES128.getMd5Bytes("123456");
            System.out.println(bytes);
            byte[] b = new byte[2];
            b[0] = 'A';
            b[1] = 0x0D;
            System.out.println(AES128.Bytes2HexString(b));
            System.out.println(Integer.toHexString(63));
        } catch (NoSuchAlgorithmException e) {
            System.out.println("exception");
            e.printStackTrace();
        }
    }

    public int add(int a, int b)
    {
        return a+b;
    }
}
