#include "GameRule7g523.h"
#include "GameActions.h"

// 实例化静态成员变量 cardRankRule，设置为一个空的map，用于存储牌的规则。
map<int, int> GameRule7g523Helper::cardRankRule = {
    // 牌的规则，数值越大在牌的规则里对应的牌就越大
    {14, 13},  // 大王, 第二大
    {13, 12},
    {12, 7},
    {11, 6},
    {10, 5},
    {9,  4},
    {8,  3},
    {7,  2},
    {6,  14},  // 7, 最大
    {5,  1},
    {4,  11},
    {3,  0},
    {2,  9},
    {1,  10},
    {0,  8}
};

GameRule7g523Helper::GameRule7g523Helper() {
    mPlayerOrder.clear();
}

bool GameRule7g523Helper::cardCompare(const Card& a, const Card& b) {  // 升序排列，即小的在前
    if (a.getValue() == b.getValue()) {
        return a.getSuit() < b.getSuit();
    } else {
        return cardRankRule[a.getValue()] < cardRankRule[b.getValue()];
    }
}

bool GameRule7g523Helper::cardValueCompare(int valueA, int valueB) {  // 升序排列，即小的在前
    return cardRankRule[valueA] < cardRankRule[valueB];
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

// 根据规则给牌型排序，从小到大
void GameRule7g523Helper::sortCards(vector<shared_ptr<Card>>& cards) {
    sort(cards.begin(), cards.end(), [](const shared_ptr<Card>& a, const shared_ptr<Card>& b) {
        return cardCompare(*a, *b);
    });
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
    // 游戏开始，先抽牌到5张
    drawCard();

    // 判断第一个出牌的玩家，即谁有最小的牌，谁先出牌
    shared_ptr<Card> minCard;
    int minCardPlayerIndex = 0;
    for (int i = 0; i < (int)mPlayers.size(); ++i) {
        auto player = mPlayers[i];
        if (minCard == nullptr) {
            minCard = dynamic_pointer_cast<GameActions>(player)->showMinCard();
        } else {
            shared_ptr<Card> card = dynamic_pointer_cast<GameActions>(player)->showMinCard();
            if (cardCompare(*card, *minCard)) {
                minCard = card;
                minCardPlayerIndex = i;
            }
        }
    }
    // 使用for 循环设置所有玩家出牌的顺序。从minCardPlayerIndex开始依次往后出牌。
    for (int i = 0; i < (int)mPlayers.size(); ++i) {
        setPlayersOrder((minCardPlayerIndex + i) % mPlayers.size());
    }

    mScene->showNotice("    " + minCard->toString() + " " + mPlayers[getPlayerOrderIndex()]->getName() + " 先出牌!!!  ");
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

// 玩家出牌顺序，轮流出牌，setPlayersOrder设置玩家出牌的顺序，getPlayerOrder获取当前玩家的出牌顺序
void GameRule7g523Helper::setPlayersOrder(int playerIndex) {
    mPlayerOrder.push_back(playerIndex);
}

// 从玩家出牌的顺序中获取下一个玩家的索引
void GameRule7g523Helper::setNextPlayerOrderIndex() {
    mCurrentPlayerOrderIndex = (mCurrentPlayerOrderIndex + 1) % mPlayerOrder.size();
}

// 获取当前玩家出牌的顺序索引，即当前轮到哪个玩家出牌 
int GameRule7g523Helper::getPlayerOrderIndex() {
    return mPlayerOrder[mCurrentPlayerOrderIndex];
}