//
// Created by 林舜铭 on 2018/5/3.
//

#ifndef IUTILS_LOGINPARAM_H
#define IUTILS_LOGINPARAM_H

#include <string>

using std::string;


class LoginParam {
public:
    string account;
    string password;
    string deviceId;
    string terminalType;
    int versionCode;
public:
    LoginParam();
    LoginParam(LoginParam &_param);
    ~LoginParam();
};


#endif //IUTILS_LOGINPARAM_H
