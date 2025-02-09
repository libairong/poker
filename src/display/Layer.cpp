/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : Layer.cpp
*   owner       : bairong.li
*   date        : 2025-02-09
*   description :
*
================================================================*/

#include "Layer.h"

Layer::Layer(int width, int height)
    : mWidth(width), mHeight(height), x(0), y(0) {
    mContent.resize(height, std::vector<Cell>(width, Cell())); // 初始化为空格、默认颜色和无特殊效果
}

void Layer::setName(const std::string& name) {
    mName = name;
}

std::string Layer::getName() const {
    return mName;
}

void Layer::setContent(int x, int y, char c, Color color, const std::vector<Color>& effects) {
    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
        mContent[y][x] = Cell(c, color, effects);  // 存储字符、颜色和特殊效果
    }
}

void Layer::setContent(int x, int y, const std::string& str, Color color, const std::vector<Color>& effects) {
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

Cell Layer::getContent(int x, int y) const {
    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight) {
        return mContent[y][x];
    }
    return Cell();  // 返回默认值
}

const std::vector<std::vector<Cell>>& Layer::getContent() const {
    return mContent;
}

void Layer::clear() {
    for (int i = 0; i < mHeight; ++i) {
        for (int j = 0; j < mWidth; ++j) {
            mContent[i][j] = Cell();  // 将每一格重置为空格、默认颜色和无效果
        }
    }
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
