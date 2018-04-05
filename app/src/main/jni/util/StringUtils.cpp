//
// Created by 林舜铭 on 2018/3/29.
//

#include "StringUtils.h"
#include <sstream>


void StringUtils::int2str(int _int, std::string &_str){
    std::stringstream stream;
    stream<<_int;
    _str = stream.str();
}

void StringUtils::str2int(std::string _str, int &_int){
    std::stringstream stream(_str);
    stream>>_int;
}