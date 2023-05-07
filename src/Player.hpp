#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "Scene.hpp"
#include "GameRule.hpp"

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
        _position(position) {}
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

/**
 * 玩家子类：电脑玩家
 */
class ComputerPlayer : public Player {
public:
    ComputerPlayer(string name, int position);
    void addCard(Card card) override;
    void sortCards() override;
    void printCards() const override;
    vector<Card> action(const Scene *scene) override;
private:
    vector<Card> getValidCards(const vector<Card>& myCards, const vector<Card>& lastDisposedCards);
    vector<Card> searchSingleCards(const vector<Card>& myCards, int lastValue);
    vector<Card> searchMultiSameValueCards(const vector<Card>& myCards, int lastValue, int cntSameValue);
    vector<Card> searchSequenceCards(const vector<Card>& myCards, int lastValue, int len, int cntSameValue);

    vector<Card> _cards;
    unordered_map<string, vector<Card>> cache;
};

