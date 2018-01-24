package com.iutils.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Map;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

/**
 * AES128加密解密工具
 * 
 * @author 刘华10208497
 * 
 */
public class AES128
{
    // private static Logger logger = LoggerFactory.getLogger(AES128.class);
    private final static String TAG = "AES128";

    /**
     * 
     * @param content
     * @param key
     * @return
     */
    public static String encrypt(String content, String key)
    {
        return aesProcess(content, key, Cipher.ENCRYPT_MODE);
    }

    /**
     * 
     * @param content
     * @param key
     * @return
     */
    public static String decrypt(String content, String key)
    {

        return aesProcess(content, key, Cipher.DECRYPT_MODE);
    }

    /**
     * 
     * @param content
     * @param key
     * @param mode
     */
    private static String aesProcess(String content, String key, int mode)
    {
        String resultString = "";
        if (content == null || content.equals("") || key == null || key.equals(""))
        {
            return "";
        }

        try
        {
            // 做MD5转换,为128位
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] md5 = md.digest(key.getBytes());
            // 获取加密实例
            SecretKeySpec secretKeySpec = new SecretKeySpec(md5, "AES");
            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(mode, secretKeySpec);
            if (mode == Cipher.ENCRYPT_MODE)
            {
                // 加密内容
                byte[] textBytes = cipher.doFinal(content.getBytes());
                resultString = Bytes2HexString(textBytes);
            }
            else if (mode == cipher.DECRYPT_MODE)
            {
                // 解密内容
                byte[] textBytes = cipher.doFinal(HexStringToByte(content));
                resultString = new String(textBytes);
            }

        }
        catch (Exception e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        return resultString;
    }

    public static byte[] getMd5Bytes(String key) throws NoSuchAlgorithmException {
        // 做MD5转换,为128位
        MessageDigest md = MessageDigest.getInstance("MD5");
        byte[] md5 = md.digest(key.getBytes());

        return md5;
    }

    /**
     * 字符串对应的32位md5字符串
     * @param key
     * @param isLowCase md5字符串是否小写
     * @return
     */
    public static String getMd5Str32(String key, boolean isLowCase)
    {
        String md5Str32 = null;
        try {
            byte[] md5Bytes = AES128.getMd5Bytes(key);
            md5Str32 = AES128.Bytes2HexString(md5Bytes);
            if(isLowCase)
            {
                md5Str32 = md5Str32.toLowerCase();
            }
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return md5Str32;
    }

    public static String getFileMd5Str(File file){
        String md5Str = null;
        FileInputStream fileInputStream = null;

        try {
            fileInputStream = new FileInputStream(file);
            MappedByteBuffer mappedByteBuffer = fileInputStream.getChannel().map(FileChannel.MapMode.READ_ONLY, 0, file.length());
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            md5.update(mappedByteBuffer);
            BigInteger bigInteger = new BigInteger(1, md5.digest());
            md5Str = bigInteger.toString(16);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        finally {
            if(fileInputStream != null)
            {
                try {
                    fileInputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        return md5Str;
    }

    /**
     * 
     * @param b
     * @return
     */
    public static String Bytes2HexString(byte[] b)
    {
        String ret = "";
        if (b != null)
        {
            for (int i = 0; i < b.length; i++)
            {
                String hex = Integer.toHexString(b[i] & 0xFF);
                if (hex.length() == 1)
                {
                    hex = '0' + hex;
                }
                ret += hex.toUpperCase();
            }
        }
        return ret;
    }

    /**
     * 16进制字符串转byte数组
     * 
     * @param hex
     * @return
     */
    public static byte[] HexStringToByte(String hex)
    {
        if (hex == null || hex.equals(""))
        {
            return new byte[0];
        }
        int len = (hex.length() / 2);
        byte[] result = new byte[len];
        char[] achar = hex.toCharArray();
        for (int i = 0; i < len; i++)
        {
            int pos = i * 2;
            result[i] = (byte) (toByte(achar[pos]) << 4 | toByte(achar[pos + 1]));
        }
        return result;
    }

    private static byte toByte(char c)
    {
        byte b = (byte) "0123456789ABCDEF".indexOf(c);
        return b;
    }

}
