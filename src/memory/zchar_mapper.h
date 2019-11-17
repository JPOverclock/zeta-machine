#ifndef ZETAMACHINE_ZCHAR_MAPPER_H
#define ZETAMACHINE_ZCHAR_MAPPER_H

#include <cstdint>
#include <string>

namespace zm {
    class Memory;

    class ZCharMapper {
    public:
        explicit ZCharMapper(Memory &memory);

        std::string map(uint32_t address);
        std::string map(uint32_t address, uint32_t length);

        uint32_t word_len(uint32_t address);
    private:
        Memory &memory;
        uint32_t abbreviations_base;
    };
}


#endif //ZETAMACHINE_ZCHAR_MAPPER_H
