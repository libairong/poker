#pragma once

#include <string>
#include <iostream>
#include <vector>

using namespace std;

const string CARD_COLOR[4]    =  {"♦", "♣", "♥", "♠"};
const string CARD_VALUE[15]   = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "LJ", "BJ"};
const int    BIG_JOKER_VALUE  = 14;
const int LITTLE_JOKER_VALUE  = 13;

/**
 * 扑克牌类
 */
class Card {
public:
    enum Suit {
        DIAMOND, CLUB, HEART, SPADE  // 街砖、梅花、红心、葵扇
    };

    Card() {}
    Card(int color, int value);
    Card& operator=(const Card&) = default;  // 显式默认拷贝运算符
    bool operator==(const Card&) const = default;  // 显式默认判断相等运算符
    void print() const;
    string toString() const;
    int get_value() const;
    int getSuit() const;
    void swap(Card& other);
    friend void swap(Card& a, Card& b);

    static int compare(const Card& a, const Card& b);
    static void getSameValueCards(const vector<Card>* input, int theSameValue, vector<Card>* output);
    static int countSameValueCard(const vector<Card>& myCards, int value);
private:
    int _color;
    int _value;
    Suit _suit;
};
