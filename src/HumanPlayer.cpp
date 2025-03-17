#include <string>
#include "HumanPlayer.h"
#include "7g523/GameRule7g523.h"

using namespace std;

// 定义一个游标移动动作枚举，包括不移动、向右移动、向左移动、上移移动、下移移动等。
enum CursorMoveDirection {
    CURSOR_MOVE_NONE = 0,
    CURSOR_MOVE_RIGHT = 1,
    CURSOR_MOVE_LEFT = 2,
    CURSOR_MOVE_UP = 3,
    CURSOR_MOVE_DOWN = 4,
    CURSOR_SELECT_CARD = 5,
    // 定义一个确认出牌动作枚举，包括确认出牌等。
    CURSOR_PLAY_CARD = 6,
    CURSOR_MOVE_INVALID = 7,
};

// 定义一个静态的映射，将用户输入转换为游标移动方向
static map<char, CursorMoveDirection> inputToCursorMoveMap = {
    {'w', CURSOR_MOVE_UP},
    {'s', CURSOR_MOVE_DOWN},
    {'a', CURSOR_MOVE_LEFT},
    {'d', CURSOR_MOVE_RIGHT},
    {' ', CURSOR_SELECT_CARD},
    {'q', CURSOR_PLAY_CARD},
    {'\n', CURSOR_PLAY_CARD}

};

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
    cardStatus.push_back(false);
    // 排序，从小到大
    sort(mCards.begin(), mCards.end(), [](shared_ptr<Card> a, shared_ptr<Card> b) {
        return GameRule7g523Helper::cardCompare(*a, *b);
    });

    // 显示手牌
    showInfo();
}

shared_ptr<Card> HumanPlayer::showMinCard(void) {
    return mCards[0];
}

enum PlayCardResult HumanPlayer::playCard(void) {
    if (getCurrentCardNum() == 0) {
        return PlayCardResult::PASS;
    }

    // 直到用户出牌，或者放弃出牌

    // 等待用户输入，直到按下'q/enter'
    getInputAndMoveCursor();

    // 根据选中的牌，出牌（要pop 掉出掉的牌和牌状态）
    vector<shared_ptr<Card>> cardsToPlay;
    for (int i = 0; i < getCurrentCardNum(); ++i) {
        if (cardStatus[i]) {
            cardsToPlay.push_back(mCards[i]);
        }
    }

    // 如果没有选中的牌，则放弃出牌
    if (cardsToPlay.empty()) {
        return PlayCardResult::PASS;
    }

    // 将当前对象转成shared_ptr<Player> 对象，然后将cardsToPlay 传递给PlayedCards 对象，然后出牌
    shared_ptr<Player> player = dynamic_pointer_cast<Player>(shared_from_this());
    shared_ptr<PlayedCards> playedCards = make_shared<PlayedCards>(player, cardsToPlay);
    mScene->playCards(playedCards);
    // 出牌，并更新状态
    for (auto card : cardsToPlay) {
        mCards.erase(remove(mCards.begin(), mCards.end(), card), mCards.end());
        cardStatus.erase(remove(cardStatus.begin(), cardStatus.end(), true), cardStatus.end());
        mCurrentCardNum = mCards.size();
    }
    cursorPos = 0; // 出牌后游标重置为0，方便下一次出牌
    showInfo();
    // DEBUG
    return PlayCardResult::PLAY_CONTINUE;
}

// 显示手牌，还包括游标位置等信息
void HumanPlayer::showInfo(void) {
    // 清空图层
    clear();
    // 显示手牌，注意有坐标位置
    int currentCol = 0;
    int currentRow = 1;
    setContentString(currentCol++, currentRow, "    ", Color::WHITE);
    for (int i = 0; i < getCurrentCardNum(); ++i) {
        Color color = mCards[i]->getSuit() % 2 == 1 ? Color::WHITE : Color::RED;
        // 给选中的牌加上背景色，方便用户看到
        std::vector<Color> effects = 0 ? std::vector<Color>{Color::BG_GREEN} : std::vector<Color>{};
        int fg = cardStatus[i] ? 0 : 0;
        int bg = cardStatus[i] ? 8 : 0;
        if (cursorPos == i) {
            effects.push_back(Color::BLINK);
        }
        setContentString(currentCol++, currentRow, mCards[i]->getSuitString() + mCards[i]->getValueString(), color, effects, fg, bg);
        setContentString(currentCol++, currentRow, " ", Color::RESET);
    }
    string space = "";
    for (int i = 0; i < (mMaxCardNum - getCurrentCardNum()) * 3; ++i) {
        space += " ";
    }
    setContentString(currentCol++, currentRow,  space + "    得分: " + to_string(getScore()), Color::WHITE);
}

void HumanPlayer::getInputAndMoveCursor(void) {
    // 获取输入，仅仅只有'a'、'd'、'w'、's'、'enter' 有效，输入其他字符无效继续等待
    // 输入'enter'，则打出牌
    char input;
    CursorMoveDirection direction;
    while (true) {
        while (true) {
            input = mBoardInput->getInput();
            auto it = inputToCursorMoveMap.find(input);
            if (it != inputToCursorMoveMap.end()) {
                direction = it->second;
                break;
            }
            // Handle other cases if needed
        }

        // 根据输入移动游标，并更新牌的状态。游标移动方向，0表示不移动（选定当前卡牌）cardStatus 对应的成员为 true，检查到左右移动则更新游标位置。
        switch (direction) {
            case CURSOR_MOVE_NONE:
                break;
            case CURSOR_MOVE_RIGHT:
                cursorPos = (cursorPos + 1) % getCurrentCardNum();
                break;
            case CURSOR_MOVE_LEFT:
                cursorPos = (cursorPos - 1 + getCurrentCardNum()) % getCurrentCardNum();
                break;
                // 暂时不实现上下移动，因为暂时不考虑牌的堆叠
                // case CURSOR_MOVE_UP:
                //     cursorPos = (cursorPos - getCurrentCardNum()) % getCurrentCardNum();
                //     break;
                // case CURSOR_MOVE_DOWN:
                //     cursorPos = (cursorPos + getCurrentCardNum()) % getCurrentCardNum();
                //     break;
            case CURSOR_SELECT_CARD:
                // 选定当前卡牌，但不出牌，只是选中，显示的时候可以高亮显示
                cardStatus[cursorPos] = !cardStatus[cursorPos];
                break;
            case CURSOR_PLAY_CARD:  // 退出输入，等出牌函数
                break;
            default:
                break;
        }
        showInfo();
        // 直到用户输入'enter'，确认出牌
        if (direction == CURSOR_PLAY_CARD) {
            break;
        }
    }
 
    return;
}
