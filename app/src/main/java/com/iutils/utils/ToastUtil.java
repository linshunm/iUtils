package com.iutils.utils;

import android.widget.Toast;

import com.iutils.IUtilsApplication;

/**
 * Created by linshunming on 2016/12/24.
 */
public class ToastUtil {
    public static Toast toast;
    public static void showToast(String text){
        if(toast==null){
            toast=Toast.makeText(IUtilsApplication.getIUtilsApplicationContext(), text, Toast.LENGTH_SHORT); } toast.setText(text); toast.show();
    }
}