package com.iutils.pattern.observer;

import com.iutils.utils.ILog;

import java.util.ArrayList;
import java.util.List;

public class CheckServerJob extends Job implements AP.ICheckApListener{
    private final static String tag = "CheckServerJob";


    @Override
    public void doJob(){
        ILog.c(tag, "check server access ip list sessionId["+sessionId+"]");
        ObserverManager.getInstance().setObserver(this);
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

    @Override
    public void onResult(AP ap) {
        ILog.c(tag, "onResult ap["+ap+"]");
        CheckServerResult result = new CheckServerResult();
        if(ap.isTimeout){
            result.resultCode = 408;
        }else{
            result.resultCode = 200;
        }
        result.sessionId = sessionId;
        result.ip = ap.domain;
        result.checkTime = ap.checkTime;
        ObserverManager.getInstance().notify(result);
    }

    private List<AP> getServerList(){
        List<AP> serverList = new ArrayList<>();
        int sipPort = 15065;
        int xcapPort = 8088;
        String domainStr = "moaportal.zte.com.cn|hk.moaportal.zte.com.cn|de.moaportal.zte.com.cn|sa.moaportal.zte.com.cn|br.moaportal.zte.com.cn|us.moaportal.zte.com.cn";
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
