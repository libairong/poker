/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : ComputerPlayer.h
*   owner       : bairong.li
*   date        : 2025-01-04
*   description :
*
================================================================*/
#ifndef __COMPUTERPLAYER_H__
#define __COMPUTERPLAYER_H__

#include "Player.h"

/**
 * 玩家子类：电脑玩家
 */
class ComputerPlayer : public Player {
public:
    ComputerPlayer(string name, int position, shared_ptr<GameRule> gameR, shared_ptr<Scene> scene);
    void action(void) override;
private:
    vector<Card> getValidCards(const vector<Card>& myCards, const vector<Card>& lastDisposedCards);
    vector<Card> searchSingleCards(const vector<Card>& myCards, const Card& lastCard);
    vector<Card> searchMultiSameValueCards(const vector<Card>& myCards, const vector<Card>& lastCards, int cntSameValue);
    vector<Card> searchSequenceCards(const vector<Card>& myCards, int lastValue, int len, int cntSameValue);

    vector<Card *> mCards;
};
#endif  // __COMPUTERPLAYER_H__

