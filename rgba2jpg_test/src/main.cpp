#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <memory>
#include <fstream>

int main(){

    int width = 2560;
    int height = 1600;
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

    int result = stbi_write_jpg("demo.jpg", width, height, comp, data, 30);
    if (result == 1) {
        std::cout<<"JPG image written successfully!\n";
    } else {
        std::cout<<"Error writing JPG image.\n";
    }
    return 0;
}