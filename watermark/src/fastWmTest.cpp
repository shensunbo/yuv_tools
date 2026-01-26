#include "FastWatermark.h"
#include "mylog.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static std::string getCurrentTimeAsString();

int main(){
    int width = NV12_WIDTH;
    int height = NV12_HEIGHT;
    int fileSize = width * height * 3 / 2;
    
    std::shared_ptr<unsigned char[]> nv12Data(new unsigned char[fileSize]);

    std::ifstream yuvFile(NV12_PATH, std::ios::binary);
    if (!yuvFile) {
        mylog(E, "Error opening file.");
        return -1;
    }

    yuvFile.read(reinterpret_cast<char*>(nv12Data.get()), fileSize);
    if (!yuvFile) {
        mylog(E, "Error reading file.");
        return -1;
    }
    yuvFile.close();

    FastWatermark waterMark;
    std::string timeWatermark = getCurrentTimeAsString();

    // calcute time cost, us
    auto start = std::chrono::high_resolution_clock::now();

    waterMark.Nv12AddDateWatermark(nv12Data.get(), width, height, timeWatermark.c_str());

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    mylog(I, "Watermarking took %ld microseconds.", duration);

    std::string oFileName = std::string("trash/") + "wm_" + std::to_string(width) + "x" + std::to_string(height) + "_nv12.yuv";
    std::ofstream output(oFileName, std::ios::binary);
    if (!output) {
        mylog(E, "Error opening output file.");
        return -1;
    }
    output.write(reinterpret_cast<char*>(nv12Data.get()), fileSize);
    output.close();

    // write png for easy view
    std::string pngFileName = std::string("trash/") + "wm_" + std::to_string(width) + "x" + std::to_string(height) + "_nv12.png";
    int result = stbi_write_png(pngFileName.c_str(), width, height, 1, nv12Data.get(), width);
    if (result == 1) {
        mylog(I, "PNG image written successfully!");
    } else {
        mylog(E, "Error writing PNG image.");
    }


    return 0;
}

static std::string getCurrentTimeAsString(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d_%H-%M-%S");

    ss << "-" << milliseconds % 1000;

    return ss.str();
}
