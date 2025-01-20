#include "PlayedCards.h"
#include "Card.h"

/**
 * 玩家位置对应出掉的牌类
 */
PlayedCards::PlayedCards(int position, std::vector<Card> cards) : position(position), cards(cards) {}

