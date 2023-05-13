#pragma once
#include <map>
#include <vector>
#include "Card.hpp"

enum CombinateType {
    SINGLE, PAIR, THREE, FOUR, STRAIGHT, SINGLE_SUIT
};

// 规则类，提供卡牌大小比较功能
class GameRule {
public:
    GameRule() = default;
    virtual ~GameRule() {};

    virtual bool cardCompare(const Card& a, const Card& b) = 0;
    virtual bool cardValueCompare(int valueA, int valueB) = 0;

    virtual CombinateType  cardsType(const vector<Card>& cards) = 0;
};

class GameRule7g523 : public GameRule {
public:
    GameRule7g523();
    bool cardCompare(const Card& a, const Card& b) override;
    bool cardValueCompare(int valueA, int valueB) override;
    CombinateType cardsType(const vector<Card>& cards);

private:
    map<int, int> cardRankRule;
};

