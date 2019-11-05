#ifndef ZETAMACHINE_CALL_STACK_H
#define ZETAMACHINE_CALL_STACK_H


#include <cstdint>
#include <stack>

namespace zm {
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
        bool store_on_return;
        uint8_t store_to;
    };

    class CallStack {
    public:
        void push(address program_counter);

        StackFrame &get_frame() { return frames.top(); }
        StackFrame pop();
    private:
        std::stack<StackFrame> frames;
    };
}

#endif //ZETAMACHINE_CALL_STACK_H
