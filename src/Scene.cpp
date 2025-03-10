/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : Scene.cpp
*   owner       : bairong.li
*   date        : 2025-01-05
*   description :
*
================================================================*/

#include <iostream>
#include <string>
#include <ctime>
#include <algorithm> // for random_shuffle
#include <cstdlib>   // for srand, rand
#include "Scene.h"

using namespace std;

void Scene::initResourceCards() {
    mPlayedCards.clear();

    // 四种花色
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            shared_ptr<Card> card = make_shared<Card>(static_cast<Suit>(i), j);
            mResourceCards.push_back(card);
        }
    }
    mResourceCards.push_back(make_shared<Card>(static_cast<Suit>(2), LITTLE_JOKER_VALUE)); // 小王
    mResourceCards.push_back(make_shared<Card>(static_cast<Suit>(3), BIG_JOKER_VALUE)); // 大王
    srand((unsigned int)time(NULL));
    random_shuffle(mResourceCards.begin(), mResourceCards.end());
}

shared_ptr<Card> Scene::takeCard() {
    shared_ptr<Card> card = mResourceCards.back();
    mResourceCards.pop_back();
    return card;
}

shared_ptr<PlayedCards> Scene::getLastPlayedCards() {
    shared_ptr<PlayedCards> playedCard = mPlayedCards.back();
    return playedCard;
}

void Scene::setPlayers(const vector<shared_ptr<Player>>& players) {
    mPlayers = players;
}

shared_ptr<Player> Scene::getPlayerByNumber(int number) {
    if (number > 0 && number < (int)mPlayers.size())
        return mPlayers.at(number);
    return nullptr;
}

void Scene::freshAndDisplay() {
    int currentCol = 3;
    int currentRow = 1;
    // 显示当前场景
    // 1. 显示资源牌堆剩余数量
    // cout << "freshAndDisplay" << endl;
    int cardsNum = mResourceCards.size();
    string resourceCardNum = "    牌堆剩余牌数：" + to_string(cardsNum);
    setContentString(currentCol++, currentRow, resourceCardNum, Color::WHITE);
    // cout << "resourceCardNum: " << resourceCardNum << endl;

    // 2. 显示出牌堆，显示最后添加shared_ptr<PlayedCards> 里面的牌
    // 暂时只显示最后一张牌
    currentCol = 3;
    currentRow = 2;
    if (mPlayedCards.empty()) {
        setContentString(currentCol, currentRow, "    暂无出牌", Color::WHITE);
        return;
    }

    shared_ptr<PlayedCards> playedCard = mPlayedCards.back();
    shared_ptr<Player> player = playedCard->mPlayer;
    string lastPlayedCard = player->getName() + "出的牌：";
    setContentString(currentCol++, currentRow, lastPlayedCard, Color::WHITE);
    for (int i = 0; i < (int)playedCard->mCards.size(); i++) {
        Color color = playedCard->mCards[i]->getSuit() % 2 == 1 ? Color::WHITE : Color::RED;
        setContentString(currentCol++, currentRow, playedCard->mCards[i]->getSuitString() + playedCard->mCards[i]->getValueString(), color);
        setContentString(currentCol++, currentRow, " ", Color::RESET);
    }

    // 3. 显示玩家手牌
    // 4. 显示玩家得分
    // 5. 显示玩家出牌
    // 6. 显示玩家不出牌
    // 7. 显示玩家不要
}
