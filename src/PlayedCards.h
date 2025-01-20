#pragma once

#include <vector>
#include "Card.h"

using namespace std;
/**
 * 玩家位置对应出掉的牌类
 */
class PlayedCards {
public:
    PlayedCards(Player* player, vector<Card> cards);
    Player* player;
    vector<Card> cards;
};

