#include "header.h"
#include "memory.h"

uint8_t zm::Header::version() { return memory.read(0x00); }

uint16_t zm::Header::high_memory_base_address() { return memory.read_word(0x04); }
uint16_t zm::Header::main_routine_address() { return memory.read_word(0x06); }
uint16_t zm::Header::dictionary_address() { return memory.read_word(0x08); }
uint16_t zm::Header::object_table_address() { return memory.read_word(0x0A); }
uint16_t zm::Header::global_variables_address() { return memory.read_word(0x0C); }
uint16_t zm::Header::static_memory_base_address() { return memory.read_word(0x0E); }
uint16_t zm::Header::abbreviations_table_address() { return memory.read_word(0x18); }
uint16_t zm::Header::terminating_characters_table_address() { return memory.read_word(0x2E); }
uint16_t zm::Header::header_extension_table_address() { return memory.read_word(0x36); }

uint16_t zm::Header::routines_offset() { return memory.read_word(0x28) >> 3; }
uint16_t zm::Header::static_strings_offset() { return memory.read_word(0x2A) >> 3; }
