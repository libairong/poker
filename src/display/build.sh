#!/bin/bash

#================================================================
#   Copyright (C) 2025 Ltd. All rights reserved.
#
#   file        : build.sh
#   owner       : bairong.li
#   date        : 2025-02-09
#   description : 
#
#================================================================

g++ Layer.cpp TerminalDisplay.cpp main.cpp -D__DISPLAY_DEBUG__ -o a.out && ./a.out
