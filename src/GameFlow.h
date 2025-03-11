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

    virtual void StartFlow() = 0;
    virtual void EndFlow() = 0;

    // 玩家加入游戏
    void AddPlayer(shared_ptr<Player> player);
    // 玩家离开游戏
    void RemovePlayer(shared_ptr<Player> player);
    // 玩家列表
    vector<shared_ptr<Player>> mPlayers;
};

#endif  // __GAMEFLOW_H__