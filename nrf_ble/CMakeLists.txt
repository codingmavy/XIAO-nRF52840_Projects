# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
# openocd runner configuration
set(BOARD_FLASH_RUNNER openocd)
set(BOARD_DEBUG_RUNNER openocd)


find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)

# using include for openocd
board_runner_args(openocd
    "--cmd-pre-init=source [find interface/stlink.cfg]"
    "--cmd-pre-init=transport select swd"
    "--cmd-pre-init=source [find target/nordic/nrf52.cfg]")
message(STATUS ">>> runner args applied")   

target_sources(app PRIVATE src/main.c)
