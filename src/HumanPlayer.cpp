#include "Player.hpp"

HumanPlayer::HumanPlayer(string name, int position):
    Player(name, position) {}

void HumanPlayer::addCard(Card card) {
    _cards.push_back(card);
    _currentCardNum = _cards.size();
}

void HumanPlayer::sortCards() {
    sort(_cards.begin(), _cards.end(), [this](Card& a, Card& b) { return gameRule->cardCompare(a, b); });
}

void HumanPlayer::printCards() const {
    cout << "[" << _name << "] ";
    for (const auto& card : _cards) {
        card.print();
        cout << " ";
    }
    cout << endl;
}

vector<Card> HumanPlayer::action(const Scene *Scene) {
    _currentCardNum = 0;
    return _cards;
}

