#pragma once

#include <string>
#include <iostream>

using namespace std;

const string CARD_COLOR[4] = {"♣", "♦", "♥", "♠"};
const string CARD_VALUE[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

/**
 * 扑克牌类
 */
class Card {
public:
    enum Suit {
        CLUB, DIAMOND, HEART, SPADE
    };

    Card() {}
    Card(int color, int value);
    Card& operator=(const Card&) = default;  // 显式默认拷贝运算符
    bool operator==(const Card&) const = default;  // 显式默认判断相等运算符
    void print() const;
    string toString() const;
    int get_value() const;
    void swap(Card& other);
    friend void swap(Card& a, Card& b);
private:
    int _color;
    int _value;
    Suit _suit;
};
