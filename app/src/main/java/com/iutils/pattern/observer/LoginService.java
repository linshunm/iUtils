package com.iutils.pattern.observer;

public class LoginService {
    private static LoginService loginService;
    private LoginService(){}

    public static LoginService getInstance(){
        if(loginService == null){
            synchronized (LoginService.class){
                if(loginService == null){
                    loginService = new LoginService();
                }
            }
        }
        return loginService;
    }

    public native void initSDK(String priDataPath, String pubDataPath);

    public native void login(LoginParam param);

    public void onLoginResult(LoginResult result){
        ObserverManager.getInstance().notify(result);
    }
}
