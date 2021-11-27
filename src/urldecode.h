/**
 * @file urldecode.h
 * @Zuoqiu-Yingyi (49649786+Zuoqiu-Yingyi@users.noreply.github.com)
 * @brief
 * @version 0.1.0
 * @date 2021-11-27
 *
 * @copyright Copyright (c) 2021
 *
 * @ref https://www.cnblogs.com/shike8080/archive/2017/01/18/6297210.html
 */
#include <assert.h>
#include <string>
#include <windows.h>

#ifndef URL2CMD_URLDECODE
#define URL2CMD_URLDECODE

namespace url2cmd
{
unsigned char hex2uint8(unsigned char x)
{
    unsigned char y;
    if (x >= '0' && x <= '9')
        y = x - '0';
    else if (x >= 'A' && x <= 'Z')
        y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z')
        y = x - 'a' + 10;
    else
        assert(0);
    return y;
}

std::string urlDecode(const std::string &str)
{
    auto   strTemp = str;
    auto   length  = str.length();
    size_t l       = 0;
    for (size_t i = 0; i < length; i++) {
        if (str[i] == '%') {
            assert(i + 2 < length);
            unsigned char high = hex2uint8((unsigned char)str[++i]);
            unsigned char low  = hex2uint8((unsigned char)str[++i]);
            strTemp[l++]       = (high << 4) + low;
        }
        else
            strTemp[l++] = str[i];
    }
    return strTemp.substr(0, l);
}

std::string UTF82GBK(const char *utf8)
{
    int      len  = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t *wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len       = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char *str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

}  // namespace url2cmd

#endif
