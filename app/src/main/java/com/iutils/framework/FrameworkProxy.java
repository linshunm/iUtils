package com.iutils.framework;

import com.iutils.utils.ILog;
import com.iutils.utils.SystemUtil;

public class FrameworkProxy {
    private final static String TAG = "FrameworkProxy";

    public String getDeviceId(){
        String deviceId = SystemUtil.getDeviceId();
        ILog.i(TAG, "getDeviceId:"+deviceId);
        return deviceId;
    }

}
