/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : Player.cpp
*   owner       : bairong.li
*   date        : 2025-03-10
*   description :
*
================================================================*/

#include "Player.h"

void Player::addCard(void) {
    shared_ptr<Card> card = mScene->takeCard();
    mCards.push_back(card);
    mCurrentCardNum = mCards.size();
};

