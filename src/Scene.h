#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <memory>
#include "Player.h"
#include "Card.h"
#include "PlayedCards.h"
#include "display/Layer.h"

class Player;
class PlayedCards;

/**
 * 场景包括：
 *     1. 玩家
 *     2. 资源牌堆
 *     3. 出牌堆
 *
 * 同时负责场景的显示。
 */
class Scene : public Layer {
public:
    Scene() = default;

    void initResourceCards();
    std::shared_ptr<Card> takeCard();
    std::shared_ptr<PlayedCards> getLastPlayedCards();
    void setPlayers(const std::vector<std::shared_ptr<Player>>& players);
    std::shared_ptr<Player> getPlayerByNumber(int number);
    void freshAndDisplay();
    void addPlayedCards(const std::shared_ptr<PlayedCards>& playedCards);

private:
    std::vector<std::shared_ptr<Card>> mResourceCards;
    std::vector<std::shared_ptr<PlayedCards>> mPlayedCards;
    std::vector<std::shared_ptr<Player>> mPlayers;
};

#endif  // __SCENE_H__