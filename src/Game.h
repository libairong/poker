#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <memory>
#include "Card.h"
#include "Player.h"
#include "Scene.h"
#include "GameFlow.h"
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

    vector<shared_ptr<Player>> mPlayers;
    shared_ptr<Scene> mScene;
    shared_ptr<GameFlow> mGameFlow;

    int mHuman_num;
    int mComputer_num;
    int mPlayersTurn;
};
#endif  // __GAME_H__

