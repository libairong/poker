#include "Card.h"

/**
 * 扑克牌类
 */

Card::Card(Suit suit, int value) : _value(value), _suit(suit) {}

void Card::print() const {
    if (_value == LITTLE_JOKER_VALUE || _value == BIG_JOKER_VALUE)
        std::cout << "[" << CARD_VALUE[_value] << "]";
    else
        std::cout << "[" << CARD_SUIT[_suit] << CARD_VALUE[_value] << "]";
}

string Card::toString() const {
    string info = CARD_SUIT[_suit] + CARD_VALUE[_value];
    return info;
}

int Card::getValue() const {
    return _value;
}

string Card::getValueString() const {
    return CARD_VALUE[_value];
}

// 返回-1表示a 小于 b，返回1 表示a 大于b
int Card::compare(const Card& a, const Card& b) {
    if (a.getValue() == b.getValue()) {
        return a.getSuit() > b.getSuit() ? 1 : -1;
    } else {
        return a.getValue() > b.getValue() ? 1 : -1;
    }
}

void Card::getSameValueCards(const vector<Card>* input, int theSameValue, vector<Card>* output) {
    for (int i = 0; i < (int)input->size(); i++) {
        if (input->at(i).getValue() == theSameValue) {
            output->push_back(input->at(i));
        }
    }
}

int Card::countSameValueCard(const vector<Card>& myCards, int value) {
    if (myCards.empty())
        return 0;

    int count = 0;
    for (int i = 0; i < (int)myCards.size(); i++) {
        if (myCards[i].getValue() == value)
            count++;
    }

    return count;
}
int Card::getSuit() const {
    return _suit;
}

// 获取牌的花色字符串
string Card::getSuitString() const {
    return CARD_SUIT[_suit];
}

void Card::swap(Card& other) {
    std::swap(_suit, other._suit);
    std::swap(_value, other._value);
}

void swap(Card& a, Card& b) {
    a.swap(b);
}

