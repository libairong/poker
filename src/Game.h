#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <memory>
#include "Card.h"
#include "Player.h"
#include "Scene.h"
#include "GameRule.h"
#include "debug.h"

using namespace std;
/**
 * 游戏管理类，管理游戏流程和整体游戏资源！
 */
class Game {
public:
    Game(int human_num, int computer_num);
    ~Game();
    void start();
private:
    void init();

    vector<Player*> _players;
    int _human_num;
    int _computer_num;
    int playersTurn;
    shared_ptr<Scene> scene;
    shared_ptr<GameRule> gameRule;
};

