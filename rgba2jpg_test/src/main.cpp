#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

std::vector<unsigned char> convertRgbaToJpg(char* rgbaData, int width, int height);

int main(){

    int width = RGBA_WIDTH;
    int height = RGBA_HEIGHT;
    int comp = 4; // RGBA
    char *data = new char[width * height * comp];

    std::ifstream file(RGBA_PATH, std::ios::binary); // open the file in binary mode
    if (file.is_open()) {
        file.read(data, width * height * comp); // read the entire file into the vector
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    // int result = stbi_write_jpg("demo.jpg", width, height, comp, data, 30);
    // if (result == 1) {
    //     std::cout<<"JPG image written successfully!\n";
    // } else {
    //     std::cout<<"Error writing JPG image.\n";
    // }

    std::vector<unsigned char> jpgImage = convertRgbaToJpg(data, width, height);

    std::cout<<"size "<<jpgImage.size()<<std::endl;

     std::ofstream outFile("output1111.jpg", std::ios::binary);
    if (!outFile) {
        std::cerr << "无法打开文件进行写入" << std::endl;
        return 1;
    }

    // 将 JPEG 数据写入文件
    outFile.write(reinterpret_cast<const char*>(jpgImage.data()), jpgImage.size());

    // 关闭文件流
    outFile.close();

    std::cout << "JPEG 图像已成功写入 output.jpg" << std::endl;


    return 0;
}

// Function to convert RGBA image to JPG in memory using stb libraries
std::vector<unsigned char> convertRgbaToJpg(char* rgbaData, int width, int height) {
    std::vector<unsigned char> jpgImage;
    
    // Convert RGBA to JPG
    int jpgSize = width * height * 4; // Assuming 4 channels for JPG
    
    // Convert RGBA to JPG format
    stbi_write_jpg_to_func([](void* context, void* data, int size) {
        std::vector<unsigned char>* jpgImage = static_cast<std::vector<unsigned char>*>(context);
        jpgImage->insert(jpgImage->end(), static_cast<unsigned char*>(data), static_cast<unsigned char*>(data) + size);
    }, &jpgImage, width, height, 4, rgbaData, 50);
    
    return jpgImage;
}