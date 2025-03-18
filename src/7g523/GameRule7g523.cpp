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
    sort(cards.begin(), cards.end(), [this](const shared_ptr<Card>& a, const shared_ptr<Card>& b) {
        return cardCompare(*a, *b);
    });
}

// 玩家出牌
void GameRule7g523Helper::playCard() {
    enum PlayCardResult result = PlayCardResult::PLAY_CONTINUE;
    while (true) {
        if (result == PlayCardResult::PASS) {
            break;
        }
        result = PlayCardResult::PASS;
        for (int i = 0; i < (int)mPlayers.size(); ++i) {
            auto player = mPlayers[getPlayerOrderIndex()];
            // 轮到玩家出牌，玩家出牌
            if (dynamic_pointer_cast<GameActions>(player)->playCard() == PlayCardResult::PLAY_CONTINUE) {
                result = PlayCardResult::PLAY_CONTINUE;
            }
            mScene->freshAndDisplay();

            // 设置下一个出牌的玩家
            setNextPlayerOrderIndex();
        }
    }

}

// 判断能不能出牌，与mScene 的 playedCards最后一个元素对比，判断是否可以出牌
bool GameRule7g523Helper::canPlayCard(const shared_ptr<PlayedCards>& playedCards) {
    // 判断playedCards是否为空
    if (playedCards == nullptr) {  // 一般不会执行这里，因为playedCards 就是PASS.
        return true;
    }

    // 判断这个要打出去的组合符不符合规则
    CombinateType combinateType = cardsType(playedCards->mCards);
    if (combinateType == CombinateType::INVALID_COMBINATE) {
        return false;
    }
    
    // 判断上一个出牌的是不是自己，如果是表面这是一轮新的开始，则可以出牌
    if (playedCards->mPlayer == mScene->getLastPlayedCards()->mPlayer) {
        return true;
    }

    // 判断playedCards的组合类型是否与上一个玩家出的牌的类型相同，先获取上一个玩家出的牌的类型
    shared_ptr<PlayedCards> lastPlayerPlayedCard = mScene->getLastPlayedCards();
    CombinateType lastPlayerCardType = cardsType(lastPlayerPlayedCard->mCards);

    // 如果大家的牌组合类型相同
    if (lastPlayerCardType == combinateType) {
        // 判断在同一个组合类型下，是否能打得过
        if (lastPlayerCardType == CombinateType::SINGLE) {
            // 单张的比较，直接比较数值大小即可。
            return !cardCompare(*(playedCards->mCards[0]), *(lastPlayerPlayedCard->mCards[0]));
        } else if (lastPlayerCardType == CombinateType::PAIR) {
            // 对子的比较，直接比较数值大小即可。对比各自的对子最大的牌即可。
            return !cardCompare(*(playedCards->mCards[1]), *(lastPlayerPlayedCard->mCards[1]));
        } else if (lastPlayerCardType == CombinateType::THREE) {
            // 三张的比较，直接比较数值大小即可。
            return !cardCompare(*(playedCards->mCards[0]), *(lastPlayerPlayedCard->mCards[0]));
        } else if (lastPlayerCardType == CombinateType::FOUR) {
            // 四张的比较，直接比较数值大小即可。
            return !cardCompare(*(playedCards->mCards[0]), *(lastPlayerPlayedCard->mCards[0]));
        } else if (lastPlayerCardType == CombinateType::STRAIGHT) {
            // 顺子的比较，直接比较数值大小即可。对比各自的对子最大的牌即可。
            return !cardCompare(*(playedCards->mCards[4]), *(lastPlayerPlayedCard->mCards[4]));
        } else if (lastPlayerCardType == CombinateType::SINGLE_SUIT) {
            // 同花的比较，比较第一张牌的花色即可。
            return !cardCompare(*(playedCards->mCards[0]), *(lastPlayerPlayedCard->mCards[0]));
        } else if (lastPlayerCardType == CombinateType::THREE_AND_A_PAIR) {
            // 三带二的比较，直接比较三张牌部分的数值大小即可。
            return !cardCompare(*(playedCards->mCards[3]), *(lastPlayerPlayedCard->mCards[3]));
        } else if (lastPlayerCardType == CombinateType::FOUR_AND_A_SINGLE) {
            // 四带二的比较，直接比较4张牌部分的数值大小即可。
            return !cardCompare(*(playedCards->mCards[3]), *(lastPlayerPlayedCard->mCards[3]));
        } else if (lastPlayerCardType == CombinateType::SIRAIGHT_AND_SINGLE_SUIT) {
            // 同花顺的比较，比较第一张牌的花色即可。
            return !cardCompare(*(playedCards->mCards[0]), *(lastPlayerPlayedCard->mCards[0]));
        }
    } else {
        /* 如果上一个玩家出的牌和自己出的牌组合类型不同，需要根据具体类型判断是否能打过. */
        // 如果上一家的类型不是五张牌的组合，那么类型不同就直接打不过
        if (lastPlayerCardType <= CombinateType::FOUR) {
            return false;
        } else {
            // 如果上一家的类型是五张牌的组合，那么类型的大小枚举数值越大就越大
            return (int)lastPlayerCardType < (int)combinateType;
        }
    }

    return true;  // 这里应该不会执行到这里，因为上面的逻辑已经覆盖了所有情况。
}

