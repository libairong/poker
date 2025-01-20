#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "Scene.h"
#include "GameRule.h"
#include "debug.h"

using namespace std;
/**
 * 玩家基类
 */
class Player {
public:
    Player(string name, int position):
        mName(name),
        mCurrentCardNum(0),
        mMaxCardNum(5),
        mPosition(position),
        mGameRule(gameR),
	mScene(scene) {}
    virtual ~Player() {}  // 将析构函数声明为虚函数
    virtual void printCards() {
        cout << "[" << mName << "] ";
        for (const auto& card : mCards) {
            card.print();
            cout << " ";
        }
        cout << endl;
    };
    virtual void action(const Scene *Scene) = 0;
    virtual int  getCurrentCardNum() { return mCurrentCardNum; };
    virtual int  getMaxCardNum() { return mMaxCardNum; }
    virtual string getName() { return mName; }
    virtual int getPosition() { return mPosition; }

    string mName;
    int mCurrentCardNum;
    int mMaxCardNum;
    int mPosition;

private:
    // 从牌堆里摸牌
    virtual void addCard(Card card) {
        mCards.push_back(card);
        mCurrentCardNum = mCards.size();   
    };
    virtual void sortCards() {
        sort(mCards.begin(), mCards.end(), [this](Card& a, Card& b) { return gameRule->cardCompare(a, b); });
    };
    shared_ptr<GameRule> mGameRule;
    shared_ptr<Scene> mScene;
};

