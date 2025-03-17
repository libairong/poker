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
    if (mResourceCards.empty()) {
        cout << "牌堆已经没有牌了" << endl;
        return nullptr;
    }
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
    static int cardsNum = 0;
    if (cardsNum != (int)mResourceCards.size()) {
        cardsNum = mResourceCards.size();
        string resourceCardNum = "    牌堆剩余牌数：" + to_string(cardsNum);
        setContentString(currentCol++, currentRow, resourceCardNum, Color::WHITE);
    }
    
    // cout << "resourceCardNum: " << resourceCardNum << endl;

    // 2. 显示出牌堆，显示最后添加shared_ptr<PlayedCards> 里面的牌
    // 暂时只显示最后一张牌
    currentCol = 3;
    currentRow = 2;
    static int playedTimes = -1;  // 打牌次数
    if (playedTimes != (int)mPlayedCards.size()) {
        playedTimes = mPlayedCards.size();

        if (mPlayedCards.empty()) {
            setContentString(currentCol, currentRow, "    暂无出牌", Color::WHITE);
        } else {
            shared_ptr<PlayedCards> playedCard = mPlayedCards.back();
            shared_ptr<Player> player = playedCard->mPlayer;
            string lastPlayedCard = player->getName() + "出的牌：";
            setContentString(currentCol++, currentRow, lastPlayedCard, Color::WHITE);
            for (int i = 0; i < (int)playedCard->mCards.size(); i++) {
                Color color = playedCard->mCards[i]->getSuit() % 2 == 1 ? Color::WHITE : Color::RED;
                setContentString(currentCol++, currentRow, playedCard->mCards[i]->getSuitString() + playedCard->mCards[i]->getValueString(), color);
                setContentString(currentCol++, currentRow, " ", Color::BLACK);
            }
            // 填充空格
            for (int i = 0; i < (int)(20 - playedCard->mCards.size()); i++) {
                setContentString(currentCol++, currentRow, " ", Color::BLACK);
            }
        }
    }

    // 3. 显示玩家手牌
    // 4. 显示玩家得分
    // 5. 显示玩家出牌
    // 6. 显示玩家不出牌
    // 7. 显示玩家不要
}

// addPlayedCards
void Scene::playCards(const shared_ptr<PlayedCards>& playedCards) {
    mPlayedCards.push_back(playedCards);
}

// 获取剩余牌数
int Scene::getRemainCardNum() const {
    return mResourceCards.size();
}

// 增加一个公告显示函数，用于显示公告类的信息
void Scene::showNotice(const string& notice) {
    setContentString(0, 5, notice, Color::WHITE);
}
