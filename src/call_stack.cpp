#include "call_stack.h"

void zm::CallStack::push(zm::address program_counter) {
    frames.emplace(StackFrame { program_counter });
}

zm::StackFrame zm::CallStack::pop() {
    zm::StackFrame frame = frames.top();
    frames.pop();

    return std::move(frame);
}
