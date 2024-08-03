add_executable(watermark src/main.cpp src/Watermark.cpp)

target_include_directories(watermark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src /usr/include/freetype2)
target_link_libraries(watermark freetype)




