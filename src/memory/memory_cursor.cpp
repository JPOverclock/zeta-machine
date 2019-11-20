#include "memory_cursor.h"
#include "memory.h"

zm::MemoryCursor::MemoryCursor(zm::Memory &memory, uint32_t address):
    memory(memory), base_address(address), current_address(address) {

}

uint8_t zm::MemoryCursor::next() {
    auto result = memory.read(current_address);
    ++current_address;
    return result;
}

uint8_t zm::MemoryCursor::next_byte() {
    return next();
}

uint16_t zm::MemoryCursor::next_word() {
    auto result = memory.read_word(current_address);
    current_address += 2;
    return result;
}

uint32_t zm::MemoryCursor::next_double_word() {
    auto result = memory.read_double_word(current_address);
    current_address += 4;
    return result;
}

void zm::MemoryCursor::next_bytes(uint32_t length, uint8_t *array) {
    memory.read_array(current_address, length, array);
    current_address += length;
}

uint8_t zm::MemoryCursor::previous() {
    --current_address;
    return memory.read(current_address);
}

uint8_t zm::MemoryCursor::previous_byte() {
    return previous();
}

uint16_t zm::MemoryCursor::previous_word() {
    current_address -= 2;
    return memory.read_word(current_address);
}

uint32_t zm::MemoryCursor::previous_double_word() {
    current_address -= 4;
    return memory.read_double_word(current_address);
}

void zm::MemoryCursor::previous_bytes(uint32_t length, uint8_t *array) {
    current_address -= length;
    memory.read_array(current_address, length, array);
}
