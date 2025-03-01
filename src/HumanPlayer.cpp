#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(string name, int position, shared_ptr<GameRule> gameR, shared_ptr<Scene> scene):
    Player(name, position, gameR, scene),
    Layer(0,0) {
        Layer::setName(name);
    }

void HumanPlayer::action(void) {
    printf("hello! i am human!\n");

    // 获取所有牌，然后显示
    while (getCurrentCardNum() < getMaxCardNum()) {
        addCard();
    }
    printf("i have %d cards\n", getCurrentCardNum());
    cout << "my cards: " << getCardString() << endl;

    // 显示手牌，注意有坐标位置
    int currentCol = 0;
    int currentRow = 1;
    setContent(0, 0, "my cards: ", Color::WHITE);
    for (int i = 0; i < getCurrentCardNum(); ++i) {
        // cout << mCards[i]->getSuitString() << " " << mCards[i]->getValueString() << endl;
        Color color = mCards[i]->getSuit() % 2 == 1 ? Color::WHITE : Color::RED;
        setContentString(currentCol++, currentRow, mCards[i]->getSuitString(), color);
        setContentString(currentCol++, currentRow, mCards[i]->getValueString(), color);
        setContentString(currentCol++, currentRow, " ", Color::RESET);
    }

}
