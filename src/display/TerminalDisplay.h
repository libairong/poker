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
#define DISPLAY_HEIGHT 40

class TerminalDisplay {
public:
    ~TerminalDisplay();
    // 禁用拷贝构造函数和赋值操作符
    TerminalDisplay(const TerminalDisplay&) = delete;
    TerminalDisplay& operator=(const TerminalDisplay&) = delete;

    static TerminalDisplay& getInstance();
    void reinitDisplayRect(int width, int height);

    bool addLayer(shared_ptr<Layer> layer);
    void overlayAndDisplay();
    void displayAll() const;
    void display(shared_ptr<Layer> layer) const;
    void clearAll();
    int getLayerCount() const;
    bool layerSizeCheck(shared_ptr<Layer> layer);

private:
    TerminalDisplay();
    void setStartRowAndCol();
    void moveCursor(int row, int col) const;

    vector<shared_ptr<Layer>> mLayers;
    shared_ptr<Layer> mDisplayRect;
    string mDisplayString;  // 合成图层的所有字符串到这里
    struct winsize w;
    int rows, cols;
    int startRow = 0;
    int startCol = 0;
};

#endif // TERMINAL_DISPLAY_H

