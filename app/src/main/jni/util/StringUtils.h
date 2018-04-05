//
// Created by 林舜铭 on 2018/3/29.
//

#ifndef IUTILS_STRINGUTILS_H
#define IUTILS_STRINGUTILS_H

#include <iostream>
#include <string>

class StringUtils {
public:
    static void int2str(int _int, std::string &_str);
    static void str2int(std::string _str, int &_int);
};


#endif //IUTILS_STRINGUTILS_H
