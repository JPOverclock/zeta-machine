#include "machine.h"
#include "memory/memory.h"
#include "memory/header.h"
#include "call_stack.h"
#include "instructions.h"

#include <iostream>
#include <vector>

#include <ratio>
#include <chrono>

enum class OperandType {
    WORD,
    VARIABLE_NUMBER,
    BYTE,
    OMITTED,
    STORE,
    BRANCH,
};

struct Operand {
    OperandType type;
    uint16_t value;
};

constexpr uint32_t word_address(uint16_t address) {
    return static_cast<uint32_t>(address) << 1;
}

constexpr uint32_t packed_address(uint16_t address, uint8_t version, uint16_t offset) {
    uint32_t result = 0;

    switch (version) {
        case 0x01:
        case 0x02:
        case 0x03:
            result = static_cast<uint32_t>(address) << 1; break;
        case 0x04:
        case 0x05:
            result = static_cast<uint32_t>(address) << 2; break;
        case 0x06:
        case 0x07:
            result = (static_cast<uint32_t>(address) << 2) + (static_cast<uint32_t>(address) << 3); break;
        default:
            result =(static_cast<uint32_t>(address) << 3);
    }

    return result;
}

uint32_t reify_operand(const Operand &operand, zm::CallStack &stack, zm::Memory &memory) {
    switch (operand.type) {
        case OperandType::VARIABLE_NUMBER :
            if (operand.value == 0x00) { // Top of stack
                return stack.get_frame().routine_stack.top();
            } else if (operand.value <= 0x0F) {
                return stack.get_frame().variables[operand.value - 1];
            } else {
                auto global_variables_address = zm::Header(memory).global_variables_address();
                return memory.read_word(global_variables_address + ((operand.value - 0x10) << 1));
            }
        case OperandType::WORD : return memory.read_word(operand.value);
        case OperandType::BYTE : return memory.read_byte(operand.value);
        default: return 0x0;
    }
}

std::string operand_type_to_string(OperandType type) {
    switch (type) {
        case OperandType::BYTE : return "BYTE";
        case OperandType::WORD : return "WORD";
        case OperandType::VARIABLE_NUMBER : return "VARIABLE";
        case OperandType::STORE : return "<<STORE>>";
        case OperandType::BRANCH : return "<<BRANCH>>";
        case OperandType::OMITTED : return "--OMITTED--";
    }
}

