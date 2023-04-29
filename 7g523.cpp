#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
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

class Player {
public:
    virtual void add_card(Card card) = 0;
    virtual void sort_cards() = 0;
    virtual void print_cards() const = 0;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string name): _name(name) {}
    void add_card(Card card) override {
        _cards.push_back(card);
    }
    void sort_cards() override {
        sort(_cards.begin(), _cards.end(), [](Card& a, Card& b) { return a.get_value() < b.get_value(); });
    }
    void print_cards() const override {
        cout << "[" << _name << "] ";
        for (const auto& card : _cards) {
            card.print();
            cout << " ";
        }
        cout << endl;
    }
private:
    string _name;
    vector<Card> _cards;
};

class ComputerPlayer : public Player {
public:
    ComputerPlayer(string name): _name(name) {}
    void add_card(Card card) override {
        _cards.push_back(card);
    }
    void sort_cards() override {
        sort(_cards.begin(), _cards.end(), [](Card& a, Card& b) { return a.get_value() < b.get_value(); });
    }
    void print_cards() const override {
        cout << "[" << _name << "] ";
        for (const auto& card : _cards) {
            cout << "** ";
        }
        cout << endl;
    }
private:
    string _name;
    vector<Card> _cards;
};

class Game {
public:
    Game(int human_num, int computer_num): _human_num(human_num), _computer_num(computer_num) {
        // 初始化扑克牌
        init_cards();
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
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < _players.size(); j++) {
                _players[j]->add_card(_cards[i*_players.size() + j]);
            }
        }

        // 输出卡牌
        for (int i = 0; i < _players.size(); i++) {
            _players[i]->sort_cards();
            _players[i]->print_cards();
        }
    }
private:
    const int CARD_NUM = 52;
    vector<Player*> _players;
    vector<Card> _cards;  // 存储所有的扑克牌
    int _human_num;
    int _computer_num;
    void init_cards() {
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

