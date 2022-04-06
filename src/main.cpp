/**
 * @file main.cpp
 * @Zuoqiu-Yingyi (49649786+Zuoqiu-Yingyi@users.noreply.github.com)
 * @brief
 * @version 0.1.0
 * @date 2021-11-27
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "clipp.h"
#include "mode.h"
#include <cstdlib>
#include <iostream>
#include <string>

void showArg(const int argc, char *argv[])
{
    for (size_t i = 0; i < argc; i++)  // 打印所有传入参数
        std::cout << i << '\t' << argv[i] << std::endl;
}

int main(int argc, char *argv[])
{
    system("chcp 65001");  // 切换字符集

    std::string   url;
    std::string   parser;
    std::string   scheme;
    bool          cmd      = false;
    bool          debug    = false;
    url2cmd::mode selected = url2cmd::mode::run;

    auto required_c_p  = clipp::required("-c", "--cmd").set(cmd).doc("使用 CMD 解析器 | use CMD parser") | clipp::required("-p", "--powershell", "--pwsh").doc("使用 PowerShell 解析器 | use PowerShell parser");
    auto option_debug  = clipp::option("--debug").set(debug).doc("输出调试信息 | output debug information");
    auto option_parser = clipp::option("--parser").doc("自定义命令解析器 | custom command parser") & clipp::value("path", parser).doc("自定义命令解析器路径 | path of custom command parser");

    auto run = (clipp::value("url", url).doc("统一资源定位符 | Uniform Resource Locator, URL"),
                required_c_p,
                option_debug,
                option_parser);

    auto init = (clipp::command("init").set(selected, url2cmd::mode::init).doc("初始化注册表文件 | initialize the registry file"),
                 clipp::option("-n", "--name").doc("自定义 URL 协议名称 | custom URL scheme name") & clipp::value("scheme", scheme).doc("协议自定义名称 | name of custom scheme"),
                 required_c_p,
                 option_debug,
                 option_parser);

    auto help = (clipp::command("help").set(selected, url2cmd::mode::help).doc("查看参数指南 | view the parameter guide"));

    auto cli = ((run | init | help));

    if (clipp::parse(argc, argv, cli)) {  // 参数解析成功
        if (debug) showArg(argc, argv);

        if (parser.empty()) {
            if (cmd)
                parser = "cmd";
            else
                parser = "powershell";
        }

        switch (selected) {
            case url2cmd::mode::run:
                url2cmd::run(url, parser, cmd, debug);
                break;
            case url2cmd::mode::init:
                url2cmd::init(scheme, parser, cmd, debug);
                std::cout << "注册表文件初始化成功 | registry file initialization was successful" << std::endl;
                break;
            case url2cmd::mode::help:
                std::cout << clipp::make_man_page(cli, argv[0]);
                break;
        }
    }
    else {
        std::cout << "ERROR: 参数解析失败! | Argument parse failed!" << std::endl;
        showArg(argc, argv);
        std::cout << clipp::make_man_page(cli, argv[0]);
        system("pause");
        return 0;
    }

    return 0;
}
