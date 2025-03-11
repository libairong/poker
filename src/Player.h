#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "Card.h"
#include "Scene.h"
#include "GameFlow.h"
#include "debug.h"

using namespace std;
class Scene;
class GameFlow;
/**
 * 玩家基类
 */
class Player {
public:
    Player(string name, int position, shared_ptr<GameFlow> gameF, shared_ptr<Scene> scene);
    virtual ~Player() {}  // 将析构函数声明为虚函数
    virtual void printCards() final;
    virtual string getCardString() final;
    virtual void action(void) = 0;
    virtual int getCurrentCardNum() final;
    virtual void setCurrentCardNum(int num) final;
    virtual int getMaxCardNum() final;
    virtual string getName() final;
    virtual int getPosition() final;
    void addCard(void);
    virtual void sortCards();
    virtual vector<shared_ptr<Card>>& getCards() final;
    virtual int getScore() final;
    virtual void setScore(int score) final;

    // 添加一个静态的方法，用于DEBUG，打印所有玩家的信息
    static void printPlayersInfo(const vector<shared_ptr<Player>>& players);

protected:
    string mName;
    shared_ptr<Scene> mScene;
    shared_ptr<GameFlow> mGameFlow;
    int mCurrentCardNum;
    int mMaxCardNum;
    int mPosition;
    vector<shared_ptr<Card>> mCards;
    int mScore;
};
#endif  // __PLAYER_H__
