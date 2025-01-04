#pragma once

#include <vector>
#include "Card.h"

using namespace std;
/**
 * 玩家位置对应出掉的牌类
 */
class PositionToCards {
public:
    PositionToCards(int position, vector<Card> cards);
    int position;
    vector<Card> cards;
};

