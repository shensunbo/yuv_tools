#include "Watermark.h"
#include "mylog.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

static std::string getCurrentTimeAsString();

int main(){
    int ret = 0;
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

    Watermark waterMark;
    std::string timeWatermark = getCurrentTimeAsString();
    waterMark.Nv12AddDateWatermark(nv12Data.get(), width, height, timeWatermark.c_str());

    std::string oFileName = "wm_" + std::to_string(width) + "x" + std::to_string(height) + "_nv12.yuv";
    std::ofstream output(oFileName, std::ios::binary);
    if (!output) {
        mylog(E, "Error opening output file.");
        return -1;
    }
    output.write(reinterpret_cast<char*>(nv12Data.get()), fileSize);
    output.close();

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
