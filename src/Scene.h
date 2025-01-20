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
        mPlayedCards.clear();

        // 四种花色
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 13; j++) {
		shared_ptr<Card> card = make_shared<Card>(i, j);
                mResourceCards.push_back(card);
            }
        }
        mResourceCards.push_back(make_shared<Card>(2, LITTLE_JOKER_VALUE)); // 小王
        mResourceCards.push_back(make_shared<Card>(3, BIG_JOKER_VALUE)); // 大王
        srand((unsigned int)time(NULL));
        random_shuffle(mResourceCards.begin(), mResourceCards.end());
    };

    shared_ptr<Card> takeCard() {
	shared_ptr<Card> card = mResourceCards.back();
	mResourceCards.pop_back();

	return card;
    };

    PlayedCard getLastPlayedCards() {
	PlayedCard playedCard = mPlayedCards.back();

	return playedCard;
    };

    void setPlayers(vector<shared_ptr<Player>> players) {
        mPlayers = players;
    };

    shared_ptr<Player> getPlayerByNumber(int number) {
        if (number > 0 && number < mPlayers.size() )
            return mPlayers.at(number);
        return NULL;
    };

private:
    vector<shared_ptr<Card>> mResourceCards;
    vector<shared_ptr<PlayedCards>>  mPlayedCards;  // 出掉的牌牌堆，玩家可以根据这个来做AI
    vector<shared_ptr<Player>> mPlayers;  // 本局游戏参与者
};

