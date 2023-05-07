#pragma once

#include <map>
#include <vector>
#include "PositionToCards.hpp"

/**
 * 当前场景类
 */
class Scene {
public:
    Scene() = default;
    map<int, bool> isPlayerActive;
    map<int, int> numOfTheCardInPlayers;
    vector<PositionToCards>* _disposed_cards;
    int theNumberOfRemainingCards;
};

