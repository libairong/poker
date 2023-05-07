#include "Game.hpp"
#include <cstdlib>
#include <ctime>

Game::Game(int human_num, int computer_num): _human_num(human_num), _computer_num(computer_num) {
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

    for (int currentPlayerIndex = 0; currentPlayerIndex < computer_num + human_num; currentPlayerIndex++) {
        if (tmpComputerNum == 0) {
            HumanPlayer* humanPlayer = new HumanPlayer("Player " + to_string(human_num - tmpHumanNum), currentPlayerIndex);
            humanPlayer->setGameRule(gameRule);
            _players.push_back(humanPlayer);
            tmpHumanNum--;
            continue;
        }

        if (tmpHumanNum == 0) {
            ComputerPlayer* computerPlayer = new ComputerPlayer("Computer " + to_string(computer_num - tmpComputerNum), currentPlayerIndex);
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
    // 发牌
    dealCards();

    // 开始
    for (int i = 0; i < (int)_players.size(); i++) {
        // 摊牌
        _players[i]->printCards();
        // get play cards and show with scene
        vector<Card> cardsToShow = _players[i]->action(&scene);
        cout << "[" << _players[i]->getName() << "] ";
        for (const auto& card : cardsToShow) {
            card.print();
        }
        scene.numOfTheCardInPlayers[_players[i]->getPosition()] = _players[i]->getCurrentCardNum();
        cout << endl;

        if (cardsToShow.size() != 0)
            disposeCards(i, cardsToShow);
    }
}

// 增加测试模式，给指定玩家发特定的牌
// #define DEBUG

#ifndef DEBUG
void Game::dealCards() {
    for (int j = 0; j < (int)_players.size(); j++) {
        Player* player = _players[j];
        while (player->getCurrentCardNum() < player->getMaxCardNum()) {
            if (_cards.size() == 0)
                return;
            Card card = takeTopCard();
            player->addCard(card);
        }
        player->sortCards();
    }
}
#else
void Game::dealCards() {
        Card card(1, 4);
        _players[0]->addCard(card);
        Card card1(1, 5);
        _players[1]->addCard(card1);
        Card card2(1, 6);
        _players[2]->addCard(card2);
        Card card3(1, 7);
        _players[3]->addCard(card3);
        Card card4(1, 14);
        _players[4]->addCard(card4);
        Card card5(1, 8);
        _players[5]->addCard(card5);
        Card card6(1, 9);
        _players[6]->addCard(card6);
        Card card7(1, 13);
        _players[7]->addCard(card7);
        Card card8(1, 2);
        _players[8]->addCard(card8);
        Card card9(1, 1);
        _players[9]->addCard(card9);
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
        scene.numOfTheCardInPlayers[i] = 5;
        scene._disposed_cards = &_disposed_cards;
        scene.theNumberOfRemainingCards = _cards.size();
    }
}

