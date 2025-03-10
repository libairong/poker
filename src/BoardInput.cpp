/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : BoardInput.cpp
*   owner       : bairong.li
*   date        : 2025-03-10
*   description :
*
================================================================*/

#include "BoardInput.h"

char BoardInput::getInput() {
    char c;
    struct termios stored_settings;
    struct termios new_settings;
    tcgetattr (0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr (0, TCSANOW, &new_settings);
    c = getchar();
    putchar('\b'); // 删除回显
    tcsetattr (0, TCSANOW, &stored_settings); // 复终端参数
    return c;
}
