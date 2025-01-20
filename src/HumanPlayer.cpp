#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(string name, int position):
    Player(name, position) {}

vector<Card> HumanPlayer::action(const Scene *Scene) {
    mCurrentCardNum = 0;
    return mCards;
}

