cmake_minimum_required(VERSION 3.15)
project(zetamachine)

set(CMAKE_CXX_STANDARD 14)

add_executable(zetamachine src/main.cpp src/random_number_generator.cpp src/random_number_generator.h src/memory.cpp src/memory.h src/video.cpp src/video.h src/timer.cpp src/timer.h src/machine.cpp src/machine.h src/call_stack.cpp src/call_stack.h src/instructions.h src/processor.cpp src/processor.h src/header.cpp src/header.h)