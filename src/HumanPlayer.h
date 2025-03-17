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
    shared_ptr<Card> showMinCard(void) override;
    enum PlayCardResult playCard(void) override;

private:
    // 显示自己信息，参数是游标移动方向，0表示不移动（选定当前卡牌），1表示向右移动，-1表示向左移动
    void showInfo(void);
    // 用户输入转换为游标移动方向
    void getInputAndMoveCursor(void);
    // 用户输入转换为出牌操
    shared_ptr<BoardInput> mBoardInput;

    // 定义游标位置变量
    int cursorPos = 0;
    // 定义变量，记录牌的状态，包括是否被选中
    vector<bool> cardStatus;
};

#endif  // __HUMANPLAYER_H__
