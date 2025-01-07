#pragma once

#include <map>
#include <vector>
#include "PlayedCards.h"

/**
 * 场景包括：
 *     1. 玩家
 *     2. 资源牌堆
 *     3. 出牌堆
 *
 * 同时负责场景的显示。
 */

/**
 * 当前场景类
 */
class Scene {
public:
    Scene() = default;

    void initResourceCards() {
    }

    Card takeCard() {
	Card card = mResourceCards.back();
	mResourceCards.pop_back();

	return card;
    };

    void playCard(Player* player, vector<Card> cards) {
        vector<Card> playedCards;
        playedCards.insert(playedCards.end(), cards.begin(), cards.end());
        PlayedCards PlayedCard(player, playedCards);  // 在栈内申请，下一行会进行拷贝
        mPlayedCards.push_back(PlayedCards);
    }

    PlayedCard getLastPlayedCards() {
	PlayedCard playedCard = mPlayedCards.back();
	mPlayedCards.pop_back();

	return playedCard;
    }

private:
    vector<Card> mResourceCards;
    vector<PlayedCards>  mPlayedCards;  // 出掉的牌牌堆，玩家可以根据这个来做AI
    vector<Player*> mPlayers;  // 本局游戏参与者
};

