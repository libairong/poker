#include "Player.hpp"

ComputerPlayer::ComputerPlayer(string name, int position): Player(name, position) {}
void ComputerPlayer::addCard(Card card) {
    _cards.push_back(card);
    _currentCardNum = _cards.size();
}
void ComputerPlayer::sortCards() {
    sort(_cards.begin(), _cards.end(), [](Card& a, Card& b) { return a.get_value() < b.get_value(); });
}
void ComputerPlayer::printCards() const {
    cout << "[" << _name << "] ";
    for (const auto& card : _cards) {
        cout << "** ";
    }
    cout << endl;
}

// 根据场上已有牌和上一个玩家的出牌出牌
vector<Card> ComputerPlayer::action(Scene *scene) {
    // 玩家当前手牌
    vector<Card> myCards = _cards;

    // 场上已经出掉的牌
    vector<PositionToCards>& disposedCards = *(scene->_disposed_cards);
    vector<Card> alreadyDisposedCards;
    for (const auto& cards : disposedCards) {
        alreadyDisposedCards.insert(alreadyDisposedCards.end(), cards.cards.begin(), cards.cards.end());
    }

    // 获取上一个玩家出的牌
    vector<Card> lastDisposedCards;
    if (disposedCards.size() > 0) {
        PositionToCards lastDisposedPositionToCards = disposedCards.back();
        if (lastDisposedPositionToCards.position != _position) {
            lastDisposedCards = lastDisposedPositionToCards.cards;
        }
    }

    // 获取可以出的牌
    vector<Card> validCards = getValidCards(myCards, lastDisposedCards, alreadyDisposedCards);

    // 随机选择一张手牌
    int idx = rand() % validCards.size();
    Card card = validCards[idx];

    // 移除手牌
    myCards.erase(remove(myCards.begin(), myCards.end(), card), myCards.end());

    // 更新场景信息
    scene->numOfTheCardInPlayers[_position] = myCards.size();
    _currentCardNum = myCards.size();
    _cards = myCards;

    // 返回出牌
    return vector<Card>{card};
}

// 查找单牌
vector<Card> ComputerPlayer::searchSingleCards(const vector<Card>& myCards, int lastValue) {
    vector<Card> validCards;
    for (int i = 0; i < myCards.size(); i++) {
        int currentValue = myCards[i].get_value();
        if (currentValue > lastValue) {
            validCards.push_back(myCards[i]);
        }
    }
    return validCards;
}

// 查找对子、三张、炸弹
vector<Card> ComputerPlayer::searchMultiSameValueCards(const vector<Card>& myCards, int lastValue, int cntSameValue) {
    vector<Card> validCards;
    if (cntSameValue <= 1) {
        return validCards;
    }
    int cntCards = count(myCards.begin(), myCards.end(), Card(lastValue, Card::SPADE));
    if (cntCards >= cntSameValue) {
        for (int i = 0; i < cntSameValue; i++) {
            validCards.push_back(Card(lastValue, Card::SPADE));
        }
    } else if (cntCards + count(myCards.begin(), myCards.end(), Card(lastValue, Card::HEART)) >= cntSameValue) {
        for (int i = 0; i < cntSameValue-cntCards; i++) {
            validCards.push_back(Card(lastValue, Card::SPADE));
        }
        for (int i = 0; i < cntCards; i++) {
            validCards.push_back(Card(lastValue, Card::HEART));
        }
    } else if (cntCards + count(myCards.begin(), myCards.end(), Card(lastValue, Card::CLUB)) >= cntSameValue) {
        for (int i = 0; i < cntSameValue-cntCards; i++) {
            validCards.push_back(Card(lastValue, Card::SPADE));
        }
        for (int i = 0; i < cntCards; i++) {
            validCards.push_back(Card(lastValue, Card::CLUB));
        }
    } else if (cntCards + count(myCards.begin(), myCards.end(), Card(lastValue, Card::DIAMOND)) >= cntSameValue) {
        for (int i = 0; i < cntSameValue-cntCards; i++) {
            validCards.push_back(Card(lastValue, Card::SPADE));
        }
        for (int i = 0; i < cntCards; i++) {
            validCards.push_back(Card(lastValue, Card::DIAMOND));
        }
    }
    return validCards;
}

