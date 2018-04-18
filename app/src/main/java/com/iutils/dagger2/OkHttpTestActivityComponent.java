package com.iutils.dagger2;

import com.iutils.okhttp.OkHttpTestActivity;

import dagger.Component;

@Component(modules = OkHttpModule.class)
public interface OkHttpTestActivityComponent {
    void inject(OkHttpTestActivity instance);
}
