package com.iutils.dagger2;

import dagger.Module;
import dagger.Provides;
import okhttp3.OkHttpClient;

@Module
public class OkHttpModule {
    @Provides
    public OkHttpClient provideOkHttpClient(){
        return new OkHttpClient();
    }
}
