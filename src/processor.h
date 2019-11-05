#ifndef ZETAMACHINE_PROCESSOR_H
#define ZETAMACHINE_PROCESSOR_H

#include <cstdint>
#include <stack>

#include "call_stack.h"

namespace zm {
    using object = uint16_t;

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
        uint16_t je(uint16_t a, uint16_t b);
        uint16_t jg(uint16_t a, uint16_t b);
        uint16_t jin(object a, object b);
        uint16_t jz(uint16_t a);
        uint16_t jump(int16_t offset);
        uint16_t jl(uint16_t a, uint16_t b);

        uint16_t load(uint16_t variable);
        uint16_t loadb(uint16_t array, uint16_t index);
        uint16_t loadw(uint16_t array, uint16_t index);
        uint16_t log_shift(uint16_t number, int8_t places);

        int16_t add(int16_t a, int16_t b);
        uint16_t and_b(uint16_t a, uint16_t b);
        int16_t art_shift(int16_t number, int8_t places);


    private:

        const InstructionSet &instruction_set;
        const Memory &memory;
        const Video &video;
        const Input &input;
    };
}


#endif //ZETAMACHINE_PROCESSOR_H
