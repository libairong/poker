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
    std::map<int, bool> isPlayerActive;
    std::map<int, int> numOfTheCardInPlayers;
    std::vector<PositionToCards>* _disposed_cards;
    int theNumberOfRemainingCards;
};

