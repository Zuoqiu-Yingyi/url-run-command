/**
 * @file format.h
 * @Zuoqiu-Yingyi (49649786+Zuoqiu-Yingyi@users.noreply.github.com)
 * @brief
 * @version 0.1.0
 * @date 2021-11-27
 *
 * @copyright Copyright (c) 2021
 *
 * @ref https://www.cnblogs.com/pretty-guy/p/11497043.html
 */
#include <assert.h>
#include <string>

#ifndef URL2CMD_FORMAT
#    define URL2CMD_FORMAT

namespace url2cmd
{

template <typename... Args>
std::string stringFormat(const char *format, Args... args)
{
    int length = std::snprintf(nullptr, 0, format, args...);
    assert(length >= 0);

    char *buf = new char[length + 1];
    std::snprintf(buf, length + 1, format, args...);

    std::string str(buf);
    delete[] buf;
    return str;
}

}  // namespace url2cmd

#endif