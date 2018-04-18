package com.iutils.dagger2;

import com.iutils.framework.JniTestActivity;

import dagger.Component;

@Component
public interface JniTestActivityComponent {
    void inject(JniTestActivity jniTestActivity);
}
