# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(multiThreadApp)
target_sources(app PRIVATE src/main.c src/app_pwm_thread.c src/app_sensor_thread.c src/app_button_thread.c)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)