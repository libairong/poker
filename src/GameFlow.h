#ifndef __GAMEFLOW_H__
#define __GAMEFLOW_H__

#include <map>
#include <vector>
#include <memory>
#include "Player.h"

using namespace std;
class Player;

enum CombinateType {
    SINGLE, PAIR, THREE, FOUR, STRAIGHT, SINGLE_SUIT
};

// 规则类，提供卡牌列表、提供卡牌大小比较功能
class GameFlow {
public:
    GameFlow() = default;
    virtual ~GameFlow() = default;

    // 游戏开始流程
    virtual void startFlow() = 0;
    // 游戏结算流程
    virtual void endFlow() = 0;

    // 玩家加入游戏
    void addPlayer(shared_ptr<Player> player);
    // 玩家离开游戏
    void removePlayer(shared_ptr<Player> player);
    // 获取玩家列表
    vector<shared_ptr<Player>>& getPlayers();
    // 设置玩家列表
    void setPlayers(const vector<shared_ptr<Player>>& players);
    // 玩家数量
    size_t playerCount() { return mPlayers.size(); }
    // 设置场景
    void setScene(shared_ptr<Scene> scene) { mScene = scene; }
    // 获取场景
    shared_ptr<Scene> getScene() { return mScene; }
protected:
    // 玩家列表
    vector<shared_ptr<Player>> mPlayers;
    // 场景
    shared_ptr<Scene> mScene;
};

#endif  // __GAMEFLOW_H__