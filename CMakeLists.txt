cmake_minimum_required(VERSION 3.10)

project(yuv_tools)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DRGBA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/2560x1600_ARGB.rgb")

add_subdirectory(rgba2jpg_test)