
package com.iutils.zip;

import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.iutils.R;
import com.iutils.utils.FileUtil;
import com.iutils.utils.ThreadUtil;
import com.iutils.utils.ZipUtil;

import net.lingala.zip4j.exception.ZipException;

import java.io.File;

public class ZipTestActivity extends AppCompatActivity implements View.OnClickListener{
    private Button btnZip;
    private Button btnUnzip;
    private TextView tvInfo;

    long time = 0L;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_zip_test);
        initView();
    }

    private void initView()
    {
        btnZip = (Button) findViewById(R.id.btn_zip);
        btnZip.setOnClickListener(this);
        btnUnzip = (Button) findViewById(R.id.btn_unzip);
        btnUnzip.setOnClickListener(this);

        tvInfo = (TextView)findViewById(R.id.tv_info);
    }

    private void zip()
    {
        final String src = FileUtil.getSDCardPath()+ File.separator+"MOA/.log/Logcat.log";
        final String dest = FileUtil.getSDCardPath()+ File.separator+"MOA/.log/Logcat_zip4j.zip";
        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                long start = SystemClock.elapsedRealtime();
                ZipUtil.zip(src, dest, false, "12345678");
                time = SystemClock.elapsedRealtime()- start;

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        File srcFile = new File(src);
                        File destFile = new File(dest);

                        String tips = "before zip file["+srcFile.exists()+"] size:"+srcFile.length()/1024+"KB\n";
                        tips += "before zip file path:"+src+"\n";
                        tips += "after zip file["+destFile.exists()+"] size:"+destFile.length()/1024+"KB\n";
                        tips += "after zip file path:"+dest+"\n";
                        tips += "total time:"+time+"ms\n";
                        tvInfo.setText(tips);
                    }
                });
            }
        });
    }

    private void unzip()
    {
        final String src = FileUtil.getSDCardPath()+ File.separator+"Download/";
//        final String dest = FileUtil.getSDCardPath()+ File.separator+"MOA/.log/Logcat_zip4j.zip";
        final String dest = FileUtil.getSDCardPath()+ File.separator+"whczip.zip";
        final File destFile = new File(dest);


        ThreadUtil.execute(new Runnable() {
            @Override
            public void run() {
                long start = SystemClock.elapsedRealtime();
                try {
                    ZipUtil.unzip(destFile, src, "1234");
                } catch (ZipException e) {
                    e.printStackTrace();
                }
                time = SystemClock.elapsedRealtime()- start;

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        String unzipfile = src+File.separator+"Logcat.log";
                        File srcFile = new File(unzipfile);
                        String tips = "before unzip file size:"+destFile.length()/1024+"KB\n";
                        tips += "before unzip file path:"+dest+"\n";
                        tips += "after unzip file size:"+srcFile.length()/1024+"KB\n";
                        tips += "after unzip file path:"+unzipfile+"\n";
                        tips += "total time:"+time+"ms\n";
                        tvInfo.setText(tips);
                    }
                });
            }
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_zip:
            {
                zip();
                break;
            }
            case R.id.btn_unzip:
            {
                unzip();
                break;
            }
        }
    }
}
