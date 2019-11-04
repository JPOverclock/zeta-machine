#ifndef ZETAMACHINE_INSTRUCTIONS_H
#define ZETAMACHINE_INSTRUCTIONS_H

#include <cstdint>
#include <memory>
#include <iostream>

#define INSTRUCTION_SET_LENGTH 256

namespace zm {

    enum class OpcodeType {
        OP0,
        OP1,
        OP2,
        VAR,
        EXT
    };

    enum class Mnemonic {
        NULL_OP,
        JE,
        JL,
        JG,
        DEC_CHK,
        INC_CHK,
        JIN,
        TEST,
        OR,
        AND,
        TEST_ATTR,
        SET_ATTR,
        CLEAR_ATTR,
        STORE,
        INSERT_OBJ,
        LOADW,
        LOADB,
        GET_PROP,
        GET_PROP_ADDR,
        GET_NEXT_PROP,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        CALL_2S,
        CALL_2N,
        SET_COLOUR,
        THROW,
        JZ,
        GET_SIBLING,
        GET_CHILD,
        GET_PARENT,
        GET_PROP_LEN,
        INC,
        DEC,
        PRINT_ADDR,
        CALL_1S,
        REMOVE_OBJ,
        PRINT_OBJ,
        RET,
        JUMP,
        PRINT_PADDR,
        LOAD,
        NOT,
        CALL_1N,
        RTRUE,
        RFALSE,
        PRINT,
        PRINT_RET,
        NOP,
        SAVE,
        RESTORE,
        RESTART,
        RET_POPPED,
        POP,
        CATCH,
        QUIT,
        NEW_LINE,
        SHOW_STATUS,
        VERIFY,
        EXTENDED,
        PIRACY,
        CALL,
        CALL_VS,
        STOREW,
        STOREB,
        PUT_PROP,
        SREAD,
        AREAD,
        PRINT_CHAR,
        PRINT_NUM,
        RANDOM,
        PUSH,
        PULL,
        SPLIT_WINDOW,
        SET_WINDOW,
        CALL_VS2,
        ERASE_WINDOW,
        ERASE_LINE,
        SET_CURSOR,
        GET_CURSOR,
        SET_TEXT_STYLE,
        BUFFER_MODE,
        OUTPUT_STREAM,
        INPUT_STREAM,
        SOUND_EFFECT,
        READ_CHAR,
        SCAN_TABLE,
        CALL_VN,
        CALL_VN2,
        TOKENISE,
        ENCODE_TEXT,
        COPY_TABLE,
        PRINT_TABLE,
        CHECK_ARG_COUNT,
        LOG_SHIFT,
        ART_SHIFT,
        SET_FONT,
        DRAW_PICTURE,
        PICTURE_DATA,
        ERASE_PICTURE,
        SET_MARGINS,
        SAVE_UNDO,
        RESTORE_UNDO,
        PRINT_UNICODE,
        CHECK_UNICODE,
        SET_TRUE_COLOUR,
        MOVE_WINDOW,
        WINDOW_SIZE,
        WINDOW_STYLE,
        GET_WIND_PROP,
        SCROLL_WINDOW,
        POP_STACK,
        READ_MOUSE,
        MOUSE_WINDOW,
        PUSH_STACK,
        PUT_WIND_PROP,
        PRINT_FORM,
        MAKE_MENU,
        PICTURE_TABLE,
        BUFFER_SCREEN
    };

    struct Instruction {
        OpcodeType opcode_type;
        Mnemonic mnemonic;
        uint8_t value;
        uint8_t base_version;
        bool store;
        bool branch;
    };

    class InstructionSet {
    public:
        InstructionSet() :
        set(new Instruction[INSTRUCTION_SET_LENGTH]),
        extended_set(new Instruction[INSTRUCTION_SET_LENGTH]) { }

        ~InstructionSet() {
            delete[] set;
            delete[] extended_set;
        }

        void load() {
            load_set();
            extend_set();
            expand_set();

            load_extended_set();
            extend_extended_set();
            expand_extended_set();
        }

        const Instruction &get(const uint8_t opcode) { return set[opcode]; }
        const Instruction &get_ext(const uint8_t opcode) { return extended_set[opcode]; }