// 判断出牌是否合法，给用户提供规则指导。判断出牌是否符合牌组合逻辑。
CombinateType GameRule7g523Helper::cardsType(const vector<shared_ptr<Card>>& cards) {
    if (cards.size() == 0) {  // 一般不会执行这里，因为playedCards 就是PASS. 这个逻辑也正常
        return CombinateType::INVALID_COMBINATE;
    }

    /* 先判断组合是否合理，即组合类型是否正确。 */
    if (cards.size() == 1) {
        return CombinateType::SINGLE;
    }

    // 调用判断是否为对子的函数
    if (isPair(cards)) {
        return CombinateType::PAIR;
    }
    // 调用判断是否为三张的函数
    if (isThree(cards)) {
        return CombinateType::THREE;
    }
    // 调用判断是否为四张的函数
    if (isFour(cards)) {
        return CombinateType::FOUR;
    }

    // 调用判断是否为顺子的函数
    if (isStraight(cards)) {
        // 如果不是同色就可以返回是顺子，否则是豹子。
        if (isSingleSuit(cards)) {
            return CombinateType::SIRAIGHT_AND_SINGLE_SUIT;
        }
        return CombinateType::STRAIGHT;
    }

    // 调用判断是否为同花且为豹子的函数
    if (isSingleSuit(cards)) {
        return CombinateType::SINGLE_SUIT;
    }

    // 判断是不是三带二
    if (isThreeAndTwo(cards)) {
        return CombinateType::THREE_AND_A_PAIR;
    }

    return CombinateType::INVALID_COMBINATE;
}

// 函数：判断是否为对子，两张牌的数值必须相同
bool GameRule7g523Helper::isPair(const vector<shared_ptr<Card>>& cards) {
    for (int i = 0; i < (int)cards.size(); i += 2) {
        if (cards[i]->getValue() != cards[i + 1]->getValue()) {
            return false;
        }
    }
    return true;
}

// 判断是否为 三张牌，数值必须相同
bool GameRule7g523Helper::isThree(const vector<shared_ptr<Card>>& cards) {
    if (cards.size() != 3) {
        return false;
    }
    if (cards[0]->getValue() != cards[1]->getValue() || cards[0]->getValue() != cards[2]->getValue()) {
        return false;
    }
    return true;
}

// 判断是否为四张牌，数值必须相同
bool GameRule7g523Helper::isFour(const vector<shared_ptr<Card>>& cards) {
    if (cards.size() != 4) {
        return false;
    }
    if (cards[0]->getValue() != cards[1]->getValue() ||
        cards[1]->getValue() != cards[2]->getValue() ||
        cards[2]->getValue() != cards[3]->getValue()) {
        return false;
    }
    return true;
}

