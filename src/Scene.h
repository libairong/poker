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
class GameFlow;
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
    void setGameFlowAndRules(const std::shared_ptr<GameFlow>& gameFlow);
    std::shared_ptr<GameFlow> getGameFlowAndRules() const;
    void freshAndDisplay();
    void playCards(const std::shared_ptr<PlayedCards>& playedCards);
    // 获取剩余牌数
    int getRemainCardNum() const;

    // 增加一个公告显示函数，用于显示公告类的信息
    void showNotice(const std::string& notice);

private:
    std::vector<std::shared_ptr<Card>> mResourceCards;
    std::vector<std::shared_ptr<PlayedCards>> mPlayedCards;
    std::vector<std::shared_ptr<Player>> mPlayers;
    // 规则说明类，类似说明书
    std::shared_ptr<GameFlow> mGameFlowAndRules;
};

#endif  // __SCENE_H__