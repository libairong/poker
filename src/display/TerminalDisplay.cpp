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

TerminalDisplay::TerminalDisplay() : rows(DISPLAY_HEIGHT), cols(DISLAY_WIDTH) {
    auto background = make_shared<Layer>(cols, rows);
    background->setName("Background");

    for (int i = 0; i < cols; ++i) {
        background->setContent(i, 0, to_string(i % 10)[0], Color::WHITE);
    }
    for (int i = 0; i < rows; ++i) {
        background->setContent(0, i, to_string(i % 10)[0], Color::WHITE);
    }
    addLayer(background);
    setStartRowAndCol();
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
