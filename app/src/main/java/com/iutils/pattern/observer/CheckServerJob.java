package com.iutils.pattern.observer;

import com.iutils.utils.ILog;

import java.util.ArrayList;
import java.util.List;

public class CheckServerJob extends Job implements AP.ICheckApListener{
    private final static String tag = "CheckServerJob";


    @Override
    public void onResult(AP ap) {
        ILog.c(tag, "onResult ap["+ap+"]");
        CheckServerResult result = new CheckServerResult();
        if(ap.isTimeout){
            result.resultCode = 408;
        }else{
            result.resultCode = 200;
        }
        result.ip = ap.domain;
        result.checkTime = ap.checkTime;
        LoginManager.getInstance().notify(result);
    }

    public void doJob(){
        ILog.c(tag, "check server access ip list");
        LoginManager.getInstance().setObserver(this);
        //TODO FOR REQUEST
        final List<AP> serverList = getServerList();
        for(int i = 0; i<serverList.size();i++){
            serverList.get(i).check( this);
        }
    }

    @Override
    public void onCallback(Result result) {
        if(result instanceof CheckServerResult){
            ILog.c(tag, result.toString());
            super.doJob();
        }
    }


    private List<AP> getServerList(){
        List<AP> serverList = new ArrayList<>();
        int sipPort = 15065;
        int xcapPort = 8088;
        String domainStr = "us.moaportal.zet.com.cn|hk.moaportal.zet.com.cn|de.moaportal.zet.com.cn|moaportal.zet.com.cn";
        String[] domainArr = domainStr.split("\\|");
        for(String domain : domainArr){
            AP ap = new AP();
            ap.sipPort = sipPort;
            ap.xcapPort = xcapPort;
            ap.domain = domain;
            serverList.add(ap);
        }
        return serverList;
    }
}
