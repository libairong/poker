#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <map>

using namespace std;

const string CARD_COLOR[4] = {"♣", "♦", "♥", "♠"};
const string CARD_VALUE[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const int CARD_NUM = 54;

// Card 类
class Card {
public:
    Card(int color, int value): _color(color), _value(value) {}
    Card& operator=(const Card&) = default;  // 显式默认拷贝运算符
    bool operator==(const Card&) const = default;  // 显式默认判断相等运算符
    void print() const {
        if (_value == 13) {
            cout << CARD_COLOR[_color] << "小王";
        } else if (_value == 14) {
            cout << CARD_COLOR[_color] << "大王";
        } else {
            cout << CARD_COLOR[_color] << CARD_VALUE[_value];
        }
    }
    int get_value() const {
        return _value;
    }
    void swap(Card& other) {
        std::swap(_color, other._color);
        std::swap(_value, other._value);
    }
    friend void swap(Card& a, Card& b) {
        a.swap(b);
    }
private:
    int _color;
    int _value;
};

class PositionToCards {
public:
    PositionToCards(int position, vector<Card> cards):
        position(position),
        cards(cards) {}
    int position;
    vector<Card> cards;
};

class Scene {
public:
    Scene() {}

    map<int, bool> isPlayerActive;
    map<int, int> numOfTheCardInPlayers;
    vector<PositionToCards>* _disposed_cards;
    int theNumberOfRemainingCards;
};

class Player {
public:
    Player(string name, int position):
        _name(name),
        _currentCardNum(0),
        _maxCardNum(5),
        _position(position) {}

    virtual void addCard(Card card) = 0;
    virtual void sortCards() = 0;
    virtual void printCards() const = 0;
    virtual vector<Card> action(Scene *Scene) = 0;
    virtual int  getCurrentCardNum() { return _currentCardNum; };
    virtual int  getMaxCardNum() { return _maxCardNum; }
    virtual string getName() { return _name; }

    string _name;
    int _currentCardNum;
    int _maxCardNum;
    int _position;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string name, int position): Player(name, position) {}
    void addCard(Card card) override {
        _cards.push_back(card);
        _currentCardNum = _cards.size();
    }
    void sortCards() override {
        sort(_cards.begin(), _cards.end(), [](Card& a, Card& b) { return a.get_value() < b.get_value(); });
    }
    void printCards() const override {
        cout << "[" << _name << "] ";
        for (const auto& card : _cards) {
            card.print();
            cout << " ";
        }
        cout << endl;
    }
    vector<Card> action(Scene *Scene) {
        _currentCardNum = 0;
        return _cards;
    }
private:
    vector<Card> _cards;
};

class ComputerPlayer : public Player {
public:
    ComputerPlayer(string name, int position): Player(name, position) {}
    void addCard(Card card) override {
        _cards.push_back(card);
        _currentCardNum = _cards.size();
    }
    void sortCards() override {
        sort(_cards.begin(), _cards.end(), [](Card& a, Card& b) { return a.get_value() < b.get_value(); });
    }
    void printCards() const override {
        cout << "[" << _name << "] ";
        for (const auto& card : _cards) {
            cout << "** ";
        }
        cout << endl;
    }

    // 根据场上已有牌和上一个玩家的出牌出牌
    vector<Card> action(Scene *scene) override {
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

private:
    vector<Card> _cards;

    // 获取可以出的牌
    vector<Card> getValidCards(vector<Card>& myCards, vector<Card>& lastDisposedCards, vector<Card>& alreadyDisposedCards) {
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

            // 判断是否有比上一个玩家出的牌更大的牌能出
            for (int i = 0; i < myCards.size(); i++) {
                int currentValue = myCards[i].get_value();
                if (currentValue > lastValue && count(myCards.begin(), myCards.end(), myCards[i]) >= lastCount) {
                    validCards.push_back(myCards[i]);
                }
            }

            // 如果没有比上一个玩家出的牌更大的牌能出，则选择任意符合规则的牌
            if (validCards.empty()) {
                for (int i = 0; i < myCards.size(); i++) {
                    int currentValue = myCards[i].get_value();
                    if (count(myCards.begin(), myCards.end(), myCards[i]) >= lastCount) {
                        validCards.push_back(myCards[i]);
                    }
                }
            }
        }

        // 如果没有符合规则的牌，则出一张最小的牌
        if (validCards.empty()) {
            validCards.push_back(*min_element(myCards.begin(), myCards.end(), [](Card& a, Card& b) { return a.get_value() < b.get_value(); }));
        }

        return validCards;
    }
};

/**
 * 流程管理
 */
class Game {
public:
    Game(int human_num, int computer_num): _human_num(human_num), _computer_num(computer_num) {
        // 初始化扑克牌
        initCards();
        // 初始化玩家列表
        int currentPlayerIndex = 0;
        for (int i = 0; i < human_num; i++) {
            _players.push_back(new HumanPlayer("Player " + to_string(i+1), currentPlayerIndex++));
        }
        for (int i = 0; i < computer_num; i++) {
            _players.push_back(new ComputerPlayer("Computer " + to_string(i+1), currentPlayerIndex++));
        }
    }

    ~Game() {
        for (auto it = _players.begin(); it != _players.end(); it++) {
            delete *it;
        }
    }
    void start() {
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
            cout << endl;

            disposeCards(i, cardsToShow);
        }
    }

    void dealCards() {
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

private:
    vector<Player*> _players;
    vector<Card> _cards;  // 抽牌牌堆
    vector<PositionToCards>  _disposed_cards;  // 出掉的牌牌堆
    int _human_num;
    int _computer_num;
    Scene scene;

    void initCards() {
        _disposed_cards.clear();

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 13; j++) {
                _cards.push_back(Card(i, j));
            }
        }
	    _cards.push_back(Card(2, 13)); // 小王
        _cards.push_back(Card(3, 14)); // 大王
        shuffle(_cards);
    }
    void shuffle(vector<Card>& cards) {
        srand((unsigned int)time(NULL));
        random_shuffle(cards.begin(), cards.end());
    }

    Card takeTopCard() {
        Card card = _cards.back();
        _cards.pop_back();
        return card;
    }

    void disposeCards(int playerPosition, vector<Card> cards) {
        vector<Card> lastCards;
        lastCards.insert(lastCards.end(), cards.begin(), cards.end());
        PositionToCards positionToCard(playerPosition, lastCards);
        _disposed_cards.push_back(positionToCard);
    }
    void initScene() {
        for (int i = 0; i < _players.size(); i++) {
            scene.isPlayerActive[i] = true;
            scene.numOfTheCardInPlayers[i] = 5;
            scene._disposed_cards = &_disposed_cards;
            scene.theNumberOfRemainingCards = _cards.size();
        }
    }
};

int main() {
    int human_num, computer_num;
    cout << "请输入玩家数量(2-10): ";
    cin >> human_num;
    if (human_num < 2 || human_num > 10) {
        cout << "输入错误" << endl;
        return 0;
    }

    computer_num = 10 - human_num;
    Game game(human_num, computer_num);
    game.start();

    return 0;
}

