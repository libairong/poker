#include "Game.hpp"
#include <cstdlib>
#include <ctime>

Game::Game(int human_num, int computer_num): _human_num(human_num), _computer_num(computer_num) {
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
            _players.push_back(new HumanPlayer("Player " + to_string(human_num - tmpHumanNum), currentPlayerIndex));
            tmpHumanNum--;
            continue;
        }

        if (tmpHumanNum == 0) {
            _players.push_back(new ComputerPlayer("Computer " + to_string(computer_num - tmpComputerNum), currentPlayerIndex));
            tmpComputerNum--;
            continue;
        }

        int randNum = rand() % tmpComputerNum + tmpHumanNum;  // randNum: 0 ~ playerNum - 1;
        bool isComputer = (randNum < tmpComputerNum) ? true : false;
        if (isComputer) {
            _players.push_back(new ComputerPlayer("Computer " + to_string(computer_num - tmpComputerNum), currentPlayerIndex));
            tmpComputerNum--;
        } else {
            _players.push_back(new HumanPlayer("Player " + to_string(human_num - tmpHumanNum), currentPlayerIndex));
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
    for (int i = 0; i < _players.size(); i++) {
        // get play cards and show with scene
        vector<Card> cardsToShow = _players[i]->action(&scene);
        cout << "[" << _players[i]->getName() << "] ";
        for (const auto& card : cardsToShow) {
            card.print();
        }
        scene.numOfTheCardInPlayers[_players[i]->getPosition()] = _players[i]->getCurrentCardNum();
        cout << endl;

        disposeCards(i, cardsToShow);
    }
}

void Game::dealCards() {
    for (int j = 0; j < _players.size(); j++) {
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
    for (int i = 0; i < _players.size(); i++) {
        scene.isPlayerActive[i] = true;
        scene.numOfTheCardInPlayers[i] = 5;
        scene._disposed_cards = &_disposed_cards;
        scene.theNumberOfRemainingCards = _cards.size();
    }
}

