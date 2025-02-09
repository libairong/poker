/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : TerminalDisplay.h
*   owner       : bairong.li
*   date        : 2025-02-09
*   description :
*
================================================================*/

#ifndef TERMINAL_DISPLAY_H
#define TERMINAL_DISPLAY_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>

#include "Layer.h"

using namespace std;

#define DISLAY_WIDTH 80
#define DISPLAY_HEIGHT 10

class TerminalDisplay {
public:
    // 禁用拷贝构造函数和赋值操作符
    TerminalDisplay(const TerminalDisplay&) = delete;
    TerminalDisplay& operator=(const TerminalDisplay&) = delete;

    static TerminalDisplay& getInstance();

    bool addLayer(shared_ptr<Layer> layer);
    void displayAll() const;
    void display(shared_ptr<Layer> layer) const;
    void clearAll();
    int getLayerCount() const;

private:
    TerminalDisplay();
    void setStartRowAndCol();
    void moveCursor(int row, int col) const;

    vector<shared_ptr<Layer>> mLayers;
    struct winsize w;
    const int rows, cols;
    int startRow = 0;
    int startCol = 0;
};

#endif // TERMINAL_DISPLAY_H

