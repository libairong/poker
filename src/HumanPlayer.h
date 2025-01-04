/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : HumanPlayer.h
*   owner       : bairong.li
*   date        : 2025-01-04
*   description :
*
================================================================*/
#include "Player.h"

/**
 * 玩家子类：人类玩家
 */
class HumanPlayer : public Player {
public:
    HumanPlayer(string name, int position);
    void addCard(Card card) override;
    void sortCards() override;
    void printCards() const override;
    vector<Card> action(const Scene *Scene) override;
private:
    vector<Card> _cards;
};