void debug(const zm::Instruction &instruction, const std::vector<Operand> &operands, zm::CallStack &stack) {
    std::cout << "PC = " << std::hex << stack.get_frame().program_counter << std::endl;
    std::cout << "Variables: " << std::endl;

    // Print variables
    for (int i = 0; i < stack.get_frame().arity; i++) {
        std::cout << "\t" << "[" << i << "] = " << std::hex << stack.get_frame().variables[i] << std::endl;
    }

    // Print instruction
    std::cout << "Instruction: ";
    switch (instruction.mnemonic) {
        case(zm::Mnemonic::NULL_OP) : std::cout << "NULL_OP"; break;
        case(zm::Mnemonic::JE) : std::cout << "JE"; break;
        case(zm::Mnemonic::JL) : std::cout << "JL"; break;
        case(zm::Mnemonic::JG) : std::cout << "JG"; break;
        case(zm::Mnemonic::DEC_CHK) : std::cout << "DEC_CHK"; break;
        case(zm::Mnemonic::INC_CHK) : std::cout << "INC_CHK"; break;
        case(zm::Mnemonic::JIN) : std::cout << "JIN"; break;
        case(zm::Mnemonic::TEST) : std::cout << "TEST"; break;
        case(zm::Mnemonic::OR) : std::cout << "OR"; break;
        case(zm::Mnemonic::AND) : std::cout << "AND"; break;
        case(zm::Mnemonic::TEST_ATTR) : std::cout << "TEST_ATTR"; break;
        case(zm::Mnemonic::SET_ATTR) : std::cout << "SET_ATTR"; break;
        case(zm::Mnemonic::CLEAR_ATTR) : std::cout << "CLEAR_ATTR"; break;
        case(zm::Mnemonic::STORE) : std::cout << "STORE"; break;
        case(zm::Mnemonic::INSERT_OBJ) : std::cout << "INSERT_OBJ"; break;
        case(zm::Mnemonic::LOADW) : std::cout << "LOADW"; break;
        case(zm::Mnemonic::LOADB) : std::cout << "LOADB"; break;
        case(zm::Mnemonic::GET_PROP) : std::cout << "GET_PROP"; break;
        case(zm::Mnemonic::GET_PROP_ADDR) : std::cout << "GET_PROP_ADDR"; break;
        case(zm::Mnemonic::GET_NEXT_PROP) : std::cout << "GET_NEXT_PROP"; break;
        case(zm::Mnemonic::ADD) : std::cout << "ADD"; break;
        case(zm::Mnemonic::SUB) : std::cout << "SUB"; break;
        case(zm::Mnemonic::MUL) : std::cout << "MUL"; break;
        case(zm::Mnemonic::DIV) : std::cout << "DIV"; break;
        case(zm::Mnemonic::MOD) : std::cout << "MOD"; break;
        case(zm::Mnemonic::CALL_2S) : std::cout << "CALL_2S"; break;
        case(zm::Mnemonic::CALL_2N) : std::cout << "CALL_2N"; break;
        case(zm::Mnemonic::SET_COLOUR) : std::cout << "SET_COLOUR"; break;
        case(zm::Mnemonic::THROW) : std::cout << "THROW"; break;
        case(zm::Mnemonic::JZ) : std::cout << "JZ"; break;
        case(zm::Mnemonic::GET_SIBLING) : std::cout << "GET_SIBLING"; break;
        case(zm::Mnemonic::GET_CHILD) : std::cout << "GET_CHILD"; break;
        case(zm::Mnemonic::GET_PARENT) : std::cout << "GET_PARENT"; break;
        case(zm::Mnemonic::GET_PROP_LEN) : std::cout << "GET_PROP_LEN"; break;
        case(zm::Mnemonic::INC) : std::cout << "INC"; break;
        case(zm::Mnemonic::DEC) : std::cout << "DEC"; break;
        case(zm::Mnemonic::PRINT_ADDR) : std::cout << "PRINT_ADDR"; break;
        case(zm::Mnemonic::CALL_1S) : std::cout << "CALL_1S"; break;
        case(zm::Mnemonic::REMOVE_OBJ) : std::cout << "REMOVE_OBJ"; break;
        case(zm::Mnemonic::PRINT_OBJ) : std::cout << "PRINT_OBJ"; break;
        case(zm::Mnemonic::RET) : std::cout << "RET"; break;
        case(zm::Mnemonic::JUMP) : std::cout << "JUMP"; break;
        case(zm::Mnemonic::PRINT_PADDR) : std::cout << "PRINT_PADDR"; break;
        case(zm::Mnemonic::LOAD) : std::cout << "LOAD"; break;
        case(zm::Mnemonic::NOT) : std::cout << "NOT"; break;
        case(zm::Mnemonic::CALL_1N) : std::cout << "CALL_1N"; break;
        case(zm::Mnemonic::RTRUE) : std::cout << "RTRUE"; break;
        case(zm::Mnemonic::RFALSE) : std::cout << "RFALSE"; break;
        case(zm::Mnemonic::PRINT) : std::cout << "PRINT"; break;
        case(zm::Mnemonic::PRINT_RET) : std::cout << "PRINT_RET"; break;
        case(zm::Mnemonic::NOP) : std::cout << "NOP"; break;
        case(zm::Mnemonic::SAVE) : std::cout << "SAVE"; break;
        case(zm::Mnemonic::RESTORE) : std::cout << "RESTORE"; break;
        case(zm::Mnemonic::RESTART) : std::cout << "RESTART"; break;
        case(zm::Mnemonic::RET_POPPED) : std::cout << "RET_POPPED"; break;
        case(zm::Mnemonic::POP) : std::cout << "POP"; break;
        case(zm::Mnemonic::CATCH) : std::cout << "CATCH"; break;
        case(zm::Mnemonic::QUIT) : std::cout << "QUIT"; break;
        case(zm::Mnemonic::NEW_LINE) : std::cout << "NEW_LINE"; break;
        case(zm::Mnemonic::SHOW_STATUS) : std::cout << "SHOW_STATUS"; break;
        case(zm::Mnemonic::VERIFY) : std::cout << "VERIFY"; break;
        case(zm::Mnemonic::EXTENDED) : std::cout << "EXTENDED"; break;
        case(zm::Mnemonic::PIRACY) : std::cout << "PIRACY"; break;
        case(zm::Mnemonic::CALL) : std::cout << "CALL"; break;
        case(zm::Mnemonic::CALL_VS) : std::cout << "CALL_VS"; break;
        case(zm::Mnemonic::STOREW) : std::cout << "STOREW"; break;
        case(zm::Mnemonic::STOREB) : std::cout << "STOREB"; break;
        case(zm::Mnemonic::PUT_PROP) : std::cout << "PUT_PROP"; break;
        case(zm::Mnemonic::SREAD) : std::cout << "SREAD"; break;
        case(zm::Mnemonic::AREAD) : std::cout << "AREAD"; break;
        case(zm::Mnemonic::PRINT_CHAR) : std::cout << "PRINT_CHAR"; break;
        case(zm::Mnemonic::PRINT_NUM) : std::cout << "PRINT_NUM"; break;
        case(zm::Mnemonic::RANDOM) : std::cout << "RANDOM"; break;
        case(zm::Mnemonic::PUSH) : std::cout << "PUSH"; break;
        case(zm::Mnemonic::PULL) : std::cout << "PULL"; break;
        case(zm::Mnemonic::SPLIT_WINDOW) : std::cout << "SPLIT_WINDOW"; break;
        case(zm::Mnemonic::SET_WINDOW) : std::cout << "SET_WINDOW"; break;
        case(zm::Mnemonic::CALL_VS2) : std::cout << "CALL_VS2"; break;
        case(zm::Mnemonic::ERASE_WINDOW) : std::cout << "ERASE_WINDOW"; break;
        case(zm::Mnemonic::ERASE_LINE) : std::cout << "ERASE_LINE"; break;
        case(zm::Mnemonic::SET_CURSOR) : std::cout << "SET_CURSOR"; break;
        case(zm::Mnemonic::GET_CURSOR) : std::cout << "GET_CURSOR"; break;
        case(zm::Mnemonic::SET_TEXT_STYLE) : std::cout << "SET_TEXT_STYLE"; break;
        case(zm::Mnemonic::BUFFER_MODE) : std::cout << "BUFFER_MODE"; break;
        case(zm::Mnemonic::OUTPUT_STREAM) : std::cout << "OUTPUT_STREAM"; break;
        case(zm::Mnemonic::INPUT_STREAM) : std::cout << "INPUT_STREAM"; break;
        case(zm::Mnemonic::SOUND_EFFECT) : std::cout << "SOUND_EFFECT"; break;
        case(zm::Mnemonic::READ_CHAR) : std::cout << "READ_CHAR"; break;
        case(zm::Mnemonic::SCAN_TABLE) : std::cout << "SCAN_TABLE"; break;
        case(zm::Mnemonic::CALL_VN) : std::cout << "CALL_VN"; break;
        case(zm::Mnemonic::CALL_VN2) : std::cout << "CALL_VN2"; break;
        case(zm::Mnemonic::TOKENISE) : std::cout << "TOKENISE"; break;
        case(zm::Mnemonic::ENCODE_TEXT) : std::cout << "ENCODE_TEXT"; break;
        case(zm::Mnemonic::COPY_TABLE) : std::cout << "COPY_TABLE"; break;
        case(zm::Mnemonic::PRINT_TABLE) : std::cout << "PRINT_TABLE"; break;
        case(zm::Mnemonic::CHECK_ARG_COUNT) : std::cout << "CHECK_ARG_COUNT"; break;
        case(zm::Mnemonic::LOG_SHIFT) : std::cout << "LOG_SHIFT"; break;
        case(zm::Mnemonic::ART_SHIFT) : std::cout << "ART_SHIFT"; break;
        case(zm::Mnemonic::SET_FONT) : std::cout << "SET_FONT"; break;
        case(zm::Mnemonic::DRAW_PICTURE) : std::cout << "DRAW_PICTURE"; break;
        case(zm::Mnemonic::PICTURE_DATA) : std::cout << "PICTURE_DATA"; break;
        case(zm::Mnemonic::ERASE_PICTURE) : std::cout << "ERASE_PICTURE"; break;
        case(zm::Mnemonic::SET_MARGINS) : std::cout << "SET_MARGINS"; break;
        case(zm::Mnemonic::SAVE_UNDO) : std::cout << "SAVE_UNDO"; break;
        case(zm::Mnemonic::RESTORE_UNDO) : std::cout << "RESTORE_UNDO"; break;
        case(zm::Mnemonic::PRINT_UNICODE) : std::cout << "PRINT_UNICODE"; break;
        case(zm::Mnemonic::CHECK_UNICODE) : std::cout << "CHECK_UNICODE"; break;
        case(zm::Mnemonic::SET_TRUE_COLOUR) : std::cout << "SET_TRUE_COLOUR"; break;
        case(zm::Mnemonic::MOVE_WINDOW) : std::cout << "MOVE_WINDOW"; break;
        case(zm::Mnemonic::WINDOW_SIZE) : std::cout << "WINDOW_SIZE"; break;
        case(zm::Mnemonic::WINDOW_STYLE) : std::cout << "WINDOW_STYLE"; break;
        case(zm::Mnemonic::GET_WIND_PROP) : std::cout << "GET_WIND_PROP"; break;
        case(zm::Mnemonic::SCROLL_WINDOW) : std::cout << "SCROLL_WINDOW"; break;
        case(zm::Mnemonic::POP_STACK) : std::cout << "POP_STACK"; break;
        case(zm::Mnemonic::READ_MOUSE) : std::cout << "READ_MOUSE"; break;
        case(zm::Mnemonic::MOUSE_WINDOW) : std::cout << "MOUSE_WINDOW"; break;
        case(zm::Mnemonic::PUSH_STACK) : std::cout << "PUSH_STACK"; break;
        case(zm::Mnemonic::PUT_WIND_PROP) : std::cout << "PUT_WIND_PROP"; break;
        case(zm::Mnemonic::PRINT_FORM) : std::cout << "PRINT_FORM"; break;
        case(zm::Mnemonic::MAKE_MENU) : std::cout << "MAKE_MENU"; break;
        case(zm::Mnemonic::PICTURE_TABLE) : std::cout << "PICTURE_TABLE"; break;
        case(zm::Mnemonic::BUFFER_SCREEN) : std::cout << "BUFFER_SCREEN"; break;
    }

    std::cout << std::endl;

    std::cout << "Operands: " << std::endl;
    for (int i = 0; i < operands.size(); i++) {
        std::cout << "\t" << "[" << i << "] = " << std::hex << operands[i].value << "::" << operand_type_to_string(operands[i].type) << std::endl;
    }
}