// 查找顺子、连对、飞机
vector<Card> ComputerPlayer::searchSequenceCards(const vector<Card>& myCards, int lastValue, int len, int cntSameValue) {
    vector<Card> validCards;
    if (myCards.size() < len || len < 5 || cntSameValue <= 1 || lastValue < 3) {
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

vector<Card> ComputerPlayer::getValidCards(const vector<Card>& myCards, const vector<Card>& lastDisposedCards,
        const vector<Card>& alreadyDisposedCards) {
    vector<Card> validCards;

    if (lastDisposedCards.empty()) {
        // 第一个出牌
        for (int i = 0; i < myCards.size(); i++) {
            validCards.push_back(myCards[i]);
        }
    } else {
        // 不是第一个出牌
        int lastValue = lastDisposedCards[0].get_value();
        int lastCount = lastDisposedCards.size();

        // 判断上一个玩家出的牌的类型
        bool isSingle = false, isPair = false, isThree = false, isBomb = false;
        if (lastCount == 1) {
            isSingle = true;
        } else if (lastCount == 2 && lastDisposedCards[0].get_value() == lastDisposedCards[1].get_value()) {
            isPair = true;
        } else if (lastCount == 3 && lastDisposedCards[0].get_value() == lastDisposedCards[1].get_value() 
                && lastDisposedCards[1].get_value() == lastDisposedCards[2].get_value()) {
            isThree = true;
        } else if (lastCount == 4 && lastDisposedCards[0].get_value() == lastDisposedCards[1].get_value()
                && lastDisposedCards[1].get_value() == lastDisposedCards[2].get_value()
                && lastDisposedCards[2].get_value() == lastDisposedCards[3].get_value()) {
            isBomb = true;
        }

        // 单牌
        if (isSingle) {
            validCards = searchSingleCards(myCards, lastValue);
            if (validCards.empty()) {
                validCards = searchMultiSameValueCards(myCards, lastValue, 2);
                if (validCards.empty()) {
                    validCards = searchMultiSameValueCards(myCards, lastValue, 3);
                    if (validCards.empty()) {
                        validCards = searchMultiSameValueCards(myCards, lastValue, 4);
                    }
                }
            }
            vector<Card> sequenceCards = searchSequenceCards(myCards, lastValue, 5, 1);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            sequenceCards = searchSequenceCards(myCards, lastValue, 6, 2);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            sequenceCards = searchSequenceCards(myCards, lastValue, 8, 3);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            // 剪枝策略
            if (validCards.empty() && myCards[0].get_value() > lastValue) {
                return validCards;
            }
        }
        // 对子
        else if (isPair) {
            validCards = searchMultiSameValueCards(myCards, lastValue, 2);
            if (validCards.empty()) {
                validCards = searchMultiSameValueCards(myCards, lastValue, 3);
                if (validCards.empty()) {
                    validCards = searchMultiSameValueCards(myCards, lastValue, 4);
                }
            }
            vector<Card> sequenceCards = searchSequenceCards(myCards, lastValue, 3, 1);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            sequenceCards = searchSequenceCards(myCards, lastValue, 4, 2);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            sequenceCards = searchSequenceCards(myCards, lastValue, 6, 3);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            // 剪枝策略
            if (validCards.empty() && myCards[0].get_value() > lastValue) {
                return validCards;
            }
        }
        // 三张
        else if (isThree) {
            validCards = searchMultiSameValueCards(myCards, lastValue, 3);
            if (validCards.empty()) {
                validCards = searchMultiSameValueCards(myCards, lastValue, 4);
            }
            vector<Card> sequenceCards = searchSequenceCards(myCards, lastValue, 2, 1);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            sequenceCards = searchSequenceCards(myCards, lastValue, 3, 2);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            sequenceCards = searchSequenceCards(myCards, lastValue, 4, 3);
            if (!sequenceCards.empty()) {
                validCards.insert(validCards.end(), sequenceCards.begin(), sequenceCards.end());
            }
            // 剪枝策略
            if (validCards.empty() && myCards[0].get_value() > lastValue) {
                return validCards;
            }
        }
        // 炸弹
        else if (isBomb) {
            validCards = searchMultiSameValueCards(myCards, lastValue, 4);
            // 剪枝策略
            if (validCards.empty() && myCards[0].get_value() > lastValue) {
                return validCards;
            }
        }
    }

    // 缓存查找的结果
    string cardString = "";
    for (int i = 0; i < lastDisposedCards.size(); i++) {
        cardString += lastDisposedCards[i].toString();
    }
    if (cache.count(cardString) > 0) {
        return cache[cardString];
    } else {
        cache[cardString] = validCards;
    }
    return validCards;
}

