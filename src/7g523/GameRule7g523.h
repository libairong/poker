/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : GameRule7g523.h
*   owner       : bairong.li
*   date        : 2025-01-13
*   description :
*
================================================================*/
#ifndef __GAMERULE7G523HELPER_H__
#define __GAMERULE7G523HELPER_H__
#include "../GameFlow.h"

enum CombinateType {
    SINGLE, PAIR, THREE, FOUR, STRAIGHT, SINGLE_SUIT
};

class GameRule7g523Helper : public GameFlow {
public:
    GameRule7g523Helper();
    static bool cardCompare(const Card& a, const Card& b);
    static bool cardValueCompare(int valueA, int valueB);
    static CombinateType cardsType(const vector<Card>& cards);
    // 增加一个静态函数，用于给牌排序
    static void sortCards(vector<shared_ptr<Card>>& cards);

    // 实现GameFlow接口
    void startFlow() override;
    void endFlow() override;

private:
    // 玩家轮流抽牌到最大手牌数
    void drawCard();

    // 玩家轮流出牌，直到所有玩家都出完牌
    void playCard();
    // 判断游戏是否结束
    bool isGameOver();
    // 比较两张牌的大小
    bool compareCards(const Card& card1, const Card& card2);
    // 玩家出牌顺序，轮流出牌，setPlayersOrder设置玩家出牌的顺序，getPlayerOrder获取当前玩家的出牌顺序
    void setPlayersOrder();
    // 从玩家出牌的顺序中获取下一个玩家的索引
    void setNextPlayerOrderIndex();
    // 获取当前玩家出牌的顺序索引，即当前轮到哪个玩家出牌
    int getPlayerOrderIndex();

    static map<int, int> cardRankRule;
    // 是否需要停止游戏
    bool mIsStopGame = false;
    // 玩家出牌的顺序，存储玩家索引
    vector<int> mPlayerOrder;
    // 当前玩家出牌的顺序索引
    int mCurrentPlayerOrderIndex = 0;
};
#endif  // __GAMERULE7G523HELPER_H__
