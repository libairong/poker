#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "ComputerPlayer.h"
#include "HumanPlayer.h"

Game::Game(int human_num, int computer_num):
    _human_num(human_num), _computer_num(computer_num),
    playersTurn(0) {
    // 初始化规则参考类
    gameRule = make_shared<GameRule7g523>();

    // 初始化扑克牌
    initCards();
    /**
     * 初始化玩家列表
     * 通过人类玩家和电脑玩家的比例计算的概率来分配玩家座次。
     */
    int tmpComputerNum = computer_num;
    int tmpHumanNum = human_num;

    srand(time(0));

    for (int currentPlayerIndex = 0; currentPlayerIndex < computer_num + human_num;
		                     currentPlayerIndex++) {
        if (tmpComputerNum == 0) {
            HumanPlayer* humanPlayer = new HumanPlayer("Player"
			                              + to_string(human_num - tmpHumanNum),
						      currentPlayerIndex);
            humanPlayer->setGameRule(gameRule);
            _players.push_back(humanPlayer);
            tmpHumanNum--;
            continue;
        }

        if (tmpHumanNum == 0) {
            ComputerPlayer* computerPlayer = new ComputerPlayer("Computer" + to_string(computer_num - tmpComputerNum), currentPlayerIndex);
            computerPlayer->setGameRule(gameRule);
            _players.push_back(computerPlayer);
            tmpComputerNum--;
            continue;
        }

        int randNum = rand() % tmpComputerNum + tmpHumanNum;  // randNum: 0 ~ playerNum - 1;
        bool isComputer = (randNum < tmpComputerNum) ? true : false;
        if (isComputer) {
            ComputerPlayer* computerPlayer = new ComputerPlayer("Computer " + to_string(computer_num - tmpComputerNum), currentPlayerIndex);
            computerPlayer->setGameRule(gameRule);
            _players.push_back(computerPlayer);
            tmpComputerNum--;
        } else {
            HumanPlayer* humanPlayer = new HumanPlayer("Player " + to_string(human_num - tmpHumanNum), currentPlayerIndex);
            humanPlayer->setGameRule(gameRule);
            _players.push_back(humanPlayer);
            tmpHumanNum--;
        }
    }

    initScene();
}

Game::~Game() {
    for (auto it = _players.begin(); it != _players.end(); it++) {
        delete *it;
    }
}

void Game::start() {
#define MAX_ROUND 5

    // 记录下一次是谁先出牌
    int round = 0;
    while (round < MAX_ROUND) {  // 这里应该是以取胜、卡牌用完为准
        cout << endl;
        cout << "round: " << round << endl;
        // 发牌
        dealCards();
        // 开始
        // 1，从之前最大的人开始出牌。2，打到最大才重新发牌
        int i = playersTurn;
        while (true) {
            // 摊牌
            _players[i]->printCards();
            // get play cards and show with scene
            vector<Card> cardsToShow = _players[i]->action(&scene);
            cout << "[" << _players[i]->getName() << "] ";

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
                scene.numOfTheCardInPlayers[_players[i]->getPosition()] = _players[i]->getCurrentCardNum();
                disposeCards(i, cardsToShow);
                scene.resetIsPlayerPass();
            }
            (i + 1) == (int)_players.size() ? i = 0 : i += 1;
        }
        playersTurn = _disposed_cards[_disposed_cards.size() - 1].position;
        round++;
    }
}

// 增加测试模式，给指定玩家发特定的牌
#ifndef DEBUG
void Game::dealCards() {
    int j = playersTurn;
    while (true) {
        Player* player = _players[j];
        while (player->getCurrentCardNum() < player->getMaxCardNum()) {
            if (_cards.size() == 0)
                break;
            Card card = takeTopCard();
            player->addCard(card);
        }
        player->sortCards();
        (j + 1) == (int)_players.size() ? j = 0 : j += 1;

        if (j == (int)playersTurn)
            break;
    }
    scene.theNumberOfRemainingCards = _cards.size();
}
#else
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
            _players[currentLine]->addCard(Card(suitValue, cardValue));
            currentColumn++;
        }

        currentLine++;
    }
    fin.close();
}
#endif

void Game::initCards() {
    _disposed_cards.clear();

    // 四种花色
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            _cards.push_back(Card(i, j));
        }
    }
    _cards.push_back(Card(2, LITTLE_JOKER_VALUE)); // 小王
    _cards.push_back(Card(3, BIG_JOKER_VALUE)); // 大王
    shuffle(_cards);
}
void Game::shuffle(vector<Card>& cards) {
    srand((unsigned int)time(NULL));
    random_shuffle(cards.begin(), cards.end());
}

Card Game::takeTopCard() {
    Card card = _cards.back();
    _cards.pop_back();
    return card;
}

void Game::disposeCards(int playerPosition, vector<Card> cards) {
    vector<Card> lastCards;
    lastCards.insert(lastCards.end(), cards.begin(), cards.end());
    PositionToCards positionToCard(playerPosition, lastCards);
    _disposed_cards.push_back(positionToCard);
}

void Game::initScene() {
    for (int i = 0; i < (int)_players.size(); i++) {
        scene.isPlayerActive[i] = true;
        scene.isPlayerPass[i] = false;
        scene.numOfTheCardInPlayers[i] = 5;
        scene._disposed_cards = &_disposed_cards;
        scene.theNumberOfRemainingCards = _cards.size();
    }
}

