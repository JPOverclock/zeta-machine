#ifndef ZETAMACHINE_PROCESSOR_H
#define ZETAMACHINE_PROCESSOR_H

#include <cstdint>
#include <stack>

namespace zm {
    class InstructionSet;
    class Random;
    class Timer;
    class Memory;
    class Input;
    class Video;

    enum class CallType {
        FUNCTION,
        PROCEDURE,
        INTERRUPT,
        NONE // For the compatibilities
    };

    using word = uint16_t;
    using address = uint32_t;

    struct StackFrame {
        address program_counter;
        word variables[16];
        CallType call_type;
        uint8_t arity;
        std::stack<word> routine_stack;
    };

    class CallStack {
    public:
        void push(address program_counter);

        StackFrame &get_frame() { return frames.top(); }
        StackFrame pop();
    private:
        std::stack<StackFrame> frames;
    };

    class Processor {
    public:
        Processor(const Memory &memory, const InstructionSet &instruction_set, const Video &video, const Input &input, uint32_t program_counter = 0) :
        memory(memory),
        instruction_set(instruction_set),
        video(video),
        input(input),
        program_counter(program_counter) { }

        bool tick();
    private:
        uint32_t program_counter;
        const InstructionSet &instruction_set;
        const Memory &memory;
        const Video &video;
        const Input &input;
    };
}


#endif //ZETAMACHINE_PROCESSOR_H
