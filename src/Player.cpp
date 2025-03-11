/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : Player.cpp
*   owner       : bairong.li
*   date        : 2025-03-10
*   description :
*
================================================================*/

#include <iostream>
#include <algorithm>
#include "Player.h"

using namespace std;

// 构造函数
Player::Player(string name, int position, shared_ptr<GameFlow> gameF, shared_ptr<Scene> scene)
    : mName(name),
      mScene(scene),
      mGameFlow(gameF),
      mCurrentCardNum(0),
      mMaxCardNum(5),
      mPosition(position),
      mScore(0) {}

// 打印手中牌
void Player::printCards() {
    cout << "[" << mName << "] ";
    for (const auto& card : mCards) {
        card->print();
        cout << " ";
    }
    cout << endl;
}

// 获取手中牌的字符串表示
string Player::getCardString() {
    string str = "";
    for (const auto& card : mCards) {
        str += card->toString() + " ";
    }
    return str;
}

// 获取当前牌数
int Player::getCurrentCardNum() {
    return mCurrentCardNum;
}

// 设置当前牌数
void Player::setCurrentCardNum(int num) {
    mCurrentCardNum = num;
}

// 获取最大牌数
int Player::getMaxCardNum() {
    return mMaxCardNum;
}

// 获取玩家名字
string Player::getName() {
    return mName;
}

// 获取玩家位置
int Player::getPosition() {
    return mPosition;
}

void Player::addCard(void) {
    shared_ptr<Card> card = mScene->takeCard();
    mCards.push_back(card);
    mCurrentCardNum = mCards.size();
}

// 对手中牌进行排序
void Player::sortCards() {
    // sort(mCards.begin(), mCards.end(), [this](Card& a, Card& b) { return gameFule->cardCompare(a, b); });
}

// 获取玩家手中的牌的引用
vector<shared_ptr<Card>>& Player::getCards() {
    return mCards;
}

// 获取分数
int Player::getScore() {
    return mScore;
}

// 设置分数
void Player::setScore(int score) {
    mScore = score;
}