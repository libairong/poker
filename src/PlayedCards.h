#ifndef __PLAYEDCARDS_H__
#define __PLAYEDCARDS_H__

#include <memory>
#include <vector>
#include "Player.h"

class Player;
using namespace std;
/**
 * 玩家位置对应出掉的牌类
 */
class PlayedCards {
public:
    PlayedCards(shared_ptr<Player> player, vector<shared_ptr<Card>> cards);

    shared_ptr<Player> player;
    vector<shared_ptr<Card>> cards;
};
#endif  // __PLAYEDCARDS_H__
