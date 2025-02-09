/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : Layer.cpp
*   owner       : bairong.li
*   date        : 2025-02-08
*   description :
*
================================================================*/
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <sys/ioctl.h>
#include <fcntl.h>

using namespace std;
class TerminalDisplay;

// 定义颜色枚举类型
enum class Color {
    RESET = 0,        // 重置颜色
    // 前景色
    RED = 31,         // 红色
    GREEN = 32,       // 绿色
    YELLOW = 33,      // 黄色
    BLUE = 34,        // 蓝色
    MAGENTA = 35,     // 品红
    CYAN = 36,        // 青色
    WHITE = 37,       // 白色
    BRIGHT_RED = 91,  // 亮红色
    BRIGHT_GREEN = 92,// 亮绿色
    BRIGHT_YELLOW = 93,// 亮黄色
    BRIGHT_BLUE = 94, // 亮蓝色
    BRIGHT_MAGENTA = 95,// 亮品红
    BRIGHT_CYAN = 96, // 亮青色
    BRIGHT_WHITE = 97,// 亮白色

    // 背景色
    BG_BLACK = 40,    // 背景色 黑色
    BG_RED = 41,      // 背景色 红色
    BG_GREEN = 42,    // 背景色 绿色
    BG_YELLOW = 43,   // 背景色 黄色
    BG_BLUE = 44,     // 背景色 蓝色
    BG_MAGENTA = 45,  // 背景色 品红
    BG_CYAN = 46,     // 背景色 青色
    BG_WHITE = 47,    // 背景色 白色
    BG_BRIGHT_BLACK = 100, // 背景色 亮黑色
    BG_BRIGHT_RED = 101,   // 背景色 亮红色
    BG_BRIGHT_GREEN = 102, // 背景色 亮绿色
    BG_BRIGHT_YELLOW = 103,// 背景色 亮黄色
    BG_BRIGHT_BLUE = 104,  // 背景色 亮蓝色
    BG_BRIGHT_MAGENTA = 105,// 背景色 亮品红
    BG_BRIGHT_CYAN = 106,  // 背景色 亮青色
    BG_BRIGHT_WHITE = 107, // 背景色 亮白色

    // 特殊效果
    BOLD = 1,          // 加粗
    UNDERLINE = 4,     // 下划线
    BLINK = 5,         // 闪烁
    REVERSED = 7,      // 反显
    INVISIBLE = 8      // 隐藏
};

struct Cell {
    char character;
    Color baseColor;
    std::vector<Color> effects;

    Cell(char c = ' ', Color col = Color::RESET, const std::vector<Color>& eff = {})
        : character(c), baseColor(col), effects(eff) {}
};

// 图层类
class Layer : public enable_shared_from_this<Layer> {
public:
    Layer(int width, int height) : mWidth(width), mHeight(height), x(0), y(0) {
        mContent.resize(height, vector<Cell>(width, Cell()));  // 初始化为空格、默认颜色和无特殊效果
    }

    // 设置图层名字，方便调试
    void setName(const string& name) {
        mName = name;
    }

    string getName() const {
        return mName;
    }

    // 设置指定位置的字符、颜色及效果
    void setContent(int x, int y, char c, Color color = Color::RESET, const vector<Color>& effects = {}) {
        if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
            mContent[y][x] = Cell(c, color, effects);  // 存储字符、颜色和特殊效果
        }
    }

    // 新增的 setContent 函数，支持传入 string 类型的内容
    void setContent(int x, int y, const string& str, Color color = Color::RESET, const vector<Color>& effects = {}) {
        int strLength = str.length();

        // 如果位置越界，直接返回
        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
            return;
        }

        // 设置字符串的内容
        for (int i = 0; i < strLength; ++i) {
            if (x + i < mWidth) {  // 确保没有越界
                mContent[y][x + i] = Cell(str[i], color, effects);  // 设置每个字符的内容
            } else {
                break;  // 如果字符超出宽度，停止设置
            }
        }
    }

    // 获取指定位置的字符及效果
    Cell getContent(int x, int y) const {
        if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
            return mContent[y][x];
        }
        return Cell();  // 返回默认值
    }

    // 获取mContent的引用
    const vector<vector<Cell>>& getContent() const {
        return mContent;
    }

    // 清空图层内容
    void clear() {
        for (int i = 0; i < mHeight; ++i) {
            for (int j = 0; j < mWidth; ++j) {
                mContent[i][j] = Cell();  // 将每一格重置为空格、默认颜色和无效果
            }
        }
    }

    // 获取图层的宽度和高度
    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    void setStartPosition(int col, int row) {
        y = row;
        x = col;
    }
    int getStartRowY() const { return y; }
    int getStartColX() const { return x; }

private:
    int mWidth, mHeight;                         // 图层的宽度和高度
    int x, y;  // 开始的坐标
    string mName;                               // 图层的名字
    vector<vector<Cell>> mContent;  // 图层内容，字符、颜色和效果的组合
};

#define DISLAY_WIDTH 80
#define DISPLAY_HEIGHT 10
// 图层管理器类
class TerminalDisplay {
public:
    // 禁用拷贝构造函数和赋值操作符
    TerminalDisplay(const TerminalDisplay&) = delete;
    TerminalDisplay& operator=(const TerminalDisplay&) = delete;

    static TerminalDisplay& getInstance() {
        static TerminalDisplay instance;
        return instance;
    }

