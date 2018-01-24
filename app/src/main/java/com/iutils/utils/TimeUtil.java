package com.iutils.utils;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;

/**
 * Created by linshunming on 2018/1/18.
 */
public class TimeUtil {

    /**
     *
     * @param gmt
     * "Tue, 29 Jul 2014 07:31:08 GMT" "Sat, 23 Aug 2014 13:39:49 GMT"
     */
    public static void convertGMT(String gmt) throws ParseException {
        SimpleDateFormat sdf = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z", Locale.US);
        sdf.setTimeZone(TimeZone.getTimeZone("GMT"));
        Date date = sdf.parse(gmt);
        long epoch = date.getTime();
        SimpleDateFormat sdf1 = new SimpleDateFormat("yyyyMMddHHmmss", Locale.US);
        String timeStr = sdf1.format(date);
        ILog.c("epoch["+epoch+"] timeStr["+timeStr+"]");
    }


    public static void main(String[] args)
    {
        try {

            //北京：epoch[1406619068000] timeStr[20140729153108]
            //奥厄本epoch[1406619068000] timeStr[20140729173108]
            TimeUtil.convertGMT("Tue, 29 Jul 2014 07:31:08 GMT");

        } catch (ParseException e) {
            e.printStackTrace();
        }
    }
}
