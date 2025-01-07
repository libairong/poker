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
    void initCards();
    void shuffle(vector<Card>& cards);
    Card takeTopCard();
    void disposeCards(int playerPosition, vector<Card> cards);
    void dealCards();
    void initScene();

    vector<Player*> _players;
    vector<PositionToCards>  _disposed_cards;  // 出掉的牌牌堆
    int _human_num;
    int _computer_num;
    int playersTurn;
    Scene scene;
    shared_ptr<GameRule> gameRule;
};