    protected:
        void load_set() {
            // 2OP
            set[0] = { OpcodeType::OP2, Mnemonic::NULL_OP, 0, 1, false, false };
            set[1] = { OpcodeType::OP2, Mnemonic::JE, 1, 1, false, true };
            set[2] = { OpcodeType::OP2, Mnemonic::JL, 2, 1, false, true };
            set[3] = { OpcodeType::OP2, Mnemonic::JG, 3, 1, false, true };
            set[4] = { OpcodeType::OP2, Mnemonic::DEC_CHK, 4, 1, false, true };
            set[5] = { OpcodeType::OP2, Mnemonic::INC_CHK, 5, 1, false, true };
            set[6] = { OpcodeType::OP2, Mnemonic::JIN, 6, 1, false, true };
            set[7] = { OpcodeType::OP2, Mnemonic::TEST, 7, 1, false, true };
            set[8] = { OpcodeType::OP2, Mnemonic::OR, 8, 1, true, false };
            set[9] = { OpcodeType::OP2, Mnemonic::AND, 9, 1, true, false };
            set[10] = { OpcodeType::OP2, Mnemonic::TEST_ATTR, 10, 1, false, true };
            set[11] = { OpcodeType::OP2, Mnemonic::SET_ATTR, 11, 1, false, false };
            set[12] = { OpcodeType::OP2, Mnemonic::CLEAR_ATTR, 12, 1, false, false };
            set[13] = { OpcodeType::OP2, Mnemonic::STORE, 13, 1, false, false };
            set[14] = { OpcodeType::OP2, Mnemonic::INSERT_OBJ, 14, 1, false, false };
            set[15] = { OpcodeType::OP2, Mnemonic::LOADW, 15, 1, true, false };
            set[16] = { OpcodeType::OP2, Mnemonic::LOADB, 16, 1, true, false };
            set[17] = { OpcodeType::OP2, Mnemonic::GET_PROP, 17, 1, true, false };
            set[18] = { OpcodeType::OP2, Mnemonic::GET_PROP_ADDR, 18, 1, true, false };
            set[19] = { OpcodeType::OP2, Mnemonic::GET_NEXT_PROP, 19, 1, true, false };
            set[20] = { OpcodeType::OP2, Mnemonic::ADD, 20, 1, true, false };
            set[21] = { OpcodeType::OP2, Mnemonic::SUB, 21, 1, true, false };
            set[22] = { OpcodeType::OP2, Mnemonic::MUL, 22, 1, true, false };
            set[23] = { OpcodeType::OP2, Mnemonic::DIV, 23, 1, true, false };
            set[24] = { OpcodeType::OP2, Mnemonic::MOD, 24, 1, true, false };
            set[25] = { OpcodeType::OP2, Mnemonic::CALL_2S, 25, 4, true, false };
            set[26] = { OpcodeType::OP2, Mnemonic::CALL_2N, 26, 5, false, false };
            set[27] = { OpcodeType::OP2, Mnemonic::SET_COLOUR, 27, 5, false, false };
            set[28] = { OpcodeType::OP2, Mnemonic::THROW, 28, 5, false, false };
            set[29] = { OpcodeType::OP2, Mnemonic::NULL_OP, 29, 1, false, false };
            set[30] = { OpcodeType::OP2, Mnemonic::NULL_OP, 30, 1, false, false };
            set[31] = { OpcodeType::OP2, Mnemonic::NULL_OP, 31, 1, false, false };

            // 1OP
            set[128] = { OpcodeType::OP1, Mnemonic::JZ, 128, 1, false, true };
            set[129] = { OpcodeType::OP1, Mnemonic::GET_SIBLING, 129, 1, true, true };
            set[130] = { OpcodeType::OP1, Mnemonic::GET_CHILD, 130, 1, true, true };
            set[131] = { OpcodeType::OP1, Mnemonic::GET_PARENT, 131, 1, true, false };
            set[132] = { OpcodeType::OP1, Mnemonic::GET_PROP_LEN, 132, 1, true, false };
            set[133] = { OpcodeType::OP1, Mnemonic::INC, 133, 1, false, true };
            set[134] = { OpcodeType::OP1, Mnemonic::DEC, 134, 1, false, true };
            set[135] = { OpcodeType::OP1, Mnemonic::PRINT_ADDR, 135, 1, true, false };
            set[136] = { OpcodeType::OP1, Mnemonic::CALL_1S, 136, 4, true, false };
            set[137] = { OpcodeType::OP1, Mnemonic::REMOVE_OBJ, 137, 1, false, false };
            set[138] = { OpcodeType::OP1, Mnemonic::PRINT_OBJ, 138, 1, false, false };
            set[139] = { OpcodeType::OP1, Mnemonic::RET, 139, 1, false, false };
            set[140] = { OpcodeType::OP1, Mnemonic::JUMP, 140, 1, false, false };
            set[141] = { OpcodeType::OP1, Mnemonic::PRINT_PADDR, 141, 1, false, false };
            set[142] = { OpcodeType::OP1, Mnemonic::LOAD, 142, 1, true, false };
            set[143] = { OpcodeType::OP1, Mnemonic::NOT, 143, 1, true, false };

            // 0OP
            set[176] = { OpcodeType::OP0, Mnemonic::RTRUE, 176, 1, false, false };
            set[177] = { OpcodeType::OP0, Mnemonic::RFALSE, 177, 1, false, false };
            set[178] = { OpcodeType::OP0, Mnemonic::PRINT, 178, 1, false, false };
            set[179] = { OpcodeType::OP0, Mnemonic::PRINT_RET, 179, 1, false, false };
            set[180] = { OpcodeType::OP0, Mnemonic::NOP, 180, 1, false, false };
            set[181] = { OpcodeType::OP0, Mnemonic::SAVE, 181, 1, false, true };
            set[182] = { OpcodeType::OP0, Mnemonic::RESTORE, 182, 1, false, true };
            set[183] = { OpcodeType::OP0, Mnemonic::RESTART, 183, 1, false, false };
            set[184] = { OpcodeType::OP0, Mnemonic::RET_POPPED, 184, 1, false, false };
            set[185] = { OpcodeType::OP0, Mnemonic::POP, 185, 1, false, false };
            set[186] = { OpcodeType::OP0, Mnemonic::QUIT, 186, 1, false, false };
            set[187] = { OpcodeType::OP0, Mnemonic::NEW_LINE, 187, 1, false, false };
            set[188] = { OpcodeType::OP0, Mnemonic::SHOW_STATUS, 188, 3, false, false };
            set[189] = { OpcodeType::OP0, Mnemonic::VERIFY, 189, 3, false, true };
            set[190] = { OpcodeType::OP0, Mnemonic::EXTENDED, 190, 5, false, false };
            set[191] = { OpcodeType::OP0, Mnemonic::PIRACY, 191, 1, false, true };

            // VAR
            set[224] = { OpcodeType::VAR, Mnemonic::CALL, 224, 1, true, false };
            set[225] = { OpcodeType::VAR, Mnemonic::STOREW, 225, 1, false, false };
            set[226] = { OpcodeType::VAR, Mnemonic::STOREB, 226, 1, false, false };
            set[227] = { OpcodeType::VAR, Mnemonic::PUT_PROP, 227, 1, false, false };
            set[228] = { OpcodeType::VAR, Mnemonic::SREAD, 228, 1, false, false };
            set[229] = { OpcodeType::VAR, Mnemonic::PRINT_CHAR, 229, 1, false, false };
            set[230] = { OpcodeType::VAR, Mnemonic::PRINT_NUM, 230, 1, false, false };
            set[231] = { OpcodeType::VAR, Mnemonic::RANDOM, 231, 1, true, false };
            set[232] = { OpcodeType::VAR, Mnemonic::PUSH, 232, 1, false, false };
            set[233] = { OpcodeType::VAR, Mnemonic::PULL, 233, 1, false, false };
            set[234] = { OpcodeType::VAR, Mnemonic::SPLIT_WINDOW, 234, 3, false, false };
            set[235] = { OpcodeType::VAR, Mnemonic::SET_WINDOW, 235, 3, false, false };
            set[236] = { OpcodeType::VAR, Mnemonic::CALL_VS2, 236, 4, true, false };
            set[237] = { OpcodeType::VAR, Mnemonic::ERASE_WINDOW, 237, 4, false, false };
            set[238] = { OpcodeType::VAR, Mnemonic::ERASE_LINE, 238, 4, false, false };
            set[239] = { OpcodeType::VAR, Mnemonic::SET_CURSOR, 239, 4, false, false };
            set[240] = { OpcodeType::VAR, Mnemonic::GET_CURSOR, 240, 4, false, false };
            set[241] = { OpcodeType::VAR, Mnemonic::SET_TEXT_STYLE, 241, 4, false, false };
            set[242] = { OpcodeType::VAR, Mnemonic::BUFFER_MODE, 242, 4, false, false };
            set[243] = { OpcodeType::VAR, Mnemonic::OUTPUT_STREAM, 243, 3, false, false };
            set[244] = { OpcodeType::VAR, Mnemonic::INPUT_STREAM, 244, 3, false, false };
            set[245] = { OpcodeType::VAR, Mnemonic::SOUND_EFFECT, 245, 3, false, false };
            set[246] = { OpcodeType::VAR, Mnemonic::READ_CHAR, 246, 4, true, false };
            set[247] = { OpcodeType::VAR, Mnemonic::SCAN_TABLE, 247, 4, true, true };
            set[248] = { OpcodeType::VAR, Mnemonic::NOT, 248, 5, true, false };
            set[249] = { OpcodeType::VAR, Mnemonic::CALL_VN, 249, 5, false, false };
            set[250] = { OpcodeType::VAR, Mnemonic::CALL_VN2, 250, 5, false, false };
            set[251] = { OpcodeType::VAR, Mnemonic::TOKENISE, 251, 5, false, false };
            set[252] = { OpcodeType::VAR, Mnemonic::ENCODE_TEXT, 252, 5, false, false };
            set[253] = { OpcodeType::VAR, Mnemonic::COPY_TABLE, 253, 5, false, false };
            set[254] = { OpcodeType::VAR, Mnemonic::PRINT_TABLE, 254, 5, false, false };
            set[255] = { OpcodeType::VAR, Mnemonic::CHECK_ARG_COUNT, 255, 5, false, true };
        }

