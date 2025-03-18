#ifndef __CARD_H__
#define __CARD_H__

#include <string>
#include <iostream>
#include <vector>
#include "display/Layer.h"

using namespace std;

// 扑克牌组合类型
enum CombinateType {
    SINGLE,
    PAIR,
    THREE,
    FOUR,
    // 下面都是5张牌的组合，排在后面的比前面的组合大
    STRAIGHT,
    SINGLE_SUIT,
    // 三带二
    THREE_AND_A_PAIR,
    FOUR_AND_A_SINGLE,
    SIRAIGHT_AND_SINGLE_SUIT,
    INVALID_COMBINATE
};

const string CARD_SUIT[4]    =  {"♦", "♣", "♥", "♠"};
const string CARD_VALUE[15]   = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "LJ", "BJ"};
const int    BIG_JOKER_VALUE  = 14;
const int LITTLE_JOKER_VALUE  = 13;

enum Suit {
    DIAMOND, CLUB, HEART, SPADE  // 街砖、梅花、红心、葵扇
};
/**
 * 扑克牌类
 */
class Card {
public:
    Card() {}
    Card(Suit suit, int value);
    Card& operator=(const Card&) = default;  // 显式默认拷贝运算符
    bool operator==(const Card&) const = default;  // 显式默认判断相等运算符
    void print() const;
    string toString() const;
    int getValue() const;
    string getValueString() const;
    int getSuit() const;
    string getSuitString() const;
    void swap(Card& other);
    friend void swap(Card& a, Card& b);

    static int compare(const Card& a, const Card& b);
    static void getSameValueCards(const vector<Card>* input, int theSameValue, vector<Card>* output);
    static int countSameValueCard(const vector<Card>& myCards, int value);
private:
    int _value;
    Suit _suit;
};
#endif  // __CARD_H__

