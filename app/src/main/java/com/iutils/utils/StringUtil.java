package com.iutils.utils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by 10110878 on 2016/12/16.
 */
public class StringUtil {
    private static Integer idSeq = 0;

    /**
     * 获取17位MsgId
     */
    public static String getUniqueStrId()
    {
        // 设置消息随机数
        String szMsgId = null;

        String timeStamp = TimeUtil.getCompleteTimeStr1();

        synchronized(idSeq)
        {
            szMsgId = String.format("%s%03x", timeStamp, idSeq++%4096);
        }

        return szMsgId;
    }

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
