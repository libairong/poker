#include "Player.hpp"

HumanPlayer::HumanPlayer(string name, int position):
    Player(name, position) {}

void HumanPlayer::addCard(Card card) {
    _cards.push_back(card);
    _currentCardNum = _cards.size();
}

void HumanPlayer::sortCards() {
    sort(_cards.begin(), _cards.end(), [](Card& a, Card& b) { return a.get_value() < b.get_value(); });
}

void HumanPlayer::printCards() const {
    cout << "[" << _name << "] ";
    for (const auto& card : _cards) {
        card.print();
        cout << " ";
    }
    cout << endl;
}

vector<Card> HumanPlayer::action(Scene *Scene) {
    _currentCardNum = 0;
    return _cards;
}

