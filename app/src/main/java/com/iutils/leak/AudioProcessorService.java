package com.iutils.leak;

import android.content.Context;

import java.util.ArrayList;
import java.util.List;

public class AudioProcessorService {
    private static List<Context> mContexts = new ArrayList<>();

    public AudioProcessorService(Context context){
        mContexts.add(context);
    }
}
