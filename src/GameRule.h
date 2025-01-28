#ifndef __GAMERULE_H__
#define __GAMERULE_H__

#include <map>
#include <vector>
#include "Card.h"

enum CombinateType {
    SINGLE, PAIR, THREE, FOUR, STRAIGHT, SINGLE_SUIT
};

// 规则类，提供卡牌列表、提供卡牌大小比较功能
class GameRule {
public:
    GameRule() = default;
    virtual ~GameRule() {};

    virtual bool cardCompare(const Card& a, const Card& b) = 0;
    virtual bool cardValueCompare(int valueA, int valueB) = 0;

    virtual CombinateType  cardsType(const vector<Card>& cards) = 0;
};
#endif  // __GAMERULE_H__

