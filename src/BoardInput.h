/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : BoardInput.h
*   owner       : bairong.li
*   date        : 2025-03-10
*   description :
*
================================================================*/

#pragma once
#include <stdio.h>
#include <termios.h>

// 设计一个获取键盘输入的类
class BoardInput {
public:
    BoardInput() = default;
    ~BoardInput() = default;

    // 获取键盘输入
    char getInput();
};
