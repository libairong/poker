#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "Card.h"
#include "Scene.h"
#include "GameRule.h"
#include "debug.h"

using namespace std;
/**
 * 玩家基类
 */
class Player {
public:
    Player(string name, int position, shared_ptr<GameRule> gameR, shared_ptr<Scene> scene):
        mName(name),
        mCurrentCardNum(0),
        mMaxCardNum(5) {
        mPosition = position;
        mGameRule.reset();
        mGameRule = gameR;
        mScene = scene;
    }
    virtual ~Player() {}  // 将析构函数声明为虚函数
    virtual void printCards() {
        cout << "[" << mName << "] ";
        for (const auto& card : mCards) {
            card->print();
            cout << " ";
        }
        cout << endl;
    };

    virtual string getCardString() {
        string str = "";
        for (const auto& card : mCards) {
            str += card->toString() + " ";
        }
        return str;
    };

    virtual void action(void) = 0;
    virtual int getCurrentCardNum() { return mCurrentCardNum; };
    virtual void setCurrentCardNum(int num) { mCurrentCardNum = num; }
    virtual int  getMaxCardNum() { return mMaxCardNum; }
    virtual string getName() { return mName; }
    virtual int getPosition() { return mPosition; }
    // 从牌堆里摸牌
    virtual void addCard(void) {
        shared_ptr<Card> card = mScene->takeCard();
        mCards.push_back(card);
        mCurrentCardNum = mCards.size();
    };
    virtual void sortCards() {
        // sort(mCards.begin(), mCards.end(), [this](Card& a, Card& b) { return gameRule->cardCompare(a, b); });
    };
    
    // 获取玩家手中的牌的引用
    virtual vector<shared_ptr<Card>>& getCards() { return mCards; }

    string mName;
    shared_ptr<Scene> mScene;
    shared_ptr<GameRule> mGameRule;
    int mCurrentCardNum = 0;
    int mMaxCardNum = 0;
    int mPosition = 0;
    vector<shared_ptr<Card>> mCards;
};
#endif  // __PLAYER_H__
