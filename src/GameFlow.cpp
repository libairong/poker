/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : GameFlow.cpp
*   owner       : bairong.li
*   date        : 2025-03-11
*   description :
*
================================================================*/

#include "GameFlow.h"
#include <algorithm>

// 玩家加入游戏
void GameFlow::AddPlayer(shared_ptr<Player> player) {
    mPlayers.push_back(player);
}

// 玩家离开游戏
void GameFlow::RemovePlayer(shared_ptr<Player> player) {
    mPlayers.erase(remove(mPlayers.begin(), mPlayers.end(), player), mPlayers.end());
}

