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

TerminalDisplay::TerminalDisplay() : rows(DISPLAY_HEIGHT), cols(DISLAY_WIDTH) {
    // 隐藏光标
    cout << "\033[?25l";

    mDisplayRect = make_shared<Layer>(cols, rows);
    mDisplayRect->setName("Background");
    setStartRowAndCol();

    // 设置一个定时任务，每隔一段时间刷新一次屏幕
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
                overlayAndDisplay();
            }
            this_thread::sleep_for(chrono::milliseconds(20));
        }
    }).detach();
}

// 析构函数
TerminalDisplay::~TerminalDisplay() {
    // 显示光标
    cout << "\033[?25h";  
}

TerminalDisplay& TerminalDisplay::getInstance() {
    static TerminalDisplay instance;
    return instance;
}

bool TerminalDisplay::addLayer(shared_ptr<Layer> layer) {
    if (layer->getWidth() + layer->getStartColX() > DISLAY_WIDTH
        || layer->getHeight() + layer->getStartRowY() > DISPLAY_HEIGHT) {
        cout << "图层大小超出显示范围！" << layer->getName() << endl;
        return false;
    }
    mLayers.push_back(layer);
    return true;
}

void TerminalDisplay::overlayAndDisplay() {
    // 清除屏幕
    mDisplayRect->clear();
    // 将所有图层叠加到在背景图层上，然后显示出来
    for (auto& layer : mLayers) {
        layer->setIsDisplaying(true);
        // 将图层内容叠加到背景图层上
        auto& content = layer->getContent();
        for (int i = 0; i < layer->getHeight(); ++i) {
            for (int j = 0; j < layer->getWidth(); ++j) {
                auto cell = content[i][j];
                mDisplayRect->setContent(j + layer->getStartColX(), i + layer->getStartRowY(), cell.character, cell.baseColor, cell.effects);
            }
        }
        layer->setIsDisplaying(false);
        layer->setDirty(false);
    }
    display(mDisplayRect);
}

void TerminalDisplay::displayAll() const {
    for (auto& layer : mLayers) {
        display(layer);
        moveCursor(startRow, startCol);
    }
}

void TerminalDisplay::display(shared_ptr<Layer> layer) const {
    int displayRow = layer->getStartRowY() + startRow;
    int displayCol = layer->getStartColX() + startCol;
    auto& mContent = layer->getContent();
    for (const auto& row : mContent) {
        moveCursor(displayRow++, displayCol);
        for (const auto& cell : row) {
            const char c = cell.character;
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

            ansiCode += "m" + string(1, c) + "\033[0m";

            cout << ansiCode;
        }
    }
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
    startCol = 0;
}

void TerminalDisplay::moveCursor(int row, int col) const {
    cout << "\033[" << row << ";" << col << "H";
}
