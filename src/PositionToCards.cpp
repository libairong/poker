#include "PositionToCards.h"
#include "Card.h"

/**
 * 玩家位置对应出掉的牌类
 */
PositionToCards::PositionToCards(int position, std::vector<Card> cards) : position(position), cards(cards) {}

