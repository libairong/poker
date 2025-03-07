#include <iostream>
#include "Game.h"

int main() {
    std::cout << "欢迎游玩~~" << std::endl;
    // 打印当前编译的时间
    std::cout << "编译时间：" << __TIME__ << std::endl;
    Game game(1, 1);
    game.start();

    return 0;
}

