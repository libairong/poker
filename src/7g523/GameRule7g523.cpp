#include "GameRule7g523.h"

GameRule7g523Helper::GameRule7g523Helper() {
    //  Card::value -> Rank
    cardRankRule.insert({14, 13});  // 大王, 第二大
    cardRankRule.insert({13, 12});
    cardRankRule.insert({12, 7});
    cardRankRule.insert({11, 6});
    cardRankRule.insert({10, 5});
    cardRankRule.insert({9,  4});
    cardRankRule.insert({8,  3});
    cardRankRule.insert({7,  2});
    cardRankRule.insert({6,  14});  // 7, 最大
    cardRankRule.insert({5,  1});
    cardRankRule.insert({4,  11});
    cardRankRule.insert({3,  0});
    cardRankRule.insert({2,  9});
    cardRankRule.insert({1,  10});
    cardRankRule.insert({0,  8});
}

bool GameRule7g523Helper::cardCompare(const Card& a, const Card& b) {
    if (a.get_value() == b.get_value())
        return a.getSuit() > b.getSuit();
    return cardRankRule[a.get_value()] > cardRankRule[b.get_value()];
}

bool GameRule7g523Helper::cardValueCompare(int valueA, int valueB) {
    return cardRankRule[valueA] > cardRankRule[valueB];
}

CombinateType GameRule7g523Helper::cardsType(const vector<Card>& cards) {
    int lastCount = cards.size();
    // 判断上一个玩家出的牌的类型, 这里涉及到玩法，可以专门定义一个类，做出牌判断
    if (lastCount == 1) {
        return CombinateType::SINGLE;
    } else if (lastCount == 2) {
        return CombinateType::PAIR;
    } else if (lastCount == 3) {
        return CombinateType::THREE;
    } else if (lastCount == 4) {
        return CombinateType::FOUR;
    } else if (lastCount == 5 && cards[4].get_value() - cards[3].get_value() == 1
                              && cards[3].get_value() - cards[2].get_value() == 1
                              && cards[2].get_value() - cards[1].get_value() == 1
                              && cards[1].get_value() - cards[0].get_value() == 1) {
        return CombinateType::STRAIGHT;
    } else if (lastCount == 5 && cards[4].getSuit() == cards[3].getSuit()
                              && cards[3].getSuit() == cards[2].getSuit()
                              && cards[2].getSuit() == cards[1].getSuit()
                              && cards[1].getSuit() == cards[0].getSuit()) {
        return CombinateType::SINGLE_SUIT;
    } else {
        // useless
        return CombinateType::SINGLE_SUIT;
    }
}


