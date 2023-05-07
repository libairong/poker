#include "GameRule.hpp"

GameRule7g523::GameRule7g523() {
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

int GameRule7g523::cardCompare(int valueA, int valueB) {
    return cardRankRule[valueA] > cardRankRule[valueB] ? 1 : -1;
}
