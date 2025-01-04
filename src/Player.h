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
        _name(name),
        _currentCardNum(0),
        _maxCardNum(5),
        _position(position),
        gameRule() {}
    virtual ~Player() {}  // 将析构函数声明为虚函数
    virtual void addCard(Card card) = 0;
    virtual void sortCards() = 0;
    virtual void printCards() const = 0;
    virtual vector<Card> action(const Scene *Scene) = 0;
    virtual int  getCurrentCardNum() { return _currentCardNum; };
    virtual int  getMaxCardNum() { return _maxCardNum; }
    virtual string getName() { return _name; }
    virtual int getPosition() { return _position; }

    void setGameRule(shared_ptr<GameRule>& gameR) { gameRule = gameR; }

    string _name;
    int _currentCardNum;
    int _maxCardNum;
    int _position;
    shared_ptr<GameRule> gameRule;
};

