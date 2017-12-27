package com.iutils.utils;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Rect;

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
     * 尺寸压缩
     *
     * @param bitmap
     * @param ratio 尺寸压缩比例
     * @param file
     */
    public static void compressSize(Bitmap bitmap, int ratio, File file) {
        Bitmap result = Bitmap.createBitmap(bitmap.getWidth() / ratio, bitmap.getHeight() / ratio, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(result);
        Rect rect = new Rect(0, 0, bitmap.getWidth() / ratio, bitmap.getHeight() / ratio);
        canvas.drawBitmap(bitmap, null, rect, null);

        compressQuality(result, 100, file);
    }

    /**
     * 采样率压缩
     *
     * @param filePath
     * @param file
     */
    public static void compressSample(String filePath, int inSampleSize, File file) {
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = false;
        options.inSampleSize = inSampleSize;
        Bitmap bitmap = BitmapFactory.decodeFile(filePath, options);

        compressQuality(bitmap, 100, file);
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
