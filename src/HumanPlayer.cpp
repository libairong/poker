#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(string name, int position, shared_ptr<GameRule> gameR, shared_ptr<Scene> scene):
    Player(name, position, gameR, scene) {}

void HumanPlayer::action(void) {
}

