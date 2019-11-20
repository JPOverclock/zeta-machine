#include "zchar_mapper.h"
#include "memory.h"

#include <iostream>
#include <sstream>
#include <string>

const char alphabet[3][32] {
        { ' ', '^', '^', '^', '^', '^', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' },
        { ' ', '^', '^', '^', '^', '^', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' },
        { ' ', '^', '^', '^', '^', '^', '^', '\n', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '!', '?', '_', '#', '\'', '"', '/', '\\', '-', ':', '(', ')' }
};

enum class CharMode {
    NORMAL,
    SHIFT,
    SPECIAL,
    ABBREV_1,
    ABBREV_2,
    ABBREV_3,
    DOUBLE_TOP,
    DOUBLE_BOTTOM
};

zm::ZCharMapper::ZCharMapper(zm::Memory &memory) : memory(memory) {
    abbreviations_base = memory.read_word(0x18);
}

std::string zm::ZCharMapper::map(uint32_t address) {
    return map(address, word_len(address));
}

std::string zm::ZCharMapper::map(uint32_t address, uint32_t length) {
    std::stringstream final;

    auto mode = CharMode::NORMAL;

    for (uint32_t i = 0; i < length; ++i) {
        uint16_t raw = memory.read_word(address + (i << 1));

        uint8_t first_char = ((raw & 0b0111110000000000) >> 10);
        uint8_t second_char = ((raw & 0b0000001111100000) >> 5);
        uint8_t third_char = (raw & 0b0000000000011111);

        uint8_t chars[3] = { first_char, second_char, third_char };

        char double_character;

        for (int j = 0; j < 3; j++) {
            uint8_t code = chars[j];

            switch(mode) {
                case CharMode::NORMAL :
                    switch (code) {
                        case 1 : mode = CharMode::ABBREV_1; break;
                        case 2 : mode = CharMode::ABBREV_2; break;
                        case 3 : mode = CharMode::ABBREV_3; break;
                        case 4 : mode = CharMode::SHIFT; break;
                        case 5 : mode = CharMode::SPECIAL; break;
                        default : final << alphabet[0][code];
                    } break;
                case CharMode::ABBREV_1 : final << map(memory.read_word((abbreviations_base + (code << 1))) << 1); mode = CharMode::NORMAL; break;
                case CharMode::ABBREV_2 : final << map(memory.read_word((abbreviations_base + ((32 + code) << 1))) << 1); mode = CharMode::NORMAL; break;
                case CharMode::ABBREV_3 : final << map(memory.read_word((abbreviations_base + ((64 + code) << 1))) << 1); mode = CharMode::NORMAL; break;
                case CharMode::SHIFT :
                    switch (code) {
                        case 1 : mode = CharMode::ABBREV_1; break;
                        case 2 : mode = CharMode::ABBREV_2; break;
                        case 3 : mode = CharMode::ABBREV_3; break;
                        case 4 : mode = CharMode::SHIFT; break;
                        case 5 : mode = CharMode::SPECIAL; break;
                        default : final << alphabet[1][code]; mode = CharMode::NORMAL;
                    } break;
                case CharMode::SPECIAL :
                    switch (code) {
                        case 1 : mode = CharMode::ABBREV_1; break;
                        case 2 : mode = CharMode::ABBREV_2; break;
                        case 3 : mode = CharMode::ABBREV_3; break;
                        case 4 : mode = CharMode::SHIFT; break;
                        case 5 : mode = CharMode::SPECIAL; break;
                        case 6 : mode = CharMode::DOUBLE_TOP; break;
                        default : final << alphabet[2][code]; mode = CharMode::NORMAL;
                    } break;
                case CharMode::DOUBLE_TOP :
                    double_character = code & 0b00000111;
                    mode = CharMode::DOUBLE_BOTTOM;
                    break;
                case CharMode::DOUBLE_BOTTOM :
                    double_character = ((double_character << 5) & (code & 0b00011111));
                    final << double_character;
                    mode = CharMode::NORMAL;
                    break;
            }
        }
    }

    return final.str();
}

uint32_t zm::ZCharMapper::word_len(uint32_t address) {
    uint32_t length = 1;

    for (int i = 0; ; i++) {
        if (!(memory.read_word(address + (i << 1)) & 0b1000000000000000)) {
            length++;
        } else {
            break;
        }
    }

    return length;
}
