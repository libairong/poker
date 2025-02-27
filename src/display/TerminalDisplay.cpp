/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : TerminalDisplay.cpp
*   owner       : bairong.li
*   date        : 2025-02-09
*   description :
*
================================================================*/

#include "TerminalDisplay.h"
// 线程相关头文件
#include <thread>
#include <chrono>
using namespace std;

#define DEBUG_MODE 0
TerminalDisplay::TerminalDisplay() : rows(DISPLAY_HEIGHT), cols(DISLAY_WIDTH) {
    // 隐藏光标
    cout << "\033[?25l";

    mDisplayRect = make_shared<Layer>(cols, rows);
    mDisplayRect->setName("Background");
    mDisplayRect->setStartPosition(0, 0);
    // offline display
    fillDisplayRect();
    addLayer(mDisplayRect);

    setStartRowAndCol();
    // 根据终端大小初始化mDisplayString，大小是纯字符串的两倍，要考虑颜色和特效字符所占的空间
    /*
    for (int i = 0; i < rows; ++i) {
        mDisplayString.insert(mDisplayString.end(), cols * 2, ' ');
    }
    */
}

// resetDisplayRect函数用于重置背景图层的大小
void TerminalDisplay::reinitDisplayRect(int width, int height) {
    mDisplayRect->resize(width, height);
    rows = height;
    cols = width;
    setStartRowAndCol();
}

// 析构函数
TerminalDisplay::~TerminalDisplay() {
    // 显示光标
    cout << "\033[?25h";
    moveCursor(w.ws_row, 0);
}

TerminalDisplay& TerminalDisplay::getInstance() {
    static TerminalDisplay instance;
    return instance;
}

// 增加一个判断图层是否超出显示范围的函数，如果超过则返回false
bool TerminalDisplay::layerSizeCheck(shared_ptr<Layer> layer) {
    if (layer->getWidth() + layer->getStartColX() > DISLAY_WIDTH
        || layer->getHeight() + layer->getStartRowY() > DISPLAY_HEIGHT) {
        cout << "图层大小超出显示范围！" << layer->getName() << endl;
        return false;
    } else {
        return true;
    }
}

bool TerminalDisplay::addLayer(shared_ptr<Layer> layer) {
    // 检查图层大小是否超出显示范围
    if (!layerSizeCheck(layer)) {
        return false;
    }
    mLayers.push_back(layer);
    return true;
}

void TerminalDisplay::overlayAndDisplay() {
    // 清除屏幕
    mDisplayRect->clear();
    fillDisplayRect();
    // 将所有图层叠加到在背景图层上，然后显示出来
    for (auto& layer : mLayers) {
        layer->setIsDisplaying(true);

        // 将图层内容叠加到mDisplayString上，并设置颜色和特效
        auto& content = layer->getContent();
        for (int i = 0; i < layer->getHeight(); ++i) {
            for (int j = 0; j < layer->getWidth(); ++j) {
                const auto& cell = content[i][j];
                mDisplayRect->setContent(j + layer->getStartColX(), i + layer->getStartRowY(), cell);
            }
        }
        layer->setIsDisplaying(false);
        layer->setDirty(false);
    }
    display(mDisplayRect);
}

void TerminalDisplay::displayAll() const {
    for (auto& layer : mLayers) {
        layer->setIsDisplaying(true);
        display(layer);
        layer->setIsDisplaying(false);
        layer->setDirty(false);
    }
}

void TerminalDisplay::display(shared_ptr<Layer> layer) const {
    int displayRow = layer->getStartRowY() + startRow;
    int displayCol = layer->getStartColX() + startCol;
    auto& mContent = layer->getContent();
    for (const auto& row : mContent) {
        moveCursor(startRow, startCol);
        moveCursor(displayRow++, displayCol);
        for (const auto& cell : row) {
            const string c = cell.character;
            const Color color = cell.baseColor;
            const vector<Color>& effects = cell.effects;

            string ansiCode = "\033[";

            bool first = true;

            for (const auto& effect : effects) {
                if (!first) ansiCode += ";";
                ansiCode += to_string(static_cast<int>(effect));
                first = false;
            }

            if (!effects.empty()) ansiCode += ";";
            ansiCode += to_string(static_cast<int>(color));

            ansiCode += "m" + c + "\033[0m";

            cout << ansiCode;
        }
    }
    cout << endl;
}

void TerminalDisplay::clearAll() {
    for (auto& layer : mLayers) {
        layer->clear();
    }
    moveCursor(w.ws_row, 0);
}

int TerminalDisplay::getLayerCount() const {
    return mLayers.size();
}

void TerminalDisplay::setStartRowAndCol() {
    if (ioctl(1, TIOCGWINSZ, &w) == -1) {
        cerr << "Error getting terminal size" << endl;
        return;
    }

    startRow = w.ws_row - rows;
    startCol = w.ws_col / 3;
}

void TerminalDisplay::moveCursor(int row, int col) const {
    cout << "\033[" << row << ";" << col << "H";
}

void TerminalDisplay::stopDisplay() {
    shouldExit = true;

    while (true) {
        if (isStop) {
            break;
        }
    }
}

void TerminalDisplay::fillDisplayRect() {
    // 设置背景图层的第一行和第一列内容
    for (int i = 0; i < mDisplayRect->getWidth(); ++i) {
        mDisplayRect->setContent(i, 0, to_string(i / 10), Color::GREEN);
    }

    for (int j = 0; j < mDisplayRect->getHeight(); ++j) {
        mDisplayRect->setContent(0, j, to_string(j / 10), Color::RED);
    }
}

// 设置一个定时任务，每隔一段时间刷新一次屏幕
bool TerminalDisplay::startDisplay() {
#if !DEBUG_MODE
    thread([this] {
        while (true) {
            // 遍历所有图层， 有脏的就更新
            bool needUpdate = false;
            for (auto& layer : mLayers) {
                if (layer->getDirty()) {
                    needUpdate = true;
                    break;
                }
            }
            if (needUpdate) {
                cout << "更新屏幕" << endl;
                // overlayAndDisplay();
                displayAll();
            }
            this_thread::sleep_for(chrono::milliseconds(20));
            if (shouldExit) {
                isStop = true;
                break;
            }
        }
    }).detach();
#else
    overlayAndDisplay();
#endif

    return true;
}