#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer(string name, int position, shared_ptr<GameRule> gameR, shared_ptr<Scene> scene):
    Player(name, position, gameR, scene), Layer(0, 0) {
        Layer::setName(name);
    }

// 根据场上已有牌和上一个玩家的出牌
void ComputerPlayer::action(void) {
    printf("hello! i am computer!\n");

    while (getCurrentCardNum() < getMaxCardNum()) {
        addCard();
    }

    printf("has per size: %d\n", getCurrentCardNum());
    // cout << getCardString() << endl;
    // setContentString(1, 0, getCardString(), Color::RED);
    static int rectangleInitFlag = 0;
    if (rectangleInitFlag == 0) {
        rectangleInitFlag = 1;
        drawBorder(0, 0, 20, 4, Color::WHITE, {Color::BOLD});
    }
    setContentString(1, 1, Player::getName(), Color::WHITE);
    setContentString(1, 2, "手牌数: " + to_string(getCurrentCardNum()), Color::WHITE);
    setContentString(2, 2, "得分: 0", Color::WHITE);

#if 0
    // 玩家当前手牌

    vector<Card> validCards;
    // 玩家当前手牌
    vector<Card> myCards = mCards;

    if (myCards.empty())
        return validCards;

    // 场上已经出掉的牌
    vector<PositionToCards>& disposedCards = *(scene->mDisposedmCards);
    vector<Card> alreadyDisposedCards;
    for (const auto& cards : disposedCards) {
        alreadyDisposedCards.insert(alreadyDisposedCards.end(), cards.cards.begin(), cards.cards.end());
    }

    // 获取上一个玩家出的牌
    vector<Card> lastDisposedCards;
    if (disposedCards.size() > 0) {
        PositionToCards lastDisposedPositionToCards = disposedCards.back();
        if (lastDisposedPositionToCards.position != mPosition) {
            lastDisposedCards = lastDisposedPositionToCards.cards;
        }
    }

    // 获取可以出的牌，不会影响到手牌
    validCards = getValidCards(myCards, lastDisposedCards);

    // 1, 要不起
    if (validCards.size() == 0)
        return validCards;

    // printf("validCards.size: %d \n", (int)validCards.size());
    // 2, 出牌，后续根据当前场上的牌和已经打出去的牌判断
    // 这里先随机选择一张手牌
    // int idx = rand() % validCards.size();
    Card card = validCards[validCards.size() - 1];

    // 这里循环移除要出的手牌
    myCards.erase(remove(myCards.begin(), myCards.end(), card), myCards.end());

    // 更新玩家手牌数
    mCurrentCardNum = myCards.size();
    mCards = myCards;

    // 返回出牌
    return vector<Card>{card};
}

// 查找单牌
vector<Card> ComputerPlayer::searchSingleCards(const vector<Card>& myCards, const Card& lastCard) {
    vector<Card> validCards;

    if (!gameRule->cardCompare(myCards[0], lastCard))
        return validCards;

    for (int i = 0; i < (int)myCards.size(); i++) {
        // 这里替换成规则判断
        if (gameRule->cardCompare(myCards[i], lastCard)) {
            validCards.push_back(myCards[i]);
        }
    }
    return validCards;
}

// 查找对子、三张、四张
vector<Card> ComputerPlayer::searchMultiSameValueCards(const vector<Card>& myCards, const vector<Card>& lastCards, int cntSameValue) {
    vector<Card> validCards;
    if ((int)myCards.size() < cntSameValue || !gameRule->cardCompare(myCards[myCards.size() - 1], lastCards[0])) {
        return validCards;
    }

    for (int i = 0; i < (int)myCards.size(); i++) {
        // 首先，牌数值要大于或等于lastCards
        if (!gameRule->cardValueCompare(myCards[i].get_value(), lastCards[0].get_value())
            || myCards[i].get_value() != lastCards[0].get_value())
            continue;

        if (Card::countSameValueCard(myCards, myCards[i].get_value()) >= cntSameValue) {
            if (cntSameValue == 2 && myCards[i].get_value() == lastCards[i].get_value()) {
                // 同数值对子
                Card::getSameValueCards(&myCards, myCards[i].get_value(), &validCards);
                if (!gameRule->cardCompare(validCards[validCards.size() - 1], lastCards[lastCards.size() - 1])) {
                    validCards.clear();
                }
            } else {
                // 对子、三张、四张
                Card::getSameValueCards(&myCards, myCards[i].get_value(), &validCards);
            }
        }
    }

    return validCards;
}

// 查找顺子、连对、飞机
vector<Card> ComputerPlayer::searchSequenceCards(const vector<Card>& myCards, int lastValue, int len, int cntSameValue) {
    vector<Card> validCards;
    if ((int)myCards.size() < len || len < 5 || cntSameValue <= 1 || lastValue < 3) {
        return validCards;
    }
    int minValue = lastValue + 1;
    for (int start = myCards.size()-len; start >= 0 && myCards[start].get_value() >= minValue; start--) {
        if (myCards[start+len-1].get_value() > lastValue) {
            bool isSequenceValid = true;
            for (int i = start; i < start + len - cntSameValue; i++) {
                if (myCards[i+cntSameValue-1].get_value() != myCards[i].get_value()) {
                    isSequenceValid = false;
                    break;
                }
            }
            if (isSequenceValid) {
                for (int i = start; i < start + len; i++) {
                    validCards.push_back(myCards[i]);
                }
            }
        }
    }
    return validCards;
}

// 这里根据规则类来判断
vector<Card> ComputerPlayer::getValidCards(const vector<Card>& myCards, const vector<Card>& lastDisposedCards) {
    vector<Card> validCards;

    if (lastDisposedCards.empty()) {
        // 第一个出牌
        for (int i = 0; i < (int)myCards.size(); i++) {
            validCards.push_back(myCards[i]);
        }
    } else {
        // 判断上家出牌类型
        CombinateType combinationType = gameRule->cardsType(lastDisposedCards);
        // printf("combinationType: %d \n", combinationType);

        // 根据当前场上的牌组合类型选择可以出的牌
        switch (combinationType) {
            case CombinateType::SINGLE: {
                validCards = searchSingleCards(myCards, lastDisposedCards[0]);
                break;
            }
            case CombinateType::PAIR:
            case CombinateType::THREE:
            case CombinateType::FOUR: {
                validCards = searchMultiSameValueCards(myCards, lastDisposedCards, lastDisposedCards.size());
                break;
            }
            case CombinateType::STRAIGHT:
            case CombinateType::SINGLE_SUIT:
            default: {
                break;
            }
        }
    }

    return validCards;
#endif
}
