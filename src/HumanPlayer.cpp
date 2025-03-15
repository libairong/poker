#include "HumanPlayer.h"
#include "7g523/GameRule7g523.h"

HumanPlayer::HumanPlayer(string name, int position, shared_ptr<Scene> scene):
    Player(name, position, scene),
    Layer(0,0) {
        Layer::setName(name);
        mBoardInput = make_shared<BoardInput>();
    }

void HumanPlayer::action(void) {
    // printf("hello! i am human!\n");

    // 获取所有牌，然后显示
    while (getCurrentCardNum() < getMaxCardNum()) {
        addCard();
    }
    // printf("i have %d cards\n", getCurrentCardNum());
    // cout << "my cards: " << getCardString() << endl;

    // 显示手牌，注意有坐标位置
    int currentCol = 0;
    int currentRow = 1;
    setContentString(currentCol++, currentRow, "    ", Color::WHITE);
    for (int i = 0; i < getCurrentCardNum(); ++i) {
        // cout << mCards[i]->getSuitString() << " " << mCards[i]->getValueString() << endl;
        Color color = mCards[i]->getSuit() % 2 == 1 ? Color::WHITE : Color::RED;
        setContentString(currentCol++, currentRow, mCards[i]->getSuitString() + mCards[i]->getValueString(), color);
        setContentString(currentCol++, currentRow, " ", Color::RESET);
    }

    setContentString(currentCol++, currentRow, "    得分: " + to_string(getScore()), Color::WHITE);
}

void HumanPlayer::addCard(void) {
    shared_ptr<Card> card = mScene->takeCard();
    if (card == nullptr) {
        cout << "没有牌可以抽" << endl;
        return;
    }
    mCards.push_back(card);
    mCurrentCardNum = mCards.size();
    // 排序，从小到大
    sort(mCards.begin(), mCards.end(), [](shared_ptr<Card> a, shared_ptr<Card> b) {
        return GameRule7g523Helper::cardCompare(*a, *b);
    });

    clear();
    // 显示手牌，注意有坐标位置
    int currentCol = 0;
    int currentRow = 1;
    setContentString(currentCol++, currentRow, "    ", Color::WHITE);
    for (int i = 0; i < getCurrentCardNum(); ++i) {
        // cout << mCards[i]->getSuitString() << " " << mCards[i]->getValueString() << endl;
        Color color = mCards[i]->getSuit() % 2 == 1 ? Color::WHITE : Color::RED;
        setContentString(currentCol++, currentRow, mCards[i]->getSuitString() + mCards[i]->getValueString(), color);
        setContentString(currentCol++, currentRow, " ", Color::RESET);
    }
    setContentString(currentCol++, currentRow, "    得分: " + to_string(getScore()), Color::WHITE);
}

shared_ptr<Card> HumanPlayer::showMinCard(void) {
    return mCards[0];
}

void HumanPlayer::playCard(void) {

    setContentString(6, 1, "    得分: " + to_string(getScore()), Color::WHITE);
    
}
