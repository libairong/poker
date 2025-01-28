#include "PlayedCards.h"

/**
 * 玩家位置对应出掉的牌类
 */
PlayedCards::PlayedCards(shared_ptr<Player> player, vector<shared_ptr<Card>> cards) :
	player(player), cards(cards) {}

