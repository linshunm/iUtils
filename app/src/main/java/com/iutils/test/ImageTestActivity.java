package com.iutils.test;

import android.graphics.Bitmap;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.iutils.R;
import com.iutils.utils.FileUtil;
import com.iutils.utils.ImageUtil;

import java.io.File;

public class ImageTestActivity extends AppCompatActivity implements View.OnClickListener{
    private static final String TAG = "ImageTestActivity";

    private Button btnLoadOriginal;
    private Button btnLoadQuality;
    private Button btnLoadSize;
    private Button btnLoadSample;
    private ImageView ivShow;
    private Bitmap originalBitmap;
    private Bitmap qualityBitmap;
    private Bitmap sizeBitmap;
    private Bitmap sampleBitmap;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_image_test);
        initView();
        initData();
    }


    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_load_original:
            {
                showOriginal();
                break;
            }
            case R.id.btn_load_quality:
            {
                showQuality();
                break;
            }
            case R.id.btn_load_size:
            {
                showSize();
                break;
            }
            case R.id.btn_load_sample:
            {
                showSample();
                break;
            }
            case R.id.iv_show:
            {
                ivShow.setVisibility(View.GONE);
            }
        }
    }

    private void initData()
    {
        originalBitmap = getTestImage();
    }


    private void initView()
    {
        btnLoadOriginal = (Button)findViewById(R.id.btn_load_original);
        btnLoadOriginal.setOnClickListener(this);
        btnLoadQuality = (Button)findViewById(R.id.btn_load_quality);
        btnLoadQuality.setOnClickListener(this);
        btnLoadSize = (Button)findViewById(R.id.btn_load_size);
        btnLoadSize.setOnClickListener(this);
        btnLoadSample = (Button)findViewById(R.id.btn_load_sample);
        btnLoadSample.setOnClickListener(this);

        ivShow = (ImageView)findViewById(R.id.iv_show);
        ivShow.setOnClickListener(this);
    }

    private Bitmap getTestImage()
    {
        String filepath = FileUtil.getSDCardPath()+File.separator+"img1.jpg";
        return ImageUtil.getBitmap(filepath);
    }

    private void showOriginal()
    {
        ivShow.setImageBitmap(originalBitmap);
        ivShow.setVisibility(View.VISIBLE);
    }

    private void showQuality()
    {
        if(qualityBitmap == null)
        {
            String filepath = FileUtil.getSDCardPath()+File.separator+"img1_quality.jpg";
            File outFile = new File(filepath);
            ImageUtil.compressQuality(originalBitmap, 65, outFile);
            qualityBitmap = ImageUtil.getBitmap(filepath);
        }

        ivShow.setImageBitmap(qualityBitmap);
        ivShow.setVisibility(View.VISIBLE);
    }

    private void showSize()
    {
        if(sizeBitmap == null)
        {
            String filepath = FileUtil.getSDCardPath()+File.separator+"img1_size.jpg";
            File outFile = new File(filepath);
            int ratio = 8;//尺寸压缩比例
            ImageUtil.compressSize(originalBitmap, ratio, outFile);
            sizeBitmap = ImageUtil.getBitmap(filepath);
        }

        ivShow.setImageBitmap(sizeBitmap);
        ivShow.setVisibility(View.VISIBLE);
    }

    private void showSample()
    {
        if(sampleBitmap == null)
        {
            String originFilepath = FileUtil.getSDCardPath()+File.separator+"img1.jpg";
            String outFilepath = FileUtil.getSDCardPath()+File.separator+"img1_sample.jpg";
            File outFile = new File(outFilepath);

            int inSampleSize = 8;//采样率设置
            ImageUtil.compressSample(originFilepath, inSampleSize, outFile);
            sampleBitmap = ImageUtil.getBitmap(outFilepath);
        }

        ivShow.setImageBitmap(sampleBitmap);
        ivShow.setVisibility(View.VISIBLE);
    }
}
