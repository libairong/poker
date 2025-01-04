#include <iostream>
#include "Game.h"

int main() {
    int human_num, computer_num;
    std::cout << "请输入玩家数量(2-10): ";
    std::cin >> human_num;
    if (human_num < 2 || human_num > 10) {
        std::cout << "输入错误" << std::endl;
        return 0;
    }

    computer_num = 10 - human_num;
    // Game game(human_num, computer_num);
    Game game(0, 3);
    game.start();

    return 0;
}

