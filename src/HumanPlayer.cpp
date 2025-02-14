#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(string name, int position, shared_ptr<GameRule> gameR, shared_ptr<Scene> scene):
    Player(name, position, gameR, scene),
    Layer(10,2) {}

void HumanPlayer::action(void) {
    printf("hello! i am human!\n");

    // 获取所有牌，然后显示
    while (getCurrentCardNum() < getMaxCardNum()) {
        addCard();
    }
    printf("i have %d cards\n", getCurrentCardNum());

    // 显示手牌
    // setContent(0, 0, getCardString(), Color::RED);

}
