#include <iostream>
#include "Game.h"

int main() {
    std::cout << "欢迎游玩~~" << std::endl;
    Game game(1, 1);
    game.start();

    return 0;
}

