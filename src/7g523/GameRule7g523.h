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

class GameRule7g523Helper : public GameFlow {
public:
    GameRule7g523Helper();
    bool cardCompare(const Card& a, const Card& b);
    bool cardValueCompare(int valueA, int valueB);
    CombinateType cardsType(const vector<Card>& cards);

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

    map<int, int> cardRankRule;
    // 是否需要停止游戏
    bool mIsStopGame = false;
};
#endif  // __GAMERULE7G523HELPER_H__
