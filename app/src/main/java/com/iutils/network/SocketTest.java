package com.iutils.network;

import com.iutils.utils.AES128;
import com.iutils.utils.ILog;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

/**
 * Created by linshunming on 2018/1/23.
 */
public class SocketTest {

    public static void main(String[] args)
    {
        FileChannel fin = null;
        FileChannel fout = null;
        try {
            File inFile = new File("/Users/linshunming/log.txt");
            ILog.c("IN FILE MD5["+ AES128.getFileMd5Str(inFile)+"]");
            FileInputStream fileInputStream = new FileInputStream(inFile);
            fin = fileInputStream.getChannel();
            File outFile = new File("/Users/linshunming/log_bak.txt");
            if(outFile.exists()&& outFile.isFile())
            {
                outFile.delete();
            }
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            fout = fileOutputStream.getChannel();
            ByteBuffer byteBuffer = ByteBuffer.allocate(1024);

            while (fin.read(byteBuffer) != -1)
            {
                byteBuffer.flip();
                fout.write(byteBuffer);
                byteBuffer.clear();
            }
            ILog.c("OUT FILE MD5["+ AES128.getFileMd5Str(outFile)+"]");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
