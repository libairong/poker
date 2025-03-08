#ifndef __SCENE_H__
#define __SCENE_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <memory>
#include "Player.h"
#include "PlayedCards.h"

class PlayedCards;
class Player;
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
                // i 为花色，j 为牌值
		        shared_ptr<Card> card = make_shared<Card>(static_cast<Suit>(i), j);
                mResourceCards.push_back(card);
            }
        }
        mResourceCards.push_back(make_shared<Card>(static_cast<Suit>(2), LITTLE_JOKER_VALUE)); // 小王
        mResourceCards.push_back(make_shared<Card>(static_cast<Suit>(3), BIG_JOKER_VALUE)); // 大王
        srand((unsigned int)time(NULL));
        random_shuffle(mResourceCards.begin(), mResourceCards.end());
    };

    shared_ptr<Card> takeCard() {
	    shared_ptr<Card> card = mResourceCards.back();
	    mResourceCards.pop_back();

	    return card;
    };

    shared_ptr<PlayedCards> getLastPlayedCards() {
	shared_ptr<PlayedCards> playedCard = mPlayedCards.back();

	return playedCard;
    };

    void setPlayers(vector<shared_ptr<Player>> players) {
        mPlayers = players;
    };

    shared_ptr<Player> getPlayerByNumber(int number) {
        if (number > 0 && number < (int)mPlayers.size() )
            return mPlayers.at(number);
        return NULL;
    };

private:
    vector<shared_ptr<Card>> mResourceCards;
    vector<shared_ptr<PlayedCards>>  mPlayedCards;  // 出掉的牌牌堆，玩家可以根据这个来做AI
    vector<shared_ptr<Player>> mPlayers;  // 本局游戏参与者，暂时用作debug
};
#endif  // __SCENE_H__

