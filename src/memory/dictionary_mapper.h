//
// Created by jpoverclock on 20/11/19.
//

#ifndef ZETAMACHINE_DICTIONARY_MAPPER_H
#define ZETAMACHINE_DICTIONARY_MAPPER_H

#include <cstdint>
#include <string>

namespace zm {
    class Memory;

    class DictionaryMapper {
    public:
        explicit DictionaryMapper(Memory &memory);
        explicit DictionaryMapper(Memory &memory, uint32_t address);

    private:
        Memory &memory;
        uint32_t base_address;

        std::string get_entry(uint16_t entry);
        uint16_t number_of_entries();
    };
}


#endif //ZETAMACHINE_DICTIONARY_MAPPER_H