// Actual instruction handlers
uint32_t call(const std::vector<Operand> operands) {

}

// RUN!
void zm::Machine::run(std::string path) {
    zm::Memory memory{ 1000000 }; // Almost 1 MB... we got space :)
    zm::CallStack call_stack {};

    memory.load(path);

    uint8_t version = memory.read(0x00);

    // Get instruction set for version
    InstructionSetV5 instruction_set;
    instruction_set.load();

    // Initialize header values...
    uint16_t global_variables_address = memory.read_word(0x0C);

    call_stack.push(memory.read_word(0x06));

    bool quit = false;

    bool process_return_value = false;
    uint32_t return_value;

    while (!quit) {
        // Process interrupts and continue if any are handled
        /*if (process_interrupts()) {
            continue;
        }*/
        uint8_t store_variable;
        bool branch_on_true;
        int16_t branch_offset;

        auto t1 = std::chrono::high_resolution_clock::now();

        // ------ Read instruction ------
        uint32_t cursor = call_stack.get_frame().program_counter;

        uint8_t opcode = memory.read_byte(cursor++);
        std::vector<Operand> operands;
        zm::Instruction instruction;

        // Figure out what kind of instruction this is...
        if (opcode == 0xBE) {
            opcode = memory.read_byte(cursor++);
            instruction = instruction_set.get_ext(opcode);
        } else {
            instruction = instruction_set.get(opcode);
        }

        // Decode operands
        if (instruction.opcode_type == OpcodeType::OP2) {
            auto operand_1_type = instruction.value & 0x40 ? OperandType::VARIABLE_NUMBER : OperandType::BYTE;
            auto operand_2_type = instruction.value & 0x20 ? OperandType::VARIABLE_NUMBER : OperandType::BYTE;

            operands.push_back(Operand { operand_1_type, memory.read_byte(cursor++) });
            operands.push_back(Operand { operand_2_type, memory.read_byte(cursor++) });

        } else if (instruction.opcode_type == OpcodeType::OP1) {
            switch (instruction.value & 0x30) {
                case (0x30) : break;
                case (0x10) : operands.push_back(Operand { OperandType::BYTE, memory.read_byte(cursor++) }); break;
                case (0x20) : operands.push_back(Operand { OperandType::VARIABLE_NUMBER, memory.read_byte(cursor++) }); break;
                case (0x00) : operands.push_back(Operand { OperandType::WORD, memory.read_word(cursor) }); cursor += 2; break;
            }
        } else if (instruction.opcode_type == OpcodeType::VAR) {
            auto operand_definition = memory.read_byte(cursor++);
            auto o1 = operand_definition >> 6;
            auto o2 = operand_definition >> 4;
            auto o3 = operand_definition >> 2;
            auto o4 = operand_definition & 0x03;

            switch (o1 & 0x03) {
                case (0x03) : break;
                case (0x01) : operands.push_back(Operand { OperandType::BYTE, memory.read_byte(cursor++) }); break;
                case (0x02) : operands.push_back(Operand { OperandType::VARIABLE_NUMBER, memory.read_byte(cursor++) }); break;
                case (0x00) : operands.push_back(Operand { OperandType::WORD, memory.read_word(cursor) }); cursor += 2; break;
            }

            switch (o2 & 0x03) {
                case (0x03) : break;
                case (0x01) : operands.push_back(Operand { OperandType::BYTE, memory.read_byte(cursor++) }); break;
                case (0x02) : operands.push_back(Operand { OperandType::VARIABLE_NUMBER, memory.read_byte(cursor++) }); break;
                case (0x00) : operands.push_back(Operand { OperandType::WORD, memory.read_word(cursor) }); cursor += 2; break;
            }

            switch (o3 & 0x03) {
                case (0x03) : break;
                case (0x01) : operands.push_back(Operand { OperandType::BYTE, memory.read_byte(cursor++) }); break;
                case (0x02) : operands.push_back(Operand { OperandType::VARIABLE_NUMBER, memory.read_byte(cursor++) }); break;
                case (0x00) : operands.push_back(Operand { OperandType::WORD, memory.read_word(cursor) }); cursor += 2; break;
            }

            switch (o4 & 0x03) {
                case (0x03) : break;
                case (0x01) : operands.push_back(Operand { OperandType::BYTE, memory.read_byte(cursor++) }); break;
                case (0x02) : operands.push_back(Operand { OperandType::VARIABLE_NUMBER, memory.read_byte(cursor++) }); break;
                case (0x00) : operands.push_back(Operand { OperandType::WORD, memory.read_word(cursor) }); cursor += 2; break;
            }
        }

        if (instruction.store) {
            store_variable = memory.read_byte(cursor++);
        }

        if (instruction.branch) {
            uint16_t operand = memory.read_byte(cursor++);

            branch_on_true = !(operand & 0x0080);

            if (!(operand & 0x0040)) { // Need to read next byte
                operand = (operand << 8) | memory.read_byte(cursor++);
                branch_offset = operand & 0x3FFF;
                branch_offset = (branch_offset & 0x2000) ? (branch_offset | 0xE0000) : branch_offset;

            } else {
                branch_offset = operand & 0x3F;
            }
        }

        auto t2 = std::chrono::high_resolution_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

        std::cout << "Instruction decode took " << std::dec << time_span << " ns." << std::endl;

        debug(instruction, operands, call_stack);

        // Process instruction
        if (instruction.mnemonic == Mnemonic::LOADB) {
            Operand index = operands[0];
            Operand increment = operands[1];
            Operand store = operands[2];

            uint8_t result = memory.read(index.value + increment.value);

            if (store.value == 0x00) {
                call_stack.get_frame().routine_stack.push(result);
            } else if (store.value <= 0x0F) {
                // Set local variable
                call_stack.get_frame().variables[store.value - 1] = result;
            } else {
                // Set global variable
                memory.write_word(global_variables_address + ((store.value - 0x10) << 1), result);
            }
        } else if (instruction.mnemonic == Mnemonic::CALL_VS || instruction.mnemonic == Mnemonic::CALL) {
            Operand address = operands[0];

            // Push a new stack frame with the address to jump to
            call_stack.push(packed_address(address.value, version, 0x00));
            cursor = call_stack.get_frame().program_counter;

            // Read number of arguments
            uint8_t n_args = memory.read(cursor++);
            call_stack.get_frame().arity = n_args;

            // Read n words and initialize variables
            if (version < 5) {
                for (int i = 0; i < n_args; ++i) {
                    uint16_t value = memory.read_word(cursor);

                    call_stack.get_frame().variables[i] = value;
                    cursor += 2;
                }
            }

            // Initialize local variables from operands
            int i = 0;
            for (auto iter = operands.begin() + 1; iter != operands.end() - 1; iter++) {
                call_stack.get_frame().variables[i++] = iter->value;
            }

            // Set store point, if available
            if (instruction.store) {
                call_stack.get_frame().store_on_return = true;
                call_stack.get_frame().store_to = store_variable;
            }

        } else if (instruction.mnemonic == Mnemonic::CALL_2S) {
            Operand address = operands[0];

            // Push a new stack frame with the address to jump to
            call_stack.push(packed_address(address.value, version, 0x00));
            cursor = call_stack.get_frame().program_counter;

            // Read number of arguments
            uint8_t n_args = memory.read(cursor++);
            call_stack.get_frame().arity = n_args;

            // Read n words and initialize variables
            if (version < 5) {
                for (int i = 0; i < n_args; ++i) {
                    uint16_t value = memory.read_word(cursor);

                    call_stack.get_frame().variables[i] = value;
                    cursor += 2;
                }
            }

            // Initialize local variables from operands
            call_stack.get_frame().variables[0] = operands[1].value;

            // Set store point, if available
            if (instruction.store) {
                call_stack.get_frame().store_on_return = true;
                call_stack.get_frame().store_to = store_variable;
            }

        } else if (instruction.mnemonic == Mnemonic::RET) {
            // Reify operand
            process_return_value = true;
            return_value = reify_operand(operands[0], call_stack, memory);

            // Pop stack frame
            call_stack.pop();
            cursor = call_stack.get_frame().program_counter;
        }

        if (instruction.store) {
            // Store value in variable
            if (store_variable == 0x00) {
                call_stack.get_frame().routine_stack.push(return_value);
            } else if (store_variable <= 0x0F) {
                // Set local variable
                call_stack.get_frame().variables[store_variable - 1] = return_value;
            } else {
                // Set global variable
                memory.write_word(global_variables_address + ((store_variable - 0x10) << 1), return_value);
            }
        }

        if (instruction.branch) {
            // Execute branching logic
        }

        // Set program counter
        call_stack.get_frame().program_counter = cursor;

        std::cout << "Cycle done" << std::endl;
    }
}