        void load_extended_set() {
            extended_set[0] = { OpcodeType::EXT, Mnemonic::SAVE, 0, 5, true, false };
            extended_set[1] = { OpcodeType::EXT, Mnemonic::RESTORE, 1, 5, true, false };
            extended_set[2] = { OpcodeType::EXT, Mnemonic::LOG_SHIFT, 2, 5, true, false };
            extended_set[3] = { OpcodeType::EXT, Mnemonic::ART_SHIFT, 3, 5, true, false };
            extended_set[4] = { OpcodeType::EXT, Mnemonic::SET_FONT, 4, 5, true, false };
            extended_set[5] = { OpcodeType::EXT, Mnemonic::DRAW_PICTURE, 5, 6, false, false };
            extended_set[6] = { OpcodeType::EXT, Mnemonic::PICTURE_DATA, 6, 6, false, true };
            extended_set[7] = { OpcodeType::EXT, Mnemonic::ERASE_PICTURE, 7, 6, false, false };
            extended_set[8] = { OpcodeType::EXT, Mnemonic::SET_MARGINS, 8, 6, false, false };
            extended_set[9] = { OpcodeType::EXT, Mnemonic::SAVE_UNDO, 9, 5, true, false };
            extended_set[10] = { OpcodeType::EXT, Mnemonic::RESTORE_UNDO, 10, 5, true, false };
            extended_set[11] = { OpcodeType::EXT, Mnemonic::PRINT_UNICODE, 11, 5, false, false };
            extended_set[12] = { OpcodeType::EXT, Mnemonic::CHECK_UNICODE, 12, 5, false, false };
            extended_set[13] = { OpcodeType::EXT, Mnemonic::SET_TRUE_COLOUR, 13, 5, false, false };
            extended_set[14] = { OpcodeType::EXT, Mnemonic::NULL_OP, 14, 1, false, false };
            extended_set[15] = { OpcodeType::EXT, Mnemonic::NULL_OP, 14, 1, false, false };
            extended_set[16] = { OpcodeType::EXT, Mnemonic::MOVE_WINDOW, 16, 6, false, false };
            extended_set[17] = { OpcodeType::EXT, Mnemonic::WINDOW_SIZE, 17, 6, false, false };
            extended_set[18] = { OpcodeType::EXT, Mnemonic::WINDOW_STYLE, 18, 6, false, false };
            extended_set[19] = { OpcodeType::EXT, Mnemonic::GET_WIND_PROP, 19, 6, true, false };
            extended_set[20] = { OpcodeType::EXT, Mnemonic::SCROLL_WINDOW, 20, 6, false, false };
            extended_set[21] = { OpcodeType::EXT, Mnemonic::POP_STACK, 21, 6, false, false };
            extended_set[22] = { OpcodeType::EXT, Mnemonic::READ_MOUSE, 22, 6, false, false };
            extended_set[23] = { OpcodeType::EXT, Mnemonic::MOUSE_WINDOW, 23, 6, false, false };
            extended_set[24] = { OpcodeType::EXT, Mnemonic::PUSH_STACK, 24, 6, false, true };
            extended_set[25] = { OpcodeType::EXT, Mnemonic::PUT_WIND_PROP, 25, 6, false, false };
            extended_set[26] = { OpcodeType::EXT, Mnemonic::PRINT_FORM, 26, 6, false, false };
            extended_set[27] = { OpcodeType::EXT, Mnemonic::MAKE_MENU, 27, 6, false, true };
            extended_set[28] = { OpcodeType::EXT, Mnemonic::PICTURE_TABLE, 28, 6, false, false };
            extended_set[29] = { OpcodeType::EXT, Mnemonic::BUFFER_SCREEN, 29, 6, true, false };
        }

