#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <memory>
#include "Card.hpp"
#include "Player.hpp"
#include "Scene.hpp"
#include "GameRule.hpp"

using namespace std;
/**
 * 游戏管理类
 */
class Game {
public:
    Game(int human_num, int computer_num);
    ~Game();
    void start();
private:
    vector<Player*> _players;
    vector<Card> _cards;  // 抽牌牌堆
    vector<PositionToCards>  _disposed_cards;  // 出掉的牌牌堆
    int _human_num;
    int _computer_num;
    Scene scene;
    // 这列应该多个游戏规则类，并将指针给到各个玩家

    void initCards();
    void shuffle(vector<Card>& cards);
    Card takeTopCard();
    void disposeCards(int playerPosition, vector<Card> cards);
    void dealCards();
    void initScene();
    shared_ptr<GameRule> gameRule;
};

