package com.iutils.zip;

import com.iutils.utils.ZipUtil;

/**
 * Created by linshunming on 2018/1/3.
 */
public class ZipTest {

    public static void main(String[] args)
    {
        String filePath = "/Users/linshunming/log.txt";
        ZipUtil.zip(filePath,null,false,"123456");
    }
}
