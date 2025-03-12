#include "HumanPlayer.h"

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
        return;
    }
    mCards.push_back(card);
    mCurrentCardNum = mCards.size();
}

void HumanPlayer::playCard(void) {
    
}
