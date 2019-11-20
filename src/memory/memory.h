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
        uint32_t read_double_word(uint32_t address) {
            return contents[address] << 24 | contents[address + 1] << 16 | contents[address + 2] << 8 | contents[address + 3];
        }

        void read_array(uint32_t source_address, uint32_t length, uint8_t *array);

        bool read_bit(uint32_t address, uint8_t position) { return ((read(address) >> position) & 0x1) != 0; }
        void write_bit(uint32_t address, uint8_t position) { }

        void write(uint32_t address, uint8_t value) { contents[address] = value; }
        void write_word(uint32_t address, uint16_t value) { contents[address] = value >> 8; contents[address + 1] = (value & 0x00FF); }
        void write_double_word(uint32_t address, uint32_t value) {
            contents[address] = value >> 24;
            contents[address + 1] = ((value >> 16) & 0x000000FF);
            contents[address + 2] = ((value >> 8) & 0x000000FF);
            contents[address + 3] = (value & 0x000000FF);
        }

        void load(std::string path);

        template<typename T>
        T* cast(uint32_t address) {
            return (T*) (contents + address);
        }

    private:
        uint32_t size;
        uint8_t *contents;
    };
}


#endif //ZETAMACHINE_MEMORY_H
