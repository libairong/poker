#pragma once
#include <map>
#include <vector>
#include "Card.hpp"

// 规则类，提供卡牌大小比较功能
class GameRule {
public:
    GameRule() = default;
    virtual ~GameRule() {};

    virtual int cardCompare(int valueA, int valueB) = 0;
};

class GameRule7g523 : public GameRule {
public:
    GameRule7g523();
    int cardCompare(int valueA, int valueB);

private:
    map<int, int> cardRankRule;
};

