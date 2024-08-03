cmake_minimum_required(VERSION 3.10)

project(yuv_tools)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DRGBA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/2560x1600_ARGB.rgb")
add_definitions(-DRGBA_WIDTH=2560)
add_definitions(-DRGBA_HEIGHT=1600)

add_definitions(-DNV12_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/2560x1600_nv12.yuv")
add_definitions(-DNV12_WIDTH=2560)
add_definitions(-DNV12_HEIGHT=1600)

# add_subdirectory(rgba2jpg_test)
add_subdirectory(watermark)