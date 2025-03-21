#include <iostream>
#include "Game.h"

// 添加检测外部ctrl+c信号的头文件
#include <csignal>

// 将game对象定义为全局变量
Game game(1, 1);

void signalHandler(int signum) {
    // std::cout << "捕获到信号：" << signum << std::endl;
    if (signum == SIGINT) {
        std::cout << "捕获到ctrl+c信号，准备退出程序..." << std::endl;
    }
    // 退出程序
    game.end();
    exit(signum);
}

int main() {
    std::cout << "欢迎游玩~~" << std::endl;
    // 打印当前编译的时间
    std::cout << "编译时间：" << __TIME__ << std::endl;
    // 注册信号处理函数
    signal(SIGINT, signalHandler);

    game.start();

    return 0;
}

