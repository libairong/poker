#include "GameRule7g523.h"
#include "GameActions.h"

// 实例化静态成员变量 cardRankRule，设置为一个空的map，用于存储牌的规则。
map<int, int> GameRule7g523Helper::cardRankRule;

GameRule7g523Helper::GameRule7g523Helper() {
    //  Card::value -> Rank(数值越大在牌的规则里对应的牌就越大)
    cardRankRule.clear();
    cardRankRule.insert({14, 13});  // 大王, 第二大
    cardRankRule.insert({13, 12});
    cardRankRule.insert({12, 7});
    cardRankRule.insert({11, 6});
    cardRankRule.insert({10, 5});
    cardRankRule.insert({9,  4});
    cardRankRule.insert({8,  3});
    cardRankRule.insert({7,  2});
    cardRankRule.insert({6,  14});  // 7, 最大
    cardRankRule.insert({5,  1});
    cardRankRule.insert({4,  11});
    cardRankRule.insert({3,  0});
    cardRankRule.insert({2,  9});
    cardRankRule.insert({1,  10});
    cardRankRule.insert({0,  8});
}

bool GameRule7g523Helper::cardCompare(const Card& a, const Card& b) {
    if (a.getValue() == b.getValue())
        return a.getSuit() > b.getSuit();
    return cardRankRule[a.getValue()] > cardRankRule[b.getValue()];
}

bool GameRule7g523Helper::cardValueCompare(int valueA, int valueB) {
    return cardRankRule[valueA] > cardRankRule[valueB];
}

CombinateType GameRule7g523Helper::cardsType(const vector<Card>& cards) {
    int lastCount = cards.size();
    // 判断上一个玩家出的牌的类型, 这里涉及到玩法，可以专门定义一个类，做出牌判断
    if (lastCount == 1) {
        return CombinateType::SINGLE;
    } else if (lastCount == 2) {
        return CombinateType::PAIR;
    } else if (lastCount == 3) {
        return CombinateType::THREE;
    } else if (lastCount == 4) {
        return CombinateType::FOUR;
    } else if (lastCount == 5 && cards[4].getValue() - cards[3].getValue() == 1
                              && cards[3].getValue() - cards[2].getValue() == 1
                              && cards[2].getValue() - cards[1].getValue() == 1
                              && cards[1].getValue() - cards[0].getValue() == 1) {
        return CombinateType::STRAIGHT;
    } else if (lastCount == 5 && cards[4].getSuit() == cards[3].getSuit()
                              && cards[3].getSuit() == cards[2].getSuit()
                              && cards[2].getSuit() == cards[1].getSuit()
                              && cards[1].getSuit() == cards[0].getSuit()) {
        return CombinateType::SINGLE_SUIT;
    } else {
        // useless
        return CombinateType::SINGLE_SUIT;
    }
}

// 玩家轮流抽牌到最大手牌数
void GameRule7g523Helper::drawCard() {
    // 轮流抽牌，每次轮到玩家，玩家仅仅抽一张牌。直到所有玩家手牌数达到最大值。
    while (mScene->getRemainCardNum() > 0) {
        bool isMaxHandCard = true;  // 记录是否还有人摸牌，如果有玩家摸牌，则继续循环。如果没有就是都摸满了
        for (const auto& player : mPlayers) {
            if (player->getCurrentCardNum() < player->getMaxCardNum()) {
                // player 转成GameActions 子类，然后再调用addCard()方法
                // cout << player->getName() << "抽牌" << endl;
                dynamic_pointer_cast<GameActions>(player)->addCard();
                isMaxHandCard = false;
            }
        }
        if (isMaxHandCard) {
            break;
        }
    }
}

// 玩家出牌
void GameRule7g523Helper::playCard() {

}

// 判断游戏是否结束
bool GameRule7g523Helper::isGameOver() {
    // 牌堆为空，且有一个玩家手牌也为空时游戏结束
    if (mScene->getRemainCardNum() == 0) {
        for (const auto& player : mPlayers) {
            if (player->getCurrentCardNum() == 0) {
                return true;
            }
        }
    }
    return false;
}

void GameRule7g523Helper::startFlow() {
    mScene->showNotice("    游戏开始!!!  ");
    while (true && !mIsStopGame) {
        drawCard();
        mScene->freshAndDisplay();
        playCard();
        mScene->freshAndDisplay();
        if (isGameOver()) {
            endFlow();
            break;
        }
    }
}

// 游戏结算流程
void GameRule7g523Helper::endFlow() {
    mIsStopGame = true;
}
