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
    private static final String TAG = "TimeUtil";

    private static SimpleDateFormat recordDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
    private static SimpleDateFormat currDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    private static SimpleDateFormat voipDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");

    /**
     * 获取当前时间，格式：yyyyMMddHHmmss
     *
     * @return 当前格式化的时间串
     */
    public static String getVoipDateStr() {
        String result = null;
        try {
            result = voipDateFormat.format(new Date());
        } catch (Exception e) {
            ILog.d(TAG, "getVoipDateStr() exception: " + e.getMessage());
        }

        return result;
    }

    /**
     * 获取当前时间，格式：yyyy-MM-dd HH:mm:ss
     *
     * @return
     */
    public static String getCurrDateStr() {
        String result = null;
        try {
            result = currDateFormat.format(new Date());
        } catch (Exception e) {
            ILog.d(TAG, "getCurrDateStr() exception: " + e.getMessage());
        }

        return result;
    }

    /**
     * 获取当前记录的时间，格式：yyyy-MM-dd HH:mm:ss.SSS
     *
     * @return 时间字符串
     */
    public static String getRecordDateStr() {
        String result = null;
        try {
            result = recordDateFormat.format(new Date());
        } catch (Exception e) {
            ILog.d(TAG, "getRecordDateStr() exception: " + e.getMessage());
        }

        return result;
    }

    /**
     * 获取当前记录的时间，格式：yyyy-MM-dd HH:mm:ss.SSS
     *
     * @param currentTimeMillis 毫秒
     * @return 时间字符串
     */
    public static String getRecordDateStr(long currentTimeMillis) {
        String result = null;
        try {
            result = recordDateFormat.format(new Date(currentTimeMillis));
        } catch (Exception e) {
            ILog.d(TAG, "getRecordDateStr() exception: " + e.getMessage());
        }

        return result;
    }

    /**
     * 时间格式转换
     *
     * @param unixTime unix时间戳 如：20140729073108
     * @return 时间字符串
     */
    public static String getRecordDateStr(String pattern, long unixTime) {
        String result = null;
        try {
            SimpleDateFormat recordDateFormat = new SimpleDateFormat(pattern);
            result = recordDateFormat.format(new Date(unixTime));
        } catch (Exception e) {
            ILog.d(TAG, "getRecordDateStr() exception: " + e.getMessage());
        }

        return result;
    }

    /**
     * 获取系统当前完整时间的毫秒
     */
    public static String getCompleteTimeStr() {
        return String.valueOf(System.currentTimeMillis());
    }

    /**
     * 获取系统当前完整时间 格式为(yyyyMMddHHmmss)
     */
    public static String getCompleteTimeStr1() {
        Date date = new Date(System.currentTimeMillis());
        SimpleDateFormat fmt = new SimpleDateFormat("yyyyMMddHHmmss");
        return fmt.format(date);
    }
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
