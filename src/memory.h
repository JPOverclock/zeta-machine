#ifndef ZETAMACHINE_MEMORY_H
#define ZETAMACHINE_MEMORY_H

#include <cstdint>
#include <string>

namespace zm {
    class Memory {
    public:
        Memory(uint32_t size) : size(size) { contents = new uint8_t[size]; }
        virtual ~Memory() { delete[] contents; }

        uint8_t read(uint32_t address) { return contents[address]; }

        uint8_t read_byte(uint32_t address) { return read(address); }
        uint16_t read_word(uint32_t address) { return contents[address] << 8 | contents[address + 1]; }

        bool read_bit(uint32_t address, uint8_t position) { return ((read(address) >> position) & 0x1) != 0; }
        void write_bit(uint32_t address, uint8_t position) { }

        void write(uint32_t address, uint8_t value) { contents[address] = value; }
        void write_word(uint32_t address, uint16_t value) { contents[address] = value >> 4; contents[address + 1] = (value & 0x00FF); }

        void load(std::string path);

    private:
        uint32_t size;
        uint8_t *contents;
    };
}


#endif //ZETAMACHINE_MEMORY_H
