package com.iutils.utils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by 10110878 on 2016/12/16.
 */
public class StringUtil {
    public static String replaceBlank(String str) {
        String dest = "";
        if (str != null) {
            Pattern p = Pattern.compile("\\s*|\t|\r|\n");
            Matcher m = p.matcher(str);
            dest = m.replaceAll("");
        }
        return dest;
    }

    public static boolean isEmpty(String str)
    {
        if(str == null || str.trim().length() == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public static  String byteArrayToString(byte[] bytes)
    {
        String str = null;

        if(bytes != null)
        {
            str = new String(bytes);
        }
        return str;
    }

}
