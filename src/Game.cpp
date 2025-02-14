#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include "7g523/GameRule7g523.h"
#include "display/Layer.h"
#include "display/TerminalDisplay.h"

Game::Game(int human_num, int computer_num):
    mHuman_num(human_num), mComputer_num(computer_num),
    mPlayersTurn(0) {
    auto& terminalDisplay = TerminalDisplay::getInstance();
    int currentPlayerIndex = 0;

    // 初始化规则参考类
    mGameRule = make_shared<GameRule7g523Helper>();

    // 初始化场景
    mScene = make_shared<Scene>();
    mScene->initResourceCards();

    /**
     * 初始化玩家列表
     */
    shared_ptr<Player> humanPlayer = make_shared<HumanPlayer>("Player",
		    currentPlayerIndex++, mGameRule, mScene);
    mPlayers.push_back(humanPlayer);
    shared_ptr<Layer> humanLayer = dynamic_pointer_cast<Layer>(humanPlayer);
    terminalDisplay.addLayer(humanLayer);

    shared_ptr<ComputerPlayer> computerPlayer = make_shared<ComputerPlayer>("Computer",
		    currentPlayerIndex++, mGameRule, mScene);
    mPlayers.push_back(computerPlayer);
    shared_ptr<Layer> computerLayer = dynamic_pointer_cast<Layer>(computerPlayer);
    terminalDisplay.addLayer(computerLayer);
}

Game::~Game() {
}

void Game::start() {
    for (auto player : mPlayers) {
        player->action();
    }
    getchar();
#define MAX_ROUND 5
#if 0
    // 记录下一次是谁先出牌
    int round = 0;
    while (round < MAX_ROUND) {  // 这里应该是以取胜、卡牌用完为准
        cout << endl;
        cout << "round: " << round << endl;
        // 发牌
        dealCards();
        // 开始
        // 1，从之前最大的人开始出牌。2，打到最大才重新发牌
        int i = mPlayersTurn;
        while (true) {
            // 摊牌
            mPlayers[i]->printCards();
            // get play cards and show with scene
            vector<Card> cardsToShow = mPlayers[i]->action(&scene);
            cout << "[" << mPlayers[i]->getName() << "] ";

            if (cardsToShow.empty()) {
                cout << "要不起！" << endl;
                scene.isPlayerPass[i] = true;
                // 判断是否是其他人也是要不起
                if (scene.getTheFalseNumOfIsPlayerPass() == 1)
                    break;
            } else {
                for (const auto& card : cardsToShow) {
                    card.print();
                }
                cout << endl;
                scene.numOfTheCardInPlayers[mPlayers[i]->getPosition()] = mPlayers[i]->getCurrentCardNum();
                disposeCards(i, cardsToShow);
                scene.resetIsPlayerPass();
            }
            (i + 1) == (int)mPlayers.size() ? i = 0 : i += 1;
        }
        mPlayersTurn = _disposed_cards[_disposed_cards.size() - 1].position;
        round++;
    }
#endif
}

// 增加测试模式，给指定玩家发特定的牌
#ifndef DEBUG

#else
/*
void Game::dealCards() {
    // Deal cards by config.txt
    string filename="debug/config.txt";
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cerr << "Error: failed to open file " << filename << endl;
        exit(0);
    }

    string strline;
    int currentLine = 0;
    while (getline(fin, strline)) {
        // cout << strline << endl;
        size_t pos = strline.find_first_of("[]"); // 找到第一个 '[' 或 ']' 的位置
        while (pos != string::npos) {
            strline.replace(pos, 1, "");             // 替换当前位置的字符为 ''
            pos = strline.find_first_of("[]", pos + 1);  // 继续查找下一个 '[' 或 ']' 的位置
        }
        // split
        istringstream iss(strline);
        vector<string> tokens;
        string token;
        char delimiter = ' ';
        while (getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }

        int currentColumn = 0;
        for (const auto& item : tokens) {
            // cout << "item: " << item << " " << endl;
            // cout << item << endl;
            if (currentColumn == 0) {
                currentColumn++;
                continue;
            }

            int suitValue = 100;
            int valueOffset = 0;
            if (!memcmp("♠", item.c_str(), strlen("♠"))) {
                suitValue = 3;
                valueOffset = strlen("♠");
            } else if (!memcmp("♥", item.c_str(), strlen("♥"))) {
                suitValue = 2;
                valueOffset = strlen("♠");
            } else if (!memcmp("♣", item.c_str(), strlen("♣"))) {
                suitValue = 1;
                valueOffset = strlen("♠");
            } else if (!memcmp("♦", item.c_str(), strlen("♦"))) {
                suitValue = 0;
                valueOffset = strlen("♠");
            } else if (!memcmp("L", item.c_str(), 1)) {
                suitValue = -1;  // Not use.
                valueOffset = 1;
            } else if (!memcmp("B", item.c_str(), 1)) {
                suitValue = -2;  // Not use.
                valueOffset = 1;
            }

            // cout << "col: " << currentColumn << " item: " << item << " suit: " << suitValue << endl;
            int cardValue;
            string value(item.c_str() + valueOffset);
            if (value == "A") {
                cardValue = 0;
            } else if (value == "J" && suitValue != -1 && suitValue != -2) {
                cardValue = 10;
            } else if (value == "Q") {
                cardValue = 11;
            } else if (value == "K") {
                cardValue = 12;
            } else if (value == "J" && suitValue == -1) {
                cardValue = 13;
            } else if (value == "J" && suitValue == -2) {
                cardValue = 14;
            } else {
                cardValue = stoi(value) - 1;
            }

            // Deal card to player.
            mPlayers[currentLine]->addCard(Card(suitValue, cardValue));
            currentColumn++;
        }

        currentLine++;
    }
    fin.close();
}
*/
#endif

#if 0
void Game::disposeCards(int playerPosition, vector<Card> cards) {
    vector<Card> lastCards;
    lastCards.insert(lastCards.end(), cards.begin(), cards.end());
    PositionToCards positionToCard(playerPosition, lastCards);
    _disposed_cards.push_back(positionToCard);
}
#endif

