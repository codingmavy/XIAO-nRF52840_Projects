# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
project(blinky)

target_sources(app PRIVATE src/main.c)
