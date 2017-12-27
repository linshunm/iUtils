package com.iutils.utils;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

/**
 * Created by kevin on 2017/9/24.
 */
public class FileUtil {

    public byte[] getDataByFilePath(String filePath) throws IOException {
        RandomAccessFile randomAccessFile = new RandomAccessFile(filePath, "rw");

        FileChannel fileChannel = randomAccessFile.getChannel();

        ByteBuffer byteBuffer = ByteBuffer.allocate((int) randomAccessFile.length());
        fileChannel.read(byteBuffer);

        return byteBuffer.array();
    }
}
