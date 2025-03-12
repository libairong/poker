/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : HumanPlayer.h
*   owner       : bairong.li
*   date        : 2025-01-04
*   description :
*
================================================================*/
#ifndef __HUMANPLAYER_H__
#define __HUMANPLAYER_H__

#include "Player.h"
#include "display/Layer.h"
#include "BoardInput.h"
#include "7g523/GameActions.h"

/**
 * 玩家子类：人类玩家
 */
class HumanPlayer : public Player, public Layer, public GameActions {
public:
    HumanPlayer(string name, int position, shared_ptr<Scene> scene);
    void action(void) override;
    void addCard(void) override;
    void showMinCard(void) override;
    void playCard(void) override;

private:
    shared_ptr<BoardInput> mBoardInput;
};

#endif  // __HUMANPLAYER_H__
