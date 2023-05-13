#include "Card.hpp"

/**
 * 扑克牌类
 */

Card::Card(int color, int value) : _color(color), _value(value), _suit(static_cast<Suit>(color)) {}

void Card::print() const {
    if (_value == LITTLE_JOKER_VALUE || _value == BIG_JOKER_VALUE)
        std::cout << "[" << CARD_VALUE[_value] << "]";
    else
        std::cout << "[" << CARD_COLOR[_color] << CARD_VALUE[_value] << "]";
}

string Card::toString() const {
    string info = CARD_COLOR[_color] + CARD_VALUE[_value];
    return info;
}

int Card::get_value() const {
    return _value;
}

// 返回-1表示a 小于 b，返回1 表示a 大于b
int Card::compare(const Card& a, const Card& b) {
    if (a.get_value() == b.get_value()) {
        return a.getSuit() > b.getSuit() ? 1 : -1;
    } else {
        return a.get_value() > b.get_value() ? 1 : -1;
    }
}

void Card::getSameValueCards(const vector<Card>* input, int theSameValue, vector<Card>* output) {
    for (int i = 0; i < (int)input->size(); i++) {
        if (input->at(i).get_value() == theSameValue) {
            output->push_back(input->at(i));
        }
    }
}

int Card::countSameValueCard(const vector<Card>& myCards, int value) {
    if (myCards.empty())
        return 0;

    int count = 0;
    for (int i = 0; i < (int)myCards.size(); i++) {
        if (myCards[i].get_value() == value)
            count++;
    }

    return count;
}
int Card::getSuit() const {
    return _suit;
}

void Card::swap(Card& other) {
    std::swap(_color, other._color);
    std::swap(_value, other._value);
}

void swap(Card& a, Card& b) {
    a.swap(b);
}

