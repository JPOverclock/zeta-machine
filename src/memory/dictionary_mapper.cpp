#include "dictionary_mapper.h"
#include "memory.h"
#include "memory_cursor.h"

zm::DictionaryMapper::DictionaryMapper(zm::Memory &memory) : memory(memory) {
    base_address = memory.read_word(0x08);
}

zm::DictionaryMapper::DictionaryMapper(zm::Memory &memory, uint32_t address) : memory(memory), base_address(address) { }

std::string zm::DictionaryMapper::get_entry(uint16_t entry) {
    return std::__cxx11::string();
}

uint16_t zm::DictionaryMapper::number_of_entries() {
    MemoryCursor cursor{ memory, base_address };

    uint8_t input_code_size = cursor.next();
    uint8_t input_code_buffer[64];

    cursor.next_bytes(input_code_size, input_code_buffer);

    uint8_t entry_length = cursor.next();
    uint16_t number_of_entries = cursor.next_word();

    return number_of_entries;
}