        void expand_set() {
            // Duplicate 2OPs again for the additional 3 versions
            for (uint8_t i = 32, j = 0; i < 64; ++i, ++j) {
                set[i] = set[j];
                set[i].value = i;
            }

            for (uint8_t i = 64, j = 0; i < 96; ++i, ++j) {
                set[i] = set[j];
                set[i].value = i;
            }

            for (uint8_t i = 96, j = 0; i < 128; ++i, ++j) {
                set[i] = set[j];
                set[i].value = i;
            }

            // Same for 1OPs
            for (uint8_t i = 144, j = 128; i < 160; ++i, ++j) {
                set[i] = set[j];
                set[i].value = i;
            }

            for (uint8_t i = 160, j = 128; i < 176; ++i, ++j) {
                set[i] = set[j];
                set[i].value = i;
            }

            // And the VAR variants of 2 OPS
            for (uint8_t i = 192, j = 0; i < 224; ++i, ++j) {
                set[i] = set[j];
                set[i].value = i;
                set[i].opcode_type = OpcodeType::VAR;
            }
        }

        void expand_extended_set() { }

        virtual void extend_set() { }
        virtual void extend_extended_set() { }

        Instruction *set;
        Instruction *extended_set;
    };

    class InstructionSetV4 : public InstructionSet {
    protected:
        void extend_set() override {
            set[181] = { OpcodeType::OP0, Mnemonic::SAVE, 181, 4, false, false };
            set[182] = { OpcodeType::OP0, Mnemonic::RESTORE, 182, 4, false, false };
            set[224] = { OpcodeType::VAR, Mnemonic::CALL_VS, 224, 4, true, false };
            set[228] = { OpcodeType::VAR, Mnemonic::SREAD, 228, 4, false, false };
        }

