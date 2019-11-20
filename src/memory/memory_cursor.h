#ifndef ZETAMACHINE_MEMORY_CURSOR_H
#define ZETAMACHINE_MEMORY_CURSOR_H

#include <cstdint>

namespace zm {
    class Memory;

    class MemoryCursor {
    public:
        explicit MemoryCursor(Memory &memory, uint32_t address);

        uint8_t next();
        uint8_t previous();

        uint8_t next_byte();
        uint8_t previous_byte();
        uint16_t next_word();
        uint16_t previous_word();
        uint32_t next_double_word();
        uint32_t previous_double_word();

        void next_bytes(uint32_t length, uint8_t *array);
        void previous_bytes(uint32_t length, uint8_t *array);

        void reset() { current_address = base_address; }
    private:
        Memory &memory;
        uint32_t base_address;
        uint32_t current_address;
    };
}

#endif //ZETAMACHINE_MEMORY_CURSOR_H
