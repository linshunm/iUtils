package com.iutils.utils;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;

/**
 * Created by kevin on 2017/12/27.
 */
public class ImageUtil {
    /**
     * 质量压缩
     *
     * @param bitmap
     * @param quality
     * @param file
     */
    public static void compressQuality(Bitmap bitmap, int quality, File file) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.JPEG, quality, baos);
        try {
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(baos.toByteArray());
            fos.flush();
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 通过图片路径获取Bitmap
     * @param filepath
     * @return
     */
    public static Bitmap getBitmap(String filepath)
    {
        File file = new File(filepath);

        if(file.exists())
        {
            return BitmapFactory.decodeFile(filepath);
        }
        else
        {
            return null;
        }
    }
}
