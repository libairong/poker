/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : Layer.h
*   owner       : bairong.li
*   date        : 2025-02-09
*   description :
*
================================================================*/


#pragma once
#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

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
    string character;
    Color baseColor;
    std::vector<Color> effects;

    Cell(string s = "", Color col = Color::RESET, const std::vector<Color>& eff = {})
        : baseColor(col), effects(eff) {
            character += s;
	}
};

/**
 * 注意：
 *    1. 字符的宽度默认为1，如果要设置非ascii字符（如中文、表情等），则需要调用setContentOutofAscii函数
 *    2. 字符的颜色默认为RESET，即默认颜色，如果要设置颜色，则需要调用setContent函数
 *    3. 字符串只需要确定起点，后面的字符串只需要cols 加1 即可，在显示的时候自动接上。不过这个有显示引擎决定，图层这里就是这么约定
 */

// Layer 类声明
class Layer : public std::enable_shared_from_this<Layer> {
public:
    // 构造函数
    Layer(int width, int height);
    Layer();

    // 析构函数
    virtual ~Layer() = default;

    // 重新设置图层大小
    void resize(int width, int height);

    // 设置图层名字，方便调试
    void setName(const std::string& name);

    // 获取图层名字
    std::string getName() const;

    // 设置指定位置的字符、颜色及效果
    void setContent(int x, int y, char c, Color color = Color::RESET, const std::vector<Color>& effects = {});

    // 设置指定位置的字符串内容、颜色及效果
    void setContent(int x, int y, const std::string& str, Color color = Color::RESET, const std::vector<Color>& effects = {});

    // 设置cell
    void setContent(int x, int y, const Cell& cell);

    // 设置非ascii字符
    void setContentString(int x, int y, const std::string& str, Color color = Color::RESET, const std::vector<Color>& effects = {});

    // 获取指定位置的字符及效果
    Cell getContent(int x, int y) const;

    // 获取整个图层内容
    const std::vector<std::vector<Cell>>& getContent() const;

    // 清空图层内容
    void clear();

    // 获取图层的宽度和高度
    int getWidth() const;
    int getHeight() const;

    // 设置图层起始坐标
    void setStartPosition(int col, int row);

    // 获取图层起始坐标
    int getStartRowY() const;
    int getStartColX() const;
    // 标记图层为脏，需要更新
    void setDirty(bool dirty);
    // 获取图层是否为脏，需要更新
    bool getDirty() const;
    // 设置是否显示
    void setIsDisplaying(bool display);
    // 获取是否显示
    bool getIsDisplaying() const;
    // 绘制边框，可以设置边框的起始坐标、宽度和高度，以及边框的颜色，默认为白色边框
    void drawBorder(int x = 0, int y = 0, int width = 1, int height = 1, Color color = Color::BRIGHT_WHITE,
                    const std::vector<Color>& effects = {});

private:
    int mWidth, mHeight;  // 图层的宽度和高度
    int x, y;             // 起始坐标
    std::string mName;    // 图层的名字
    //  应该被更新的标记
    bool mDirty;
    bool isDisplaying;
    std::vector<std::vector<Cell>> mContent; // 图层内容，字符、颜色和效果的组合
};

#endif // LAYER_H