    // 添加一个图层
    bool addLayer(shared_ptr<Layer> layer) {
        // 增加对图层的宽度和高度检查，如果超出DISLAY_WIDTH/DISPLAY_HEIGHT大小则返回 false
        if (layer->getWidth() + layer->getStartColX() > DISLAY_WIDTH
            || layer->getHeight() + layer->getStartRowY() > DISPLAY_HEIGHT) {
            cout << "图层大小超出显示范围！" << layer->getName() << endl;
            return false;
        }
        mLayers.push_back(layer);
        return true;
    }

    // 显示所有图层内容（按顺序叠加显示）
    void displayAll() const {
        for (auto& layer : mLayers) {
            display(layer);
            // 每次显示后将光标复位
            moveCursor(startRow, startCol);
        }
    }

    // 打印当前图层的内容
    void display(shared_ptr<Layer> layer) const {
        int displayRow = layer->getStartRowY() + startRow;
        int displayCol = layer->getStartColX() + startCol;
	    auto& mContent = layer->getContent();
        for (const auto& row : mContent) {
            moveCursor(displayRow++, displayCol);
            for (const auto& cell : row) {
                const char c = cell.character;
                const Color color = cell.baseColor;
                const vector<Color>& effects = cell.effects;

                // 生成 ANSI 转义序列：首先设置效果，然后设置颜色
                string ansiCode = "\033[";

                bool first = true;

                // 添加特殊效果
                for (const auto& effect : effects) {
                    if (!first) ansiCode += ";";
                    ansiCode += to_string(static_cast<int>(effect));
                    first = false;
                }

                // 添加颜色
                if (!effects.empty()) ansiCode += ";";
                ansiCode += to_string(static_cast<int>(color));

                ansiCode += "m" + string(1, c) + "\033[0m";  // 结束样式的 ANSI 码

                cout << ansiCode;
            }
        }
    }

    // 清空所有图层
    void clearAll() {
        for (auto& layer : mLayers) {
            layer->clear();
        }
        moveCursor(w.ws_row, 0);
    }

    // 获取图层数量
    int getLayerCount() const {
        return mLayers.size();
    }
private:
    // 私有构造函数，防止外部直接创建实例
    TerminalDisplay() : rows(DISPLAY_HEIGHT), cols(DISLAY_WIDTH) {
        // 初始化代码，例如设置背景图层大小等，默认图层就是背景，大小由常量成员指定
        auto background = make_shared<Layer>(cols, rows);
        background->setName("Background");
        // 设置background图层的内容，设置成顶部一行显示列数字，左侧一列显示行数字
        for (int i = 0; i < cols; ++i) {
            background->setContent(i, 0, to_string(i % 10)[0], Color::WHITE);
        }
        for (int i = 0; i < rows; ++i) {
            background->setContent(0, i, to_string(i % 10)[0], Color::WHITE);
        }
        addLayer(background);
        setStartRowAndCol();
    }

    // 获取屏幕的行数和列数
    void setStartRowAndCol() {
        if (ioctl(1, TIOCGWINSZ, &w) == -1) {
            cerr << "Error getting terminal size" << endl;
            return;
        }
    
        // cout << "Terminal width: " << w.ws_col << endl;
        // cout << "Terminal height: " << w.ws_row << endl;
        startRow = w.ws_row - rows;
        // startCol = w.ws_col - cols;
        startCol = 0;
    }

    // 移动光标到指定位置
    void moveCursor(int row, int col) const {
        // 移动光标到指定行列
        cout << "\033[" << row << ";" << col << "H";
    }

    vector<shared_ptr<Layer>> mLayers;  // 存储多个图层
    struct winsize w;  // 存储终端窗口大小信息
    const int rows, cols;  // 默认背景图层的大小
    // 屏幕显示的起始坐标
    int startRow = 0;
    int startCol = 0;
};

#if 0
class A : public Layer {
public:
    A() : Layer(10, 5) {
        // cout << "A()" << endl;
    }
    ~A() {
        // cout << "~A()" << endl;
    }

    void init() {
        // cout << "init()" << endl;
        setContent(0, 0, getName(), Color::RED);

    }

};

int main() {
    // 创建两个图层
    auto layer1 = make_shared<Layer>(10, 5); // 创建一个10x5的图层
    layer1->setStartPosition(0, 0);
    layer1->setName("layer1");
    auto layer2 = make_shared<Layer>(10, 5); // 创建另一个10x5的图层
    layer2->setStartPosition(10, 5);
    layer2->setName("layer2");

    // 向图层1添加一些内容，并设置颜色
    layer1->setContent(1, 1, layer1->getName(), Color::RED, {Color::BLINK});
    layer1->setContent(2, 2, 'O', Color::GREEN);

    // 向图层2添加一些内容，并设置颜色
    layer2->setContent(0, 0, layer2->getName(), Color::BLUE, {Color::UNDERLINE});
    layer2->setContent(9, 4, '@', Color::MAGENTA);
    // 创建图层管理器并添加图层

    auto& terminalDisplay = TerminalDisplay::getInstance();
    terminalDisplay.addLayer(layer1);
    terminalDisplay.addLayer(layer2);

    auto a = make_shared<A>();
    a->setName("class A");
    a->init();
    a->setStartPosition(20, 5);
    std::shared_ptr<Layer> parentPtr = std::dynamic_pointer_cast<Layer>(a);
    terminalDisplay.addLayer(parentPtr);
    // 显示图层数量
    // cout << "图层数量：" << terminalDisplay.getLayerCount() << endl;
    // 显示所有图层的内容
    cout << "显示所有图层内容：\n";
    terminalDisplay.displayAll();

    // 清空所有图层内容
    terminalDisplay.clearAll();

    cout << "\n所有图层内容已清空！\n";


    return 0;
}
#endif
