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
    void StartFlow() override;
    void EndFlow() override;

private:
    map<int, int> cardRankRule;
};
#endif  // __GAMERULE7G523HELPER_H__
