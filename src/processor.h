#ifndef ZETAMACHINE_PROCESSOR_H
#define ZETAMACHINE_PROCESSOR_H

#include <cstdint>
#include <stack>

#include "call_stack.h"

namespace zm {
    class InstructionSet;
    class Random;
    class Timer;
    class Memory;
    class Input;
    class Video;

    class Processor {
    public:
        Processor(const Memory &memory, const InstructionSet &instruction_set, const Video &video, const Input &input) :
        memory(memory),
        instruction_set(instruction_set),
        video(video),
        input(input) { }


        bool tick();

    protected:
        // Jump operations
        uint16_t je(uint16_t a, uint16_t b);
        uint16_t jg(uint16_t a, uint16_t b);
        uint16_t jin(uint16_t a, uint32_t b);
        uint16_t jz(uint16_t a);
        uint16_t jump(int16_t offset);
        uint16_t jl(uint16_t a, uint16_t b);

        // Routine operations
        uint16_t call_1s(uint16_t routine);
        uint16_t call_1n(uint16_t routine);
        uint16_t call_2s(uint16_t routine, uint16_t arg);
        uint16_t call_2n(uint16_t routine, uint16_t arg);
        uint16_t call(uint16_t arg_count, uint16_t args[arg_count]);
        uint16_t call_vs(uint16_t arg_count, uint16_t args[arg_count]);
        uint16_t call_vs2(uint16_t arg_count, uint16_t args[arg_count]);

        // Storage operations
        uint16_t load(uint16_t variable);
        uint16_t loadb(uint16_t array, uint8_t index);
        uint16_t loadw(uint16_t array, uint16_t index);
        uint16_t store(uint16_t variable, uint16_t value);
        uint16_t storeb(uint16_t array, uint8_t index, uint16_t value);
        uint16_t storew(uint16_t array, uint16_t index, uint16_t value);

        // Arithmetic operations
        int16_t add(int16_t a, int16_t b);
        int16_t sub(int16_t a, int16_t b);
        int16_t mul(int16_t a, int16_t b);
        int16_t mod(int16_t a, int16_t b);
        int16_t div(int16_t a, int16_t b);
        int16_t inc(int16_t variable);
        int16_t dec(int16_t variable);

        uint16_t log_shift(uint16_t number, int8_t places);
        uint16_t art_shift(uint16_t number, int8_t places);

        // Boolean operations
        uint16_t and_b(uint16_t a, uint16_t b);
        uint16_t or_b(uint16_t a, uint16_t b);
        uint16_t not_b(uint16_t a);
        uint16_t r_true();
        uint16_t r_false();

        // Object operations
        uint16_t test_attr(uint16_t object, uint16_t attribute);
        uint16_t set_attr(uint16_t object, uint16_t attribute);
        uint16_t clear_attr(uint16_t object, uint16_t attribute);
        uint16_t insert_obj(uint16_t object, uint16_t destination);
        uint16_t get_prop(uint16_t object, uint16_t property);
        uint16_t get_prop_addr(uint16_t object, uint16_t property);
        uint16_t get_next_prop(uint16_t object, uint16_t property);
        uint16_t get_sibling(uint16_t object);
        uint16_t get_child(uint16_t object);
        uint16_t get_parent(uint16_t object);
        uint16_t get_prop_len(uint16_t property_address);

        // Print operations
        uint16_t print();
        uint16_t print_ret();
        uint16_t print_addr(uint16_t byte_address);
        uint16_t print_paddr(uint16_t packed_address);
        uint16_t print_obj(uint16_t object);
        uint16_t print_char(uint16_t output_character_code);
        uint16_t print_num(int16_t number);
        uint16_t print_table(uint16_t zscii_text, uint16_t width, uint16_t height, uint16_t skip);
        uint16_t print_unicode(uint16_t char_number);
        uint16_t print_form(uint16_t formatted_table);

    private:

        const InstructionSet &instruction_set;
        const Memory &memory;
        const Video &video;
        const Input &input;
    };
}


#endif //ZETAMACHINE_PROCESSOR_H
