/**
 * @file mode.h
 * @Zuoqiu-Yingyi (49649786+Zuoqiu-Yingyi@users.noreply.github.com)
 * @brief
 * @version 0.1.0
 * @date 2021-11-27
 *
 * @copyright Copyright (c) 2021
 * @ref https://blog.csdn.net/sunjikui1255326447/article/details/88802927
 * @ref https://jingyan.baidu.com/article/9faa72318d67a5473c28cbe6.html
 */

#include "format.h"
#include "urldecode.h"
#include <cstdlib>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

#ifndef URL2CMD_MODE
#    define URL2CMD_MODE

namespace url2cmd
{
enum class mode
{
    run,
    init,
    help
};

void run(const std::string &url, const std::string &parser, const bool cmd, const bool debug)
{
    system("cls");         // 清空命令行

    /* 提取 url 载荷 */
    auto payload = url.substr(url.find(':') + 1);
    auto begin   = payload.find_first_not_of("/");
    auto end     = payload.find_last_not_of("/");
    if (begin == std::string::npos || end == std::string::npos) throw "ERROR: URL 载荷为空! / URL payload is EMPTY!";

    auto command = urlDecode(payload.substr(begin, end - begin + 1));

    char cwd[1 << 15];
    _getcwd(cwd, 1 << 15);  // 获得程序运行目录

    std::cout << (cmd ? "" : "PS ") << cwd << "> " << command << std::endl;  // 打印将运行的命令

    if (debug) system("pause");

    if (cmd)
        system(stringFormat("%s /C \"%s\"", parser.c_str(), UTF82GBK(command.c_str()).c_str()).c_str());
    else
        system(stringFormat("%s -c \"%s\"", parser.c_str(), UTF82GBK(command.c_str()).c_str()).c_str());

    if (debug) system("pause");
}

void init(const std::string &scheme, std::string parser, const bool cmd, const bool debug)
{
    auto scheme_c_str = scheme.c_str();

    /* 将路径中的 '\' 替换为 '\\' */
    std::string pgmptr = _pgmptr;
    for (auto pos = pgmptr.find("\\"); pos != -1; pos = pgmptr.find("\\")) pgmptr.replace(pos, 1, "/");
    for (auto pos = pgmptr.find("/"); pos != -1; pos = pgmptr.find("/")) pgmptr.replace(pos, 1, "\\\\");
    for (auto pos = parser.find("\\"); pos != -1; pos = parser.find("\\")) parser.replace(pos, 1, "/");
    for (auto pos = parser.find("/"); pos != -1; pos = parser.find("/")) parser.replace(pos, 1, "\\\\");

    std::ofstream f_insert_reg, f_delete_reg;
    f_insert_reg.open("insert.reg");
    f_delete_reg.open("delete.reg");

    f_insert_reg << "Windows Registry Editor Version 5.00" << std::endl
              << std::endl
              << stringFormat("[HKEY_CURRENT_USER\\Software\\Classes\\%s]", scheme_c_str) << std::endl
              << stringFormat("@=\"URL:%s\"", scheme_c_str) << std::endl
              << "\"URL Protocol\"=\"\"" << std::endl
              << std::endl
              << stringFormat("[HKEY_CURRENT_USER\\Software\\Classes\\%s\\shell]", scheme_c_str) << std::endl
              << std::endl
              << stringFormat("[HKEY_CURRENT_USER\\Software\\Classes\\%s\\shell\\open]", scheme_c_str) << std::endl
              << std::endl
              << stringFormat("[HKEY_CURRENT_USER\\Software\\Classes\\%s\\shell\\open\\command]", scheme_c_str) << std::endl
              << stringFormat("@=\"\\\"%s\\\" %%1 %s %s --parser \\\"%s\\\"\"",
                              pgmptr.c_str(),
                              (cmd ? "--cmd" : "--powershell"),
                              (debug ? "--debug" : ""),
                              parser.c_str())
              << std::endl;
    f_delete_reg << "Windows Registry Editor Version 5.00" << std::endl
                << std::endl
                << stringFormat("[-HKEY_CURRENT_USER\\Software\\Classes\\%s]", scheme_c_str) << std::endl;

    f_insert_reg.close();
    f_delete_reg.close();
}

}  // namespace url2cmd

#endif
