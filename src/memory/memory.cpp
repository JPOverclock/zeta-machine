#include "memory.h"

#include <fstream>
#include <iostream>

void zm::Memory::load(std::string path) {
    // Load file into memory
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (file.read((char *) contents, size)) {
        std::cout << "Finished loading " << path << ", size = " << size << std::endl;
    } else {
        std::cerr << "Loading failed!" << std::endl;
    }
}