        void extend_extended_set() override { }
    };

    class InstructionSetV5 : public InstructionSet {
    protected:
        void extend_set() override {
            set[143] = { OpcodeType::OP1, Mnemonic::CALL_1N, 143, 5, false, false };
            set[181] = { OpcodeType::OP0, Mnemonic::SAVE, 181, 4, false, false };
            set[182] = { OpcodeType::OP0, Mnemonic::RESTORE, 182, 4, false, false };
            set[185] = { OpcodeType::OP0, Mnemonic::CATCH, 185, 5, true, false };
            set[224] = { OpcodeType::VAR, Mnemonic::CALL_VS, 224, 4, true, false };
            set[228] = { OpcodeType::VAR, Mnemonic::AREAD, 228, 1, true, false };
            set[243] = { OpcodeType::VAR, Mnemonic::OUTPUT_STREAM, 243, 5, false, false };
        }

        void extend_extended_set() override { }
    };

    class InstructionSetV6: public InstructionSet {
    protected:
        void extend_set() override {
            set[143] = { OpcodeType::OP1, Mnemonic::CALL_1N, 143, 5, false, false };
            set[181] = { OpcodeType::OP0, Mnemonic::SAVE, 181, 4, false, false };
            set[182] = { OpcodeType::OP0, Mnemonic::RESTORE, 182, 4, false, false };
            set[185] = { OpcodeType::OP0, Mnemonic::CATCH, 185, 5, true, false };
            set[224] = { OpcodeType::VAR, Mnemonic::CALL_VS, 224, 4, true, false };
            set[228] = { OpcodeType::VAR, Mnemonic::AREAD, 228, 1, true, false };
            set[233] = { OpcodeType::VAR, Mnemonic::PULL, 233, 6, true, false };
            set[238] = { OpcodeType::VAR, Mnemonic::ERASE_LINE, 238, 6, false, false };
            set[239] = { OpcodeType::VAR, Mnemonic::SET_CURSOR, 239, 6, false, false };
            set[243] = { OpcodeType::VAR, Mnemonic::OUTPUT_STREAM, 243, 6, false, false };
        }

        void extend_extended_set() override {
            extended_set[4] = { OpcodeType::EXT, Mnemonic::SET_FONT, 4, 6, true, false };
            extended_set[13] = { OpcodeType::EXT, Mnemonic::SET_TRUE_COLOUR, 13, 6, false, false };
        }
    };
}

#endif //ZETAMACHINE_INSTRUCTIONS_H
