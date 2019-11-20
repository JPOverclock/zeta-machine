#include "memory.h"

#include <fstream>
#include <iostream>
#include <cstring>

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

void zm::Memory::read_array(uint32_t source_address, uint32_t length, uint8_t *array) {
    memcpy(array, contents + source_address, length);
}
