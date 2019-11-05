#ifndef ZETAMACHINE_HEADER_H
#define ZETAMACHINE_HEADER_H

#include <cstdint>

namespace zm {
    class Memory;

    class Header {
    public:
        explicit Header(Memory& memory) : memory(memory) { }

        uint8_t version();

        uint16_t high_memory_base_address();
        uint16_t main_routine_address();
        uint16_t dictionary_address();
        uint16_t object_table_address();
        uint16_t global_variables_address();
        uint16_t static_memory_base_address();
        uint16_t abbreviations_table_address();
        uint16_t terminating_characters_table_address();
        uint16_t header_extension_table_address();

        uint16_t routines_offset();
        uint16_t static_strings_offset();

    private:
        Memory& memory;
    };
}


#endif //ZETAMACHINE_HEADER_H
