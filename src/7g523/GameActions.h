// 文件名：GameActions.h
// 作者：你的名字
// 日期：当前日期
// 描述：这个头文件定义了一个抽象类 GameActions，它包含了一系列游戏动作的接口。这些动作包括添加卡片和播放卡片等操作。
// 请注意，这个文件是一个接口文件，不应该包含任何实现。需要被继承的类来实现这些动作。

#ifndef __GAMEACTIONS_H__
#define __GAMEACTIONS_H__
#include "../Card.h"

// 增加一个playCard 后的返回值，用于判断是否出牌成功
enum PlayCardResult {
    PLAY_CONTINUE,  // 出牌
    PASS      // 要不起
};

// 定义一个抽象类 GameActions，包含一系列游戏动作的接口。
class GameActions {
public:
    virtual void addCard() = 0;
    // 展示最小的牌是什么
    virtual shared_ptr<Card> showMinCard() = 0;
    virtual enum PlayCardResult playCard() = 0;
};

#endif // __GAMEACTIONS_H__
