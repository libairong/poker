#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <map>

using namespace std;

const string CARD_COLOR[4] = {"♣", "♦", "♥", "♠"};
const string CARD_VALUE[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

// Card 类
class Card {
public:
    Card(int color, int value): _color(color), _value(value) {}
    // 拷贝构造函数、拷贝赋值运算符、移动构造函数、移动赋值运算符同上文示例，这里不再重复
    Card& operator=(const Card&) = default;  // 显式默认拷贝运算符
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

class Scene {
public:
    Scene() {}

    map<int, bool> isPlayerActive;
    map<int, int> numOfTheCardInPlayers;
    vector<vector<Card>> _disposed_cards;
    int theNumberOfRemainingCards;
};

class Player {
public:
    Player(string name):
        _name(name),
        _currentCardNum(0),
        _maxCardNum(5) {}

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
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string name): Player(name) {}
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
    ComputerPlayer(string name): Player(name) {}
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
    vector<Card> action(Scene *Scene) {
        _currentCardNum = 0;
        return _cards;
    }
private:
    vector<Card> _cards;
};

// 应该有个场景资源管理类，Game只是流程管理

/**
 * 流程管理
 */
class Game {
public:
    Game(int human_num, int computer_num): _human_num(human_num), _computer_num(computer_num) {
        // 初始化扑克牌
        initCards();
        // 初始化玩家列表
        for (int i = 0; i < human_num; i++) {
            _players.push_back(new HumanPlayer("Player " + to_string(i+1)));
        }
        for (int i = 0; i < computer_num; i++) {
            _players.push_back(new ComputerPlayer("Computer " + to_string(i+1)));
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
            vector<Card> cardsToShow = _players[i]->action(NULL);
            cout << "[" << _players[i]->getName() << "] ";
            for (const auto& card : cardsToShow) {
                card.print();
            }
            cout << endl;

            disposeCards(cardsToShow);
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
    const int CARD_NUM = 54;
    vector<Player*> _players;
    vector<Card> _cards;  // 抽牌牌堆
    vector<vector<Card>>  _disposed_cards;  // 出掉的牌牌堆
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

    void disposeCards(vector<Card> cards) {
        vector<Card> lastCards;
        lastCards.insert(lastCards.end(), cards.begin(), cards.end());
        _disposed_cards.push_back(lastCards);
    }
    void initScene() {
        for (int i = 0; i < _players.size(); i++) {
            scene.isPlayerActive[i] = true;
            scene.numOfTheCardInPlayers[i] = 5;
            scene._disposed_cards = _disposed_cards;
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

