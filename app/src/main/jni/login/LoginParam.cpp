//
// Created by 林舜铭 on 2018/5/3.
//

#include "LoginParam.h"
#include "LogUtils.h"

LoginParam::LoginParam() {
    LOGI("login param created.");
}

LoginParam::LoginParam(LoginParam &_param) {
    this->account = _param.account;
    this->password = _param.password;
    this->deviceId = _param.deviceId;
    this->terminalType = _param.terminalType;
    this->versionCode = _param.versionCode;
}

LoginParam::~LoginParam() {
    LOGI("login param destroyed.");
}

