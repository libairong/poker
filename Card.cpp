#include "Card.hpp"

/**
 * 扑克牌类
 */

Card::Card(int color, int value) : _color(color), _value(value), _suit(static_cast<Suit>(color)) {}

void Card::print() const {
    std::cout << CARD_COLOR[_color] << CARD_VALUE[_value];
}

string Card::toString() const {
    string info = CARD_COLOR[_color] + CARD_VALUE[_value];
    return info;
}

int Card::get_value() const {
    return _value;
}

void Card::swap(Card& other) {
    std::swap(_color, other._color);
    std::swap(_value, other._value);
}

void swap(Card& a, Card& b) {
    a.swap(b);
}

