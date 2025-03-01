/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : Layer.cpp
*   owner       : bairong.li
*   date        : 2025-02-09
*   description :
*
================================================================*/
#include <iostream>
// this_thread
#include <thread>
#include "Layer.h"

using namespace std;

Layer::Layer(int width, int height)
    : mWidth(width), mHeight(height), x(0), y(0), mDirty(false), isDisplaying(false) {
    mContent.resize(height, vector<Cell>(width, Cell())); // 初始化为空格、默认颜色和无特殊效果
}

Layer::Layer() {}

void Layer::resize(int width, int height) {
    mWidth = width;
    mHeight = height;
    mContent.resize(height, vector<Cell>(width, Cell()));  // 重新设置大小
}

void Layer::setName(const string& name) {
    mName = name;
}

string Layer::getName() const {
    return mName;
}

void Layer::setContent(int x, int y, char c, Color color, const vector<Color>& effects) {
    // 需要忙等待直到前一帧渲染完成
    while (getIsDisplaying()) {
    }

    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
        // 将char 转成string 以便可以存储单个字符
        string str(1, c);
        mContent[y][x] = Cell(str, color, effects);  // 存储字符、颜色和特殊效果
    }
    // 标记应该更新
    setDirty(true);
}

// 自动生成的strlength 仅仅对ascii 有效，要做成可以一个str ，加长度来表示
void Layer::setContent(int x, int y, const string& str, Color color, const vector<Color>& effects) {
    // 需要忙等待直到前一帧渲染完成
    while (getIsDisplaying()) {
        cout << __func__ << " busy waiting..." << endl;
    }

    int strLength = str.size();

    // 如果位置越界，直接返回
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
        return;
    }

    // 设置字符串的内容
    for (int i = 0; i < strLength; ++i) {
        if (x + i < mWidth) {  // 确保没有越界
            // 将char 转成string 以便可以存储单个字符
            string str_c(1, str[i]);
            mContent[y][x + i] = Cell(str_c, color, effects);  // 设置每个字符的内容
        } else {
            break;  // 如果字符超出宽度，停止设置
        }
    }
    // 标记应该更新
    setDirty(true);
}

// 设置cell
void Layer::setContent(int x, int y, const Cell& cell) {
    // 需要忙等待直到前一帧渲染完成
    while (getIsDisplaying()) {
        cout << __func__ << " busy waiting..." << endl;
    }

    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
        mContent[y][x] = cell;  // 设置cell
    }
    setDirty(true);
}

// 编写函数void setContentOutoffAscii(int x, int y, const std::string& str, Color color = Color::RESET, const std::vector<Color>& effects = {});
// 用于设置非ascii字符
void Layer::setContentString(int x, int y, const string& str, Color color,
                                const vector<Color>& effects) {
    // 需要忙等待直到前一帧渲染完成
    while (getIsDisplaying()) {
        cout << __func__ << " busy waiting..." << endl;
    }

    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
        mContent[y][x] = Cell(str, color, effects);  // 设置字符、颜色和特殊效果
    }
    setDirty(true);
}

Cell Layer::getContent(int x, int y) const {
    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
        return mContent[y][x];
    }
    return Cell();  // 返回默认值
}

void Layer::setDirty(bool dirty) {
    mDirty = dirty;
}

bool Layer::getDirty() const {
    return mDirty;
}

const vector<vector<Cell>>& Layer::getContent() const {
    return mContent;
}

void Layer::clear() {
    for (int i = 0; i < mHeight; ++i) {
        for (int j = 0; j < mWidth; ++j) {
            mContent[i][j] = Cell();  // 将每一格重置为空格、默认颜色和无效果
        }
    }
    setDirty(true);
}

void Layer::setIsDisplaying(bool display) {
    isDisplaying = display;
}

bool Layer::getIsDisplaying() const {
    return isDisplaying;
}

int Layer::getWidth() const {
    return mWidth;
}

int Layer::getHeight() const {
    return mHeight;
}

void Layer::setStartPosition(int col, int row) {
    x = col;
    y = row;
}

int Layer::getStartRowY() const {
    return y;
}

int Layer::getStartColX() const {
    return x;
}
