cmake_minimum_required(VERSION 3.15)
project(zetamachine)

set(CMAKE_CXX_STANDARD 14)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/spdlog/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(extern/spdlog)

add_executable(zetamachine src/main.cpp src/random_number_generator.cpp src/random_number_generator.h src/memory.cpp src/memory.h src/video.cpp src/video.h src/timer.cpp src/timer.h src/machine.cpp src/machine.h src/call_stack.cpp src/call_stack.h src/instructions.h src/processor.cpp src/processor.h src/header.cpp src/header.h src/object_mapper.cpp src/object_mapper.h)