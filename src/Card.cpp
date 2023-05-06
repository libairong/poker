#include "Card.hpp"

/**
 * 扑克牌类
 */

Card::Card(int color, int value) : _color(color), _value(value), _suit(static_cast<Suit>(color)) {}

void Card::print() const {
    if (_value == LITTLE_JOKER_VALUE || _value == BIG_JOKER_VALUE)
        std::cout << CARD_VALUE[_value];
    else
        std::cout << CARD_COLOR[_color] << CARD_VALUE[_value];
}

string Card::toString() const {
    string info = CARD_COLOR[_color] + CARD_VALUE[_value];
    return info;
}

int Card::get_value() const {
    return _value;
}

// 返回-1表示a 小于 b，返回1 表示a 大于b
int Card::compare(const Card& b) {
    if (_value == b.get_value()) {
        return _suit > b.getSuit() ? 1 : -1;
    } else {

        return _value > b.get_value() ? 1 : -1;
    }
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