// 函数：判断是否为顺子，五张牌的数值必须是连续的
bool GameRule7g523Helper::isStraight(const vector<shared_ptr<Card>>& cards) {
    if (cards.size() != 5) {
        return false;
    }
    for (int i = 0; i < (int)cards.size() - 1; ++i) {
        if (cards[i]->getValue() + 1 != cards[i + 1]->getValue()) {
                return false;
        }
    }
    return true;
}

// 函数：判断是否为同花的五张牌组合
bool GameRule7g523Helper::isSingleSuit(const vector<shared_ptr<Card>>& cards) {
    if (cards.size() != 5) {
        return false;
    }
    for (int i = 0; i < (int)cards.size(); ++i) {
        if (cards[i]->getSuit() != cards[0]->getSuit()) {
            return false;
        }
    }
    return true;
}

// STRAIGHT_AND_SINGEL_SUIT
bool GameRule7g523Helper::isStraightAndSingleSuit(const vector<shared_ptr<Card>>& cards) {
    // 调用上面函数实现
    if (!isStraight(cards)) {
        return false;
    }
    if (!isSingleSuit(cards)) {
        return false;
    }
    return true;
}

// 判断是不是三带二
bool GameRule7g523Helper::isThreeAndTwo(const vector<shared_ptr<Card>>& cards) {
    if (cards.size() != 5) {
        return false;
    }
    // 需要判断5张牌中前三张是否为三张还是后三张是三张
    if (isThree(vector<shared_ptr<Card>>(cards.begin(), cards.begin() + 3)) &&
        isPair(vector<shared_ptr<Card>>(cards.begin() + 3, cards.end()))) {
        return true;
    }
    if (isThree(vector<shared_ptr<Card>>(cards.begin() + 2, cards.end())) &&
        isPair(vector<shared_ptr<Card>>(cards.begin(), cards.begin() + 2))) {
        return true;
    }
    return false;
}

// 判断是不是四个带一个单牌
bool GameRule7g523Helper::isFourAndOne(const vector<shared_ptr<Card>>& cards) {
    if (cards.size() != 5) {
        return false;
    }
    if (isFour(vector<shared_ptr<Card>>(cards.begin(), cards.begin() + 4))
        || isFour(vector<shared_ptr<Card>>(cards.begin() + 1, cards.end())))  {
        return true;
    }
    return false;
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
    sleep(2);
    // 游戏开始，先抽牌到5张
    drawCard();
    mScene->freshAndDisplay();
    setPlayersOrder();

    while (true && !mIsStopGame) {
        playCard();
        mScene->freshAndDisplay();
        if (isGameOver()) {
            endFlow();
            break;
        }
        drawCard();
        mScene->freshAndDisplay();
    }
}

// 游戏结算流程
void GameRule7g523Helper::endFlow() {
    mIsStopGame = true;
}

// 玩家出牌顺序，轮流出牌，setPlayersOrder设置玩家出牌的顺序，getPlayerOrder获取当前玩家的出牌顺序
void GameRule7g523Helper::setPlayersOrder() {
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
        mPlayerOrder.push_back((minCardPlayerIndex + i) % mPlayers.size());
    }
    mScene->showNotice("    " + minCard->toString() + " " + mPlayers[getPlayerOrderIndex()]->getName() + " 先出牌!!!  ");
    sleep(5);
}

// 从玩家出牌的顺序中获取下一个玩家的索引
void GameRule7g523Helper::setNextPlayerOrderIndex() {
    mCurrentPlayerOrderIndex = (mCurrentPlayerOrderIndex + 1) % mPlayerOrder.size();
}

// 获取当前玩家出牌的顺序索引，即当前轮到哪个玩家出牌 
int GameRule7g523Helper::getPlayerOrderIndex() {
    return mPlayerOrder[mCurrentPlayerOrderIndex];
}