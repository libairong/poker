#!/bin/bash

#================================================================
#   Copyright (C) 2025 Ltd. All rights reserved.
#
#   file        : build.sh
#   owner       : bairong.li
#   date        : 2025-03-07
#   description : 
#
#================================================================

make clean
make -j8
./build/my_game

